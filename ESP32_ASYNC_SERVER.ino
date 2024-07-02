/*
  ESP32 Web Server - v2.0.0
  Update to the latest version here:
  https://github.com/danilppzz/ESP32-Web-Server-Controller
*/

#include "WiFi.h"
#include "ESPAsyncWebServer.h"

const char* ssid = "ESP32-SERVER";
const char* password = "2ed53fa87b";

#define LED_PIN 32
bool ledState = false;

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    body {
      display: flex;
    }
  </style>
</head>
<body>
  <button id="toggle" onclick="sendRequest('/ledtoggle')">Toggle</button>
  <h1>LED STATUS: </h1>
  <p id="led">%LED_STATUS%</p>
</body>
<script>
setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("led").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/led", true);
  xhttp.send();
}, 5000);

function sendRequest(state) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById('toggle').innerHTML = this.responseText;
    }
  };
  xhttp.open('GET', state, true);
  xhttp.send();
}
</script>
</html>)rawliteral";

String readLedStatus() {
  return ledState ? "ON" : "OFF";
}

String processor(const String& var) {
  if (var == "LED_STATUS") {
    return readLedStatus();
  }
  return String();
}

void ledToggle(AsyncWebServerRequest *request) {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  request->send(200, "text/plain", readLedStatus().c_str());
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/led", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", readLedStatus().c_str());
  });
  server.on("/ledtoggle", HTTP_GET, ledToggle);

  server.begin();
}

void loop() {
}
