// Boss.h
#pragma once
#include "Enemy.h"

class Boss : public Enemy {
public:
    Boss(const sf::Texture& texture, float x, float y, float speed);
    void update(float deltaTime, const std::vector<int>& level, int levelWidth, int levelHeight , const sf::Vector2f& playerPosition) override;
    void drawWithHealthBar(sf::RenderWindow& window) const override;
    void drawBossBar(sf::RenderWindow& window) const;
    // Można dodać unikalne zdolności
    void phaseLogic();

};
