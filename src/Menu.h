#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include <vector>

class Menu {
public:
    Menu(sf::Font const& font, float width, float height);
    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    int getSelectedIndex() const;
private:
    std::vector<sf::Text> options;
    std::size_t selectedIndex = 0;
    sf::Texture texture;
    sf::Sprite sprite;

};

#endif