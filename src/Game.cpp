//
// Created by KAMIL on 11.05.2025.
//

#include "Game.h"
Game::Game(sf::RenderWindow& window) : window(window), state(GameState::Menu), currentLevel(0),backgroundTexture("ds3.png"), background(backgroundTexture) , scoreText(font) {
        // Inicjalizacja
    float scaleX = 800.f / static_cast<float>(backgroundTexture.getSize().x);
    float scaleY = 640.f / static_cast<float>(backgroundTexture.getSize().y);
    background.setScale({scaleX, scaleY});
        if (!chestClosedTex.loadFromFile("chest_closed.png") ||
      !chestOpenedTex.loadFromFile("chest_open.png") ||
       !bonfireTex.loadFromFile("bonfire.png"))
{
            std::cerr << "Błąd ładowania tekstur!" << std::endl;
      }

        levelFiles = {"./levels/level1.txt", "./levels/level2.txt", "./levels/level3.txt" ,"./levels/level4.txt"};
        if (!font.openFromFile("arial.ttf")) {
            std::cerr << "Czcionka NIE załadowana!" << std::endl;
        }
        scoreText.setFont(font);
        scoreText.setCharacterSize(17); // wielkość tekstu
        scoreText.setFillColor(sf::Color::White); // kolor
        scoreText.setPosition({10.f, 10.f}); // lewy górny róg
        scoreText.setString("Dusze: " + std::to_string(score));
        chestConent = {1 , 3 , 2};
        // Ładowanie tekstur
    if (!Enemy::bossTexture.loadFromFile("boss.png")) {
        std::cerr << "Nie udało się załadować tekstury bossa!\n";
    }
        // Konfiguracja szablonów wrogów
        setupEnemyTemplates();

        // Załadowanie pierwszego poziomu
        loadLevel(currentLevel);

    }


    void Game::UpdatePoints(int newValue) {
        score+=newValue;
        scoreText.setString("Dusze: " + std::to_string(score));
    }
    void Game::setupEnemyTemplates() {
        // Poziom 0
        std::vector<EnemyTemplate> level0 = {
            {{100.f, 300.f}, &Enemy::smallSlimeTexture, 50.f},
            {{300.f, 100.f}, &Enemy::bigSlimeTexture, 30.f},
            {{700.f, 400.f}, &Enemy::smallSlimeTexture, 70.f}
        };

        // Poziom 1
        std::vector<EnemyTemplate> level1 = {
            {{50.f, 300.f}, &Enemy::fatKnight, 60.f},
            {{200.f, 300.f}, &Enemy::fatKnight, 40.f}
        };

        // Poziom 2
        std::vector<EnemyTemplate> level2 = {
            {{100.f, 400.f}, &Enemy::smallSlimeTexture, 50.f},
            {{300.f, 400.f}, &Enemy::shadow, 30.f},
            {{400.f, 400.f}, &Enemy::smallSlimeTexture, 70.f}
        };
        std::vector<EnemyTemplate> level3 = {

        };
        enemyTemplates = {level0, level1, level2 , level3};
    }

    void Game::loadLevel(int level) {
        if (level < 0 || level >= static_cast<int>(levelFiles.size())) {
            std::cout << "Brak kolejnych poziomów!\n";
            state = GameState::Menu;
            return;
        }

        currentLevel = level;
        chests.clear();
        bonefires.clear();

        // Ładowanie danych poziomu
        currentLevelData = loadLevelFromFile(levelFiles[currentLevel], levelWidth, levelHeight);
        if (currentLevelData.empty()) {
            std::cerr << "Błąd ładowania poziomu!" << std::endl;
            return;
        }

        // Załadowanie mapy
        if (!map.load("Tile.png", {32, 32}, currentLevelData.data(), levelWidth, levelHeight)) {
            std::cerr << "Nie udało się załadować mapy!\n";
            return;
        }

        // Ustawienie pozycji mapy
        map.setPosition({0.f, 0.f});
        map.setScale({1.f, 1.f});

        // Ładowanie skrzyń
        loadChests();
        loadBonefires();

        // Ładowanie wrogów
        loadEnemies();

        std::cout << "Załadowano poziom: " << currentLevel << std::endl;
    }

    void Game::loadChests() {
        int i = 0;
        for (int y = 0; y < levelHeight; ++y) {
            for (int x = 0; x < levelWidth; ++x) {
                int tile = currentLevelData[y * levelWidth + x];
                if (tile == 6) { // skrzynka
                    chests.emplace_back(chestClosedTex, chestOpenedTex, sf::Vector2f(x * 32.f, y * 32.f) , chestConent[i] , player);
                    currentLevelData[y * levelWidth + x] = 0; // zastępuj tilemapę pustym tilem
                    ++i;
                }
            }
        }
    }

    void Game::loadBonefires() {
        for (int y = 0; y < levelHeight; ++y) {
            for (int x = 0; x < levelWidth; ++x) {
                int tile = currentLevelData[y * levelWidth + x];
                if (tile == 14) { // skrzynka
                    bonefires.emplace_back(bonfireTex, sf::Vector2f(x * 32.f, y * 32.f),*this );
                    currentLevelData[y * levelWidth + x] = 0; // zastępuj tilemapę pustym tilem
                }
            }
        }
    }

    void Game::loadEnemies() {
        enemies.clear();
    boss.reset(); // usuń starego bossa

    if (currentLevel == 3) {
        boss = std::make_unique<Boss>(Enemy::bossTexture, 600.f, 300.f, 80.f);
    }
        if (currentLevel < enemyTemplates.size()) {
            for (const auto& enemyTemplate : enemyTemplates[currentLevel]) {
                enemies.emplace_back(*enemyTemplate.texture, enemyTemplate.position.x,
                                     enemyTemplate.position.y, enemyTemplate.speed);
            }
        }
    }

    void Game::changeLevel(int direction) {
        float newPlayerX = (direction > 0) ? 0.f : (levelWidth - 1) * 32.f;
        loadLevel(currentLevel + direction);
        player.setPosition(newPlayerX, player.getPositionY());
    }

    void Game::handleInput() {
        // Obsługa ataków
        bool attackTriggered = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z);
        if (attackTriggered && attackClock.getElapsedTime().asSeconds() >= attackCooldown) {
            player.attack();
            attackClock.restart();
            processPlayerAttack();
        }
    }

    void Game::processPlayerAttack() {
        sf::FloatRect attackBox = player.getAttackHitbox();
    if (boss && boss->getGlobalBounds().findIntersection(attackBox)) {
        boss->updateHealth(player.getAttack());
        if (boss->getHealth() <= 0) {
            boss.reset();
            UpdatePoints(100); // dużo dusz za bossa
        }
    }
        for (auto it = enemies.begin(); it != enemies.end(); ) {
            if (it->getGlobalBounds().findIntersection(attackBox).has_value()) {
                it->updateHealth(player.getAttack());
                std::cout << player.getAttack();
                if (it->getHealth() <= 0) {
                    it = enemies.erase(it);
                    UpdatePoints(20);
                } else {
                    ++it;
                }
            } else {
                ++it;
            }
        }
    }

    void Game::update(float deltaTime) {
        if (state != GameState::Playing) return;
        // Aktualizacja gracza
        player.update(currentLevelData.data(), levelWidth, levelHeight, deltaTime);
    if (boss) {
        boss->update(deltaTime, currentLevelData, levelWidth, levelHeight,
                     {player.getPositionX(), player.getPositionY()});
        sf::FloatRect playerHitbox = player.getGlobalBounds();

        // Zmniejsz rozmiar hitboxa
        float shrinkX = 70.f; // im większe, tym „późniejsza” kolizja
        float shrinkY = 20.f;

        playerHitbox.position.x   += shrinkX / 2.f;
        playerHitbox.position.y    += shrinkY / 2.f;
        playerHitbox.size.x -= shrinkX;
        playerHitbox.size.y -= shrinkY;

        // Wykrywanie kolizji
        if (boss->checkCollision({playerHitbox})) {
            player.takeDamage(boss->getAttack());
        }

    }
        // Sprawdzenie czy gracz przeszedł do następnego poziomu
        if (player.getPositionX() > levelWidth * 32) {
            changeLevel(1);
        } else if (player.getPositionX() < 0.f) {
            changeLevel(-1);
        }

        // Aktualizacja wrogów
        for (auto& enemy : enemies) {
            enemy.update(deltaTime, currentLevelData, levelWidth, levelHeight,
                        {player.getPositionX(), player.getPositionY()});

            sf::FloatRect playerHitbox = player.getGlobalBounds();
            float shrinkX = 60.f;
            float shrinkY = 10.f;
            playerHitbox.position.x   += shrinkX / 2.f;
            playerHitbox.position.y    += shrinkY / 2.f;
            playerHitbox.size.x -= shrinkX;
            playerHitbox.size.y -= shrinkY;

            // Wykrywanie kolizji
            if (enemy.checkCollision({playerHitbox})) {
                player.takeDamage(enemy.getAttack());
            }
        }

        // Aktualizacja skrzyń
        for (Chest& chest : chests) {
            chest.tryOpen(player.getGlobalBounds());
        }
        for (Bonefire& bonefire : bonefires) {
            bonefire.tryInteract(player.getGlobalBounds());
            bonefire.updateInput(player, score);
        }
    }

    void Game::render() {
        window.clear(sf::Color::Black);
        if (state == GameState::Menu) {
            // Rysowanie menu będzie obsługiwane w głównej pętli
        } else if (state == GameState::Playing) {

            window.draw(background);
            window.draw(map);
            window.draw(scoreText);
            // Rysowanie skrzyń
            for (const Chest& chest : chests) {
                chest.draw(window);
            }
            for (Bonefire& bonefire : bonefires) {
                bonefire.draw(window);
            }
            // Rysowanie wrogów
            for (auto& enemy : enemies) {
                enemy.drawWithHealthBar(window);
            }
            if (boss) {
                boss->drawWithHealthBar(window);
                boss->drawBossBar(window);
            }

            // Rysowanie gracza
            player.draw(window);
            player.drawHealthBar(window);
        }
    }

    GameState Game::getState() const {
        return state;
    }

    void Game::setState(GameState newState) {
        state = newState;
    }