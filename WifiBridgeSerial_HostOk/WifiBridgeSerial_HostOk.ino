#include <WiFi.h>
#include "config.h"

#define STATIC_IP_ADDR

int networkport = 23;

const char *ssid = "ESP32";
const char *password = "password";

IPAddress host(192,168,1,2);

WiFiServer localServer(networkport);
//WiFiServer server(80); // server port 80
WiFiClient localClient;

#ifdef STATIC_IP_ADDR
  //IPAddress localIP(192,168,1,1);
  //IPAddress gateway(192,168,1,0);
  //IPAddress subnet(255,255,255,0);
  IPAddress staticIP(192,168,1,1);
  IPAddress gateway(192,168,1,0);
  IPAddress subnet(255,255,255,0);
#endif

void setup() {
  Serial.begin(115200);
  //WiFi.begin(ssid, password);

  WiFi.softAPConfig(staticIP, gateway, subnet);

//#ifdef STATIC_IP_ADDR
//  WiFi.config(staticIP, gateway, subnet);
//#endif
//  Serial.print("\nConnecting to "); Serial.println(ssid);
  WiFi.softAP(ssid, password);
 
  IPAddress espIP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(espIP);
  
  //server.begin();

  
  //uint8_t i = 0;
  //while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  //if(i == 21){
  //  Serial.print("Could not connect to "); Serial.println(ssid);
  //  while(1) delay(500);
 // }
  
  //start UART and the server
  localServer.begin();
  localServer.setNoDelay(true);

  Serial.print("Ready! Use 'Uart-WiFi Bridge ");
  Serial.print(WiFi.localIP());
  Serial.println(" to connect");

}

void loop() {
  
  //check if there are any new clients
  if (localServer.hasClient()){
    if (!localClient.connected()){
      if(localClient) localClient.stop();
      localClient = localServer.available();
    }
  }
      
  //check a client for data
  if (localClient && localClient.connected()){
    if(localClient.available()){
      size_t len = localClient.available();
      uint8_t sbuf[len];
      localClient.readBytes(sbuf, len);
      Serial.write(sbuf, len);      
    }
  }

  //check UART for data
 // if(Serial.available()){
   // size_t len = Serial.available();
  //  uint8_t sbuf[len];
  //  Serial.readBytes(sbuf, len);
  //  if (localClient && localClient.connected()){
  //    localClient.write(sbuf, len);
  //  }
 // }

 //check UART for data

  if(Serial.available()){
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    
    //if (localClient && localClient.connected()){
    //  localClient.write(sbuf, len);
    //} else
    //{
      // Use WiFiClient class to create TCP connections
      WiFiClient client;

      if (!client.connect(host, networkport)) {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
        return;
      }
      client.write(sbuf,len);
      client.stop();
     Serial.write( sbuf , len );
    //}
  }
  
}
