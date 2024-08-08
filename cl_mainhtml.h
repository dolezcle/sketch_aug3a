#include <WiFi.h>
class cl_mainhtml
{

public:
    WiFiServer pub_server;
    WiFiClient pub_client;
    String llx_inputval = "INIT";
    String lx_header;

    void m_display()
    {

        if (!pub_server)
        {
            pub_server.begin();
            Serial.println("nach Pub_server.begin");
            delay(2000);
        }
        while (llx_inputval != "X")
        {
            pub_client = pub_server.available();

            while (pub_client.available()) // browser connects and reads data
            {
                Serial.println("nach pub_client.connected");
                delay(2000);
                // while (pub_client.available()){                                   //polls for data from browser

                lx_header = pub_client.readStringUntil('\n');
                Serial.print("lx_header: ");
                Serial.println(lx_header);
                delay(2000);
                // pub_client.println("HTTP/1.1 200 OK");
                // pub_client.println("Content-type:text/html");
                // pub_client.println("Connection: close");
                // pub_client.println();
                //  HTML header
                pub_client.println("<!DOCTYPE html><html>");
                pub_client.println(
                    "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" >");
                pub_client.print("<br></head>");
                pub_client.print("<br><body>");
                pub_client.print("Hallo Welt<br>");
                //        pub_client.print(WiFi.localIP());
                pub_client.print("</body>");
                Serial.println("Vor dem Stop");
                delay(5000);                

                pub_client.stop();
                
            }
        }
    }
};