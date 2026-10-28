#pragma once

#include <SFML/Graphics.hpp>

inline sf::View view;

inline void getPlayerCoordinateForView(float x, float y) {
    float cameraX = x;
    float cameraY = y;

    if (x < 300.0f) cameraX = 300.0f;
    if (x > 960.0f) cameraX = 960.0f;
    if (y < 240.0f) cameraY = 240.0f;
    if (y > 240.0f) cameraY = 240.0f;

    view.setCenter(cameraX, cameraY);
}
