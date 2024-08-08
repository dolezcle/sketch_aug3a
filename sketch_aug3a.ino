#include <WiFi.h>
#include <Preferences.h>
#include <ESP_Mail_Client.h>
#include "gen_funcs.h"
#include "cl_mainhtml.h"
#include "cl_conman.h"
#include "cl_mail.h"

cl_conman lo_conman;
int S1A = 0;
String gs_content;
WiFiServer pub_server(80);
WiFiClient pub_client; 
String gx_header;
const int S1U = 18;

void setup()
{

  Serial.begin(115200);
  pinMode(S1U, INPUT_PULLDOWN);
  // pinMode(15, INPUT_PULLUP);
  // pinMode(15, INPUT);
  Serial.println("SETUP");
  delay(1000);
  Serial.print(__FILE__);
  Serial.print("-----debugging function: ");
  Serial.println(__FUNCTION__);
  lo_conman.m_startconn();
  lo_conman.m_save_creds();
  lo_conman.m_read_creds();
  cl_mail lo_mail(lo_conman.cgst_senderemail, lo_conman.cgst_sender_pw, lo_conman.cgst_receivermail, lo_conman.cgst_sender_smtp_host, lo_conman.cgst_SSID, 587);
  lo_mail.m_initialize();
  lo_mail.m_send_mail();
}

void loop()
{
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  Serial.println("LOOP");
  gs_content = "";
  cl_mail lo_mail(lo_conman.cgst_senderemail, lo_conman.cgst_sender_pw, lo_conman.cgst_receivermail, lo_conman.cgst_sender_smtp_host, lo_conman.cgst_SSID, 587);
  delay(500);
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  		if (!pub_server)
		{
			pub_server.begin();
		}
  pub_client = pub_server.available();
  while (pub_client.connected())
	{
		Serial.print("im pub_client.connected ");
		// delay(1000);
		//		Serial.println(WiFi.localIP());
		gx_header = pub_client.readStringUntil('\n');
		Serial.print("gx_header: ");
		Serial.println(gx_header);

    if (gx_header.indexOf("GET /?on") > -1)
    {

    }
//    		mainhtml1(pub_client);

		pub_client.stop();
  }
pinMode(15, INPUT_PULLDOWN);
  int S1 = digitalRead(S1U);        //0 - aktiv, 1 - inaktiv 
  // int S1 = touchRead(S1U);       //frei = 86, beruehrt <50
  // int A1 = analogRead(S1U);

  if (S1 == 0 && S1A == 1)
  //if (S1 > 50 && S1A < 50)
  {
    Serial.print("S1/S1A im if-Zweig   :");
    Serial.println(S1); // 0=nicht ausgeloest, 1=ausgeloest
    Serial.println(S1A);
    // Serial.println(A1);
    delay(500);
    {
      lo_mail.m_initialize();
      lo_mail.m_setsubject("Bewegung inaktiv");
      lo_mail.m_setcontent("Melder 1");
      lo_mail.m_send_mail();
      S1A = S1;
      delay(500);
    }
  }
  if (S1 == 1 && S1A == 0)
  //if (S1 < 50 && S1A > 50)
  {
    Serial.print("S1/S1A im elseif-Zweig   :");
    Serial.println(S1); // 0=nicht ausgeloest, 1=ausgeloest
    Serial.println(S1A);
    delay(500);
    gs_content.concat("Digital: ");
    gs_content.concat(S1);
    // gs_content.concat("Analog: ");
    // gs_content.concat(A1);
    lo_mail.m_initialize();
    lo_mail.m_setsubject("Bewegung erkannt");
    lo_mail.m_setcontent(gs_content);
    lo_mail.m_send_mail();
    S1A = S1;
    delay(10000);
  }

  Serial.print("S1/S1A am Ende   :");
  Serial.println(S1); // 0=nicht ausgeloest, 1=ausgeloest
  Serial.println(S1A);
  if (S1 == 1)
  {
    Serial.println("********************************************");
    delay(15000);
  }
  delay(500);
}
