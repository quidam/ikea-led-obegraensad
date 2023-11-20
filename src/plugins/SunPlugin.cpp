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
    sunrise = httpGet(apiStringSunrise);
    sunset = httpGet(apiStringSunset);
}

std::string SunPlugin::httpGet(String url) {
#ifdef ESP32
    http.begin(url);
#endif
#ifdef ESP8266
    http.begin(wiFiClient, weatherApiString);
#endif

    int code = http.GET();
    std::string value = "0000-00-00 00:00";

    if (code == HTTP_CODE_OK)
    {
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, http.getString());
        value = doc["state"].as<std::string>();        
    }

    http.end();

    return value;
}

void SunPlugin::draw()
{
    Screen.clear();

    // Sunrise
    std::string sSunriseHour = sunrise.substr(11, 2);
    std::string sSunriseMinute = sunrise.substr(14, 2);

    int sunriseHour = stoi(sSunriseHour);
    int sunriseMinute = stoi(sSunriseMinute);

    Screen.drawNumbers(0, 2, {(sunriseHour - sunriseHour % 10) / 10, sunriseHour % 10});
    Screen.drawNumbers(8, 2, {(sunriseMinute - sunriseMinute % 10) / 10, sunriseMinute % 10});
    Screen.drawLine(7, 0, 8, 0, 1, 50);
    Screen.drawLine(6, 1, 9, 1, 1, 50);

    // Sunset
    std::string sSunsetHour = sunset.substr(11, 2);
    std::string sSunsetMinute = sunset.substr(14, 2);

    int sunsetHour = stoi(sSunsetHour);
    int sunsetMinute = stoi(sSunsetMinute);

    Screen.drawNumbers(0, 10, {(sunsetHour - sunsetHour % 10) / 10, sunsetHour % 10});
    Screen.drawNumbers(8, 10, {(sunsetMinute - sunsetMinute % 10) / 10, sunsetMinute % 10});
    Screen.drawLine(6, 8, 9, 8, 1, 50);
    Screen.drawLine(7, 9, 8, 9, 1, 50);
}

const unsigned long SunPlugin::getDuration() const
{
    return 10000uL;
}

const char *SunPlugin::getName() const
{
    return "Sun";
}
