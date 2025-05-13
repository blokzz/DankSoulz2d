#include "Bonefire.h"
#include <iostream>
#include "Player.h"
#include "Game.h"
Bonefire::Bonefire(const sf::Texture& texture ,  sf::Vector2f position , Game& gameRef)
    : Sprite(texture), game(gameRef), texture(texture), option1(font), option2(font), infoText(font) {
    setScale({2.f, 2.f}); // dowolna skala
    if (!font.openFromFile("arial.ttf")) {
        std::cerr << "Nie wczytano czcionki!\n";
    }
    setTexture(texture);
    setPosition(position);
    // Tło modalne
    modalBackground.setSize({300, 200});
    modalBackground.setFillColor(sf::Color(0, 0, 0, 200));
    modalBackground.setPosition({250, 200});
    // Opcje
    option1.setFont(font);
    option1.setString("Zapisz gre");
    option1.setCharacterSize(20);
    option1.setPosition({270, 220});

    option2.setFont(font);
    option2.setString("Wydaj dusze");
    option2.setCharacterSize(20);
    option2.setPosition({270, 260});

    infoText.setFont(font);
    infoText.setCharacterSize(15);
    infoText.setPosition({270, 225});
    infoText.setString("Aktualny koszt ulepszenia:  " + std::to_string(game.levelCost));

    sf::Text strengthText(font);
    strengthText.setFont(font);
    strengthText.setCharacterSize(20);
    strengthText.setPosition({270, 260});
    strengthText.setString("Sila: " + std::to_string(game.attackLevel));

    sf::Text healthText(font);
    healthText.setFont(font);
    healthText.setCharacterSize(20);
    healthText.setPosition({270, 300});
    healthText.setString("Zdrowie: " + std::to_string(game.healthLevel));

    upgradeOptions.push_back(strengthText);
    upgradeOptions.push_back(healthText);
}

void Bonefire::tryInteract(const sf::FloatRect& playerBounds) {
    if (!isModalOpen && getGlobalBounds().findIntersection(playerBounds).has_value()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
            isModalOpen = true;
        }
    }
}

void Bonefire::updateInput(Player& player , int& score) {
    auto wasKeyPressed = [this](sf::Keyboard::Key key) {
        bool isDown = sf::Keyboard::isKeyPressed(key);
        bool wasDown = previousKeyState[key];
        previousKeyState[key] = isDown;
        return isDown && !wasDown;
    };
    if (isModalOpen) {
        player.reFillFlask();
    }
    // Najpierw obsługa podmenu ulepszania
    if (isUpgradeMenuOpen) {
        if (wasKeyPressed(sf::Keyboard::Key::Up)) {
            selectedUpgrade = (selectedUpgrade - 1 + upgradeOptions.size()) % upgradeOptions.size();
        }
        if (wasKeyPressed(sf::Keyboard::Key::Down)) {
            selectedUpgrade = (selectedUpgrade + 1) % upgradeOptions.size();  // poprawka: teraz idzie w dół
        }

        if (wasKeyPressed(sf::Keyboard::Key::Enter)) {
            if (score-game.levelCost>=0) {
                if (selectedUpgrade == 0) {
                    game.UpdatePoints(-game.levelCost);
                    game.attackLevel++;
                    game.levelCost+=10;
                    player.setAttack();
                } else if (selectedUpgrade == 1) {
                    game.UpdatePoints(-game.levelCost);
                    game.healthLevel++;
                    game.levelCost+=10;
                    player.setHealth();
                }
            }
            infoText.setString("Aktualny koszt ulepszenia:  " + std::to_string(game.levelCost));
        }

        upgradeOptions[0].setString("Sila: " + std::to_string(game.attackLevel));
        upgradeOptions[1].setString("Zdrowie: " + std::to_string(game.healthLevel));

        if (wasKeyPressed(sf::Keyboard::Key::Escape)) {
            isUpgradeMenuOpen = false;
            isModalOpen = true;
        }

        return;  // NIE przechodzimy dalej, jeśli to menu było aktywne
    }

    // Jeśli nie otwarte podmenu – sprawdzamy główne menu
    if (!isModalOpen) return;

    if (wasKeyPressed(sf::Keyboard::Key::Up)) {
        selectedOption = (selectedOption - 1 + 2) % 2;
    }
    if (wasKeyPressed(sf::Keyboard::Key::Down)) {
        selectedOption = (selectedOption + 1) % 2;
    }

    if (wasKeyPressed(sf::Keyboard::Key::Enter)) {
        if (selectedOption == 0) {
            std::cout << "Zapisz grę (brak implementacji)\n";
        } else if (selectedOption == 1) {

            isUpgradeMenuOpen = true;
            isModalOpen = false;
            selectedUpgrade = 0;  // resetuj wybór
        }
    }

    if (wasKeyPressed(sf::Keyboard::Key::Escape)) {
        isModalOpen = false;
    }

}
void Bonefire::draw(sf::RenderWindow& window) {
    window.draw(*this);
    if (isModalOpen) {
        window.draw(modalBackground);

        // Podświetlanie aktualnie wybranej opcji
        option1.setFillColor(selectedOption == 0 ? sf::Color::Yellow : sf::Color::White);
        option2.setFillColor(selectedOption == 1 ? sf::Color::Yellow : sf::Color::White);

        window.draw(option1);
        window.draw(option2);
    }
    if (isUpgradeMenuOpen) {
        drawUpgradeMenu(window);
    } else if (isModalOpen) {
        window.draw(modalBackground);
        option1.setFillColor(selectedOption == 0 ? sf::Color::Yellow : sf::Color::White);
        option2.setFillColor(selectedOption == 1 ? sf::Color::Yellow : sf::Color::White);
        window.draw(option1);
        window.draw(option2);
    }
}
void Bonefire::drawUpgradeMenu(sf::RenderWindow& window) {
    window.draw(modalBackground);

    for (size_t i = 0; i < upgradeOptions.size(); ++i) {
        upgradeOptions[i].setFillColor(i == selectedUpgrade ? sf::Color::Yellow : sf::Color::White);
        window.draw(upgradeOptions[i]);
    }
    window.draw(infoText);
}
