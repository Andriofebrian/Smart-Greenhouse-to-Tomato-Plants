/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi

 */

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>

#define RX 10
#define TX 11
#define DHTPIN 2
#define DHTTYPE DHT11   // DHT 11 

DHT dht(DHTPIN, DHTTYPE);


RTC_DS1307 RTC;

File myFile;

int val1 = 0; //value for storing moisture value 
int soilPin1 = A8; //Declare a variable for the soil moisture sensor 

int val2 = 0; //value for storing moisture value 
int soilPin2 = A9; //Declare a variable for the soil moisture sensor 

int val3 = 0; //value for storing moisture value 
int soilPin3 = A10; //Declare a variable for the soil moisture sensor 

int val4 = 0; //value for storing moisture value 
int soilPin4 = A11; //Declare a variable for the soil moisture sensor 

int val5 = 0; //value for storing moisture value 
int soilPin5 = A12; //Declare a variable for the soil moisture sensor 

int val6 = 0; //value for storing moisture value 
int soilPin6 = A13; //Declare a variable for the soil moisture sensor 

int val7 = 0; //value for storing moisture value 
int soilPin7 = A14; //Declare a variable for the soil moisture sensor 

int val8 = 0; //value for storing moisture value 
int soilPin8 = A15; //Declare a variable for the soil moisture sensor 

int valtotal = 0;

int LDR = A0;//Declare a variable for the soil moisture sensor
int value = 0; //value for storing moisture value 


int relay1 = 5;
int relay2 = 6;
int relay3 = 7;



float t; // temperature in celcius
float f; // temperature in fahrenheit
float h; // humidity

String AP = "iPhone";       // CHANGE ME
String PASS = "andriofebrian"; // CHANGE ME

String API = "PK39TQA2TW4YSS3Q";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";
String field1 = "field1";
String field2 = "field2";
String field3 = "field3";
String field4 = "field4";

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 


SoftwareSerial esp8266(RX,TX); 

// Create variable to store incoming SMS characters
char incomingChar;


void setup() {
    Serial.begin(9600);
  esp8266.begin(115200);
   dht.begin();
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
    
  dht.begin(); 
     Wire.begin();
    RTC.begin();
  // Check to see if the RTC is keeping time.  If it is, load the time from your computer.
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // This will reflect the time that your sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

   Serial.println("Membuka Micro SD . . .");
 delay(1000);
 if (!SD.begin(4)) 

  {
 Serial.println("Gagal Membuka Micro SD!");
 return;
 }
 Serial.println("Berhasil Membuka Micro SD");
 delay(1000);

  
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

}


void loop() {
  // listen for incoming clients

 

  Serial.print("Soil Moisture = ");    
  val1 = analogRead(soilPin1);//Read the SIG value form sensor 
  Serial.println(val1);

    Serial.print("Soil Moisture = ");    
  val2 = analogRead(soilPin2);//Read the SIG value form sensor 
  Serial.println(val2);

  Serial.print("Soil Moisture = ");    
  val3 = analogRead(soilPin3);//Read the SIG value form sensor 
  Serial.println(val3);

  Serial.print("Soil Moisture = ");    
  val4 = analogRead(soilPin4);//Read the SIG value form sensor 
  Serial.println(val4);

  Serial.print("Soil Moisture = ");    
  val5 = analogRead(soilPin5);//Read the SIG value form sensor 
  Serial.println(val5);

  Serial.print("Soil Moisture = ");    
  val6 = analogRead(soilPin6);//Read the SIG value form sensor 
  Serial.println(val6);

  Serial.print("Soil Moisture = ");    
  val7 = analogRead(soilPin7);//Read the SIG value form sensor 
  Serial.println(val7);

  Serial.print("Soil Moisture = ");    
  val8 = analogRead(soilPin8);//Read the SIG value form sensor 
  Serial.println(val8);

  Serial.print("Soil Moisturetotal = ");   
valtotal = (val1+val2+val3+val4+val5+val6+val7+val8)/8;
Serial.println ("*K"+String(valtotal)+"*");
//   rumus ubah ke percent
//   val = map(val,1023,300,0,100); //550 bacaan max, 10 bacaan min
//   Serial.print("Mositure : ");
//   Serial.print(val);
//   Serial.println("%");

  Serial.print("LDR = ");    
  value = analogRead(LDR);//Read the SIG value form sensor 
  Serial.println("*C"+String(value)+"*");


  h = dht.readHumidity();
  // Read temperature as Celsius
  t = dht.readTemperature();
  // Read temperature as Fahrenheit
  f = dht.readTemperature(true);

  // Compute temperature values in Celcius
 // t = dht.computeHeatIndex(t,h,false);
  Serial.print("Humidity: "); 
  Serial.print("*H"+String(h)+"*");
  Serial.println(" %\t");
  Serial.print("Temperature: "); 
  Serial.print("*T"+String(t)+"*");
  Serial.println(" *C ");


  String getData = "GET /update?api_key="+ API +"&"+ field1 +"="+String(valtotal)+"&"+ field2 +"="+String(t)+"&"+ field3 +"="+String(value)+"&"+ field4 +"="+String(h);

sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");


  
  

////send data bluetooth
//Serial.print("*T"+String(t)+"*");
//Serial.print("*H"+String(h)+"*");
//Serial.print("*K"+String(valtotal)+"*");
//Serial.print("*C"+String(value)+"*");


    DateTime now = RTC.now(); 
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();   

    

  
   // Kirim data ke SD card 
 myFile = SD.open("logger.txt", FILE_WRITE); //Membuka File test.txt
 if (myFile) // jika file tersedia tulis data
 {
      myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.day(), DEC);
    myFile.print('/');
    myFile.print(now.year(), DEC);
    myFile.print(' ');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);
    myFile.println();
     myFile.print(" Kelembapan Tanah: ");
 myFile.println(valtotal);
 myFile.print("Temperature: ");
 myFile.println(t); // menulis data temperatur di MicroSD
 myFile.print("Intensitas cahaya: ");
 myFile.println(value);
 myFile.close();
 }
 else 
 {
 Serial.println("gagal membuka test.txt"); // jika gagal print error
 }
 delay(10000); //memberi jeda 5 detik, karena proses pengiriman data berlangsung selama 3 detik, jadi total 5 detik

/////////////////////relay///////////////////
  
  if(t < 30){
    digitalWrite(relay1, HIGH);
  }else{
     digitalWrite(relay1, LOW);
  }
  if (valtotal < 509){
        digitalWrite(relay2, HIGH);
  }else{
     digitalWrite(relay2, LOW);
  }
  if (value >259){
    digitalWrite(relay3, HIGH);
  }else{
    (value <120);
     digitalWrite(relay3, LOW);
     
  }
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
