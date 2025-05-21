
#include "Chest.h"

#include "Game.h"

Chest::Chest(const sf::Texture& closedTexture, const sf::Texture& openedTexture, sf::Vector2f position , int content, int& state , Player& player)
    : Sprite(closedTexture), m_closedTexture(closedTexture), m_openedTexture(openedTexture), m_isOpened(false), ChestContent(content) ,opened(state) ,  player(player) {
    setTexture(m_closedTexture);
    setPosition(position);
    if (opened==1) {
        setTexture(m_openedTexture);
    }
}

void Chest::tryOpen(const sf::FloatRect& playerBounds , std::vector<int>& chestState , int& i )
{
    if (!m_isOpened && getGlobalBounds().findIntersection(playerBounds).has_value() && opened==0) {
        if (isKeyPressed(sf::Keyboard::Key::E)) {
            opened = 1;
            chestState[i] = 1;
            std::cout << "Chest opened" << std::endl;
            m_isOpened = true;
            setTexture(m_openedTexture);
           switch (ChestContent) {
               case 1:
                   player.upgradeFlask();
                   player.notificationText = "Otrzymano: Ulepszenie flaszki!";
                   player.notificationClock.restart();
                   player.showNotification = true;
                   break;
               case 2 :
                   player.upgradeAttack(10.0);
                   player.notificationText = "Otrzymano: Ulepszenie do ataku!";
                   player.notificationClock.restart();
                   player.showNotification = true;
               break;
               case 3 :
               player.notificationText = "GRATULACJE UDALO SIE PRZEJSC GRE";
               player.notificationClock.restart();
               player.showNotification = true;

               break;

           }
        }
    }
}

void Chest::draw(sf::RenderTarget& target) const
{
    target.draw(*this);
}

