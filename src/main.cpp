#include <Arduino.h>
#include <server.h>
#include <send_mail.h>
#include <setup_wifi.h>
#include <init_sd.h>

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.print("Connecting to AP");
  setup_wifi(Serial);

  // Serial.println("Mounting SD Card...");
  init_sd(Serial);

  Serial.println("Initializing server...");
  init_server();

  // send_mail(Serial);

}

void loop()
{
}
