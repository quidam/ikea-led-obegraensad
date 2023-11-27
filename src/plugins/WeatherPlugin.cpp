#include "plugins/WeatherPlugin.h"

// https://github.com/chubin/wttr.in/blob/master/share/translations/en.txt
#ifdef ESP8266
WiFiClient wiFiClient;
#endif

void WeatherPlugin::setup()
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
    this->temperature = "undefined";
    this->updateWeather();
    this->updateTemperature();
    this->lastUpdate = millis();
    currentStatus = NONE;
}

void WeatherPlugin::activate() {
    draw();
}

void WeatherPlugin::loop()
{
    int checkInterval = this->temperature == "undefined" ? 1000 * 30 : 1000 * 60 * 30;
    if (millis() >= this->lastUpdate + checkInterval)
    {
        Serial.println("updating weather");
        this->updateWeather();

        Serial.println("updating temperature");
        this->updateTemperature();

        this->lastUpdate = millis();
        draw();
    };
}

void WeatherPlugin::updateWeather()
{
    String weatherApiString = "https://wttr.in/" + String(WEATHER_LOCATION) + "?format=j2&lang=en";
#ifdef ESP32
    httpWeather.begin(weatherApiString);
#endif
#ifdef ESP8266
    httpWeather.begin(wiFiClient, weatherApiString);
#endif

    int code = httpWeather.GET();

    if (code == HTTP_CODE_OK)
    {
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, httpWeather.getString());

        weatherCode = doc["current_condition"][0]["weatherCode"].as<int>();
        weatherIcon = 0;
        iconY = 4;

        if (std::find(thunderCodes.begin(), thunderCodes.end(), weatherCode) != thunderCodes.end())
        {
            weatherIcon = 1;
        }
        else if (std::find(rainCodes.begin(), rainCodes.end(), weatherCode) != rainCodes.end())
        {
            weatherIcon = 4;
        }
        else if (std::find(snowCodes.begin(), snowCodes.end(), weatherCode) != snowCodes.end())
        {
            weatherIcon = 5;
        }
        else if (std::find(fogCodes.begin(), fogCodes.end(), weatherCode) != fogCodes.end())
        {
            weatherIcon = 6;
            iconY = 5;
        }
        else if (std::find(clearCodes.begin(), clearCodes.end(), weatherCode) != clearCodes.end())
        {
            weatherIcon = 2;
            iconY = 3;
        }
        else if (std::find(cloudyCodes.begin(), cloudyCodes.end(), weatherCode) != cloudyCodes.end())
        {
            weatherIcon = 0;
            iconY = 5;
        }
        else if (std::find(partyCloudyCodes.begin(), partyCloudyCodes.end(), weatherCode) != partyCloudyCodes.end())
        {
            weatherIcon = 3;
            iconY = 5;
        }

        draw();
    }

    httpWeather.end();
}

void WeatherPlugin::updateTemperature()
{
#ifdef ESP32
    httpTemperature.begin(apiString);
#endif
#ifdef ESP8266
    httpTemperature.begin(wiFiClient, weatherApiString);
#endif

    int code = httpTemperature.GET();

    if (code == HTTP_CODE_OK)
    {
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, httpTemperature.getString());
        this->temperature = doc["state"].as<std::string>();
    }

    httpTemperature.end();
}

void WeatherPlugin::draw()
{
    Screen.clear();
    Screen.drawWeather(0, iconY, weatherIcon, 100);

    uint8_t canvasCols = 16;

    // Temperature

    if (temperature != "undefined") {
        int dot = temperature.find(".");
        std::string degrees = temperature.substr(0, dot);

        int tempY = 10;

        if (degrees.substr(0, 1).compare("-") == 0) {
            degrees = degrees.substr(1);
            int iDegrees = stoi(degrees);
            if(iDegrees >= 10) {
                Screen.drawBigMinusSign(16, 4);
                Screen.drawBigNumbers(24, 4, {(iDegrees - iDegrees % 10) / 10, iDegrees % 10});
                Screen.drawBigDegreeSign(40, 4);
                canvasCols = 48;
            } else {
                Screen.drawBigMinusSign(16, 4);
                Screen.drawBigNumbers(24, 4, {iDegrees});
                Screen.drawBigDegreeSign(32, 4);
                canvasCols = 70;
            }
        } else {
            int iDegrees = stoi(degrees);
            if(iDegrees >= 10) {
                Screen.drawBigNumbers(16, 4, {(iDegrees - iDegrees % 10) / 10, iDegrees % 10});
                Screen.drawBigDegreeSign(32, 4);
                canvasCols = 40;
            } else {
                Screen.drawBigNumbers(16, 4, {iDegrees});
                Screen.drawBigDegreeSign(24, 4);
                canvasCols = 32;
            }
        }
    }

    Screen.switchScreen(canvasCols);
}

const char *WeatherPlugin::getName() const
{
    return "Weather";
}