#include <SFML/Graphics.hpp>
#include "Player.h"
#include "iostream"
#include "Game.h"
Player::Player() : speed(200.0f) , texture("../../src/Assets/player.png"), sprite(texture) ,totalFlasks(3) , flasks(totalFlasks) {
    sprite.setTexture(texture);
    sprite.setScale({5.f, 5.f});
    sprite.setPosition({130.f, 400.f});
    sprite.setOrigin({0.f,sprite.getLocalBounds().position.y+sprite.getLocalBounds().size.y});
  if (!font.openFromFile("../../src/Assets/arial.ttf")) {

  };

}
void Player::attack() {
    sword.startAttack();
}
float Player::getPositionX() const {
    return sprite.getPosition().x;
}
float Player::getPositionY() const {
    return sprite.getPosition().y;
}

sf::FloatRect Player::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

float Player::getAttack() const {
    return attackPower;
}

void Player::setAttack() {
    attackPower +=10;
}

void Player::setHealth() {
    health+=10;
}

void Player::setPosition(float x, float y) {
    sprite.setPosition({x,y});
}
void Player::update(const int* tileMap, int mapWidth, int mapHeight, float deltaTime) {
    sword.update(deltaTime);
    if (health==0) {
        health = maxHealth;
        flasks = totalFlasks;
    }


    //unikanie

    if (!isDodging && !dodgeOnCooldown && isKeyPressed(sf::Keyboard::Key::X)) {
        isDodging = true;
        dodgeClock.restart();
        dodgeCooldownClock.restart();
        dodgeOnCooldown = true;
        isInvincible = true;
        invincibilityClock.restart();

        if (isKeyPressed(sf::Keyboard::Key::Left)) {
            dodgeDirection = {-1.f, 0.f};
        } else if (isKeyPressed(sf::Keyboard::Key::Right)) {
            dodgeDirection = {1.f, 0.f};
        } else {
            dodgeDirection = facingRight ? sf::Vector2f{1.f, 0.f} : sf::Vector2f{-1.f, 0.f};
        }
        if (isDodging) {
    if (dodgeClock.getElapsedTime().asSeconds() >= dodgeDuration) {
        isDodging = false;
    }
}

if (dodgeOnCooldown && dodgeCooldownClock.getElapsedTime().asSeconds() >= dodgeCooldown) {
    dodgeOnCooldown = false;
}

    }
    if (isDodging) {
        if (dodgeClock.getElapsedTime().asSeconds() >= dodgeDuration) {
            isDodging = false;
        }
    }

    if (dodgeOnCooldown && dodgeCooldownClock.getElapsedTime().asSeconds() >= dodgeCooldown) {
        dodgeOnCooldown = false;
    }



    //chodzenie


    float offsetX = 10.f;
    float offsetY = -75.f;

    sf::Vector2f playerPos = sprite.getPosition();
    sf::Vector2f swordPos;

    if (facingRight) {
        swordPos.x = playerPos.x + sprite.getGlobalBounds().size.x / 2.f - offsetX;
    } else {
        swordPos.x = playerPos.x - sprite.getGlobalBounds().size.x / 2.f + offsetX;
    }
    swordPos.y = playerPos.y + offsetY;

    sword.setPosition(swordPos.x, swordPos.y);

    if (facingRight) {
        sword.setScale(1.1f, 1.5f);
    } else {
        sword.setScale(-1.1f, 1.5f);
    }
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


    //skakanie

    if (isKeyPressed(sf::Keyboard::Key::Up) && isOnGround) {
        velocityY = jumpStrength;
        isOnGround = false;
    }
    velocityY += gravity * deltaTime;
    if (velocityY > maxFallSpeed) velocityY = maxFallSpeed;

    sf::Vector2f deltaMove;


    //znowu unikanie

    if (isDodging) {
        deltaMove.x = dodgeDirection.x * dodgeSpeed * deltaTime;
        deltaMove.y = 0.f;
    } else {
        deltaMove.x = velocityX * deltaTime;
        deltaMove.y = velocityY * deltaTime;
    }


    sf::FloatRect futureBounds = sprite.getGlobalBounds();
    futureBounds.position.x += deltaMove.x;
    int tileX = static_cast<int>((futureBounds.position.x + (deltaMove.x > 0 ? futureBounds.size.x : 0)) / tileSize);
    int tileY = static_cast<int>((futureBounds.position.y + futureBounds.size.y / 2.f) / tileSize);
    bool blocked = false;
    for (float yOffset : {1.f, futureBounds.size.y / 2.f, futureBounds.size.y - 1.f}) {
        int tileY2 = static_cast<int>((futureBounds.position.y + yOffset) / tileSize);
        int tileX2= static_cast<int>((futureBounds.position.x + (deltaMove.x > 0 ? futureBounds.size.x : 0)) / tileSize);

        if (tileX2 >= 0 && tileX2 < mapWidth && tileY2 >= 0 && tileY2 < mapHeight) {
            int tile = tileMap[tileY2 * mapWidth + tileX2];
            if (tile != 2 && tile!=6) {
                blocked = true;
                break;
            }
        }
    }
    if (blocked) {
        deltaMove.x = 0.f;
    }

    if (isKeyPressed(sf::Keyboard::Key::Left)) {
        facingRight = false;
    }
    if (isKeyPressed(sf::Keyboard::Key::Right)) {
        facingRight = true;
    }

    //kolizje skakanie

    futureBounds = sprite.getGlobalBounds();
    futureBounds.position.y += deltaMove.y;
    tileX = static_cast<int>((futureBounds.position.x + futureBounds.size.x / 2.f) / tileSize);
    tileY = static_cast<int>((futureBounds.position.y + futureBounds.size.y) / tileSize);

    if (tileX >= 0 && tileX < mapWidth && tileY >= 0 && tileY < mapHeight) {
        int tile = tileMap[tileY * mapWidth + tileX];
        if (tile != 2 && tile!=6 && velocityY > 0) {
            deltaMove.y = 0.f;
            isOnGround = true;
            velocityY = 0.f;
        } else {
            isOnGround = false;
        }
    }
    if (isInvincible && invincibilityClock.getElapsedTime().asSeconds() >= invincibilityDuration) {
        isInvincible = false;
    }
    if (damageCooldown > 0.f)
        damageCooldown -= deltaTime;

    drinkFlask();
    sprite.move(deltaMove);

}

