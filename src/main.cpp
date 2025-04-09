#include <iostream>
#include <SFML/Graphics.hpp>
#include "Menu.h"
enum class GameState {
    Menu,
    Playing
};
auto main() -> int
{
    auto window = sf::RenderWindow(
        sf::VideoMode({800, 600}), "Test",
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
    Menu menu(font, 800, 600);
    while (window.isOpen())
    {
        while (auto const event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        window.clear(sf::Color::Black);
        menu.draw(window);
        window.display();
    }
}