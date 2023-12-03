#include "plugins/SunPlugin.h"

#ifdef ESP8266
WiFiClient wiFiClient;
#endif

void SunPlugin::setup()
{
    currentStatus = LOADING;
    this->update();
    if (getLocalTime(&timeinfo)) {
        this->lastUpdateDay = timeinfo.tm_mday;
    }
    currentStatus = NONE;
}

void SunPlugin::activate() {
    if (getLocalTime(&timeinfo)) {
        bool needsUpdate = (sunrise == "0000-00-00 00:00" || sunset == "0000-00-00 00:00") && (millis() >= this->lastUpdate + 1000 * 30);
        if (needsUpdate || (lastUpdateDay != timeinfo.tm_mday)) {
            Serial.println("updating sunrise and sunset");
            this->update();
            this->lastUpdateDay = timeinfo.tm_mday;
            this->lastUpdate = millis();
        }
    }
    draw();
}

void SunPlugin::update()
{
    this->sunrise = httpGet(apiStringSunrise);
    this->sunset = httpGet(apiStringSunset);
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
    std::string sSunriseHour = this->sunrise.substr(11, 2);
    std::string sSunriseMinute = this->sunrise.substr(14, 2);

    int sunriseHour = stoi(sSunriseHour);
    int sunriseMinute = stoi(sSunriseMinute);

    Screen.drawBigArrowUp(4, 4);
    Screen.drawBigNumbers(12, 4, {(sunriseHour - sunriseHour % 10) / 10, sunriseHour % 10});
    Screen.setPixel(30, 5, 1);
    Screen.setPixel(31, 5, 1);
    Screen.setPixel(30, 6, 1);
    Screen.setPixel(31, 6, 1);
    Screen.setPixel(30, 9, 1);
    Screen.setPixel(31, 9, 1);
    Screen.setPixel(30, 10, 1);
    Screen.setPixel(31, 10, 1);
    Screen.drawBigNumbers(34, 4, {(sunriseMinute - sunriseMinute % 10) / 10, sunriseMinute % 10});

    // Sunset
    std::string sSunsetHour = sunset.substr(11, 2);
    std::string sSunsetMinute = sunset.substr(14, 2);

    int sunsetHour = stoi(sSunsetHour);
    int sunsetMinute = stoi(sSunsetMinute);

    Screen.drawBigArrowDown(54, 4);
    Screen.drawBigNumbers(62, 4, {(sunsetHour - sunsetHour % 10) / 10, sunsetHour % 10});
    Screen.setPixel(80, 5, 1);
    Screen.setPixel(81, 5, 1);
    Screen.setPixel(80, 6, 1);
    Screen.setPixel(81, 6, 1);
    Screen.setPixel(80, 9, 1);
    Screen.setPixel(81, 9, 1);
    Screen.setPixel(80, 10, 1);
    Screen.setPixel(81, 10, 1);
    Screen.drawBigNumbers(84, 4, {(sunsetMinute - sunsetMinute % 10) / 10, sunsetMinute % 10});

    Screen.switchScreen(104);
}

const char *SunPlugin::getName() const
{
    return "Sun";
}
