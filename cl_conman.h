#include <WiFi.h>
#include <nvs_flash.h>
#include <String.h>
#include <Preferences.h>
#include <chrono>

class cl_conman
{
    int cgi_number;

public:
    const char *cga_debug = "X";
    // int gci_count;

    int cgi_maxlogon_tries = 500; // max tries to connect to WLAN -> reset if reached
    int cgi_logon_tries;          // current no. of tries to connect to WLAN
    String cgst_DHCPhostname;     // String for input, NVM
    String cgst_myip;
    String cgst_host;
    String cgst_header;
    String cgsc_off = "Aus";
    String cgsc_on = "An";
    uint8_t la_chipid[6]; // chip id
    IPAddress cg_ip;
    //    WiFiServer lo_esp_server(); // setup (standalone AP)
    Preferences cgo_prefs;
    const char *Pref_con = "ac_data";

    String cgst_SSID;
    String cgst_password;
    // Mail values
    String cgst_sendername = "Dein ESP";
    String cgst_senderemail;
    String cgst_sender_pw = "";
    String cgst_receivermail;
    String cgst_sender_smtp_host;

    struct tm la_loc_time_tm; 
    long int li_loctime;         // Laufzeit seit boot in lokaler Zeit in sec gemaess ntp

    void m_startconn()
    {
        Serial.print("-----debugging function: ");
        Serial.println(__FUNCTION__);
        WiFi.disconnect();

        delay(20);
        short li_fast_start = m_scan_wlan(); // 0: saved credentials found, 1: not found
        if (li_fast_start < 1)
        {
            int rc = m_connect();
            Serial.println("in m_startconn -- nach m_connect, lesen der prefs**********************");
            cgo_prefs.begin(Pref_con, true); // read only
            cgst_senderemail = cgo_prefs.getString("cgst_sendmail", "");
            cgst_sender_pw = cgo_prefs.getString("cgst_sender_pw", "");
            cgst_receivermail = cgo_prefs.getString("cgst_recmail", "");
            cgst_sender_smtp_host = cgo_prefs.getString("cgst_snd_smtph", "");
            cgo_prefs.end(); // read only

            Serial.print("cgst_senderemail");
            Serial.println(cgst_senderemail);
            Serial.print("cgst_sender_pw");
            Serial.println(cgst_sender_pw);
            Serial.print("cgst_receivermail");
            Serial.println(cgst_receivermail);
            Serial.print("cgst_sender_smtp_host");
            Serial.println(cgst_sender_smtp_host);

            /*
            if (cgst_senderemail == "")
            {
                cgo_prefs.begin(Pref_con, false);
                cgo_prefs.clear();
                cgo_prefs.end();
                ESP.restart();
            }
            */
        }
        else
        {
            m_dialog_credentials();
        }
    }

    void m_reset_NVS()
    {
        nvs_flash_erase(); // erase the NVS partition and...
        nvs_flash_init();  // initialize the NVS partition.
    }

    void m_reset_conndata()
    {
        cgst_SSID = "";
        cgst_password = "";
        cgst_header = "";
        cgst_header = "";
    }

    short m_scan_wlan() // returns 1 if no password, 0 if found
    {
        String lmst_SSID;
        Serial.print("-----debugging function: ");
        Serial.println(__FUNCTION__);

        cgi_number = WiFi.scanNetworks();
        cgo_prefs.begin(Pref_con, true); // read only
        for (int cgi_count = 0; cgi_count < cgi_number; ++cgi_count)
        {
            lmst_SSID = WiFi.SSID(cgi_count);
            Serial.print("WiFi.SSID found: ");
            Serial.println(WiFi.SSID(cgi_count));

            cgst_password = cgo_prefs.getString(lmst_SSID.c_str(), "?");
            Serial.print("cgst_password:   ");
            Serial.println(cgst_password);
            if (cgst_password != "?")
            {
                cgst_SSID = lmst_SSID;
                cgo_prefs.end();
                return 0;
            }
        }
        // cgst_DHCPhostname =

        cgo_prefs.end();
        Serial.println("exit m_scan_wlan ");
        return 1;
    }

