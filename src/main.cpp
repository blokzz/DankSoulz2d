#include <iostream>
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Player.h"
#include "TileMap.cpp"
enum class GameState {
    Menu,
    Playing
};
auto main() -> int {
    auto window = sf::RenderWindow(
        sf::VideoMode({800, 640}), "DankSoulz2d",
        sf::Style::Default, sf::State::Windowed,
        sf::ContextSettings{.antiAliasingLevel = 8}
    );
    constexpr std::array level = {
        2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 2,2,2,2,2,2,2,2,2,2,
        2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 2,2,2,2,2,2,2,2,2,2,
        2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 2,2,2,2,2,2,2,2,2,2,
        2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 2,2,2,2,2,2,2,2,2,2,
        2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 2,2,2,2,2,2,2,2,2,2,
        2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 2,2,2,2,2,2,2,2,2,2,
        2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 2,2,2,2,2,2,2,2,2,2,
        2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 2,2,2,2,2,2,2,2,2,2,
        2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 2,2,2,2,2,2,2,2,2,2,
        2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 2,2,2,2,2,2,2,2,2,2,
        2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 2,2,2,2,2,2,2,2,2,2,
        2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 2,2,2,2,2,2,2,2,2,2,
        2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 3,2,2,2,2,2,2,2,2,2,
        2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 4,2,2,2,2,2,2,2,2,2,
        2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 4,2,2,2,2,2,2,2,2,2,
        2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 4,2,2,2,2,2,2,2,2,2,
        2, 2, 2, 2, 2, 2, 2, 2,2, 2, 2, 2,2, 2, 2, 5,2,2,2,2,2,2,2,2,2,
        0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,
        0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,
        0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0
    };
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("ds3.png")) {
        std::cerr << "Nie udało się załadować tła!\n";
    }
    sf::Sprite background(backgroundTexture);
    background.setScale({800.f / backgroundTexture.getSize().x, 640.f / backgroundTexture.getSize().y}); // dopasuj do okna

    TileMap map;
    if (!map.load("Tile.png", {32, 32}, level.data(), 25, 20)) {
        std::cerr << "Nie udało się załadować mapy!\n";
        return -1;
    }
    map.setPosition({0.f, 0.f});
    map.setScale({1.f, 1.f});

    sf::Font font;
    if (!font.openFromFile("arial.ttf")) {
        std::cerr << "Czcionka NIE załadowana! Sprawdź ścieżkę lub plik!" << std::endl;
        return -1;
    } else {
        std::cout << "Czcionka załadowana poprawnie!" << std::endl;
    }
    GameState state = GameState::Menu;
    Menu menu(font, 800, 640);
    Player player;
    sf::Clock clock;
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();


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


        }

        if (state == GameState::Playing) {
            player.update(level.data(), 25, 20, deltaTime);
        }

        window.clear(sf::Color::Black);
        if (state == GameState::Menu) {
            menu.draw(window);
        } else if (state == GameState::Playing) {
            window.draw(background);
            window.draw(map);
            player.draw(window);
        }
        window.display();
    }

}