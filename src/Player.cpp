#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Player.h"
#include "iostream"

Player::Player() : speed(200.0f) , texture("player.png"), sprite(texture) {
    sprite.setTexture(texture);
    sprite.setScale({5.f, 5.f});
    sprite.setPosition({170.f, 30.f});

}

void Player::handleInput(std::optional<sf::Event> event) {
    if (auto const e = event->getIf<sf::Event::KeyPressed>()) {
        sprite.setOrigin({5.f, 5.f});
        if (e->code == sf::Keyboard::Key::Left) {
            sprite.setScale({-5.f, 5.f});
            sprite.move({-speed / 60.f, 0});

        }
        else if (e->code == sf::Keyboard::Key::Right) {
            sprite.setScale({5.f, 5.f});
            sprite.move({speed / 60.f, 0});
        }
    }
}

void Player::update() {

}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
