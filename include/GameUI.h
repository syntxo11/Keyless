#pragma once

#include <SFML/Graphics.hpp>

void showMenu(sf::RenderWindow &window, sf::Font &font);
bool showDied(sf::RenderWindow &window, sf::Font &font);
void showEndScreen(sf::Vector2f viewCenter, int gameTime, int enemiesKilled,
                   int keysCollected, sf::RenderWindow &window, sf::Font &font);
void drawTileMap(sf::RenderWindow &window, sf::Sprite &mapSprite);
