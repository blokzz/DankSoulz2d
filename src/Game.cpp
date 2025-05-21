
#include "Game.h"
#include "Menu.h"
#include "Player.h"
#include "TileMap.cpp"
#include <fstream>
#include "loadLevelFromFile.h"
#include "Enemy.h"
#include "Bonefire.h"
#include "Chest.h"
#include "Boss.h"
Game::Game(sf::RenderWindow& window) : window(window), state(GameState::Menu), currentLevel(0),backgroundTexture("../../src/Assets/ds3.png"), background(backgroundTexture) , scoreText(font) {
    float scaleX = 800.f / static_cast<float>(backgroundTexture.getSize().x);
    float scaleY = 640.f / static_cast<float>(backgroundTexture.getSize().y);
    background.setScale({scaleX, scaleY});
        if (!chestClosedTex.loadFromFile("../../src/Assets/chest_closed.png") ||
      !chestOpenedTex.loadFromFile("../../src/Assets/chest_open.png") ||
       !bonfireTex.loadFromFile("../../src/Assets/bonfire.png"))
{
            std::cerr << "Błąd ładowania tekstur!" << std::endl;
      }
        levelFiles = {"../../src/Assets/levels/level1.txt", "../../src/Assets/levels/level2.txt", "../../src/Assets/levels/level3.txt" ,"../../src/Assets/levels/level4.txt"
        ,"../../src/Assets/levels/level5.txt","../../src/Assets/levels/level6.txt","../../src/Assets/levels/level7.txt"};
        if (!font.openFromFile("../../src/Assets/arial.ttf")) {
            std::cerr << "Czcionka NIE załadowana!" << std::endl;
        }
        scoreText.setFont(font);
        scoreText.setCharacterSize(17);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition({10.f, 10.f});
        scoreText.setString("Dusze: " + std::to_string(score));
    allChestContents = {
        {1},
        {},
        {},
        {2},
        {},
        {},
        {3}
    };
    allChestStates = {
        {0},
        {},
        {},
        {0},
        {},
        {},
        {0}
    };

    if (!Enemy::bossTexture.loadFromFile("../../src/Assets/boss.png")) {
        std::cerr << "Nie udało się załadować tekstury bossa!\n";
    }
        setupEnemyTemplates();

        loadLevel(currentLevel);


    }
int& Game::getCurrentLevel() {
    return currentLevel;
}
GameState Game::getState() const {
    return state;
}

void Game::setState(GameState newState) {
    state = newState;
}

void Game::saveGame(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nie można zapisać gry!" << std::endl;
        return;
    }

    file << currentLevel << "\n";
    file << score << "\n";
    file << attackLevel << "\n";
    file << healthLevel << "\n";
    file << player.getTotalFlasks() << "\n";
    file << levelCost << "\n";

    file << allChestStates.size() << "\n";

    for (size_t level = 0; level < allChestStates.size(); ++level) {
        file << allChestStates[level].size() << "\n";
        for (int state : allChestStates[level])
            file << state << " ";
        file << "\n";

        file << allChestContents[level].size() << "\n";
        for (int content : allChestContents[level])
            file << content << " ";
        file << "\n";
    }
    file << player.getPositionX() << " " << player.getPositionY() << "\n";

    file.close();
    std::cout << "Gra zapisana!\n";
}
void Game::loadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nie można wczytać gry!" << std::endl;
        return;
    }
    file >> currentLevel;
    file >> score;
    file >> attackLevel;
    file >> healthLevel;

    int total;
    file >> total;
    player.setTotalFlasks(total);

    file >> levelCost;

    size_t levelCount;
    file >> levelCount;

    allChestStates.clear();
    allChestContents.clear();
    allChestStates.resize(levelCount);
    allChestContents.resize(levelCount);

    for (size_t level = 0; level < levelCount; ++level) {
        size_t chestCount;
        file >> chestCount;
        allChestStates[level].resize(chestCount);
        for (size_t i = 0; i < chestCount; ++i)
            file >> allChestStates[level][i];

        size_t contentCount;
        file >> contentCount;
        allChestContents[level].resize(contentCount);
        for (size_t i = 0; i < contentCount; ++i)
            file >> allChestContents[level][i];
    }

    float x, y;
    file >> x >> y;

    file.close();

    chestStates = allChestStates[currentLevel];
    chestConent = allChestContents[currentLevel];

    loadLevel(currentLevel);
    player.setPosition(x, y);
    player.setAttack();
    player.setHealth();
    scoreText.setString("Dusze: " + std::to_string(score));
    std::cout << "Gra wczytana!\n";
}



    void Game::UpdatePoints(int newValue) {
        score+=newValue;
        scoreText.setString("Dusze: " + std::to_string(score));
    }
    void Game::setupEnemyTemplates() {

        std::vector<EnemyTemplate> level0 = {
            {{300.f, 100.f}, &Enemy::bigSlimeTexture, 30.f},
            {{700.f, 400.f}, &Enemy::smallSlimeTexture, 70.f}
        };

        std::vector<EnemyTemplate> level1 = {
            {{200.f, 300.f}, &Enemy::fatKnight, 40.f}
        };

        std::vector<EnemyTemplate> level2 = {
            {{100.f, 400.f}, &Enemy::smallSlimeTexture, 50.f},
            {{400.f, 400.f}, &Enemy::smallSlimeTexture, 70.f}
        };
        std::vector<EnemyTemplate> level3 = {
            {{100.f, 400.f}, &Enemy::shadow, 50.f},
                      {{400.f, 400.f}, &Enemy::smallSlimeTexture, 70.f}
        };
    std::vector<EnemyTemplate> level4 = {
        {{100.f, 400.f}, &Enemy::shadow, 20.f},
                  {{400.f, 400.f}, &Enemy::bigSlimeTexture, 60.f}
    };
        enemyTemplates = {level0, level1, level2 , level3 , level4 ,};
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
      chestConent = allChestContents[level];
       chestStates = allChestStates[level];
        currentLevelData = loadLevelFromFile(levelFiles[currentLevel], levelWidth, levelHeight);
        if (currentLevelData.empty()) {
            std::cerr << "Błąd ładowania poziomu!" << std::endl;
            return;
        }
        if (!map.load("../../src/Assets/Tile.png", {32, 32}, currentLevelData.data(), levelWidth, levelHeight)) {
            std::cerr << "Nie udało się załadować mapy!\n";
            return;
        }
        map.setPosition({0.f, 0.f});
        map.setScale({1.f, 1.f});

        loadChests();
        loadBonefires();
        loadEnemies();
        std::cout << "Załadowano poziom: " << currentLevel << std::endl;
    }

