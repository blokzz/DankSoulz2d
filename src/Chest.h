// Chest.h
#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
class Chest : public sf::Sprite {
public:
    Chest(const sf::Texture& closedTexture, const sf::Texture& openedTexture, sf::Vector2f position , int content , Player& player);
    void tryOpen(const sf::FloatRect& playerBounds);
    void draw(sf::RenderTarget& target) const;

private:
     Player& player;
    int ChestContent;
    const sf::Texture& m_closedTexture;
    const sf::Texture& m_openedTexture;
    bool m_isOpened;
};
