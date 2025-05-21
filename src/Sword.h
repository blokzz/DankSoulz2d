
#include <SFML/Graphics.hpp>
#include "iostream"
class Sword {
public:
    Sword();
    void setScale(float x, float y);
    void update(float deltaTime);
    void startAttack();
    void draw(sf::RenderWindow& window);
    void setPosition(float x, float y);
private:
    sf::Texture texture;
    sf::Sprite sprite;
    int currentFrame;
    int frameCount;
    int frameWidth;
    int frameHeight;
    float attackDuration;
    float elapsedTime;
    bool attacking;
};
