#include "PluginManager.h"

Plugin::Plugin() : id(-1) {}

void Plugin::setId(int id)
{
    this->id = id;
}

int Plugin::getId() const
{
    return id;
}

void Plugin::teardown() {}

PluginManager::PluginManager() : nextPluginId(1) {}

void PluginManager::init()
{
    Screen.clear();
    std::vector<Plugin *> &allPlugins = pluginManager.getAllPlugins();
    for (Plugin *plugin : allPlugins)
    {
        Serial.print("init plugin: ");
        Serial.print(plugin->getName());
        Serial.print(" id: ");
        Serial.println(plugin->getId());

        plugin->setup();
    }

    scheduleIndex = 0;
    if (!activePlugin)
    {
        std::tuple<int, int> firstPlugin = schedule[scheduleIndex];
        int pluginId = std::get<0>(firstPlugin);
        activePluginDuration = 1000UL * std::get<1>(firstPlugin);
        pluginManager.setActivePluginById(pluginId);
    }
}

int PluginManager::addPlugin(Plugin *plugin)
{
    plugin->setId(nextPluginId++);
    plugins.push_back(plugin);
    return plugin->getId();
}

void PluginManager::addScheduleItem(int pluginId, int duration) {
    schedule.push_back(std::make_tuple(pluginId, duration));
}

void PluginManager::setActivePlugin(const char *pluginName)
{
    if (activePlugin)
    {
        activePlugin->teardown();
        activePlugin = nullptr;
    }

    for (Plugin *plugin : plugins)
    {
        if (strcmp(plugin->getName(), pluginName) == 0)
        {
            Serial.print("activate: ");
            Serial.println(plugin->getName());

            Screen.clear();
            activePlugin = plugin;
            activePlugin->activate();
            break;
        }
    }
}

void PluginManager::setActivePluginById(int pluginId)
{
    for (Plugin *plugin : plugins) {
        if (plugin->getId() == pluginId) {
            setActivePlugin(plugin->getName());
            previousPluginSwitch = millis();
            return;
        }
    }
    Serial.print("Plugin not found: ");
    Serial.println(pluginId);
}

int modeButtonState = 0;
int lastModeButtonState = 1;

void PluginManager::runActivePlugin()
{
    if (currentStatus != LOADING)
    {
        modeButtonState = digitalRead(PIN_BUTTON);
        if (modeButtonState != lastModeButtonState && modeButtonState == HIGH)
        {
            pluginManager.activateNextPlugin();
        }
        lastModeButtonState = modeButtonState;
        currentStatus = NONE;
    }
    if (currentStatus != UPDATE && currentStatus != LOADING && currentStatus != WSBINARY) {
        if (activePlugin) {
            if (screenStatus == STATIC || screenStatus == FINISHED) {
                if (millis() - previousPluginSwitch > activePluginDuration)
                {
                    activateNextPlugin();
                }
            } else {
                previousPluginSwitch = millis();
            }
        }
    }
}

Plugin *PluginManager::getActivePlugin() const
{
    return activePlugin;
}

std::vector<Plugin *> &PluginManager::getAllPlugins()
{
    return plugins;
}

size_t PluginManager::getNumPlugins()
{
    return plugins.size();
}

void PluginManager::activateNextPlugin()
{
    scheduleIndex = scheduleIndex == schedule.size() - 1 ? 0 : scheduleIndex + 1;
    std::tuple<int, int> nextPlugin = schedule[scheduleIndex];
    int pluginId = std::get<0>(nextPlugin);
    this->activePluginDuration = 1000UL * std::get<1>(nextPlugin);

    setActivePluginById(pluginId);
}