//Library to Send Commands to the Module ESP8266
#include "SoftwareSerial.h"
//Library to Read The Sensor data
#include "DHT.h"

//Define The pin of the sensor
#define DHTPIN 8
//Define the model of the sensor
#define DHTTYPE DHT22

//SSID and Password to wifi connection
String ssid = "SSID";
String password = "PASSWORD";

//server will be receive the data example.com
String server = "example.com";

//url to send the data in server/main.js POST url
String uri = "/";
// Data to send to the server
String data;

//Variables to store the Temperature and Humidity 
float TempRead;
float HumRead;

//Set the function to read the data of the sensor
DHT dht(DHTPIN, DHTTYPE);

//Set RX / TX of the module ESP8266
SoftwareSerial esp(2, 3);

void setup(){
  esp.begin(115200);
  dht.begin();
  
  Serial.begin(115200);

  reset();
  connectWifi();
}

//Reset Module ESP8266
void reset(){
  esp.println("AT+CWMODE=1");
  esp.println("AT+RST");
  delay(1000);

  if (esp.find("OK"))
    Serial.println("Module Reset");
}

//Connect to WIFI network
void connectWifi(){

  String cmd = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\"";
  esp.println(cmd);
  delay(4000);

  if (esp.find("OK")){
    Serial.println("Connected!");
  }
  else{
    connectWifi();
    Serial.println("Cannot connect to wifi");
  }
}

void loop(){
  TempRead = dht.readTemperature();
  HumRead = dht.readHumidity();

  // The data will be sent like the String postRequest inside httppost() method
  data = "temperature=" + String(TempRead) + "&humidity=" + String(HumRead); 

  httppost();
  delay(10000);
}

void httppost(){
  esp.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80"); //start a TCP connection.

  if (esp.find("OK")){
    Serial.println("TCP connection ready");
  }
  delay(1000);
  String postRequest =
      "POST " + uri + " HTTP/1.1\r\n" +
      "Host: " + server + "\r\n" +
      "Accept: *" + "/" + "*\r\n" +
      "Content-Length: " + data.length() + "\r\n" +
      "Cache-Control: no-cache\r\n" +
      "Content-Type: application/x-www-form-urlencoded\r\n" +
      "\r\n" + data;

  //Set number of caracters that will be sent
  String sendCmd = "AT+CIPSEND=";

  esp.print(sendCmd);
  esp.println(postRequest.length());
  delay(500);

  if (esp.find(">")){
    Serial.println("Sending..");
    esp.print(postRequest);

    if (esp.find("SEND OK")){
      Serial.println("Packet sent");

      while (esp.available()){
        String tmpResp = esp.readString();
        Serial.println(tmpResp);
        delay(5000);
      }
      // Close Connection
      esp.println("AT+CIPCLOSE");
    }
  }
}