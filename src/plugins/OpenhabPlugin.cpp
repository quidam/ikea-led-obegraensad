#include "plugins/OpenhabPlugin.h"

// https://github.com/chubin/wttr.in/blob/master/share/translations/en.txt
#ifdef ESP8266
WiFiClient wiFiClient;
#endif

void OpenhabPlugin::setup()
{
    // loading screen
    Screen.clear();
    currentStatus = LOADING;
    Screen.setPixel(4, 7, 1);
    Screen.setPixel(5, 7, 1);
    Screen.setPixel(7, 7, 1);
    Screen.setPixel(8, 7, 1);
    Screen.setPixel(10, 7, 1);
    Screen.setPixel(11, 7, 1);
    this->lastUpdate = millis();
    this->update();
    currentStatus = NONE;
}

void OpenhabPlugin::activate() {
    draw();
}

void OpenhabPlugin::loop()
{
    if (millis() >= this->lastUpdate + (1000 * 60 * 30))
    {
        this->update();
        this->lastUpdate = millis();
        Serial.println("updating openhab");
    };
}

void OpenhabPlugin::update()
{
    String apiString = "http://" + String(OPENHAB_SERVER) +":" + String(OPENHAB_PORT) + "/rest/items/" + String(OPENHAB_ITEM);
#ifdef ESP32
    http.begin(apiString);
#endif
#ifdef ESP8266
    http.begin(wiFiClient, weatherApiString);
#endif

    int code = http.GET();

    if (code == HTTP_CODE_OK)
    {
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, http.getString());
        value = doc["state"].as<std::string>();
    }
}

void OpenhabPlugin::draw()
{
    Screen.clear();

    int dot = value.find(".");
    std::string degrees = value.substr(0, dot);

    int tempY = 10;

    if (degrees.substr(0, 1).compare("-") == 0) {
        degrees = degrees.substr(1);
        int iDegrees = stoi(degrees);
        if(iDegrees >= 10) {
            Screen.drawCharacter(0, tempY, Screen.readBytes(minusSymbol), 4);
            Screen.drawCharacter(11, tempY, Screen.readBytes(degreeSymbol), 4, 50);
            Screen.drawNumbers(3, tempY, {(iDegrees - iDegrees % 10) / 10, iDegrees % 10});
        } else {
            Screen.drawCharacter(0, tempY, Screen.readBytes(minusSymbol), 4);
            Screen.drawCharacter(9, tempY, Screen.readBytes(degreeSymbol), 4, 50);
            Screen.drawNumbers(3, tempY, {iDegrees});
        }
    } else {
        int iDegrees = stoi(degrees);
        if(iDegrees >= 10) {
            Screen.drawCharacter(9, tempY, Screen.readBytes(degreeSymbol), 4, 50);
            Screen.drawNumbers(1, tempY, {(iDegrees - iDegrees % 10) / 10, iDegrees % 10});
        } else {
            Screen.drawCharacter(7, tempY, Screen.readBytes(degreeSymbol), 4, 50);
            Screen.drawNumbers(4, tempY, {iDegrees});
        }
    }
}

const unsigned long OpenhabPlugin::getDuration() const
{
    return 10000uL;
}

const char *OpenhabPlugin::getName() const
{
    return "Openhab";
}
