#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Player.h"
#include "TileMap.cpp"
#include <fstream>
#include "Enemy.h"
#include "Bonefire.h"
#include "Chest.h"
#include "Boss.h"
#pragma once
enum class GameState {
    Menu,
    Playing
};

class Game {
public:
    std::vector<std::vector<int>> allChestContents;
    std::vector<std::vector<int>> allChestStates;
    bool isAlive = true;
    std::vector<int> chestStates;
    sf::Texture backgroundTexture;
    sf::RenderWindow& window;
    GameState state;
    TileMap map;
    Player player;
    std::unique_ptr<Boss> boss;
    std::vector<Enemy> enemies;
    std::vector<Chest> chests;
    std::vector<std::string> levelFiles;
    std::vector<int> chestConent;
    std::vector<Bonefire> bonefires;
    int currentLevel;
    sf::Font font ;
    int levelWidth, levelHeight;
    std::vector<int> currentLevelData;
    sf::Sprite background;
    sf::Text scoreText;
    int score=0;
    sf::Clock attackClock;
    float attackCooldown = 0.5f;
    int levelCost = 10;
    int healthLevel = 1;
    int attackLevel = 1;
    sf::Texture bonfireTex;
    sf::Texture chestClosedTex, chestOpenedTex;
    struct EnemyTemplate {
        sf::Vector2f position;
        const sf::Texture* texture;
        float speed;
    };
    std::vector<std::vector<EnemyTemplate>> enemyTemplates;

public:
    Game(sf::RenderWindow &window);

    void respawnPlayer();

    void UpdatePoints(int newValue);

    void setupEnemyTemplates();

    int& getCurrentLevel();

    void loadLevel(int level);

    void loadChests();

    void loadBonefires();

    void loadEnemies();

    void changeLevel(int direction);

    void handleInput();

    void processPlayerAttack();

    void update(float deltaTime);

    void render();

    void saveGame(const std::string& filename);

    void loadGame(const std::string& filename);

    GameState getState() const;

    void setState(GameState newState);
};
#endif
