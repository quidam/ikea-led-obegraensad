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
    this->temperature = -99;
    this->update();
    this->lastUpdate = millis();
    currentStatus = NONE;
}

void WeatherPlugin::activate() {
    draw();
}

void WeatherPlugin::loop()
{
    int checkInterval = this->temperature == -99 ? 1000 * 30 : 1000 * 60 * 30;
    if (millis() >= this->lastUpdate + checkInterval)
    {
        Serial.println("updating weather");
        this->update();
        this->lastUpdate = millis();
    };
}

void WeatherPlugin::update()
{
    String weatherApiString = "https://wttr.in/" + String(WEATHER_LOCATION) + "?format=j2&lang=en";
#ifdef ESP32
    http.begin(weatherApiString);
#endif
#ifdef ESP8266
    http.begin(wiFiClient, weatherApiString);
#endif

    int code = http.GET();

    if (code == HTTP_CODE_OK)
    {
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, http.getString());

        temperature = round(doc["current_condition"][0]["temp_C"].as<float>());
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
}

void WeatherPlugin::draw()
{
    Screen.clear();
    Screen.drawWeather(0, iconY, weatherIcon, 100);

    uint8_t canvasCols;

    if (temperature >= 10)
    {
        Screen.drawBigNumbers(16, 4, {(temperature - temperature % 10) / 10, temperature % 10});
        Screen.drawBigDegreeSign(32, 4);
        canvasCols = 40;
    }
    else if (temperature <= -10)
    {
        Screen.drawBigMinusSign(16, 4);
        Screen.drawBigNumbers(24, 4, {(temperature - temperature % 10) / 10, temperature % 10});
        Screen.drawBigDegreeSign(40, 4);
        canvasCols = 48;
    }
    else if (temperature >= 0)
    {
        Screen.drawBigNumbers(16, 4, {temperature});
        Screen.drawBigDegreeSign(24, 4);
        canvasCols = 32;
    }
    else
    {
        Screen.drawBigMinusSign(16, 4);
        Screen.drawBigNumbers(24, 4, {temperature});
        Screen.drawBigDegreeSign(32, 4);
        canvasCols = 40;
    }
    Screen.switchScreen(canvasCols);
}

const char *WeatherPlugin::getName() const
{
    return "Weather";
}