#pragma once

#include "SFML/Graphics.hpp"

class IChessBoard {
public:
    virtual ~IChessBoard() = default;
    
    // Управление клетками
    virtual void setCellColor(int x, int y, sf::Color color) = 0;
    virtual sf::Color getCellColor(int x, int y) const = 0;
    virtual void toggleCell(int x, int y) = 0;
    virtual void resetBoard() = 0;
    
    // Управление состоянием доски
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual bool isEnabled() const = 0;
    
    // Размеры доски
    virtual std::pair<int, int> getSize() const = 0;
};