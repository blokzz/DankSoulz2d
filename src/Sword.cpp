
#include "Sword.h"
Sword::Sword(): texture("../../src/Assets/miecz.png"),sprite(texture) {
        if (!texture.loadFromFile("../../src/Assets/miecz.png")) {
            std::cout << "Nie udało się załadować tekstury miecza!" << std::endl;
        }
        sprite.setTexture(texture);
        currentFrame = 0;
        frameCount = 4;
        frameWidth = texture.getSize().x / frameCount;
        frameHeight = texture.getSize().y;
        sprite.setTextureRect(sf::IntRect({0, 0},{ frameWidth, frameHeight}));

        attackDuration = 0.3f;
        elapsedTime = 0.f;
        attacking = false;
    }
    void Sword::setScale(float x, float y) {
        sprite.setScale({x, y});
    }

    void Sword::update(float deltaTime) {
        if (attacking) {
            elapsedTime += deltaTime*3;
            if (elapsedTime >= attackDuration) {
                elapsedTime = 0.f;
                currentFrame++;
                if (currentFrame >= frameCount) {
                    currentFrame = 0;
                    attacking = false;
                }
            }
            sprite.setTextureRect(sf::IntRect({currentFrame * frameWidth, 0},{frameWidth, frameHeight}));
        }
    }

    void Sword::startAttack() {
        attacking = true;
        currentFrame = 0;
        elapsedTime = 0.f;
    }
    void Sword::draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
    void Sword::setPosition(float x, float y) {
        sprite.setPosition({x, y});
    }
