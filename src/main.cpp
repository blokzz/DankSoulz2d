#include <iostream>
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Player.h"
enum class GameState {
    Menu,
    Playing
};
auto main() -> int {
    auto window = sf::RenderWindow(
        sf::VideoMode({800, 600}), "DankSoulz2d",
        sf::Style::Default, sf::State::Windowed,
        sf::ContextSettings{.antiAliasingLevel = 8}
    );

    sf::Font font;
    if (!font.openFromFile("arial.ttf")) {
        std::cerr << "Czcionka NIE załadowana! Sprawdź ścieżkę lub plik!" << std::endl;
        return -1;
    } else {
        std::cout << "Czcionka załadowana poprawnie!" << std::endl;
    }
    GameState state = GameState::Menu;
    Menu menu(font, 800, 600);
    Player player;
    while (window.isOpen())
    {
        while (auto const event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (state == GameState::Menu) {
                if (auto const e = event->getIf<sf::Event::KeyPressed>()) {
                    if (e->code == sf::Keyboard::Key::Up) {
                        menu.moveUp();
                    }
                    else if (e->code == sf::Keyboard::Key::Down) {
                        menu.moveDown();
                    }
                    else if (e->code == sf::Keyboard::Key::Enter) {
                        if (menu.getSelectedIndex() == 0) {
                            state = GameState::Playing;
                        } else if (menu.getSelectedIndex() == 3) {
                            window.close();
                        }
                    }
                }
            }
            window.clear(sf::Color::Black);
            if (state == GameState::Menu) {
                menu.draw(window);
            } else if (state == GameState::Playing) {
                player.handleInput(event);
                player.draw(window);
            }
            window.display();
        }
    }
}