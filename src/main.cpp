#include "Enemy.h"
#include "GameUI.h"
#include "Hud.h"
#include "Player.h"
#include "map.h"
#include "view.h"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

using namespace sf;

int main() {
    RenderWindow window(VideoMode::getDesktopMode(), "Keyless", Style::Default, ContextSettings(0, 0, 8));
    view.reset(FloatRect(0, 0, 640, 480));

    Font font;
    if (!font.loadFromFile("assets/fonts/8B.ttf")) {
        std::cerr << "Failed to load font: assets/fonts/8B.ttf\n";
        return -1;
    }

    Image mapImage;
    if (!mapImage.loadFromFile("assets/images/Dark.png")) {
        std::cerr << "Failed to load map image: assets/images/Dark.png\n";
        return -1;
    }

    Texture mapTexture;
    mapTexture.loadFromImage(mapImage);
    Sprite mapSprite;
    mapSprite.setTexture(mapTexture);

    Image playerImage;
    if (!playerImage.loadFromFile("assets/images/ninga5.png")) {
        std::cerr << "Failed to load player image: assets/images/ninga5.png\n";
        return -1;
    }

    Image enemyImage;
    if (!enemyImage.loadFromFile("assets/images/slime.png")) {
        std::cerr << "Failed to load enemy image: assets/images/slime.png\n";
        return -1;
    }

    showMenu(window, font);
    if (!window.isOpen()) return 0;

    // Keep the window alive across restarts while rebuilding all per-run state.
    while (window.isOpen()) {
        resetTileMap();
        view.reset(FloatRect(0, 0, 640, 480));
        window.setView(view);

        Player player(playerImage, 50, 350, 28, 22, "Player1");
        std::vector<std::unique_ptr<Enemy>> enemies;
        enemies.emplace_back(std::make_unique<Enemy>(enemyImage, 65 * 16, 350, 57, 33, "EasyEnemy"));
        enemies.emplace_back(std::make_unique<Enemy>(enemyImage, 60, 222 + 16, 57, 33, "EasyEnemy"));
        enemies.emplace_back(std::make_unique<Enemy>(enemyImage, 70 * 16, 190 - 16 * 3, 57, 33, "EasyEnemy"));
        const int initialEnemyCount = static_cast<int>(enemies.size());

        HealthBar healthBar(400, 5);
        PowerUpBar powerUpBar(200, 3);
        Clock gameTimeClock;
        Clock frameClock;
        int gameTime = 0;
        bool restartGame = false;

        while (window.isOpen() && !restartGame) {
            float time = frameClock.getElapsedTime().asMicroseconds();
            frameClock.restart();
            time /= 700.0f;

            if (player.life) {
                gameTime = gameTimeClock.getElapsedTime().asSeconds();
            }

            Event event{};
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::R) {
                    restartGame = true;
                }
            }

            if (!window.isOpen() || restartGame) break;

            player.update(time, enemies);
            for (auto &enemy : enemies) {
                enemy->update(time, player);
            }

            enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                         [](const std::unique_ptr<Enemy> &enemy) {
                                             return !enemy->life;
                                         }),
                          enemies.end());

            if (player.showEndScreen) {
                const int enemiesKilled = initialEnemyCount - static_cast<int>(enemies.size());
                showEndScreen(view.getCenter(), gameTime, enemiesKilled, player.keys, window, font);
                return 0;
            }

            window.setView(view);
            window.clear();
            drawTileMap(window, mapSprite);

            healthBar.setPosition(view.getCenter().x - 320, view.getCenter().y - 230);
            healthBar.update(static_cast<float>(player.health));

            Text gameTimeText("", font, 17);
            gameTimeText.setFillColor(Color::White);
            gameTimeText.setString(std::to_string(gameTime));
            gameTimeText.setPosition(view.getCenter().x + 260, view.getCenter().y - 230);
            window.draw(gameTimeText);

            if (player.powerUpActive) {
                int remainingTime = std::chrono::duration_cast<std::chrono::seconds>(
                    player.powerUpEndTime - std::chrono::steady_clock::now()).count();
                remainingTime = std::max(0, remainingTime);
                const float timePercent = (remainingTime / 60.0f) * 100.0f;
                powerUpBar.setPosition(view.getCenter().x - 320, view.getCenter().y - 210);
                powerUpBar.update(timePercent);
                powerUpBar.draw(window);
            }

            for (const auto &enemy : enemies) {
                window.draw(enemy->sprite);
            }
            window.draw(player.sprite);
            healthBar.draw(window);
            window.display();

            if (!player.life) {
                const bool wantsRestart = showDied(window, font);
                if (!wantsRestart) return 0;
                restartGame = true;
            }
        }
    }

    return 0;
}
