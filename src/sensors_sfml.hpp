#pragma once
#include "sensors_handler.hpp"

using sensors_t = std::vector<std::vector<bool>>;
class DebugWindow;

class SFMLSensors
{
public:
    DebugWindow &window;
    sensors_t state;
    uint8_t size;

    SFMLSensors(DebugWindow &window, uint8_t size);
    void display();
    void setHandler(ISensorsHandler *handler) { m_handler = handler; }
    void triggerSensor(uint8_t x, uint8_t y, bool state);

private:
    ISensorsHandler *m_handler = nullptr;
};