#include <WiFi.h>
class cl_maindialog
{

public:
    WiFiServer a_pub_server;
    WiFiClient a_pub_client;
    String a_inputval = "INIT";
    String ax_header;
    char a_time[20];
    struct tm a_timeinfo;

    void m_read()
    {
        Serial.print("-----debugging function: ");
        Serial.println(__FUNCTION__);
        if (!a_pub_server)
        {
            a_pub_server.begin();
            Serial.println("nach a_pub_server.begin");
            delay(2000);
            ax_header = a_pub_client.readStringUntil('\n');
            
        }
    }

    void m_display()
    {
        Serial.print("-----debugging function: ");
        Serial.println(__FUNCTION__);
        if (!a_pub_server)
        {
            a_pub_server.begin();
            Serial.println("nach a_pub_server.begin");
            delay(2000);
        }
        getLocalTime(&a_timeinfo);
        while (a_inputval != "X")
        {
            a_pub_client = a_pub_server.available();
            
//            while (a_pub_client.available()) // browser connects and reads data
            if (a_pub_client.available()) // browser connects and reads data
            {
                Serial.println("nach a_pub_client.available");
                delay(2000);
                // while (pub_client.available()){                                   //polls for data from browser

                ax_header = a_pub_client.readStringUntil('\n');
                Serial.print("ax_header: ");
                Serial.println(ax_header);
                delay(2000);
                a_pub_client.println("HTTP/1.1 200 OK");
                a_pub_client.println("Content-type:text/html");
                a_pub_client.println("Connection: close");
                a_pub_client.println();
                //  HTML header****************
                a_pub_client.println("<!DOCTYPE html><html>");
                a_pub_client.println(
                    "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" >");
                a_pub_client.print("<br></head>");
                a_pub_client.print("<br><body>");
                a_pub_client.print("Hallo Welt<br>");
                //sprintf(a_time, &a_timeinfo, "%A, %B %d %Y %H:%M:%S");
                a_pub_client.print(&a_timeinfo);
                a_pub_client.print("<form>Schalten: <select>");
                a_pub_client.print("<option>An</option><option>Aus</option></<option>An</option><option>Aus</option></select><br>");
                a_pub_client.print("<br><input type='submit' value='OK'></form></p>");
                //ax_header="<input type=";
                
                //        pub_client.print(WiFi.localIP());
                a_pub_client.print("</body>");
                Serial.println("Vor dem Stop");
                delay(5000);                

//                a_pub_client.stop();
                
            }
            a_pub_client.stop();
        }
    }
};