#include <cmath>
#include <vector>
#include <array>
#include "sensors_handler.hpp"
#include "sensors_sfml.hpp"
#include "debug_window.hpp"

SFMLSensors::SFMLSensors(DebugWindow &window, uint8_t size) : size(size),
                                                              window(window)
{
    state = sensors_t(
        size,
        std::vector<bool>(size));
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            if (y < 2 || y > 5)
            {
                state[y][x] = true; // Изначально датчики в этих строках включены
            }
            else
            {
                state[y][x] = false; // Изначально датчики в этих строках выключены
            }
        }
    }
    // display();
}
void SFMLSensors::display()
{
    window.display();
}
void SFMLSensors::triggerSensor(uint8_t x, uint8_t y, bool state)
{
    if (m_handler)
    {
        m_handler->handle(x, y, state);
    }
}