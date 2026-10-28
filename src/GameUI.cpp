#include "GameUI.h"

#include "map.h"
#include "view.h"

#include <sstream>

using namespace sf;

void showMenu(RenderWindow &window, Font &font) {
    Text title("Keyless", font, 70);
    title.setFillColor(Color::Red);
    title.setStyle(Text::Bold);
    title.setPosition(window.getSize().x / 2.0f - title.getLocalBounds().width / 2.0f, 100.0f);

    Text subtitle("An Epic Samurai Adventure", font, 30);
    subtitle.setFillColor(Color::White);
    subtitle.setStyle(Text::Bold);
    subtitle.setPosition(window.getSize().x / 2.0f - subtitle.getLocalBounds().width / 2.0f, 200.0f);

    Text start("Press Enter to Start", font, 40);
    start.setFillColor(Color::White);
    start.setStyle(Text::Bold);
    start.setPosition(window.getSize().x / 2.0f - start.getLocalBounds().width / 2.0f, 300.0f);

    window.clear(Color::Black);
    window.draw(title);
    window.draw(subtitle);
    window.draw(start);
    window.display();

    while (window.isOpen()) {
        Event event{};
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                return;
            }
        }
    }
}

bool showDied(RenderWindow &window, Font &font) {
    Text gameOverText("You died", font, 40);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(view.getCenter().x - gameOverText.getLocalBounds().width / 2.0f,
                             view.getCenter().y - 200.0f);

    Text restartText("Press R to Restart or Q to Quit", font, 10);
    restartText.setFillColor(Color::White);
    restartText.setStyle(Text::Bold);
    restartText.setPosition(view.getCenter().x - restartText.getLocalBounds().width / 2.0f,
                            view.getCenter().y - 100.0f);

    window.clear(Color::Black);
    window.draw(gameOverText);
    window.draw(restartText);
    window.display();

    while (window.isOpen()) {
        Event event{};
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return false;
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::R) return true;
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Q) {
                window.close();
                return false;
            }
        }
    }
    return false;
}

void showEndScreen(Vector2f viewCenter, int gameTime, int enemiesKilled,
                   int keysCollected, RenderWindow &window, Font &font) {
    Text endText("Thanks for playing", font, 30);
    endText.setFillColor(Color::Red);
    endText.setPosition(viewCenter.x - endText.getLocalBounds().width / 2.0f, viewCenter.y - 200.0f);

    std::ostringstream statsStream;
    statsStream << "Time played: " << gameTime << " seconds\n"
                << "Enemies killed: " << enemiesKilled << "\n"
                << "Keys collected: " << keysCollected;

    Text statsText(statsStream.str(), font, 20);
    statsText.setFillColor(Color::White);
    statsText.setPosition(viewCenter.x - statsText.getLocalBounds().width / 2.0f, viewCenter.y - 100.0f);

    window.clear(Color::Black);
    window.draw(endText);
    window.draw(statsText);
    window.display();

    while (window.isOpen()) {
        Event event{};
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Q) {
                window.close();
                return;
            }
        }
    }
}

