#include "Hud.h"

#include <algorithm>

HealthBar::HealthBar(float width, float height) {
    outerBar.setSize({width, height});
    outerBar.setFillColor(sf::Color::Transparent);
    outerBar.setOutlineColor(sf::Color::Black);
    outerBar.setOutlineThickness(1.0f);

    innerBar.setSize({width, height});
    innerBar.setFillColor(sf::Color(149, 255, 108));
}

void HealthBar::setPosition(float x, float y) {
    outerBar.setPosition(x, y);
    innerBar.setPosition(x, y);
}

void HealthBar::update(float healthPercent) {
    healthPercent = std::clamp(healthPercent, 0.0f, 100.0f);
    innerBar.setSize({outerBar.getSize().x * (healthPercent / 100.0f), innerBar.getSize().y});

    if (healthPercent > 50.0f) {
        innerBar.setFillColor(sf::Color(149, 255, 108));
    } else if (healthPercent > 30.0f) {
        innerBar.setFillColor(sf::Color(244, 255, 38));
    } else if (healthPercent > 20.0f) {
        innerBar.setFillColor(sf::Color(247, 255, 134));
    } else {
        innerBar.setFillColor(sf::Color(255, 94, 94));
    }
}

void HealthBar::draw(sf::RenderWindow &window) const {
    window.draw(outerBar);
    window.draw(innerBar);
}

PowerUpBar::PowerUpBar(float width, float height) {
    outerBar.setSize({width, height});
    outerBar.setFillColor(sf::Color::Transparent);
    outerBar.setOutlineColor(sf::Color::Black);
    outerBar.setOutlineThickness(1.0f);

    innerBar.setSize({width, height});
    innerBar.setFillColor(sf::Color::Blue);
}

void PowerUpBar::setPosition(float x, float y) {
    outerBar.setPosition(x, y);
    innerBar.setPosition(x, y);
}

void PowerUpBar::update(float timePercent) {
    timePercent = std::clamp(timePercent, 0.0f, 100.0f);
    innerBar.setSize({outerBar.getSize().x * (timePercent / 100.0f), innerBar.getSize().y});
}

void PowerUpBar::draw(sf::RenderWindow &window) const {
    window.draw(outerBar);
    window.draw(innerBar);
}
