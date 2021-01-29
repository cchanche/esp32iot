#include <Arduino.h>
#include <SD.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <send_mail.h>
#include <LoRa.h>
#include <ds3231.h>
#include <SPI.h>

OLED_CLASS_OBJ display(OLED_ADDRESS, OLED_SDA, OLED_SCL);

#define WIFI_SSID       "OnePlusClem"
#define WIFI_PASSWORD   "commedab"

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>BME680 Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <link
      rel="stylesheet"
      href="https://use.fontawesome.com/releases/v5.7.2/css/all.css"
      integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr"
      crossorigin="anonymous"
    />
    <link rel="icon" href="data:," />
    <style>
      html {
        font-family: Arial;
        display: inline-block;
        text-align: center;
      }
      p {
        font-size: 1.2rem;
      }
      body {
        margin: 0;
      }
      .topnav {
        overflow: hidden;
        background-color: #4b1d3f;
        color: white;
        font-size: 1.7rem;
      }
      .content {
        padding: 20px;
      }
      .card {
        background-color: white;
        box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, 0.5);
      }
      .cards {
        max-width: 1000px;
        height: 300px;
        margin: 0 auto;
        display: grid;
        grid-gap: 2rem;
        grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
      }
      .reading {
        font-size: 2.8rem;
      }
      .card.temperature {
        color: #0e7c7b;
      }
      .card.mail {
        color: #d62246;
      }
      .card.image {
        color: #40d622;
      }
      .card.serial {
        color: #000000;
      }
    </style>
  </head>
  <body>
    <div class="topnav">
      <h3>WEB SERVER</h3>
      <span id="message">fetching LoRa status...</span>
    </div>
    <div class="content">
      <div class="cards">
        <div class="card temperature">
          <h4><i class="fas fa-commenting"></i>Lora Messages</h4>
          <p>
            <span class="reading">
                <span id="sensor_RSSI">Fetching Sensor Signal</span>
                <span id="sensor">Fetching Sensor Data</span>
            </span>
          </p>
        </div>
        <div class="card mail">
          <h4><i class="fas fa-mail"></i>Send a mail</h4>
          <div id="container">
            <form id="signup">
              <small></small>
              <input type="text" placeholder="Email Address" />
              <small></small>
              <input type="text" placeholder="Object" />
              <small></small>
              <input type="text" placeholder="Content" />
              <input type="submit" value="Send" />
            </form>
          </div>
        </div>
        <div class="card image">
            <h4>A nice image</h4>
            <img src="http://irt.enseeiht.fr/beylot/photo/photo.jpg" />
        </div>
    </div>
    <script>
      if (!!window.EventSource) {
        var source = new EventSource("/events");

        source.addEventListener(
          "open",
          function (e) {
            console.log("Events Connected");
          },
          false
        );
        source.addEventListener(
          "error",
          function (e) {
            if (e.target.readyState != EventSource.OPEN) {
              console.log("Events Disconnected");
            }
          },
          false
        );

        source.addEventListener(
          "message",
          function (e) {
            console.log("message", e.data);
            document.getElementById("message").innerHTML = e.data;
          },
          false
        );

        source.addEventListener(
          "sensor",
          function (e) {
            console.log("sensor", e.data);
            document.getElementById("sensor").innerHTML = e.data;
          },
          false
        );

        source.addEventListener(
          "sensor_RSSI",
          function (e) {
            console.log("sensor_RSSI", e.data);
            document.getElementById("sensor_RSSI").innerHTML = e.data;
          },
          false
        );
      }
      var xhr = new XMLHttpRequest();
      const form = document.getElementById("signup");
      form.addEventListener("submit", (event) => {
        // check required fields
        email = event.target[0].value;
        object = event.target[1].value;
        content = event.target[2].value;
        console.log(email, object, content);
        xhr.open(
          "GET",
          "/update?email=" +
            email +
            "&object=" +
            object +
            "&content=" +
            content,
          true
        );
        xhr.send();
      })
    </script>
  </body>
</html>
)rawliteral";

AsyncWebServer server(80);

