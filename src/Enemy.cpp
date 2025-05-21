#include "Enemy.h"
#include <iostream>
#include <cmath>
sf::Texture Enemy::smallSlimeTexture;
sf::Texture Enemy::bigSlimeTexture;
sf::Texture Enemy::fatKnight;
sf::Texture Enemy::shadow;
sf::Texture Enemy::bossTexture;

Enemy::Enemy(const sf::Texture& texture, float startX, float startY, float speed)
    : sf::Sprite(texture), m_speed(speed) ,health(250.f),attack(10){;
    setPosition({startX, startY});
    velocity.x = 50.f;
    setScale({2.f,2.f});
    sf::FloatRect bounds = getLocalBounds();
    setOrigin({bounds.size.x / 2.f, bounds.size.y});
}

void Enemy::loadTextures() {
    if (!smallSlimeTexture.loadFromFile("../../src/Assets/enemy1.png")) {
        std::cerr << "Nie udało się załadować enemy1.png\n";
    }
    if (!bigSlimeTexture.loadFromFile("../../src/Assets/enemy2.png")) {
        std::cerr << "Nie udało się załadować enemy2.png\n";
    }
    if (!fatKnight.loadFromFile("../../src/Assets/enemy3.png")) {
        std::cerr << "Nie udało się załadować enemy2.png\n";
    }
    if (!shadow.loadFromFile("../../src/Assets/enemy4.png")) {
        std::cerr << "Nie udało się załadować enemy2.png\n";
    }
}
void Enemy::drawWithHealthBar(sf::RenderWindow& window) const {
    window.draw(*this);

    sf::RectangleShape backgroundBar(sf::Vector2f(40.f, 6.f));
    backgroundBar.setFillColor(sf::Color(50, 50, 50));
    backgroundBar.setPosition({getPosition().x - 20.f, getPosition().y - getGlobalBounds().size.y - 10.f});

    float maxHealth = 250.f;
    float healthPercent = health / maxHealth;

    sf::RectangleShape healthBar(sf::Vector2f(40.f * healthPercent, 6.f));
    healthBar.setFillColor(
        healthPercent > 0.6f ? sf::Color::Green :
        (healthPercent > 0.3f ? sf::Color::Yellow : sf::Color::Red)
    );
    healthBar.setPosition(backgroundBar.getPosition());

    window.draw(backgroundBar);
    window.draw(healthBar);
}



void Enemy::update(float deltaTime, const std::vector<int>& level, int levelWidth, int levelHeight , const sf::Vector2f& playerPosition) {
    velocity.y += gravity * deltaTime;

    const float probeDistance = 2.f;
    sf::Vector2f pos = getPosition();
    sf::FloatRect bounds = getGlobalBounds();

    float checkX = pos.x + (velocity.x > 0 ? bounds.size.x / 2.f + probeDistance : -bounds.size.x / 2.f - probeDistance);
    float checkTopY = pos.y - bounds.size.y + 4.f;
    float checkBottomY = pos.y - 2.f;

    int tileX = static_cast<int>(checkX / 32);
    int tileYTop = static_cast<int>(checkTopY / 32);
    int tileYBottom = static_cast<int>(checkBottomY / 32);

    bool blocked = false;
    if (tileX < 0 || tileX >= levelWidth) {
        blocked = true;
    } else {
        if (tileYTop >= 0 && tileYTop < levelHeight) {
            if (level[tileYTop * levelWidth + tileX] != 2) {
                blocked = true;
            }
        }
        if (tileYBottom >= 0 && tileYBottom < levelHeight) {
            if (level[tileYBottom * levelWidth + tileX] != 2) {
                blocked = true;
            }
        }
    }

    if (blocked) {
        velocity.x = -velocity.x;
        setScale({-getScale().x, getScale().y});
    }
    float edgeX = pos.x + (velocity.x > 0 ? bounds.size.x / 2.f + 1.f : -bounds.size.y / 2.f - 1.f);
    float edgeY = pos.y + 1.f;

    int tileEdgeX = static_cast<int>(edgeX / 32);
    int tileEdgeY = static_cast<int>(edgeY / 32);

    bool shouldTurnAround = false;
    if (tileEdgeX < 0 || tileEdgeX >= levelWidth || tileEdgeY < 0 || tileEdgeY >= levelHeight) {
        shouldTurnAround = true;
    } else {
        if (level[tileEdgeY * levelWidth + tileEdgeX] == 2) {
            shouldTurnAround = true;
        }
    }

    if (shouldTurnAround) {
        velocity.x = -velocity.x;
        setScale({-getScale().x, getScale().y});
    }

    move(velocity * deltaTime);

    pos = getPosition();
    float footX = pos.x;
    float footY = pos.y;

    int tileFootX = static_cast<int>(footX / 32);
    int tileFootY = static_cast<int>(footY / 32);

    if (tileFootX >= 0 && tileFootX < levelWidth && tileFootY >= 0 && tileFootY < levelHeight) {
        int belowIndex = tileFootY * levelWidth + tileFootX;

        if (level[belowIndex] != 2) {
            setPosition({pos.x, static_cast<float>(tileFootY * 32)});
            velocity.y = 0.f;
        }
    }
    sf::Vector2f enemyPos = getPosition();
    float dx = playerPosition.x - enemyPos.x;
    float dy = playerPosition.y - enemyPos.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    const float chaseRange = 200.f;
    if (distance < chaseRange) {
        velocity.x = (dx > 0 ? m_speed : -m_speed);
        if ((dx > 0 && getScale().x < 0) || (dx < 0 && getScale().x > 0)) {
            setScale({ -getScale().x, getScale().y });
        }
    }

}

void Enemy::updateHealth(float attackDamage) {
    this->health -= attackDamage;
}

float Enemy::getHealth() const {

    return this->health;
}

float Enemy::getAttack() const {
    return this->attack;
}



bool Enemy::checkCollision(const sf::FloatRect& playerRect) {
    return getGlobalBounds().findIntersection(playerRect).has_value();
}
