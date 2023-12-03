#include "plugins/WeatherPlugin.h"

// https://github.com/chubin/wttr.in/blob/master/share/translations/en.txt
#ifdef ESP8266
WiFiClient wiFiClient;
#endif

void WeatherPlugin::setup()
{
    currentStatus = LOADING;
    this->updateWeather();
    this->updateTemperature();
    currentStatus = NONE;
}

void WeatherPlugin::activate() {
    unsigned long now = millis();
    int checkIntervalWeather = this->weatherIcon == -1 ? 1000 * 30 : 1000 * 60 * 30;
    int checkIntervalTemperature = this->temperature == "undefined" ? 1000 * 30 : 1000 * 60 * 30;
    if (now >= this->lastUpdateWeather + checkIntervalWeather) {
        Serial.println("updating weather");
        this->updateWeather();
    }

    if (now >= this->lastUpdateTemperature + checkIntervalTemperature) {
        Serial.println("updating temperature");
        this->updateTemperature();
    };
    draw();
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

    weatherIcon = -1;
    iconY = 4;

    int code = httpWeather.GET();

    if (code == HTTP_CODE_OK) {
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, httpWeather.getString());

        weatherCode = doc["current_condition"][0]["weatherCode"].as<int>();
        weatherIcon = 0;

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
    }

    httpWeather.end();
    this->lastUpdateWeather = millis();
}

void WeatherPlugin::updateTemperature()
{
#ifdef ESP32
    httpTemperature.begin(apiString);
#endif
#ifdef ESP8266
    httpTemperature.begin(wiFiClient, weatherApiString);
#endif

    this->temperature = "undefined";

    int code = httpTemperature.GET();

    if (code == HTTP_CODE_OK)
    {
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, httpTemperature.getString());
        this->temperature = doc["state"].as<std::string>();
    }

    httpTemperature.end();
    this->lastUpdateTemperature = millis();
}

void WeatherPlugin::draw()
{
    Screen.clear();
    if(weatherIcon >= 0) {
        Screen.drawWeather(4, iconY, weatherIcon, 100);
    }

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
                Screen.drawBigMinusSign(20, 4);
                Screen.drawBigNumbers(28, 4, {(iDegrees - iDegrees % 10) / 10, iDegrees % 10});
                Screen.drawBigDegreeSign(45, 4);
                canvasCols = 56;
            } else {
                Screen.drawBigMinusSign(20, 4);
                Screen.drawBigNumbers(28, 4, {iDegrees});
                Screen.drawBigDegreeSign(37, 4);
                canvasCols = 48;
            }
        } else {
            int iDegrees = stoi(degrees);
            if(iDegrees >= 10) {
                Screen.drawBigNumbers(20, 4, {(iDegrees - iDegrees % 10) / 10, iDegrees % 10});
                Screen.drawBigDegreeSign(37, 4);
                canvasCols = 48;
            } else {
                Screen.drawBigNumbers(20, 4, {iDegrees});
                Screen.drawBigDegreeSign(29, 4);
                canvasCols = 40;
            }
        }
    }

    Screen.switchScreen(canvasCols);
}

const char *WeatherPlugin::getName() const
{
    return "Weather";
}