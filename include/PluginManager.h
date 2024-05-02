#pragma once

#include <ArduinoJson.h>
#include <Arduino.h>
#include <vector>
#include <string>
#include <list>

#include "screen.h"
#include "signs.h"
#include "websocket.h"

class Plugin
{
private:
    int id;

public:
    Plugin();

    virtual ~Plugin() {}

    virtual void teardown();
    virtual void websocketHook(DynamicJsonDocument &request);
    virtual void setup() = 0;
    virtual void activate() = 0;
    virtual const char *getName() const = 0;

    void setId(int id);
    int getId() const;
};

class PluginManager
{
private:
    std::vector<Plugin *> plugins;
    Plugin *activePlugin;
    int nextPluginId;
    unsigned long previousPluginSwitch = 0;
    std::vector< std::tuple< int, int > > schedule;
    int scheduleIndex;
    unsigned long activePluginDuration;

public:
    PluginManager();

    int addPlugin(Plugin *plugin);
    void addScheduleItem(int duration, int pluginId);
    void setActivePlugin(const char *pluginName);
    void setActivePluginById(int pluginId);
    void runActivePlugin();
    void activateNextPlugin();
    void init();
    Plugin *getActivePlugin() const;
    std::vector<Plugin *> &getAllPlugins();
    size_t getNumPlugins();
};

extern PluginManager pluginManager;