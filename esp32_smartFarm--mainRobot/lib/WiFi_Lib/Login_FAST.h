#ifndef WIFI_DASHBOARD_LOGIN_HTML_H
#define WIFI_DASHBOARD_LOGIN_HTML_H

#include <Arduino.h>
#include <WebServer.h>
#include <Preferences.h>

extern Preferences pref;

extern WebServer web;

extern String pref_ssid;
extern String pref_pass;

// HTML for Login Dashboard
const char HTML_dashboard[] PROGMEM = R"==(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>FAST Login DashBoard</title>
    <style>
        body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; display: flex; justify-content: center; background-color: #eef2f3; margin: 0; padding: 20px; }
        .card { background: white; padding: 30px; border-radius: 15px; box-shadow: 0 10px 25px rgba(0,0,0,0.1); width: 100%; max-width: 350px; }
        h2 { color: #333; margin-top: 0; text-align: center; }
        .input-group { position: relative; margin: 10px 0; }
        input { width: 100%; padding: 12px; border: 1px solid #ddd; border-radius: 8px; box-sizing: border-box; outline: none; }
        input:focus { border-color: #4CAF50; }
        .toggle-password { position: absolute; right: 15px; top: 50%; transform: translateY(-50%); cursor: pointer; user-select: none; font-size: 18px; }
        button { width: 100%; background-color: #4CAF50; color: white; padding: 12px; margin-top: 10px; border: none; border-radius: 8px; cursor: pointer; font-weight: bold; font-size: 16px; transition: 0.3s; }
        button:hover { background-color: #45a049; }
        #status { margin-top: 15px; padding: 10px; border-radius: 5px; text-align: center; display: none; font-size: 14px; }
        .success { background-color: #d4edda; color: #155724; border: 1px solid #c3e6cb; }
        .error { background-color: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; }
    </style>
</head>
<body>
    <div class="card">
        <h2>FAST Login DashBoard</h2>
        <div class="input-group">
            <input type="text" id="ssid" placeholder="Nama WiFi (SSID)">
        </div>
        <div class="input-group">
            <input type="password" id="pass" placeholder="Password (PASS)">
            <span class="toggle-password" onclick="togglePassword()">👁️</span>
        </div>
        <button onclick="sendData()">HUBUNGKAN</button>
        <div id="status"></div>
    </div>

    <script>
        function togglePassword() {
            var passInput = document.getElementById("pass");
            var toggleIcon = document.querySelector(".toggle-password");
            if (passInput.type === "password") {
                passInput.type = "text";
                toggleIcon.innerText = "🫣";
            } else {
                passInput.type = "password";
                toggleIcon.innerText = "👁️";
            }
        }

        function sendData() {
            var s = document.getElementById("ssid").value;
            var p = document.getElementById("pass").value;
            var statusDiv = document.getElementById("status");

            if (!s) {
                showStatus("SSID tidak boleh kosong!", "error");
                return;
            }

            showStatus("Mengirim data...", "");

            // Mengirim data menggunakan AJAX fetch
            fetch(`/save?s=${encodeURIComponent(s)}&p=${encodeURIComponent(p)}`)
            .then(response => {
                if (response.ok) {
                    showStatus("Berhasil! ESP32 mencoba terhubung...", "success");
                } else {
                    showStatus("Gagal mengirim data.", "error");
                }
            })
            .catch(err => {
                showStatus("Terjadi kesalahan koneksi.", "error");
            });
        }

        function showStatus(msg, type) {
            var statusDiv = document.getElementById("status");
            statusDiv.innerText = msg;
            statusDiv.style.display = "block";
            statusDiv.className = type;
        }
    </script>
</body>
</html>
)==";

void handleURL();
void handleSave();
void getConf();

#endif
