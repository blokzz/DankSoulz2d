#include "Menu.h"
#include "iostream"
Menu::Menu(const sf::Font& font, float width, float height)
    : sprite(texture)  {
    std::array<std::string, 4> labels = {"Start", "Wczytaj", "Opcje", "Wyjscie"};

    for (std::size_t i = 0; i < labels.size(); ++i) {
        sf::Text text(font, labels[i]);
        text.setFont(font);
        text.setString(labels[i]);
        text.setCharacterSize(36);
        text.setFillColor(i == selectedIndex ? sf::Color::Yellow : sf::Color::White);
        text.setPosition({width / 2.f - 60.f, height / 2.f + static_cast<float>(i * 50)});
        options.push_back(text);
    }


}

void Menu::draw(sf::RenderWindow& window) {
    sf::Texture texture;
    if (!texture.loadFromFile("logo.png")) {
        std::cerr << "Nie udało się załadować obrazka!\n";
    } else {
        std::cout << "Obrazek załadowany! Rozmiar: " << texture.getSize().x << "x" << texture.getSize().y << "\n";
    }
    sf::Sprite sprite(texture);
    sprite.setScale({0.1f, 0.1f});
    sprite.setPosition({170.f, 30.f});
    window.draw(sprite);
    for (auto& option : options)
        window.draw(option);
}


int Menu::getSelectedIndex() const {
    return selectedIndex;
}
