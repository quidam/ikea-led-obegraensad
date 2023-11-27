#include "plugins/DateTempPlugin.h"

#ifdef ESP8266
WiFiClient wiFiClient;
#endif

void DateTempPlugin::setup()
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
    this->update();
    this->lastUpdate = millis();
    currentStatus = NONE;
}

void DateTempPlugin::activate() {
    dayOfMonth = 0;
    month = 0;
    draw();
}

void DateTempPlugin::loop()
{
    if (getLocalTime(&timeinfo))
    {
        if (month != timeinfo.tm_mon + 1 || dayOfMonth != timeinfo.tm_mday)
        {
            dayOfMonth = timeinfo.tm_mday;
            month = timeinfo.tm_mon + 1;
            draw();
        }
    }

    int checkInterval = this->temperature == "undefined" ? 1000 * 30 : 1000 * 60 * 30;
    if (millis() >= this->lastUpdate + checkInterval)
    {
        Serial.println("updating temperature");
        this->update();
        this->lastUpdate = millis();
        draw();
    };
}

void DateTempPlugin::update()
{
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
        this->temperature = doc["state"].as<std::string>();
    }

    http.end();
}

void DateTempPlugin::draw()
{
    Screen.clear();

    // Date

    // Screen.drawNumbers(0, 2, {(dayOfMonth - dayOfMonth % 10) / 10, dayOfMonth % 10});
    // Screen.drawNumbers(8, 2, {(month - month % 10) / 10, month % 10});
    // Screen.setPixel(7, 6, 1, 30);
    // Screen.setPixel(15, 6, 1, 30);
    Screen.drawBigNumbers(0, 4, {(dayOfMonth - dayOfMonth % 10) / 10, dayOfMonth % 10});
    Screen.setPixel(16, 11, 1);
    Screen.drawBigNumbers(18, 4, {(month - month % 10) / 10, month % 10});
    Screen.setPixel(34, 11, 1);

    int canvasCols = 36;

    // Temperature

    if (temperature != "undefined") {
        int dot = temperature.find(".");
        std::string degrees = temperature.substr(0, dot);

        int tempY = 10;

        if (degrees.substr(0, 1).compare("-") == 0) {
            degrees = degrees.substr(1);
            int iDegrees = stoi(degrees);
            if(iDegrees >= 10) {
                // Screen.drawCharacter(0, tempY, Screen.readBytes(minusSymbol), 4);
                // Screen.drawCharacter(11, tempY, Screen.readBytes(degreeSymbol), 4, 50);
                // Screen.drawNumbers(4, tempY, {(iDegrees - iDegrees % 10) / 10, iDegrees % 10});
                Screen.drawBigMinusSign(36, 4);
                Screen.drawBigNumbers(44, 4, {(iDegrees - iDegrees % 10) / 10, iDegrees % 10});
                Screen.drawBigDegreeSign(60, 4);
                canvasCols = 68;
            } else {
                // Screen.drawCharacter(0, tempY, Screen.readBytes(minusSymbol), 4);
                // Screen.drawCharacter(9, tempY, Screen.readBytes(degreeSymbol), 4, 50);
                // Screen.drawNumbers(4, tempY, {iDegrees});
                Screen.drawBigMinusSign(36, 4);
                Screen.drawBigNumbers(44, 4, {iDegrees});
                Screen.drawBigDegreeSign(52, 4);
                canvasCols = 60;
            }
        } else {
            int iDegrees = stoi(degrees);
            if(iDegrees >= 10) {
                // Screen.drawCharacter(9, tempY, Screen.readBytes(degreeSymbol), 4, 50);
                // Screen.drawNumbers(2, tempY, {(iDegrees - iDegrees % 10) / 10, iDegrees % 10});
                Screen.drawBigNumbers(36, 4, {(iDegrees - iDegrees % 10) / 10, iDegrees % 10});
                Screen.drawBigDegreeSign(52, 4);
                canvasCols = 60;
            } else {
                // Screen.drawCharacter(7, tempY, Screen.readBytes(degreeSymbol), 4, 50);
                // Screen.drawNumbers(5, tempY, {iDegrees});
                Screen.drawBigNumbers(36, 4, {iDegrees});
                Screen.drawBigDegreeSign(44, 4);
                canvasCols = 52;
            }
        }
    }
    Screen.switchScreen(canvasCols);
}

const char *DateTempPlugin::getName() const
{
    return "DateTemp";
}
