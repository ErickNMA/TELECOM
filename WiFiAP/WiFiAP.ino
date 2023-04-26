#include <WiFi.h>
 
/* Set to preferred SSID and password */
const char *ssid = "ESP32";
const char *password = "password";
 
int ledPin = 2; // GPIO2 of ESP32 (built-in LED)
WiFiServer server(80); // server port 80
 
void setup() {
  Serial.begin(115200);
  
  Serial.println();
  Serial.println("Starting ESP32 Access Point...");

  IPAddress localIP(192,168,1,1);
  IPAddress gateway(192,168,1,0);
  IPAddress subnet(255,255,255,0);

  WiFi.softAPConfig(localIP, gateway, subnet);
  //WiFi.softAP(SSID, Password);
  
  //Serial.println(WiFi.softAPIP());
  
  WiFi.softAP(ssid, password);
 
  IPAddress espIP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(espIP);
  
  server.begin();
  Serial.println("ESP32 Web Server ready...");
  Serial.println();
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}
 
 
void loop() {
  
  // Check if a client has connected
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  /*****************************************************
  NOTE: This snippet from library examples does not work
  
  // Wait until the client sends some data
  
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  *****************************************************/
 
  // Read the first line of GET containing request command
  
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Check command string and perform request
 
  int value = LOW;
  
  if (request.indexOf("/gpio/0") != -1){
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
  
  if (request.indexOf("/gpio/1") != -1) {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  } 
  
  // Server Response
 
  // Header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); // header end marker
 
  // Content
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("LED STATUS: ");
 
  if(value == HIGH)
    client.print("ON");   
  else 
    client.print("OFF");
 
  client.println("<br><br>");
  client.println("<a href=\"/gpio/1\">TURN ON LED</a><br>");
  client.println("<a href=\"/gpio/0\">TURN OFF LED</a><br>");
  client.println("</html>"); // content end
  // End of Response
 
 
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}
