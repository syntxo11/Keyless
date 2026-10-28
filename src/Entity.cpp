#include "Entity.h"
#include "game_config.h"

Entity::Entity(sf::Image &image, float X, float Y, int W, int H, sf::String Name)
    : x(X), y(Y), w(W), h(H), health(keyless::config::PLAYER_MAX_HEALTH), name(std::move(Name)) {
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sprite.setOrigin(w / 2.0f, h / 2.0f);
}

