//
// A simple server implementation showing how to:
//  * serve static messages
//  * read GET and POST parameters
//  * handle missing pages / 404s
//

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <server.h>
#include <send_mail.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>ESP Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <link rel="icon" href="data:," />
    <style>
      html {
        font-family: Arial;
        display: inline-block;
        text-align: center;
      }
      h2 {
        font-size: 3rem;
      }
      p {
        font-size: 3rem;
      }
      body {
        max-width: 600px;
        margin: 0px auto;
        padding-bottom: 25px;
      }
      .switch {
        position: relative;
        display: inline-block;
        width: 120px;
        height: 68px;
      }
      .switch input {
        display: none;
      }
      .slider {
        position: absolute;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #ccc;
        border-radius: 6px;
      }
      .slider:before {
        position: absolute;
        content: "";
        height: 52px;
        width: 52px;
        left: 8px;
        bottom: 8px;
        background-color: #fff;
        -webkit-transition: 0.4s;
        transition: 0.4s;
        border-radius: 3px;
      }
      input:checked + .slider {
        background-color: #b30000;
      }
      input:checked + .slider:before {
        -webkit-transform: translateX(52px);
        -ms-transform: translateX(52px);
        transform: translateX(52px);
      }
    </style>
  </head>
  <body>
    <h2>ESP Web Server</h2>
    <h4>Send Mail</h4>
    <img src="http://irt.enseeiht.fr/beylot/photo/photo.jpg">
    <div id="container">
      <form id="signup">
        <h1>Send an Email</h1>
        <small></small>
        <input type="text" placeholder="Email Address" />
        <small></small>
        <input type="text" placeholder="Object" />
        <small></small>
        <input type="text" placeholder="Content" />
        <input type="submit" value="Send" />
      </form>
    </div>
    ;
    <script>
      var xhr = new XMLHttpRequest();
      const form = document.getElementById("signup");
      form.addEventListener("submit", (event) => {
        // check required fields
        email = event.target[0].value;
        object = event.target[1].value;
        content = event.target[2].value;
        console.log(email,object,content);
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
        event.preventDefault();
      });
    </script>
  </body>
</html>
)rawliteral";

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void init_server() {

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

    server.begin();
}