#pragma once

#include <SFML/Graphics.hpp>

class HealthBar {
public:
    HealthBar(float width, float height);
    void setPosition(float x, float y);
    void update(float healthPercent);
    void draw(sf::RenderWindow &window) const;

private:
    sf::RectangleShape outerBar;
    sf::RectangleShape innerBar;
};

class PowerUpBar {
public:
    PowerUpBar(float width, float height);
    void setPosition(float x, float y);
    void update(float timePercent);
    void draw(sf::RenderWindow &window) const;

private:
    sf::RectangleShape outerBar;
    sf::RectangleShape innerBar;
};
