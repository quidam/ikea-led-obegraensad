#pragma once

#include "PluginManager.h"

class BigClockPlugin : public Plugin
{
private:
    struct tm timeinfo;

public:
    void setup() override;
    void activate() override;
    void loop() override;
    const char *getName() const override;
};
