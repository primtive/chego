#pragma once
#include <cstdint>

class ISensorsHandler
{
public:
    virtual void handle(uint8_t x, uint8_t y, bool state) = 0;
    virtual ~ISensorsHandler() = default;
};