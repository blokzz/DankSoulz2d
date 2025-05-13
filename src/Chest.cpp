// Chest.cpp
#include "Chest.h"

Chest::Chest(const sf::Texture& closedTexture, const sf::Texture& openedTexture, sf::Vector2f position , int content , Player& player)
    : Sprite(closedTexture), m_closedTexture(closedTexture), m_openedTexture(openedTexture), m_isOpened(false), ChestContent(content) , player(player) {
    setTexture(m_closedTexture);
    setPosition(position);
}

void Chest::tryOpen(const sf::FloatRect& playerBounds)
{
    if (!m_isOpened && getGlobalBounds().findIntersection(playerBounds).has_value()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
            m_isOpened = true;
            setTexture(m_openedTexture);
           switch (ChestContent) {
               case 1:
                   player.upgradeFlask();
                   player.notificationText = "Otrzymano: Ulepszenie flaszki!";
                   player.notificationClock.restart();
                   player.showNotification = true;

           }
        }
    }
}

void Chest::draw(sf::RenderTarget& target) const
{

    target.draw(*this);
}

