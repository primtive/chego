#include "matrix_sfml.hpp"
#include <cmath>
#include <vector>
#include <chrono>
#include <thread>
#include "globals.hpp"
#include "debug_window.hpp"

using namespace std::chrono_literals;

SFMLMatrix::SFMLMatrix(DebugWindow &window, uint8_t size) : leds(leds_t(size, std::vector<Color>(size, Color::Black))),
                                                            window(window),
                                                            IMatrix(size) {}
void SFMLMatrix::display()
{
    window.display();
}
void SFMLMatrix::displayWithAnim(leds_t _leds)
{
    for (float r = 0; r <= size; r += 0.3)
    {
        for (int y = 0; y < size; y++)
        {
            for (int x = 0; x < size; x++)
            {
                // Расчет расстояния от центра
                float distance = sqrt(pow(x - center, 2) + pow(y - center, 2));

                // Если диод внутри круга с затуханием на границе
                if (r - distance < 1.0 && distance <= r)
                {
                    setCellColor(x, y, _leds[y][x]);
                }
            }
        }
        std::this_thread::sleep_for(30ms); // Эмуляция задержки для обновления экрана
        display();
    }
}

void SFMLMatrix::setCellColor(uint8_t x, uint8_t y, Color color)
{
    leds[y][x] = color;
}

void SFMLMatrix::fill(Color color)
{

    for (uint8_t x = 0; x < size; ++x)
    {
        for (uint8_t y = 0; y < size; ++y)
        {
            setCellColor(x, y, color);
        }
    }
}