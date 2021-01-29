#include <send_mail.h>
#include "ESP32_MailClient.h"

void send_mail(String objet, String email, String content) {

  //The Email Sending data object contains config and data to send
  SMTPData smtpData;

  //Callback function to get the Email sending status
  void sendCallback(SendStatus info);
  
  Serial.println("Sending email...");

  //Set the Email host, port, account and password
  smtpData.setLogin("smtp.gmail.com", 465, "shrek.famille@gmail.com", "projetBigSanchos62");

  //For library version 1.2.0 and later which STARTTLS protocol was supported,the STARTTLS will be 
  //enabled automatically when port 587 was used, or enable it manually using setSTARTTLS function.
  //smtpData.setSTARTTLS(true);

  //Set the sender name and Email
  smtpData.setSender("ESP32", "shrek.famille@gmail.com");

  //Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");

  //Set the subject
  smtpData.setSubject(objet);

  //Set the message - normal text or html format
  smtpData.setMessage(content, true);

  //Add recipients, can add more than one recipient
  smtpData.addRecipient(email);
  // smtpData.addRecipient("boury.samuel82@gmail.com");
  // smtpData.addRecipient("elianegreteau@gmail.com");
  // smtpData.addRecipient("amnayk@gmail.com");



  //Add attachments, can add the file or binary data from flash memory, file in SD card
  //Data from internal memory
  // smtpData.addAttachData("firebase_logo.png", "image/png", (uint8_t *)dummyImageData, sizeof dummyImageData);

  //Add attach files from SD card
  //Comment these two lines, if no SD card connected
  //Two files that previousely created.
  // smtpData.addAttachFile("/photo.jpg");
  // smtpData.addAttachFile("/text_file.txt");


  //Add some custom header to message
  //See https://tools.ietf.org/html/rfc822
  //These header fields can be read from raw or source of message when it received)
  smtpData.addCustomMessageHeader("Date: Sat, 10 Aug 2019 21:39:56 -0700 (PDT)");
  //Be careful when set Message-ID, it should be unique, otherwise message will not store
  //smtpData.addCustomMessageHeader("Message-ID: <abcde.fghij@gmail.com>");

  //Set the storage types to read the attach files (SD is default)
  //smtpData.setFileStorageType(MailClientStorageType::SPIFFS);
  // smtpData.setFileStorageType(MailClientStorageType::SD);


  smtpData.setSendCallback(sendCallback);

  //Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

  //Clear all data from Email object to free memory
  smtpData.empty();

}

//Callback function to get the Email sending status
void sendCallback(SendStatus msg)
{
  //Print the current status
  Serial.println(msg.info());

  //Do something when complete
  if (msg.success())
  {
    Serial.println("----------------");
  }
}