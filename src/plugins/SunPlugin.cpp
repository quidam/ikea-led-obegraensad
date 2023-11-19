#include "plugins/SunPlugin.h"

#ifdef ESP8266
WiFiClient wiFiClient;
#endif

void SunPlugin::setup()
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

void SunPlugin::activate() {
    draw();
}

void SunPlugin::loop()
{
    if (millis() >= this->lastUpdate + (1000 * 60 * 30))
    {
        this->update();
        this->lastUpdate = millis();
        Serial.println("updating sunrise and sunset");
        draw();
    };
}

void SunPlugin::update()
{
    String apiStringSunrise = "http://" + String(OPENHAB_SERVER) +":" + String(OPENHAB_PORT) + "/rest/items/" + String(OPENHAB_ITEM_SUNRISE);
    String apiStringSunset = "http://" + String(OPENHAB_SERVER) +":" + String(OPENHAB_PORT) + "/rest/items/" + String(OPENHAB_ITEM_SUNSET);
#ifdef ESP32
    http.begin(apiStringSunrise);
#endif
#ifdef ESP8266
    http.begin(wiFiClient, weatherApiString);
#endif

    int code = http.GET();

    if (code == HTTP_CODE_OK)
    {
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, http.getString());
        sunrise = doc["state"].as<std::string>();        
    }

    http.end();
}

void SunPlugin::draw()
{
    Screen.clear();

    // Sunrise
    std::string sSunriseHour = sunrise.substr(11, 2);
    std::string sSunriseMinute = sunrise.substr(14, 2);
    Serial.print(sSunriseHour.c_str());
    Serial.print(":");
    Serial.println(sSunriseMinute.c_str());

    int sunriseHour = stoi(sSunriseHour);
    int sunriseMinute = stoi(sSunriseMinute);

    Screen.drawNumbers(0, 2, {(sunriseHour - sunriseHour % 10) / 10, sunriseHour % 10});
    Screen.drawNumbers(8, 2, {(sunriseMinute - sunriseMinute % 10) / 10, sunriseMinute % 10});
    Screen.setPixel(7, 3, 1, 50);
    Screen.setPixel(7, 5, 1, 50);

    // Date

    // Screen.drawNumbers(0, 2, {(dayOfMonth - dayOfMonth % 10) / 10, dayOfMonth % 10});
    // Screen.drawNumbers(8, 2, {(month - month % 10) / 10, month % 10});
    // Screen.setPixel(7, 6, 1, 50);
    // Screen.setPixel(15, 6, 1, 50);

    // // Temperature

    // int dot = value.find(".");
    // std::string degrees = value.substr(0, dot);

    // int tempY = 10;

    // if (degrees.substr(0, 1).compare("-") == 0) {
    //     degrees = degrees.substr(1);
    //     int iDegrees = stoi(degrees);
    //     if(iDegrees >= 10) {
    //         Screen.drawCharacter(0, tempY, Screen.readBytes(minusSymbol), 4);
    //         Screen.drawCharacter(11, tempY, Screen.readBytes(degreeSymbol), 4, 50);
    //         Screen.drawNumbers(3, tempY, {(iDegrees - iDegrees % 10) / 10, iDegrees % 10});
    //     } else {
    //         Screen.drawCharacter(0, tempY, Screen.readBytes(minusSymbol), 4);
    //         Screen.drawCharacter(9, tempY, Screen.readBytes(degreeSymbol), 4, 50);
    //         Screen.drawNumbers(3, tempY, {iDegrees});
    //     }
    // } else {
    //     int iDegrees = stoi(degrees);
    //     if(iDegrees >= 10) {
    //         Screen.drawCharacter(9, tempY, Screen.readBytes(degreeSymbol), 4, 50);
    //         Screen.drawNumbers(1, tempY, {(iDegrees - iDegrees % 10) / 10, iDegrees % 10});
    //     } else {
    //         Screen.drawCharacter(7, tempY, Screen.readBytes(degreeSymbol), 4, 50);
    //         Screen.drawNumbers(4, tempY, {iDegrees});
    //     }
    // }
}

const unsigned long SunPlugin::getDuration() const
{
    return 10000uL;
}

const char *SunPlugin::getName() const
{
    return "Sun";
}
