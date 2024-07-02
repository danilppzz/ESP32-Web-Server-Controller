/*
  ESP32 Web Server - v1.0.0 
  Update to the latest version here:
  https://github.com/danilppzz/ESP32-Web-Server-Controller
*/

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32-SERVER";
const char* password = "2ed53fa87b";

WebServer server(80);

bool ledState = false;
const int ledPin = 32;

void handleRoot() {
  String html = "<html>\
  <head>\
    <title>ESP32 Web Server</title>\
    <style>\
      body { font-family: Arial; text-align: center; }\
      button { padding: 20px; font-size: 20px; margin: 5px; }\
    </style>\
    <script>\
      function sendRequest(state) {\
        var xhttp = new XMLHttpRequest();\
        xhttp.onreadystatechange = function() {\
          if (this.readyState == 4 && this.status == 200) {\
            document.getElementById('ledState').innerHTML = this.responseText;\
          }\
        };\
        xhttp.open('GET', state, true);\
        xhttp.send();\
      }\
    </script>\
  </head>\
  <body>\
    <h1>ESP32 Web Server</h1>\
    <button onclick=\"sendRequest('/on')\">Encender LED</button>\
    <button onclick=\"sendRequest('/off')\">Apagar LED</button>\
    <p>Estado del LED: <span id='ledState'>" + String(ledState ? "Encendido" : "Apagado") + "</span></p>\
  </body>\
  </html>";
  
  server.send(200, "text/html", html);
}

void handleOn() {
  ledState = true;
  digitalWrite(ledPin, HIGH);
  server.send(200, "text/plain", "Encendido");
}

void handleOff() {
  ledState = false;
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "Apagado");
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(115200);
  
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi conectado.");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
  
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  
  server.begin();
}

void loop() {
  server.handleClient();
}
