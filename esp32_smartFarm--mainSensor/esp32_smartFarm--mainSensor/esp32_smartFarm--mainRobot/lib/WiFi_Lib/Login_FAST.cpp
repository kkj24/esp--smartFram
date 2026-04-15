#include "WiFi_Lib.h"

WebServer web(80);
Preferences pref;

String pref_ssid = "";
String pref_pass = "";

void handleURL() {
    // Show Web Form
    web.send(200, "text/html", HTML_dashboard);
}

void handleSave() {
    // Take SSID and PASS
    pref_ssid = web.arg("s");
    pref_pass = web.arg("p");

    // Write Config into Flash Memory
    pref.begin("WiFi_Config", false);
    pref.putString("ssid_conf", pref_ssid);
    pref.putString("pass_conf", pref_pass);
    pref.end();

    // Read Config from Flash Memory
    pref.begin("WiFi_Config", true);
    STA_ssid = pref.getString("ssid_conf", "");
    STA_pass = pref.getString("pass_conf", "");
    pref.end();

    // Instant Reconnect
    WiFi.disconnect();
    WiFi.begin(STA_ssid.c_str(), STA_pass.c_str());

    String enx_pass = "***************************";

    Serial.print(F("\nUpdated!\n"));
    Serial.print(F("\nSSID: "));
    Serial.print(STA_ssid);
    Serial.print(F("\nPASS: "));
    Serial.print(enx_pass.substring(0, STA_pass.length()));
    Serial.print(F("\n"));

    web.send(200, "text/plain", "OK");
}

void getConf() {
    unsigned long now = millis();
    static unsigned long last = 0;
    int interval = 1000;

    static String lastSsid = "not Available";
    static String lastPass = "not Available";

    String pass_enx = "**************************";

    if(now - last >= interval) {
        last = now;

        if(STA_ssid != lastSsid || STA_pass != lastPass) {
            // Read Config from Flash Memory
            pref.begin("WiFi_Config", true);
            STA_ssid = pref.getString("ssid_conf", "");
            STA_pass = pref.getString("pass_conf", "");
            pref.end();

            // Update
            lastSsid = STA_ssid;
            lastPass = STA_pass;

            // Notification in Serial Monitor
            Serial.print(F("\nRead Configuration\n"));
            Serial.print(F("\nSSID : "));
            Serial.print(STA_ssid);
            Serial.print(F("\nPASS : "));
            Serial.print(pass_enx.substring(0, STA_pass.length()));
            Serial.print(F("\n"));

            // Instant Reconnect
            WiFi.begin(STA_ssid.c_str(), STA_pass.c_str());
        }
    }
}
