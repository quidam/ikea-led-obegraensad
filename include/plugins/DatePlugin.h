#pragma once

#include "PluginManager.h"

class DatePlugin : public Plugin
{
private:
    struct tm timeinfo;

    int previousDay;
    int previousMonth;

public:
    void setup() override;
    void activate() override;
    void loop() override;
    const unsigned long getDuration() const override;
    const char *getName() const override;
};
