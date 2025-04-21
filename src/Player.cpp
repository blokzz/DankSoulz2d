#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Player.h"
#include "iostream"

Player::Player() : speed(200.0f) , texture("player.png"), sprite(texture) {
    sprite.setTexture(texture);
    sprite.setScale({5.f, 5.f});
    sprite.setPosition({130.f, 400.f});
    sprite.setOrigin({0.f,sprite.getLocalBounds().position.y+sprite.getLocalBounds().size.y});

}
void Player::update(const int* tileMap, int mapWidth, int mapHeight, float deltaTime) {
    sf::Vector2f pos = sprite.getPosition();
    float tileSize = 32.f;
    sprite.setOrigin({5.f, sprite.getLocalBounds().position.y+sprite.getLocalBounds().size.y});
    if (isKeyPressed(sf::Keyboard::Key::Left)) {
        sprite.setScale({-5.f, 5.f});
        velocityX = -speed;
    }
    else if (isKeyPressed(sf::Keyboard::Key::Right)) {
        sprite.setScale({5.f, 5.f});
        velocityX = speed;
    }
    else {
        velocityX = 0.f;
    }

    if (isKeyPressed(sf::Keyboard::Key::Up) && isOnGround) {
        velocityY = jumpStrength;
        isOnGround = false;
    }
    const float gravity = 900.f;
    const float maxFallSpeed = 600.f;

    velocityY += gravity * deltaTime;

    if (velocityY > maxFallSpeed) {
        velocityY = maxFallSpeed;
    }

    sprite.move({velocityX * deltaTime, velocityY * deltaTime});
    velocityX = 0.f;

    sf::Vector2f newPos = sprite.getPosition();
    int newTileX = static_cast<int>(newPos.x / tileSize);
    int newTileY = static_cast<int>(newPos.y / tileSize);

    if (newTileY < mapHeight) {
        int belowIndex = (newTileY) * mapWidth + newTileX;
        int tileBelow = tileMap[belowIndex];

        if (tileBelow != 2) {
            isOnGround = true;
            velocityY = 0.f;
            sprite.setPosition(sf::Vector2f(newPos.x, static_cast<float>(newTileY) * tileSize));
        } else {
            isOnGround = false;
        }
    }
    sprite.move({velocityX * deltaTime, velocityY * deltaTime});
    velocityX = 0.f;
}



void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
