#include <Arduino.h>
#include <GyverMotor2.h>
GMotor2<DRIVER2WIRE_PWM_POWER> motorR(D1, D2);
GMotor2<DRIVER2WIRE_PWM_POWER> motorL(D5, D6);

#include <ESP8266WiFi.h>
#include <GyverHTTP.h>
ghttp::Server<WiFiServer, WiFiClient> server(80);

#include <DNSServer.h>
DNSServer dns;

#include "webjoy.h"

void setup() {
    Serial.begin(115200);

    WiFi.mode(WIFI_AP);
    WiFi.softAP("WebJoy");

    server.begin();
    dns.start(53, "*", WiFi.softAPIP());

    server.onRequest([](ghttp::ServerBase::Request req) {
        switch (req.path().hash()) {
            case SH("/script.js"):
                server.sendFile_P(webjoy_script_gz, sizeof(webjoy_script_gz), "text/javascript", true, true);
                break;

            case su::SH("/style.css"):
                server.sendFile_P(webjoy_style_gz, sizeof(webjoy_style_gz), "text/css", true, true);
                break;

            case SH("/xy"): {
                int x = req.param("x");
                int y = req.param("y");
                motorL.setSpeed(y + x);
                motorR.setSpeed(y - x);
            } break;

            default:
                server.sendFile_P(webjoy_index_gz, sizeof(webjoy_index_gz), "text/html", false, true);
                break;
        }
    });

    motorL.setAccel(300);
    motorR.setAccel(300);
    analogWriteFreq(6000);
}

void loop() {
    motorL.tick();
    motorR.tick();
    server.tick();
    dns.processNextRequest();
}