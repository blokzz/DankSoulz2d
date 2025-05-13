//
#include <SFML/Graphics.hpp>
#include "iostream"
class Sword {
public:
    Sword(): texture("miecz.png"),sprite(texture) {
        // Załaduj teksturę animacji miecza (arkusz z klatkami)
        if (!texture.loadFromFile("miecz.png")) {
            std::cout << "Nie udało się załadować tekstury miecza!" << std::endl;
        }
        sprite.setTexture(texture);
        // Przygotuj animację (tutaj zakładam, że masz 4 klatki)
        currentFrame = 0;
        frameCount = 4;
        frameWidth = texture.getSize().x / frameCount; // Załóżmy, że wszystkie klatki mają tę samą szerokość
        frameHeight = texture.getSize().y;
        sprite.setTextureRect(sf::IntRect({0, 0},{ frameWidth, frameHeight}));

        attackDuration = 0.3f; // Czas trwania animacji w sekundach
        elapsedTime = 0.f;
        attacking = false;
    }
    void setScale(float x, float y) {
        sprite.setScale({x, y});
    }

    void update(float deltaTime) {
        if (attacking) {
            elapsedTime += deltaTime*3;
            if (elapsedTime >= attackDuration) {
                elapsedTime = 0.f;
                currentFrame++;
                if (currentFrame >= frameCount) {
                    currentFrame = 0;  // Restart animacji lub zakończenie
                    attacking = false; // Można zakończyć animację
                }
            }
            sprite.setTextureRect(sf::IntRect({currentFrame * frameWidth, 0},{frameWidth, frameHeight}));
        }
    }

    void startAttack() {
        attacking = true;
        currentFrame = 0; // Zaczynaj od pierwszej klatki
        elapsedTime = 0.f;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    void setPosition(float x, float y) {
        sprite.setPosition({x, y});
    }

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
