#include <iostream>
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Player.h"
#include "TileMap.cpp"
#include <fstream>
#include "loadLevelFromFile.h"
#include "Enemy.h"
#include "Bonefire.h"
#include "Chest.h"
#include "Boss.h"
#pragma once
enum class GameState {
    Menu,
    Playing
};

// Klasa zarządzająca grą
class Game {
public:
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

    int defLevel = 1;


    // Tekstury
    sf::Texture bonfireTex;
    sf::Texture chestClosedTex, chestOpenedTex;


    // Struktura dla przechowywania danych wrogów na poziomach
    struct EnemyTemplate {
        sf::Vector2f position;
        const sf::Texture* texture;
        float speed;
    };
    std::vector<std::vector<EnemyTemplate>> enemyTemplates;

public:
    Game(sf::RenderWindow &window);

    void UpdatePoints(int newValue);

    void setupEnemyTemplates();

    void loadLevel(int level);

    void loadChests();

    void loadBonefires();

    void loadEnemies();

    void changeLevel(int direction);

    void handleInput();

    void processPlayerAttack();

    void update(float deltaTime);

    void render();

    GameState getState() const;

    void setState(GameState newState);
};