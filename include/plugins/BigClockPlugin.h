#pragma once

#include "PluginManager.h"

class BigClockPlugin : public Plugin
{
private:
    struct tm timeinfo;

    int previousMinutes;
    int previousHour;

public:
    void setup() override;
    void activate() override;
    void loop() override;
    const unsigned long getDuration() const override;
    const char *getName() const override;
};
