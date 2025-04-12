#include "Menu.h"
#include "iostream"
Menu::Menu(const sf::Font& font, float width, float height): texture("logo.png"), sprite(texture) {
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
        sprite.setTexture(texture);
        sprite.setScale({0.1f, 0.1f});
        sprite.setPosition({170.f, 30.f});
        std::cout << "Obrazek załadowany! Rozmiar: " << texture.getSize().x << "x" << texture.getSize().y << "\n";
}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    for (auto& option : options)
        window.draw(option);
}


int Menu::getSelectedIndex() const {
    return selectedIndex;
}
void Menu::moveUp() {
    if (selectedIndex > 0) {
        options[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex--;
        options[selectedIndex].setFillColor(sf::Color::Yellow);
    }
}

void Menu::moveDown() {
    if (selectedIndex < options.size() - 1) {
        options[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        options[selectedIndex].setFillColor(sf::Color::Yellow);
    }
}
