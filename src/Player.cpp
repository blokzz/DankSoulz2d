#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Player.h"
#include "iostream"
Player::Player() : speed(200.0f) , texture("player.png"), sprite(texture) ,totalFlasks(3) , flasks(totalFlasks) {
    sprite.setTexture(texture);
    sprite.setScale({5.f, 5.f});
    sprite.setPosition({130.f, 400.f});
    sprite.setOrigin({0.f,sprite.getLocalBounds().position.y+sprite.getLocalBounds().size.y});
  if (!font.openFromFile("arial.ttf")) {

  };


}
void Player::attack() {
    sword.startAttack();  // Rozpocznij animację ataku mieczem
}
float Player::getPositionX() {
    return sprite.getPosition().x;
}
float Player::getPositionY() {
    return sprite.getPosition().y;
}

sf::FloatRect Player::getGlobalBounds() {
    return sprite.getGlobalBounds();
}

float Player::getAttack() {

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
    // Uruchamianie uniku
    if (!isDodging && !dodgeOnCooldown && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) {
        isDodging = true;
        dodgeClock.restart();
        dodgeCooldownClock.restart();
        dodgeOnCooldown = true;
        isInvincible = true;
        invincibilityClock.restart();

        // Kierunek uniku = ostatni kierunek ruchu lub patrzenia
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            dodgeDirection = {-1.f, 0.f};
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
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


    float offsetX = 10.f; // jak daleko od środka gracza
    float offsetY = -75.f; // jak nisko względem gracza

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
        sword.setScale(1.1f, 1.5f); // normalna orientacja
    } else {
        sword.setScale(-1.1f, 1.5f); // lustrzane odbicie
    }
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
    sf::Vector2f deltaMove;

    if (isDodging) {
        deltaMove.x = dodgeDirection.x * dodgeSpeed * deltaTime;
        deltaMove.y = 0.f; // nie dashujemy w górę
    } else {
        deltaMove.x = velocityX * deltaTime;
        deltaMove.y = velocityY * deltaTime;
    }


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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        facingRight = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        facingRight = true;
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
        return sf::FloatRect(
    { pos.x + bounds.size.x/ 2.f, pos.y - bounds.size.y + 10.f },
    { width, height }
);

    } else {
        return sf::FloatRect(
    { pos.x - bounds.size.x / 2.f - width, pos.y - bounds.size.y + 10.f },
    { width, height }
);


    }
}

void Player::takeDamage(float damage) {
    if (!isInvincible && damageCooldown <= 0.f) {
        std::cout << totalFlasks << std::endl;
        health -= damage;
        if (health < 0.f) health = 0.f;
        damageCooldown = damageCooldownMax;
    }
}
void Player::drinkFlask() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) {
        if (!flaskUsed && flasks > 0) {
            if (health + 100.f>maxHealth) {
                health = maxHealth;
            }
            else {
                health += 100.f;
            }
            flasks -= 1;
            flaskUsed = true;  // oznacz, że został użyty
        }
    } else {
        flaskUsed = false; // reset gdy przycisk puszczony
    }
}

void Player::reFillFlask() {
    flasks=totalFlasks;
    health = maxHealth;
}

void Player::upgradeFlask() {
totalFlasks +=1;
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

    // Tło paska (szare)
    sf::RectangleShape backgroundBar(sf::Vector2f(barWidth, barHeight));
    backgroundBar.setFillColor(sf::Color(50, 50, 50));
    backgroundBar.setPosition({left, right});

    // Pasek zdrowia (czerwony)
    float healthPercent = health / maxHealth;
    sf::RectangleShape healthBar(sf::Vector2f(barWidth * healthPercent, barHeight));
    healthBar.setFillColor(sf::Color::Red);
    healthBar.setPosition({left, right});

    window.draw(backgroundBar);
    window.draw(healthBar);
}



void Player::draw(sf::RenderWindow& window) {
    if (showNotification && notificationClock.getElapsedTime().asSeconds() < 3.0f) {
        sf::Text notification(font);
        notification.setFont(font); // załaduj wcześniej font
        notification.setString(notificationText);
        notification.setCharacterSize(20);
        notification.setFillColor(sf::Color::White);
        notification.setPosition({50.f, 100.f}); // np. środek ekranu lub górny pasek

        window.draw(notification);
    } else {
        showNotification = false;
    }
    window.draw(sprite);
    sword.draw(window);
}
