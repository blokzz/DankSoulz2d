
#include "Boss.h"

Boss::Boss(const sf::Texture& texture, float x, float y, float speed)
    : Enemy(texture, x, y, speed)
{
    health = 3000; // więcej życia
    attack = 30;  // silniejszy atak
    m_speed = 100;
    // inne cechy bossa
    setScale({4.f,7.f});
}

void Boss::update(float deltaTime, const std::vector<int>& level, int levelWidth, int levelHeight , const sf::Vector2f& playerPosition)
{
    Enemy::update(deltaTime, level, levelWidth, levelHeight, playerPosition);
    phaseLogic(); // np. zmiana zachowania poniżej 50% HP
}

void Boss::phaseLogic() {
    if (health < 200) {
        m_speed = 100.f; // boss się wścieka i jest szybszy
    }
}



void Boss::drawWithHealthBar(sf::RenderWindow& window) const {
    window.draw(*this);
    // możesz dodać specjalny pasek bossowy
}
void Boss::drawBossBar(sf::RenderWindow& window) const {
    sf::RectangleShape background(sf::Vector2f(500.f, 25.f));
    background.setFillColor(sf::Color(50, 50, 50));
    background.setPosition({150.f, 25.f});
    float hpRatio = static_cast<float>(health) / 3000.f;
    sf::RectangleShape bar(sf::Vector2f(500.f * hpRatio, 25.f));
    bar.setFillColor(sf::Color::Red);
    bar.setPosition({150.f, 25.f});
    sf::Font font;
    if (font.openFromFile("arial.ttf")) {
        sf::Text name(font,"Pontyfikus Janus Bogdanus", 16);
        name.setFillColor(sf::Color::White);
        name.setPosition({320.f, 0.f});
        window.draw(name);
    }
    window.draw(background);
    window.draw(bar);
}
