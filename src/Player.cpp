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
    const float tileSize = 32.f;
    const float gravity = 900.f;
    const float maxFallSpeed = 600.f;
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
    } // Grawitacja
    velocityY += gravity * deltaTime;
    if (velocityY > maxFallSpeed) velocityY = maxFallSpeed;

    // Oblicz nową pozycję
    sf::Vector2f newPos = sprite.getPosition();
    sf::Vector2f deltaMove{velocityX * deltaTime, velocityY * deltaTime};

    // Kolizje poziome
    sf::FloatRect futureBounds = sprite.getGlobalBounds();
    futureBounds.position.x += deltaMove.x;
    int tileX = static_cast<int>((futureBounds.position.x + (deltaMove.x > 0 ? futureBounds.size.x : 0)) / tileSize);
    int tileY = static_cast<int>((futureBounds.position.y + futureBounds.size.y / 2.f) / tileSize);

    if (tileX >= 0 && tileX < mapWidth && tileY >= 0 && tileY < mapHeight) {
        int tile = tileMap[tileY * mapWidth + tileX];
        if (tile != 2) {
            deltaMove.x = 0.f; // Blokujemy ruch
        }
    }

    // Kolizje pionowe (głównie z dołu)
    futureBounds = sprite.getGlobalBounds();
    futureBounds.position.y += deltaMove.y;
    tileX = static_cast<int>((futureBounds.position.x + futureBounds.size.x / 2.f) / tileSize);
    tileY = static_cast<int>((futureBounds.position.y + futureBounds.size.y) / tileSize);

    if (tileX >= 0 && tileX < mapWidth && tileY >= 0 && tileY < mapHeight) {
        int tile = tileMap[tileY * mapWidth + tileX];
        if (tile != 2 && velocityY > 0) {
            deltaMove.y = 0.f;
            isOnGround = true;
            velocityY = 0.f;
        } else {
            isOnGround = false;
        }
    }

    sprite.move(deltaMove);
}



void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
