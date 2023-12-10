#include "plugins/SunPlugin.h"

#ifdef ESP8266
WiFiClient wiFiClient;
#endif

void SunPlugin::setup()
{
    currentStatus = LOADING;
    this->updateSun();
    if (getLocalTime(&timeinfo)) {
        this->lastUpdateDay = timeinfo.tm_mday;
    }
    currentStatus = NONE;
}

void SunPlugin::activate() {
    if (getLocalTime(&timeinfo)) {
        unsigned long now = millis();
        bool sunNeedsUpdate = (this->sunrise == "0000-00-00 00:00" || this->sunset == "0000-00-00 00:00") && (now >= this->lastUpdateSun + 1000 * 30);
        if (sunNeedsUpdate || (lastUpdateDay != timeinfo.tm_mday)) {
            Serial.println("updating sunrise and sunset");
            this->updateSun();
            this->lastUpdateDay = timeinfo.tm_mday;
            this->lastUpdateSun = now;
        }
        bool waterLevelNeedsUpdate = (this->waterLevel == 0) && (now >= this->lastUpdateWaterLevel + 1000 * 30);
        if (waterLevelNeedsUpdate || now >= this->lastUpdateWaterLevel + 1000 * 900) {
            Serial.println("updating water level");
            this->updateWaterLevel();
            this->lastUpdateWaterLevel = now;
        }
    }
    draw();
}

void SunPlugin::updateSun()
{
    this->sunrise = httpGetSun(apiStringSunrise);
    this->sunset = httpGetSun(apiStringSunset);
}

std::string SunPlugin::httpGetSun(String url) {
#ifdef ESP32
    httpSun.begin(url);
#endif
#ifdef ESP8266
    httpSun.begin(wiFiClient, url);
#endif

    int code = httpSun.GET();
    std::string value = "0000-00-00 00:00";

    if (code == HTTP_CODE_OK)
    {
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, httpSun.getString());
        value = doc["state"].as<std::string>();
    }

    httpSun.end();

    return value;
}

void SunPlugin::updateWaterLevel() {
    String waterLevelApiString = "https://pegelonline.wsv.de/webservices/rest-api/v2/stations/" + String(WATER_LEVEL_UUID) + "/W/measurements.json?start=P0DT0H15M";
#ifdef ESP32
    httpWaterLevel.begin(waterLevelApiString);
#endif
#ifdef ESP8266
    httpWaterLevel.begin(wiFiClient, waterLevelApiString);
#endif

    int code = httpWaterLevel.GET();
    this->waterLevel = 0.0;

    if (code == HTTP_CODE_OK)
    {
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, httpWaterLevel.getString());
        this->waterLevel = doc[0]["value"].as<float>();
        Serial.print("Received water level: ");
        Serial.println(this->waterLevel);
    } else {
        Serial.print("Return code: ");
        Serial.println(code);
    }

    httpWaterLevel.end();
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

    // Water level
    int displayNumber = (int) roundf(this->waterLevel);
    if (displayNumber > 0) {
        int hundreds = (displayNumber - displayNumber % 100) / 100;
        displayNumber = displayNumber % 100;
        int tens = (displayNumber - displayNumber % 10) / 10;
        int ones = displayNumber % 10;
        Screen.drawBigWaterSign(108, 4);
        if (this->waterLevel > 99) {
            Screen.drawBigNumbers(116, 4, {hundreds, tens, ones});
            Screen.switchScreen(144);
        } else if (this->waterLevel > 9) {
            Screen.drawBigNumbers(116, 4, {tens, ones});
            Screen.switchScreen(136);
        } else {
            Screen.drawBigNumbers(116, 4, {ones});
            Screen.switchScreen(128);
        }
    } else {
        Screen.switchScreen(104);
    }
}

const char *SunPlugin::getName() const
{
    return "Sun";
}
