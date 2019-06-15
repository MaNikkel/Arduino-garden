#include "DHT.h"
#include <math.h>
#include <SPI.h>
#include <SD.h>

#define DHTTYPE DHT11
#define LOG_INTERVAL 600000

#define rele 7
#define meninao A1

#define sensorSolo A2
#define DHTPIN A3

int valorSolo, val;
float u, t, temp;

unsigned long lastTime;

File myFile;
DHT dht(DHTPIN, DHTTYPE);

void lerDados()
{
  u = dht.readHumidity();   //umidade do ar
  delay(100);
  t = dht.readTemperature();//temperatura do ar
  delay(100);
  val = analogRead(meninao);
  temp = (0.0000000014*pow(val,4)) + (0.0000015*pow(val,3)) - (0.00066*pow(val,2)) + (0.28*val) - 14;
          //temperatura do meninao
}

void gravarDados()
{
  myFile = SD.open("tempo.txt", FILE_WRITE);
  if(myFile)
  {
    myFile.print(millis());
    myFile.println(" ,");
    myFile.close();
  }
  myFile = SD.open("umidade.txt", FILE_WRITE);
  if(myFile)
  {
    myFile.print(u);
    myFile.println(" ,");
    myFile.close();
  }
  myFile = SD.open("ar.txt", FILE_WRITE);
  if(myFile)
  {
    myFile.print(t);
    myFile.println(" ,");
    myFile.close();
  }
  myFile = SD.open("meninao.txt", FILE_WRITE);
  if(myFile)
  {
    myFile.print(temp);
    myFile.println(" ,");
    myFile.close();
  }
  myFile = SD.open("solo.txt", FILE_WRITE);
  if(myFile)
  {
    myFile.print(valorSolo);
    myFile.println(" ,");
    myFile.close();
  }
}

void setup() 
{
 //Serial.begin(9600);
 pinMode(rele, OUTPUT);
 pinMode(sensorSolo, INPUT);
 digitalWrite(rele, LOW);
 dht.begin();
 if (!SD.begin(4)) 
 {
    //Serial.println("initialization failed!");
    while (1);
 }
 lastTime = millis();
}

void loop() 
{
  
  if((millis()- lastTime) >= LOG_INTERVAL)
  {
    lastTime = millis();
    lerDados();
    gravarDados();
  }
  valorSolo = analogRead(sensorSolo);
  if(valorSolo > 760)
  {
    digitalWrite(rele, HIGH);
    delay(10000);
  }
  else 
    digitalWrite(rele, LOW);

}