    void m_save_creds()
    {
        Serial.print("-----debugging function: ");
        Serial.println(__FUNCTION__);
        Serial.println("**********************************************");
        Serial.println(cgst_SSID.c_str());
        Serial.println(cgst_password);
        Serial.print("cgst_host  :");
        Serial.println(cgst_host);
        Serial.print("cgst_sendername  :");
        Serial.println(cgst_sendername);
        Serial.print("cgst_sendmail  :");
        Serial.println(cgst_senderemail);
        Serial.print("cgst_sender_pw  :");
        Serial.println(cgst_sender_pw);
        Serial.print("cgst_recmail  :");
        Serial.println(cgst_receivermail);
        Serial.print("cgst_sender_smtp_host  :");
        Serial.println(cgst_sender_smtp_host);

        cgo_prefs.begin(Pref_con, false);
        cgo_prefs.putString(cgst_SSID.c_str(), cgst_password);
        cgo_prefs.putString("cgst_host", cgst_host);
        cgo_prefs.putString("cgst_sendname", cgst_sendername);
        cgo_prefs.putString("cgst_sendmail", cgst_senderemail);
        cgo_prefs.putString("cgst_sender_pw", cgst_sender_pw);
        cgo_prefs.putString("cgst_recmail", cgst_receivermail);
        cgo_prefs.putString("cgst_snd_smtph", cgst_sender_smtp_host);
        cgo_prefs.end();
    }

    void m_read_creds()
    {
        String ls_SSID, ls_password, ls_host, ls_sendername, ls_senderemail, ls_sender_pw, ls_receivermail, ls_sender_smtp_host, ls_test;
        Serial.print("-----debugging function: ");
        Serial.println(__FUNCTION__);
        cgo_prefs.begin(Pref_con, true);

        cgst_password = cgo_prefs.getString(cgst_SSID.c_str());
        cgst_sendername = cgo_prefs.getString("cgst_sendname");
        cgst_senderemail = cgo_prefs.getString("cgst_sendmail");
        cgst_sender_pw = cgo_prefs.getString("cgst_sender_pw");
        cgst_receivermail = cgo_prefs.getString("cgst_recmail");
        cgst_sender_smtp_host = cgo_prefs.getString("cgst_snd_smtph");
        cgo_prefs.end();
        Serial.println("**********************************************");
        Serial.print("cgst_host  :");
        Serial.println(cgst_host);
        Serial.print("cgst_sendername  :");
        Serial.println(cgst_sendername);
        Serial.print("cgst_sendmail  :");
        Serial.println(cgst_senderemail);
        Serial.print("cgst_sender_pw  :");
        Serial.println(cgst_sender_pw);
        Serial.print("cgst_receivermail  :");
        Serial.println(cgst_receivermail);
        Serial.print("cgst_sender_smtp_host  :");
        Serial.println(cgst_sender_smtp_host);
    }
    void m_save_test_creds()
    {
        /*
        cgo_prefs.begin(Pref_con, false);
        cgo_prefs.putString("Wurmnetz", "");
        cgo_prefs.end();
        cgo_prefs.begin(Pref_con, false);
        cgo_prefs.putString("WLAN-318841", "");
        cgo_prefs.end();
*/
    }

    short m_get_conn_data()
    {
        Serial.print(__FILE__);
        Serial.print("-----debugging function: ");
        Serial.println(__FUNCTION__);
        cg_ip = WiFi.localIP();
        Serial.println(WiFi.localIP());
        return 0;
    }

