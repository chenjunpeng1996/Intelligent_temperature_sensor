/*
 * @Author: your name
 * @Date: 2021-03-16 18:38:11
 * @LastEditTime: 2021-03-18 12:43:14
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /Temp/src/main.cpp
 */
#include <Arduino.h>
#include <OLED.h>
#include <SimpleTimer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <string.h>
#include <OneWire.h>
#include <EEPROM.h>

WiFiServer server(80);
SimpleTimer timer;
OLED led;
OneWire oneWire_14(14);

DallasTemperature sensors_14(&oneWire_14);
DeviceAddress insideThermometer;
char* ssid     = "ssidssidssidssidssid";
char* password = "passpasspasspasspass";

int SSRI,Power;
bool Bty=0;
float Temp=0.0;
char* host = "255.255.255.255";
uint16_t port = 1010;
WiFiClient client;
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
String SerialStr = "";
String string[2];
uint16_t ShangXian=70;
uint16_t XiangXian=10;
String TCP_Data="";
void serialEvent()
{
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
String JieShou_Serial()
{
  String Str = "";
  if (stringComplete) {
    //Serial.println(inputString);
    Str = inputString;
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  return Str;
}

String* QieGe_ZiFuChuan(String str, String* str3)
{
  String str1 = "";
  String str2 = "";
  for (int a = 0; a < str.length(); a++)
  {
    if (a < 5)
    {
      str1 += str[a];
    } else {
      str2 += str[a];
    }

  }
  str3[0] = str1;
  str3[1] = str2;

  return str3;
}
char* string_char(String str)
{
  char* ch = "12,34";
  for (int i = 0; i < str.length();i++)
  {
    ch[i] = str[i];
    //Serial.print(ch[i]);
  }
  //Serial.print(ch);
  return ch;
}

void Timer1()
{
  sensors_14.requestTemperatures();
  if(Bty==0)
    Temp=sensors_14.getTempC(insideThermometer);
  else
    Temp=sensors_14.getTempF(insideThermometer);
}
void Timer2()
{
  led.LED_Print(3,Temp,Bty,2);
}
void TCP_LianJie()
{
  Serial.print("连接到tcp/ip ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // 使用WiFiClient类创建TCP连接
  if (!client.connect(host, port)) {
    Serial.println("连接失败");
    delay(3000);
    return;
  }
}
void WiFi_LianJie()
{
  // 我们从连接WiFi网络开始

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /*显式设置ESP8266为WiFi客户端，否则默认为，
    将尝试同时充当客户端和访问点，并可能导致
    WiFi网络上的其他WiFi设备出现网络问题。*/
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //开始链接Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("已连接WiFi");
  Serial.println("IP 地址: ");
  Serial.println(WiFi.localIP());
}
void TCP_Print(String str)
{
  //这将向服务器发送一个字符串
  //Serial.print("向服务器发送数据:");
  //Serial.println(str);
  if (client.connected()) {
    client.println(str);
  }
}
void TCP_Timeout()
{
  // 等待数据可用
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(6000);
      return;
    }
  }
}
String TCP_Read()
{
  // 阅读服务器的所有回复行
  String Read_Data = "";
  //Serial.print("从远程服务器接收:");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Read_Data = Read_Data + ch;
    Serial.print(ch);
  }
  return Read_Data;
}
void Scan_Wifi()
{
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
}

String PanDuanBaoJing(float t)
{
  if(t>ShangXian)
    return "S";
  else if(t<XiangXian)
    return "X";
  else
    return "Z";
}
void setup()
{
  led.LED_Init();
  sensors_14.getAddress(insideThermometer, 0);
  sensors_14.setResolution(insideThermometer, 9);
  timer.setInterval(500L, Timer1);
  timer.setInterval(1000L, Timer2);
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect();
  Serial.println("Connected.");
  server.begin();
  inputString.reserve(200);
  EEPROM.begin(10);
  ShangXian=EEPROM.read(3);
  XiangXian=EEPROM.read(4);
}
void loop()
{
  if (!client.connected())
    {
      delay(5000);
      TCP_LianJie();
    }else{
      char*  c="22.34";
      dtostrf(Temp,2,2,c);
      String str=String(c);
      str=str+"_"+PanDuanBaoJing(Temp);
      TCP_Print(str);
      TCP_Data=TCP_Read();
    }
  serialEvent();
  SerialStr = "";
  SerialStr = JieShou_Serial();
  if(TCP_Data!="")
  {
    String Name = String(TCP_Data).substring(0,5);
    String NeiRong = String(TCP_Data).substring(5,(String(TCP_Data).length() - 1));
    if (Name == "szsx:")
      {

        ShangXian = NeiRong.toInt();
        Serial.print("Set ShangXian to:");
        Serial.println(ShangXian);
      }else if (Name == "szxx:")
      {

        XiangXian = NeiRong.toInt();
        Serial.print("Set XiangXian to:");
        Serial.println(XiangXian);
      }
    TCP_Data="";  
  }
  if (SerialStr != "")
  {
    if (SerialStr == "扫描wifi\n")
    {
      Scan_Wifi();
    } else if (SerialStr == "连接wifi\n")
    {
      Serial.println(ssid);
      Serial.println(password);
      WiFi_LianJie();
    } else if (SerialStr == "连接tcp\n")
    {
      TCP_LianJie();
    } else {
//      Serial.println(SerialStr);
      String Name = String(SerialStr).substring(0,5);
      String NeiRong = String(SerialStr).substring(5,(String(SerialStr).length() - 1));
//      Serial.print("Name=");
//      Serial.println(Name);
//      Serial.print("NeiRong=");
//      Serial.println(NeiRong);
      if (Name == "ssid:")
      {
        strcpy(ssid,NeiRong.c_str());
        Serial.print("Set SSID to:");
        Serial.println(ssid);
      } else if (Name == "pass:")
      {
        strcpy(password,NeiRong.c_str());
        Serial.print("Set pass to:");
        Serial.println(password);
      } else if (Name == "host:")
      {
        strcpy(host,NeiRong.c_str());
        Serial.print("Set Host to:");
      } else if (Name == "port:")
      {

        port = NeiRong.toInt();
        Serial.print("Set Port to:");
        Serial.println(port);
      }else if (Name == "szsx:")
      {

        ShangXian = NeiRong.toInt();
        Serial.print("Set ShangXian to:");
        Serial.println(ShangXian);
        EEPROM.write(3, ShangXian);
      }else if (Name == "szxx:")
      {

        XiangXian = NeiRong.toInt();
        Serial.print("Set XiangXian to:");
        Serial.println(XiangXian);
        EEPROM.write(4, XiangXian);
      }
    }
  }
  timer.run();
  
}