#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "Player.h"
class Game;
class Bonefire : public sf::Sprite {
private:
    Game& game;
    const sf::Texture& texture;
    bool isModalOpen = false;
    bool isUpgradeMenuOpen = false;
    std::vector<sf::Text> upgradeOptions;
    int selectedUpgrade = 0;
    std::map<sf::Keyboard::Key, bool> previousKeyState;
    sf::RectangleShape modalBackground;
    sf::Font font;
    sf::Text option1;
    sf::Text option2;
    sf::Text infoText;

    int selectedOption = 0;


public:
    Bonefire(const sf::Texture& texture , sf::Vector2f position , Game& game );

    void tryInteract(const sf::FloatRect& playerBounds);
    void updateInput(Player& player , int& score);  // obsługuje klawisze po otwarciu menu
    void draw(sf::RenderWindow& window);
    void drawUpgradeMenu(sf::RenderWindow& window);
    bool getIsModalOpen() const { return isModalOpen; }
};
