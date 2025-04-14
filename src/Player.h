#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player {
public:
    Player();

    void handleInput(std::optional<sf::Event>event);
    void update();
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape shape;
    float speed;
    sf::Texture texture;
    sf::Sprite sprite;
};

#endif
