#pragma once
#include <SFML/Graphics.hpp>

class Enemy : public sf::Sprite {
public:
    static sf::Texture bossTexture;

    Enemy(const sf::Texture& texture, float startX, float startY, float speed);

    virtual void update(float deltaTime, const std::vector<int>& level, int levelWidth, int levelHeight , const sf::Vector2f& playerPosition);
    bool checkCollision(const sf::FloatRect& playerRect);
    static void loadTextures();
    void updateHealth(float attackDamage);
    float getHealth() const;
    float getAttack() const;
    static sf::Texture smallSlimeTexture;
    static sf::Texture bigSlimeTexture;
    static sf::Texture fatKnight;
    static sf::Texture shadow;

    virtual void drawWithHealthBar(sf::RenderWindow& window) const;

protected:
    float m_speed;
    sf::Vector2f velocity;
    float gravity = 981.f;
    float health ;
    float attack;
};