sf::FloatRect Player::getAttackHitbox() const {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::Vector2f pos = sprite.getPosition();

    float width = 42.f;
    float height = bounds.size.y - 10.f;

    if (facingRight) {
        return {
    { pos.x + bounds.size.x/ 2.f, pos.y - bounds.size.y + 10.f },
    { width, height }
};

    } else {
        return {
    { pos.x - bounds.size.x / 2.f - width, pos.y - bounds.size.y + 10.f },
    { width, height }
};


    }
}

int Player::getHealth() const {
    return health;
}


void Player::takeDamage(float damage) {
    if (!isInvincible && damageCooldown <= 0.f) {
        std::cout << totalFlasks << std::endl;
        health -= damage;
        if (health < 0.f) {
            health = 0.f;
        }
        damageCooldown = damageCooldownMax;
    }
}
void Player::drinkFlask() {
    if (isKeyPressed(sf::Keyboard::Key::C)) {
        if (!flaskUsed && flasks > 0) {
            if (health + 100.f>maxHealth) {
                health = maxHealth;
            }
            else {
                health += 100.f;
            }
            flasks -= 1;
            flaskUsed = true;
        }
    } else {
        flaskUsed = false;
    }
}

void Player::reFillFlask() {
    flasks=totalFlasks;
    health = maxHealth;
}

void Player::upgradeFlask() {
totalFlasks +=1;
}

void Player::upgradeAttack(float power) {
   attackPower +=power;
}

void Player::drawHealthBar(sf::RenderWindow& window) const {
    float barWidth = 200.f;
    float barHeight = 20.f;
    float left = 10.f;
    float right = 50.f;

    sf::Text str(font,"Butelka estusa: "+ std::to_string(flasks), 13);
    str.setFillColor(sf::Color::White);
    str.setPosition({left, right+30.f});
    window.draw(str);

    sf::RectangleShape backgroundBar(sf::Vector2f(barWidth, barHeight));
    backgroundBar.setFillColor(sf::Color(50, 50, 50));
    backgroundBar.setPosition({left, right});

    float healthPercent = health / maxHealth;
    sf::RectangleShape healthBar(sf::Vector2f(barWidth * healthPercent, barHeight));
    healthBar.setFillColor(sf::Color::Red);
    healthBar.setPosition({left, right});

    window.draw(backgroundBar);
    window.draw(healthBar);
}

int Player::getTotalFlasks() const {
    return totalFlasks;
}

void Player::setTotalFlasks(int total) {
    totalFlasks = total;
    flasks = total;
}


void Player::draw(sf::RenderWindow& window) {
    if (showNotification && notificationClock.getElapsedTime().asSeconds() < 3.0f) {
        sf::Text notification(font);
        notification.setFont(font);
        notification.setString(notificationText);
        notification.setCharacterSize(20);
        notification.setFillColor(sf::Color::White);
        notification.setPosition({50.f, 100.f});

        window.draw(notification);
    } else {
        showNotification = false;
    }
    window.draw(sprite);
    sword.draw(window);
}
