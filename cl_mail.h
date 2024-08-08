class cl_mail
{
public:
    Preferences cgo_prefs;
    const char *Pref_con = "mail_data";
    String sendername = "ESP Sensor";
    String senderemail = "";
    String sender_pw = "";
    String receivermail = "";
    //   String sender_smtp_host = "mx.freenet.de";
    String sender_smtp_host = "";
    String as_subject;
    String as_content = "IP Adresse: ";
    String ls_SSID;
    int smtp_port = 587;
    ESP_Mail_Session mail_session;
    SMTP_Message mail_msg;
    SMTPSession lo_smtp_sess;

    cl_mail() {}
    cl_mail(String i_senderemail, String i_sender_pw, String i_receivermail, String i_sender_smtp_host, String i_ls_SSID, int i_smtp_port) : senderemail(i_senderemail),
                                                                                                                                             sender_pw(i_sender_pw),
                                                                                                                                             receivermail(i_receivermail),
                                                                                                                                             sender_smtp_host(i_sender_smtp_host),
                                                                                                                                             ls_SSID(i_ls_SSID),
                                                                                                                                             smtp_port(i_smtp_port)
    {
    }

    void m_initialize()
    {
        Serial.print(__FILE__);
        Serial.print("-----debugging function: ");
        Serial.println(__FUNCTION__);
        lo_smtp_sess.debug(1);
        mail_session.server.host_name = sender_smtp_host;
        mail_session.server.port = smtp_port;
        mail_session.login.email = senderemail;

        mail_session.login.password = sender_pw;

        mail_session.login.user_domain = "";
        mail_msg.sender.name = sendername;
        mail_msg.sender.email = senderemail;
        mail_msg.subject = "ich bin online";
        //        if (receivermail != "")
        mail_msg.addRecipient("mailEmpfaenger", receivermail);
        as_content = as_content + "http://" + WiFi.localIP().toString().c_str() + " im WLAN: " + ls_SSID;
        mail_msg.text.content = as_content.c_str();
        Serial.print("senderemail   :");
        Serial.println(senderemail);
        Serial.print("sendername   :");
        Serial.println(sendername);
        Serial.print("receivermail   :");
        Serial.println(receivermail);
        Serial.print("sender_smtp_host   :");
        Serial.println(sender_smtp_host);
        Serial.print("sender_pw   :");
        Serial.println(sender_pw);
        Serial.print("smtp_port   :");
        Serial.println(smtp_port);
        // Serial.println("im initialize7");
    }
    void m_setsubject(String i_subject)
    {
        as_subject = i_subject;
        mail_msg.subject = as_subject;
    }

    void m_setcontent(String i_content)
    {
        as_content = i_content;
        mail_msg.text.content = as_content.c_str();
    }

    void m_send_mail()
    {
        Serial.print(__FILE__);
        Serial.print("-----debugging function: ");
        Serial.print(__FUNCTION__);
        Serial.println(" Start");
        if (!lo_smtp_sess.connect(&mail_session))
        {
            Serial.println("***********lo_smtp_sess: not connected **************");
            return;
        }
        Serial.print(__FUNCTION__);
        Serial.println(" vor Mail");
        Serial.print("&mail_msg   ");
        Serial.println(as_content);
        if (!MailClient.sendMail(&lo_smtp_sess, &mail_msg))
        {
            Serial.println("Error sending Email, " + lo_smtp_sess.errorReason());
            Serial.println("***************R E S T A R T************************");
            ESP.restart();
        }
        Serial.print(__FUNCTION__);
        Serial.println(" End");
    }
};