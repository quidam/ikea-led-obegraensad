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
    this->update();
    if (getLocalTime(&timeinfo)) {
        this->lastUpdateDay = timeinfo.tm_mday;
    }
    currentStatus = NONE;
}

void SunPlugin::activate() {
    draw();
}

void SunPlugin::loop()
{
    if (getLocalTime(&timeinfo)) {
        if (lastUpdateDay != timeinfo.tm_mday) {
            Serial.println("updating sunrise and sunset");
            this->update();
            draw();
            lastUpdateDay = timeinfo.tm_mday;
        }
    }
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
    http.begin(wiFiClient, url);
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

    Screen.drawBigArrowUp(4, 4);
    Screen.drawBigNumbers(12, 4, {(sunriseHour - sunriseHour % 10) / 10, sunriseHour % 10});
    Screen.drawBigColon(28, 4);
    Screen.drawBigNumbers(36, 4, {(sunriseMinute - sunriseMinute % 10) / 10, sunriseMinute % 10});

    // Sunset
    std::string sSunsetHour = sunset.substr(11, 2);
    std::string sSunsetMinute = sunset.substr(14, 2);

    int sunsetHour = stoi(sSunsetHour);
    int sunsetMinute = stoi(sSunsetMinute);

    Screen.drawBigArrowDown(56, 4);
    Screen.drawBigNumbers(64, 4, {(sunsetHour - sunsetHour % 10) / 10, sunsetHour % 10});
    Screen.drawBigColon(80, 4);
    Screen.drawBigNumbers(88, 4, {(sunsetMinute - sunsetMinute % 10) / 10, sunsetMinute % 10});

    Screen.switchScreen(108);
}

const char *SunPlugin::getName() const
{
    return "Sun";
}
