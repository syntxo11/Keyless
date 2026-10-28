#pragma once

#include <SFML/Graphics.hpp>

class Entity {
public:
    float dx = 0.0f;
    float dy = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    float speed = 0.0f;
    float moveTimer = 0.0f;
    float currentFrame = 0.0f;
    int w = 0;
    int h = 0;
    int health = 100;
    bool life = true;
    bool isMove = false;
    bool onGround = false;
    bool canClimb = false;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::String name;

    Entity(sf::Image &image, float X, float Y, int W, int H, sf::String Name);
    virtual ~Entity() = default;

};
