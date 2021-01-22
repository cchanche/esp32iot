#include <Arduino.h>
#include <server.h>
#include <send_mail.h>
#include <setup_wifi.h>
#include <init_sd.h>
#include <lora_cpp.h>

void setup()
{
  Serial.begin(115200);
  Serial.println();

  // Serial.print("Connecting to AP");
  // setup_wifi(Serial);

  // // Serial.println("Mounting SD Card...");
  // init_sd(Serial);

  Serial.println("Initializing lora...");
  lora_setup();
  Serial.println("Lora OK");

  Serial.println("Initializing server...");
  init_server();
  Serial.println("Server OK");


  // send_mail(Serial);

  Serial.println("Begin lora loop...");
}

void loop()
{
  lora_loop();
}