    int m_connect() // returns 4 = not connected, 0 = connected
    //connect, get ntp time, set local time
    {
        Serial.print(__FILE__);
        Serial.print("----->debugging function: ");
        /*
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("WiFi.status() == WL_CONNECTED");
            delay(2000);
            return 0;}
        */
        Serial.println("     vor WIFI Setup...");
        WiFi.mode(WIFI_STA);
        if (cgst_password == "?")                  //chg cgst_password.c_str()
        {
            Serial.println("cgst_password.c_str im m_connect ist nicht korrekt.. ");
            Serial.println(cgst_SSID.c_str());
            delay(1000);
            return 4;
        }
        WiFi.begin(cgst_SSID.c_str(), cgst_password.c_str());
        Serial.print("cgst_SSID.c_str(), cgst_password.c_str():    ");
        Serial.print(cgst_SSID.c_str());
        Serial.println(cgst_password.c_str());
        // delay(1000);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(100);
            Serial.println("Mit WiFi verbinden im m_connect.. ");
            //    delay(500);
            Serial.println(cgst_SSID.c_str());
            Serial.println(cgst_password.c_str());
            Serial.print("WiFi.status() m_connect ");
            //    delay(500);
            switch (WiFi.status())
            {
            case 1:
                Serial.println("1 - No SSID (WL_NO_SSID_AVAIL)");
                break;
            case 3:
                Serial.println("3 - Connected (WL_CONNECTED)");
                break;
            case 4:
                Serial.println("4 - Connect failed (WL_CONNECT_FAILED)");
                break;
            case 5:
                Serial.println("5 - Connection lost (WL_CONNECTION_LOST)");
                break;
            case 6:
                Serial.println("6 - Disconnected (WL_DISCONNECTED)");
                break;
            default:
                Serial.print("------------------------------------Other - jetzt kommt der break:   ");
                Serial.println(WiFi.status());
                // delay(1500);
                break;
            }

            if (cgi_maxlogon_tries < cgi_logon_tries)
            {
                //                Serial.println("R E S T A R T ");
                //                ESP.restart();
                Serial.println("     Limit überschritten--> raus...");
                return 4;
            }
            cgi_logon_tries++;
        }
        if (WiFi.status() == WL_CONNECTED)
        {
            get_ntptime();
            getLocalTime(&la_loc_time_tm);
            setenv("TZ", "CET-1CEST,M3.5.0/2,M10.5.0/3", 1); // hier ok???
            li_loctime = mktime(&la_loc_time_tm);
            Serial.print("----->>>debugging function: ");
            Serial.print(__FUNCTION__);
            return 0;
        }
    }

    void m_dialog_credentials()
    {
        Serial.print(__FILE__);
        Serial.print("-----debugging function: ");
        Serial.print(__FUNCTION__);
        delay(20);
        WiFiServer lo_esp_server;
        WiFiClient lo_esp_client;
        const char *ESP_ssid = "MeinschlauerSchalter";
        const char *ESP_pwd = "";
        WiFi.mode(WIFI_AP);
        String llx_inputval = "INIT"; // empty: complete reset, llx_inputval = "OK": data entered
                                      //        uint64_t lli_delay_ini = 600000 * 360;              // 3 Min max
                                      //        long lli_ttime = esp_timer_get_time();              // Start
                                      //        uint64_t lli_etime_ini = lli_ttime + lli_delay_ini; // max time to enter creds before restart
        char llx_datar[100];

        WiFi.softAP(ESP_ssid, ESP_pwd); // Start ESP Wifi
        //        int l_client_started = 0;
        lo_esp_server.begin(80);
        IPAddress iniIP = WiFi.softAPIP();

        /*	client.available: Returns the number of bytes available for reading
            client.connected(): Connect to the IP address and port specified in the constructor. The return value indicates success or failure.
                                check before sending to server
            client.read(): read from server
            client.println(...): send to server
        */
        while (llx_inputval != cgsc_on)
        {
            lo_esp_client = lo_esp_server.available();
            //            Serial.print("m_dialog_credentials: lo_esp_client available -->");
            //            Serial.println(lo_esp_client);
            // delay(2000);

            String lp_ssid = cgst_SSID;
            lp_ssid = "'" + lp_ssid + "'";
            // if (lo_esp_client >= 0)
            //{
            if (lo_esp_client.connected()) // while?

            {
                Serial.println("---im client.connected-iniIP: ---");
                Serial.println(iniIP);
                //    delay(500);
                // send HTML form for SSID & password & fixed ip & hostname
                /*
                lo_esp_client.println("HTTP/1.1 200 OK");
                lo_esp_client.println("Content-type:text/html");
                lo_esp_client.println("Connection: close");
                lo_esp_client.println();
                */
                lo_esp_client.println("<!DOCTYPE html><html>");
                lo_esp_client.println(
                    "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" >");
                lo_esp_client.println(
                    "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: left;}");
                lo_esp_client.println(
                    ".input { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
                lo_esp_client.println(
                    "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                lo_esp_client.println(
                    ".button2 {background-color: #555555;}</style></head>");

                // **********************Web Page Header end, Body starts***********************************
                lo_esp_client.println("<body>");
                lo_esp_client.println("<h1>WLAN W&auml;hlen</h1>");
                // if there is a network known preset it
                /*                if (lp_ssid != "\n")
                {
                    lo_esp_client.println(
                        "<p><form><label for='SSID'>SSID gespeichert:<br><input style='background-color:#FFFFE0' type='text' name='SSID' value=");
                    lo_esp_client.println(lp_ssid);
                    lo_esp_client.println(
                        "></label>");
                Serial.print("nach if lp_ssid");
                //delay(5000);
                }
                else
                */
                // new network
                {
                    lo_esp_client.println(
                        "<p><form><label for='SSID'>SSID w&auml;hlen:</label><br><select style='background-color:#FFFFE0' type='text' name='SSID'>");
                    for (int cgi_count = 0; cgi_count < cgi_number; ++cgi_count)
                    {
                        cgst_SSID = WiFi.SSID(cgi_count);
                        lo_esp_client.println("<option>");
                        lo_esp_client.println(WiFi.SSID(cgi_count));
                        lo_esp_client.println("</option>");
                    }
                    lo_esp_client.println(
                        "</select><br><label for='password'>Password:<br><input style='background-color:#FFFFE0' type='password' name='password' value='6115818735575925'");
                    lo_esp_client.println("></label><br><hr>");
                    /*
                    lo_esp_client.println(
                        "></label><br><label for='myip'>IP der letzten Verbindung:<br><input style='background-color:#FFFFE0' type='text' name='myip' value=");
                    lo_esp_client.println(cgst_myip);
                    lo_esp_client.println(
                        "></label><br><label for='host'>Host:<br><input style='background-color:#FFFFE0' type='text' name='host' value=");
                    lo_esp_client.println(cgst_host);
                    lo_esp_client.println(
                        "></label><br><hr>");
                    */
                    lo_esp_client.println(
                        "<br><label for='mailsender'>SMTP-Benutzername:<br><input style='background-color:#FFFFE0' type='email' name='mailsender'value='dolezcle@web.de'");
                    lo_esp_client.println(
                        "></label><br><label for='sender_pw'>SMTP-Kennwort:<br><input style='background-color:#FFFFE0' type='password' name='sender_pw' value='kathar1na'");
                    lo_esp_client.println(
                        "></label><br><label for='sender_smtp_host'>SMTP-Serveradresse:<br><input style='background-color:#FFFFE0' type='text' name='sender_smtp_host' value='smtp.web.de'");
                    lo_esp_client.println(
                        "></label><br><label for='mail_receiver'>Mail Empf&auml;nger:<br><input style='background-color:#FFFFE0' type='email' name='mail_receiver' value='dolezcle@gmail.com'");
                    lo_esp_client.println(
                        "></label><label><input type='hidden' name='$$$'value='***'");
                    lo_esp_client.println(
                        "></label><br><input type='submit' value='mit Netzwerk verbinden'></form></p>");
                    lo_esp_client.println(
                        "<p><form>Reset <select name='reset'><option></option><option>Reset</option></select><br><br><input type='submit' value='Reset WLAN Anmeldedaten'></form></p>");
                    lo_esp_client.println("</body>"); // Ende der Seite

                    while (lo_esp_client.available())
                    { // client has data?
                        cgst_header = lo_esp_client.readStringUntil('\n');
                        Serial.println("----------------------------------------------");
                        Serial.print("cgst_header when client available    : ");
                        Serial.println(cgst_header);
                        Serial.print("cgst_headerlength: ");
                        Serial.println(cgst_header.length());
                        int li_offset;
                        String l_SSID = get_substring(cgst_header, "?SSID", "&password", 1);
                        Serial.print("l_SSID:");
                        Serial.println(l_SSID);

                        String l_PWD = get_substring(cgst_header, "password=", "&mail", 0);
                        Serial.print("l_PWD:");
                        Serial.println(l_PWD);
                        if (l_PWD == "")
                        {
                            //                        l_PWD = cgst_header;
                        }; // PWD not entered -> take from NVM
                        String l_myip = get_substring(cgst_header, "myip=", "&host", 0);
                        Serial.print("l_myip:");
                        Serial.println(l_myip);
                        if (l_myip == "") // if empty add -1 to the length
                        {
                            li_offset = -1;
                        }
                       // cgst_host = get_substring(cgst_header, "host=", " HTTP", 0);

                        // mail entry fields **************************************************************************
                        cgst_senderemail = get_substring(cgst_header, "mailsender=", "&sender_pw", 0);
                        cgst_senderemail.replace("%40", "@");
                        Serial.print("Mailsender: ");
                        Serial.println(cgst_senderemail);
                        cgst_sender_pw = get_substring(cgst_header, "sender_pw=", "&sender_smtp_host", 0);
                        Serial.print("sender_pw: ");
                        Serial.println(cgst_sender_pw);
                        cgst_sender_smtp_host = get_substring(cgst_header, "sender_smtp_host=", "&mail_receiver", 0);
                        cgst_host = cgst_sender_smtp_host;   //obsolete cgst_sender_smtp_host??
                        cgst_receivermail = get_substring(cgst_header, "mail_receiver=", "&%24%24%24=***", 0);
                        cgst_receivermail.replace("%40", "@");
                        Serial.print("SSID vor Replace: ");
                        Serial.println(l_SSID);
                        l_SSID.replace("+", " ");   // replace '+' by space (GET converts space to +?)
                        l_SSID.replace("%21", "!"); // replace space by !
                        Serial.print("SSID nach Replace: ");
                        Serial.println(l_SSID);
                        Serial.print("  PWD: ");
                        Serial.println(l_PWD);
                        Serial.print("cgst_host: ");
                        Serial.println(cgst_host);
                        if (l_SSID.indexOf("HTTP") > 0)
                        {
                            l_SSID = "";
                        }
                        if (l_PWD.indexOf("GET") > 0)
                        {
                            l_PWD = "";
                        }
                        if (l_SSID.indexOf("Reset") > 0)
                        {
                            Serial.print("Im iniloop R E S E T nach l_SSID: <------------------");
                            Serial.print(l_SSID);
                            //    delay(500);
                            l_SSID = "";
                            l_PWD = "";
                            cgo_prefs.clear(); //-------kompletter reset des NVM
                            llx_inputval = "";
                            Serial.println("prefs C L E A R E D");
                            delay(20);
                            break;
                        }
                        if (l_SSID != "" && l_PWD != "" && l_PWD != "?")
                        {
                            if (l_PWD > "")
                            {
                                //                           l_PWD.toCharArray(cgst_password, l_PWD.length() + 1);
                            }
                            cgst_SSID = l_SSID;
                            cgst_password = l_PWD;
                            //                                                l_SSID.toCharArray(cgst_SSID, l_SSID.length() + 1);
                            Serial.print("cgst_password in m_dialog_credentials: ");
                            Serial.println(cgst_password);
                            Serial.print("  cgst_SSID in m_dialog_credentials: ");
                            Serial.println(cgst_SSID);
                            delay(20);
                            // gs_DHCPhostname = l_host;
                            if (l_myip > " ")
                            {
                                cgst_myip = l_myip;
                            }
                            llx_inputval = "OK";
                        }
                        else
                        {
                        }

                        if (llx_inputval == "OK")
                        { // wenn "OK" dann verbinde an normales Netzwerk
                            Serial.println("Im m_dialog_credentials vor break");

                            lo_esp_client.stop();
                            //                          i_ret = 0;
                            //                          return (i_ret); // ini_loop beenden nach Eingabe
                            Serial.print("llx_inputval (Soll=OK): ");
                            //    delay(5000);
                            Serial.print("vor break lo_esp_client.connected - llx_inputval: ");
                            Serial.println(llx_inputval);
                            Serial.println("jetzt muss der connect kommen ");
                            //    delay(5000);
                            //if (cgst_SSID.c_str() != "" && cgst_password.c_str() != "" && cgst_password.c_str() != "?") chg 240715
                            if (cgst_SSID != "" && cgst_password != "" && cgst_password != "?")
                            {
                                int li_connect = m_connect();
                                if (li_connect > 0)
                                {
                                    llx_inputval = "";
                                    Serial.print("Connection Failed ");
                                }
                            }
                            if (WiFi.status() == WL_CONNECTED)
                            {
                                llx_inputval = cgsc_on;
                                Serial.print("************************************Connected to WiFi: ");
                                Serial.println((WiFi.localIP()));

                                // log_d("Total PSRAM: %d", ESP.getPsramSize());
                                //                                 gs_myip = ipv42string(WiFi.localIP());
                                //  delay(5000);
                                break;
                            }
                        }
                    } // while lo_esp_client.available()
                }
            } // if (lo_esp_client.connected())
            // break;
        } // while (llx_inputval != cgsc_on)
    }
};
