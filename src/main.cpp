#include <iostream>
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include <fstream>
#include "Enemy.h"
#include "Game.h"
int main() {
    Enemy::loadTextures();
    auto window = sf::RenderWindow(
        sf::VideoMode({800, 640}), "DankSoulz2d",
        sf::Style::Default, sf::State::Windowed,
        sf::ContextSettings{.antiAliasingLevel = 8}
    );

    sf::Font font;
    if (!font.openFromFile("../../src/Assets/arial.ttf")) {
        std::cerr << "blad ladowanaia czcionki dla menu!" << std::endl;
        return -1;
    }
    Menu menu(font, 800, 640);
    Game game(window);
    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        while (auto const event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (game.getState() == GameState::Menu) {
                if (auto const e = event->getIf<sf::Event::KeyPressed>()) {
                    if (e->code == sf::Keyboard::Key::Up) {
                        menu.moveUp();
                    } else if (e->code == sf::Keyboard::Key::Down) {
                        menu.moveDown();
                    } else if (e->code == sf::Keyboard::Key::Enter) {
                        if (menu.getSelectedIndex() == 0) {
                            game.setState(GameState::Playing);
                        }
                        else if (menu.getSelectedIndex() == 1) {
                            game.loadGame("../../src/Assets/save.txt");
                            game.setState(GameState::Playing);

                        }
                        else if (menu.getSelectedIndex() == 3) {
                            window.close();
                        }
                    }
                }
            }
            else if (game.getState() == GameState::Playing) {
                if (auto const e = event->getIf<sf::Event::KeyPressed>()) {
                    if (e->code == sf::Keyboard::Key::Tab) {
                        game.setState(GameState::Menu);
                    }
                }
            }
        }
        game.handleInput();
        game.update(deltaTime);
        game.render();

        if (game.getState() == GameState::Menu) {
            menu.draw(window);
        }

        window.display();
    }

    return 0;
}