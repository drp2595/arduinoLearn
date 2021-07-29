//MAY BE YOU NEED TO ADD FILES/FOLDERS TO RUN THIS CODE PROPERLY.THIS CODE IS FOR REFERENCES ONLY.
//I USED WROOM32- ESP32 DEV KIT.
#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>

#define WIFI_SSID "Workstation 2"
#define WIFI_PASSWORD "Ttpl@UX-zDkaZ!@#Workstation2()"

#define SMTP_HOST "smtp.gmail.com"

#define SMTP_PORT 587

#define AUTHOR_EMAIL "dhruv.pandya@thinkitive.com"
#define AUTHOR_PASSWORD "qzjcfapdmoqxxogl"

const char* ssid = "workstation3"; //Provide your SSID
const char* password = "Ttpl@y2x!CVJS!@#Workstation3()"; // Provide Password

SMTPSession smtp;

void smtpCallback(SMTP_Status status);

void setup()
{

  Serial.begin(115200);

#if defined(ARDUINO_ARCH_SAMD)
  while (!Serial)
    ;
  Serial.println();
  Serial.println("**** Custom built WiFiNINA firmware need to be installed.****\nTo install firmware, read the instruction here, https://github.com/mobizt/ESP-Mail-Client#install-custom-built-wifinina-firmware");

#endif

  Serial.println();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  smtp.debug(1);
  
  smtp.callback(smtpCallback);

  ESP_Mail_Session session;
  
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "mydomain.net";

  SMTP_Message message;

  message.sender.name = "ESP Mail";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Test sending html Email";
  message.addRecipient("Admin", "pandya2368@yahoo.in");

  String htmlMsg = "<p>This is the <span style=\"color:#ff0000;\">html text</span> message.</p><p>The message was sent via ESP device.</p>";
  message.html.content = htmlMsg.c_str();
  message.html.charSet = "us-ascii";

  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  if (!smtp.connect(&session))
    return;
    
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());

  ESP_MAIL_PRINTF("Free Heap: %d\n", MailClient.getFreeHeap());
}

void loop()
{
}

void smtpCallback(SMTP_Status status)
{

  Serial.println(status.info());

  if (status.success())
  {
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++)
    {
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients);
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject);
    }
    Serial.println("----------------\n");
  }
}
