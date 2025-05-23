#ifndef PLAYER_H
#define PLAYER_H
#include "Sword.h"
#include <SFML/Graphics.hpp>
#pragma once
class Player {
public:
    Player();
    void update(const int* tileMap, int mapWidth, int mapHeight, float deltaTime);
    void draw(sf::RenderWindow& window);
    float getPositionX() const;
    float getPositionY() const;
    void setPosition(float x, float y);
    sf::FloatRect getGlobalBounds() const;
    sf::FloatRect getAttackHitbox() const;
    void attack();
    float getAttack() const;
    void setAttack();
    void setHealth();
    void takeDamage(float damage);
    int getHealth() const;
    void upgradeAttack(float power);
    void drawHealthBar(sf::RenderWindow& window) const;
    void drinkFlask();
    void reFillFlask();
    void upgradeFlask();
    std::string notificationText;
    sf::Clock notificationClock;
    bool showNotification = false;
    int getTotalFlasks() const;
    void setTotalFlasks(int total);
private:
    sf::Font font;
    int totalFlasks;
    bool flaskUsed = false;
    int flasks ;
    float damageCooldown = 0.f;  
    const float damageCooldownMax = 0.5f;
    bool isInvincible = false;
    sf::Clock invincibilityClock;
    float invincibilityDuration = 0.8f;
    bool isDodging = false;
    sf::Clock dodgeClock;
    float dodgeDuration = 0.25f;
    float dodgeSpeed = 600.f;
    sf::Vector2f dodgeDirection = {0.f, 0.f};
    bool dodgeOnCooldown = false;
    sf::Clock dodgeCooldownClock;
    float dodgeCooldown = 0.6f;
    float velocityX = 0.f;
    bool isOnGround = false;
    float velocityY = 0.f;
    float gravity = 1000.f;
    float jumpStrength = -450.f;
    sf::RectangleShape shape;
    float speed;
    sf::Texture texture;
    sf::Sprite sprite;
    float exp = 0;
    float attackPower = 50;
    float health = 100.f;
    float maxHealth = 100.f;

    bool facingRight = true;
    Sword sword;
};

#endif