void drawTileMap(RenderWindow &window, Sprite &s_map) {
    for (int i = 0; i < HEIGHT_MAP; ++i) {
        for (int j = 0; j < WIDTH_MAP; ++j) {
            const char tile = static_cast<char>(TileMap[i][j]);

            if (tile == ' ') s_map.setTextureRect(IntRect(32, 33, 16, 16));
            if (tile == 'a') s_map.setTextureRect(IntRect(32, 16, 16, 16));
            if (tile == 'b') s_map.setTextureRect(IntRect(49, 33, 16, 16));
            if (tile == 'c') s_map.setTextureRect(IntRect(16, 33, 16, 16));
            if (tile == 'd') s_map.setTextureRect(IntRect(32, 51, 16, 16));
            if (tile == 's') s_map.setTextureRect(IntRect(1, 1, 16, 16));

            if (tile == 'k') s_map.setTextureRect(IntRect(176, 143, 16, 16));
            if (tile == 'z') s_map.setTextureRect(IntRect(160, 16, 16, 16));
            if (tile == 'x') s_map.setTextureRect(IntRect(176, 16, 16, 16));
            if (tile == 'n') s_map.setTextureRect(IntRect(160, 32, 16, 16));
            if (tile == 'v') s_map.setTextureRect(IntRect(176, 32, 16, 16));

            if (tile == '1') s_map.setTextureRect(IntRect(161, 225, 16, 16));
            if (tile == '2') s_map.setTextureRect(IntRect(176, 225, 16, 16));
            if (tile == '3') s_map.setTextureRect(IntRect(192, 225, 16, 16));
            if (tile == '4') s_map.setTextureRect(IntRect(208, 225, 16, 16));

            if (tile == '!' || tile == ';' || tile == ':') s_map.setTextureRect(IntRect(32, 33, 16, 16));
            if (tile == 'g') s_map.setTextureRect(IntRect(208, 161, 16, 16));
            if (tile == '0') s_map.setTextureRect(IntRect(208, 176, 16, 16));
            if (tile == '[') s_map.setTextureRect(IntRect(191, 161, 16, 16));
            if (tile == ']') s_map.setTextureRect(IntRect(191, 176, 16, 16));

            if (tile == 'u') s_map.setTextureRect(IntRect(80, 144, 16, 16));
            if (tile == 'p') s_map.setTextureRect(IntRect(80, 176, 16, 16));
            if (tile == '>') s_map.setTextureRect(IntRect(112, 160, 16, 16));
            if (tile == '<') s_map.setTextureRect(IntRect(129, 160, 16, 16));
            if (tile == 'i') s_map.setTextureRect(IntRect(48, 224, 16, 16));

            if (tile == 'Y') s_map.setTextureRect(IntRect(48, 187, 16, 16));
            if (tile == 'y') s_map.setTextureRect(IntRect(32, 203, 16, 16));
            if (tile == '5') s_map.setTextureRect(IntRect(64, 208, 16, 16));
            if (tile == '6') s_map.setTextureRect(IntRect(80, 208, 16, 16));
            if (tile == '7') s_map.setTextureRect(IntRect(96, 208, 16, 16));

            if (tile == '{') s_map.setTextureRect(IntRect(120, 237, 16, 16));
            if (tile == '}') s_map.setTextureRect(IntRect(120, 253, 16, 16));
            if (tile == '/') s_map.setTextureRect(IntRect(137, 237, 16, 16));
            if (tile == '|') s_map.setTextureRect(IntRect(137, 253, 16, 16));

            if (tile == '(') s_map.setTextureRect(IntRect(176, 248, 16, 16));
            if (tile == ')') s_map.setTextureRect(IntRect(176, 264, 16, 16));
            if (tile == '.') s_map.setTextureRect(IntRect(192, 248, 16, 16));
            if (tile == ',') s_map.setTextureRect(IntRect(192, 264, 16, 16));

            if (tile == 'r') s_map.setTextureRect(IntRect(64, 240, 16, 16));
            if (tile == 'l') s_map.setTextureRect(IntRect(48, 240, 16, 16));
            if (tile == 'h') s_map.setTextureRect(IntRect(48, 256, 16, 16));
            if (tile == 'f') s_map.setTextureRect(IntRect(64, 256, 16, 16));

            if (tile == 'P') s_map.setTextureRect(IntRect(16, 144, 48, 16));
            if (tile == 'M') s_map.setTextureRect(IntRect(16, 144, 16, 16));
            if (tile == 'L') s_map.setTextureRect(IntRect(32, 160, 16, 16));
            if (tile == 'W') s_map.setTextureRect(IntRect(48, 144, 16, 16));

            if (tile == 'H') s_map.setTextureRect(IntRect(192, 143, 16, 16));
            if (tile == 'O') s_map.setTextureRect(IntRect(208, 143, 16, 16));

            if (tile == 'R') s_map.setTextureRect(IntRect(94, 238, 16, 16));
            if (tile == 'U') s_map.setTextureRect(IntRect(110, 238, -16, 16));
            if (tile == 'T') s_map.setTextureRect(IntRect(94, 255, 16, 16));

            s_map.setPosition(j * 16.0f, i * 16.0f);
            window.draw(s_map);
        }
    }
}
