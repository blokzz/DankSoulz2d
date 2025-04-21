#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    void update(const int* tileMap, int mapWidth, int mapHeight, float deltaTime);
    void draw(sf::RenderWindow& window);

private:
    float velocityX = 0.f;
    bool isOnGround = false;
    float velocityY = 0.f;
    float gravity = 1000.f;
    float jumpStrength = -450.f;
    sf::RectangleShape shape;
    float speed;
    sf::Texture texture;
    sf::Sprite sprite;
};

#endif