void Game::loadChests() {
    int i = 0;
    for (int y = 0; y < levelHeight; ++y) {
        for (int x = 0; x < levelWidth; ++x) {
            int tile = currentLevelData[y * levelWidth + x];
            if (tile == 6) {

                if (i >= chestConent.size() || i >= chestStates.size()) {
                    std::cerr << "blaaad" << std::endl;
                    chestConent.push_back(0);
                    chestStates.push_back(0);
                }

                chests.emplace_back(
                    chestClosedTex,
                    chestOpenedTex,
                    sf::Vector2f(x * 32.f, y * 32.f),
                    chestConent[i],
                    chestStates[i],
                    player
                );
                currentLevelData[y * levelWidth + x] = 2;
                ++i;
            }
        }
    }
}


    void Game::loadBonefires() {
        for (int y = 0; y < levelHeight; ++y) {
            for (int x = 0; x < levelWidth; ++x) {
                int tile = currentLevelData[y * levelWidth + x];
                if (tile == 14) {
                    bonefires.emplace_back(bonfireTex, sf::Vector2f(x * 32.f, y * 32.f),*this );
                    currentLevelData[y * levelWidth + x] = 0;
                }
            }
        }
    }

    void Game::loadEnemies() {
        enemies.clear();
    boss.reset();

    if (currentLevel == 5) {
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
        bool attackTriggered = isKeyPressed(sf::Keyboard::Key::Z);
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
            UpdatePoints(100);
        }
    }
        for (auto it = enemies.begin(); it != enemies.end(); ) {
            if (it->getGlobalBounds().findIntersection(attackBox).has_value()) {
                it->updateHealth(player.getAttack());
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


void Game::respawnPlayer() {
    if (player.getHealth()==0 && isAlive) {
        if (currentLevel>0) {
            loadLevel(currentLevel-1);
            isAlive = false;
        }
        player.setPosition(0.f,500.f);
    }
}


    void Game::update(float deltaTime) {
    isAlive = true;
        if (state != GameState::Playing) return;
        player.update(currentLevelData.data(), levelWidth, levelHeight, deltaTime);
    if (boss) {
        boss->update(deltaTime, currentLevelData, levelWidth, levelHeight,
                     {player.getPositionX(), player.getPositionY()});
        sf::FloatRect playerHitbox = player.getGlobalBounds();
        float shrinkX = 70.f;
        float shrinkY = 20.f;
        playerHitbox.position.x   += shrinkX / 2.f;
        playerHitbox.position.y    += shrinkY / 2.f;
        playerHitbox.size.x -= shrinkX;
        playerHitbox.size.y -= shrinkY;
        if (boss->checkCollision({playerHitbox})) {
            player.takeDamage(boss->getAttack());
            respawnPlayer();
        }

    }
    if ((currentLevel == 0 || boss) && player.getPositionX() < 0.f) {
        player.setPosition(0.f, player.getPositionY());
    }
    else if (boss && player.getPositionX() > levelWidth * 32) {
        if (!boss) {
            changeLevel(1);
        } else {
            player.setPosition(levelWidth * 32.f - 10.f, player.getPositionY());
        }
    }
    else {
        if (player.getPositionX() > levelWidth * 32) {
            changeLevel(1);
        } else if (player.getPositionX() < 0.f) {
            changeLevel(-1);
        }
    }

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
            if (enemy.checkCollision({playerHitbox})) {
                player.takeDamage(enemy.getAttack());
                respawnPlayer();
            }
        }
        int i=0;
        for (Chest& chest : chests) {
            chest.tryOpen(player.getGlobalBounds() , allChestStates[currentLevel] , i);
            i++;
        }
        for (Bonefire& bonefire : bonefires) {
            bonefire.tryInteract(player.getGlobalBounds());
            bonefire.updateInput(player, score);
        }
    }

    void Game::render() {
        window.clear(sf::Color::Black);
        if (state == GameState::Menu) {
        } else if (state == GameState::Playing) {

            window.draw(background);
            window.draw(map);
            window.draw(scoreText);
            for (const Chest& chest : chests) {
                chest.draw(window);
            }
            for (Bonefire& bonefire : bonefires) {
                bonefire.draw(window);
            }
            for (auto& enemy : enemies) {
                enemy.drawWithHealthBar(window);
            }
            if (boss) {
                boss->drawWithHealthBar(window);
                boss->drawBossBar(window);
            }
            player.draw(window);
            player.drawHealthBar(window);
        }
    }

