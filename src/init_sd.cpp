#include <HardwareSerial.h>
#include "ESP32_MailClient.h"
#include "SD.h"


void init_sd(HardwareSerial Serial) {
  if (MailClient.sdBegin(14,2,15,13)) // MailClient.sdBegin(14,2,15,13) for TTGO T8 v1.7 or 1.8
  {

    Serial.println("Preparing attach file...");

    File file = SD.open("/text_file.txt", FILE_WRITE);
    file.print("Hello World!\r\nHello World!");
    file.close();

    file = SD.open("/binary_file.dat", FILE_WRITE);

    static uint8_t buf[512];

    buf[0] = 'H';
    buf[1] = 'E';
    buf[2] = 'A';
    buf[3] = 'D';
    file.write(buf, 4);

    size_t i;
    memset(buf, 0xff, 512);
    for (i = 0; i < 2048; i++)
    {
      file.write(buf, 512);
    }

    buf[0] = 'T';
    buf[1] = 'A';
    buf[2] = 'I';
    buf[3] = 'L';
    file.write(buf, 4);

    file.close();
  }
  else
  {
    Serial.println("SD Card Monting Failed");
  }

}
