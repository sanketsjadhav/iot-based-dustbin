#include <LiquidCrystal.h>
LiquidCrystal lcd(5,6,8,7,9,10);

#include <HCSR04.h>
HCSR04 hc(18,19);

int garbaje = 0;
int level = 0;

#include <SoftwareSerial.h>
#define RX 17
#define TX 16
String AP = "garbajeiot";       // CHANGE ME
String PASS = "12345678"; // CHANGE ME
String API = "78R35YZWDPW0VDTL";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";

String field1 = "field1";

int countTrueCommand;
int countTimeCommand; 

boolean found = false; 
SoftwareSerial esp8266(RX,TX); 

int sen = 0;

#include <Servo.h>
Servo myservo; 

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2); 

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("IOT BASED DUST");
  lcd.setCursor(0,1);
  lcd.print(" -BIN SYSTEM");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connect to WiFi");
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");

  pinMode(3,INPUT);
   myservo.attach(12); 
}

void loop() {
 garbaje = hc.dist(); 
 level = map(garbaje, 0, 31 , 100 , 0);
 // Serial.println(garbaje);
  
 sen = digitalRead(3);

 if(sen == HIGH)
 {
  myservo.write(10); 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Drop Your Garbaje");
  lcd.setCursor(0,1);
  lcd.print("  Inside Me");
  delay(3000);
  }
  else
  {
    myservo.write(90); 
  }

lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Garbaje Level :--");
  lcd.setCursor(0,1);
  lcd.print(level);

  if(level > 80)
  {
    lcd.setCursor(8,1);
  lcd.print("| FULL");
  digitalWrite(14,HIGH);
    }

    if(level < 80 && level > 40)
  {
    lcd.setCursor(8,1);
  lcd.print("| MED");
  digitalWrite(14,LOW);
    }

    if(level < 20)
  {
    lcd.setCursor(8,1);
    digitalWrite(14,LOW);
  lcd.print("| EMPTY");
    }

    String getData = "GET /update?api_key="+ API +"&"+ field1 +"="+String(level);
  sendCommand("AT+CIPMUX=1",5,"OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
  sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
  esp8266.println(getData);delay(1500);countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0",5,"OK");
}

 void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
