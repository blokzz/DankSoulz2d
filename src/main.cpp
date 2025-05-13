#include <iostream>
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Player.h"
#include "TileMap.cpp"
#include <fstream>
#include "loadLevelFromFile.h"
#include "Enemy.h"
#include "Bonefire.h"
#include "Chest.h"
#include "Game.h"



int main() {
    // Inicjalizacja tekstur wrogów
    Enemy::loadTextures();

    // Utworzenie okna
    auto window = sf::RenderWindow(
        sf::VideoMode({800, 640}), "DankSoulz2d",
        sf::Style::Default, sf::State::Windowed,
        sf::ContextSettings{.antiAliasingLevel = 8}
    );

    // Ładowanie czcionki dla menu
    sf::Font font;
    if (!font.openFromFile("arial.ttf")) {
        std::cerr << "Czcionka NIE załadowana!" << std::endl;
        return -1;
    }
    // Utworzenie menu
    Menu menu(font, 800, 640);

    // Utworzenie obiektu gry
    Game game(window);

    // Zegar dla obliczania delta time
    sf::Clock clock;

    // Główna pętla gry
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // Obsługa zdarzeń
        while (auto const event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Obsługa menu
            if (game.getState() == GameState::Menu) {
                if (auto const e = event->getIf<sf::Event::KeyPressed>()) {
                    if (e->code == sf::Keyboard::Key::Up) {
                        menu.moveUp();
                    } else if (e->code == sf::Keyboard::Key::Down) {
                        menu.moveDown();
                    } else if (e->code == sf::Keyboard::Key::Enter) {
                        if (menu.getSelectedIndex() == 0) {
                            game.setState(GameState::Playing);
                        } else if (menu.getSelectedIndex() == 3) {
                            window.close();
                        }
                    }
                }
            }

            // Obsługa ataków
            if (game.getState() == GameState::Playing) {
                if (auto const e = event->getIf<sf::Event::KeyPressed>()) {
                    if (e->code == sf::Keyboard::Key::Z) {
                        // Obsługa ataku została przeniesiona do metody handleInput w klasie Game
                    }
                }
            }
        }

        // Obsługa wejścia, aktualizacja i renderowanie
        game.handleInput();
        game.update(deltaTime);
        game.render();

        // Rysowanie menu jeśli potrzeba
        if (game.getState() == GameState::Menu) {
            menu.draw(window);
        }

        window.display();
    }

    return 0;
}