AsyncEventSource events("/events");

String serial_print;

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.println("Starting Lora...");
  if (OLED_RST > 0) {
      pinMode(OLED_RST, OUTPUT);
      digitalWrite(OLED_RST, HIGH);
      delay(100);
      digitalWrite(OLED_RST, LOW);
      delay(100);
      digitalWrite(OLED_RST, HIGH);
  }

  display.init();
  display.flipScreenVertically();
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(display.getWidth() / 2, display.getHeight() / 2, LORA_SENDER ? "LoRa Sender" : "LoRa Receiver");
  display.display();
  delay(2000);

  String info = ds3231_test();
  if (info != "") {
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(display.getWidth() / 2, display.getHeight() / 2, info);
    display.display();
    delay(2000);
  }

  #if !LORA_SENDER
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      display.clear();
      Serial.println("WiFi Connect Fail");
      display.drawString(display.getWidth() / 2, display.getHeight() / 2, "WiFi Connect Fail");
      display.display();
      delay(2000);
      // esp_restart();
    }

    Serial.print("Connected : ");
    Serial.println(WiFi.SSID());
    Serial.print("IP:");
    Serial.println(WiFi.localIP().toString());
    display.clear();
    display.drawString(display.getWidth() / 2, display.getHeight() / 2, "IP:" + WiFi.localIP().toString());
    display.display();
    delay(2000);
  #endif

  SPI.begin(CONFIG_CLK, CONFIG_MISO, CONFIG_MOSI, CONFIG_NSS);
  LoRa.setPins(CONFIG_NSS, CONFIG_RST, CONFIG_DIO0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  if (!LORA_SENDER) {
    display.clear();
    display.drawString(display.getWidth() / 2, display.getHeight() / 2, "LoraRecv Ready");
    display.display();
  }
  Serial.println("LoRa OK !");

  #if !LORA_SENDER
    Serial.println("Starting server...");
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });

    // Send a GET request to <IP>/get?message=<message>
    server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
      String email;
      String object;
      String content;
      email = request->getParam("email")->value();
      object = request->getParam("object")->value();
      content = request->getParam("content")->value();
      send_mail(object, email, content);
      request->send_P(200, "text/html", index_html);
    });

    server.onNotFound(notFound);

    // Handle Web Server Events
    events.onConnect([](AsyncEventSourceClient *client){
      if(client->lastId()){
        Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
      }
      // send event with message "hello!", id current millis
      // and set reconnect delay to 1 second
      client->send("LoRa running !", NULL, millis(), 10000);
    });

    server.addHandler(&events);

    server.begin();
    Serial.println("Server OK !");
  #endif

  Serial.println("Begin lora loop...");
}

void loop()
{
  #if LORA_SENDER
    // int32_t rssi;
    // if (WiFi.status() == WL_CONNECTED) {
    //     rssi = WiFi.RSSI();
    //     display.clear();
    //     display.setTextAlignment(TEXT_ALIGN_CENTER);
    //     display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Send RSSI:" + String(rssi));
    //     display.display();
    //     LoRa.beginPacket();
    //     LoRa.print("WiFi RSSI: ");
    //     LoRa.print(rssi);
    //     LoRa.endPacket();
    // } else {
    //     Serial.println("WiFi Connect lost ...");
    //     display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Wifi Lost");
    // }
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Send message...");
    display.display();
    LoRa.beginPacket();
    LoRa.print("Custom sensor data...");
    LoRa.endPacket();
    delay(2500);
  #else
      if (LoRa.parsePacket()) {
          String recv = "";
          while (LoRa.available()) {
              recv += (char)LoRa.read();
          }
          display.clear();
          display.drawString(display.getWidth() / 2, display.getHeight() / 2, recv);
          String info = "RSSI = " + String(LoRa.packetRssi());
          display.drawString(display.getWidth() / 2, display.getHeight() / 2 - 16, info);
          display.display();
          events.send(info.c_str(),"sensor_RSSI",millis());
          events.send(recv.c_str(),"sensor",millis());
      }
  #endif
}
