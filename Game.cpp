#include "Game.h"
#include "raylib.h"
#include "Engine.h"
#include "RaylibWrapper.h"
#include "EntityManager.h"
#include <math.h>

// DEFINITIONS
#define PLAYER_SPEED            5.0f
#define PLAYER_SIDESPEED        2.5f
#define PLAYER_SCALE            0.5
#define PLAYER_SCALE_DIVISION   2

#define ASTEROID_SPAWN_RATE     4.0f
#define ASTEROID_SPAWN_MULTIPLIER 0.0025f
#define ASTEROID_MAX_SPAWN_RATE 0.35f

#define ASTEROID_SPEED_MIN      1.0f
#define ASTEROID_SPEED_MAX      1.75f
#define SMALL_ASTEROID_SPEED_MIN 1.5f
#define SMALL_ASTEROID_SPEED_MAX 2.5f
#define MINI_ASTEROID_SPEED_MIN 3.0f
#define MINI_ASTEROID_SPEED_MAX 4.0f

Vector2 Game::shipPosition = {0, 0};
Vector2 shipSpeed = {0, 0};
float shipMove = 0.0f;

float shipRotation = 0.0f;

Texture2D playerShip;
Rectangle shipRectangle;
Vector2 shipCenter;

float Game::timeElapsed = 0.0f;
float timeElapsedSinceAsteroid = 0.0f;
float asteroidSpawnRateScale = 1.0f;

bool Game::PIERCING;

int Game::score;

bool displayDebug = false;

Color debugColor = GREEN;
Color uiColor = RAYWHITE;

// left right top bottom example vec4: 10, 15, 20, 24 = position vec2: (-10)-(15), (20)-(-24)
Vector4 randomAsteroidPosition = RayDefs::GetCenteredVec4(Engine::screenWidthHalf - 10.0f, Engine::screenWidthHalf - 10.0f, Engine::screenHeightHalf - 10.0f, Engine::screenHeightHalf - 10.0f, Engine::screenWidthHalf, Engine::screenHeightHalf);

void Game::OnRender(float deltaTime)
{
    ClearBackground(BLACK);

    if (displayDebug)
    {
        float fps = 1 / deltaTime;

        //DrawFPS(0, 0);
        DrawText(TextFormat("FPS: %f", fps), 0, 0, 20, debugColor);
        DrawText(TextFormat("Position: (%f, %f)", shipPosition.x, shipPosition.y), 0, 20, 20, debugColor);
        DrawText(TextFormat("Rotation: %f", shipRotation), 0, 40, 20, debugColor);
        DrawText(TextFormat("Time Elapsed: %f", timeElapsed), 0, 60, 20, debugColor);
        DrawText(TextFormat("Time Elapsed Since Asteroid: %f", timeElapsedSinceAsteroid), 0, 80, 20, debugColor);
        DrawText(TextFormat("Piercing: %s", PIERCING ? "true" : "false"), 0, 100, 20, debugColor);

        DrawText("Entity System:", 0, 140, 20, debugColor);

        DrawText(TextFormat("Bullet Count: %i", EntityManager::bulletEntities.size()), 0, 160, 20, debugColor);
        DrawText(TextFormat("Asteroid Count: %i", EntityManager::asteroidEntities.size()), 0, 180, 20, debugColor);
        DrawText(TextFormat("Small Asteroid Count: %i", EntityManager::smallAsteroidEntities.size()), 0, 200, 20, debugColor);
        DrawText(TextFormat("Mini Asteroid Count: %i", EntityManager::miniAsteroidEntities.size()), 0, 220, 20, debugColor);
    }

    DrawTexturePro(playerShip,
            shipRectangle,
            RayDefs::GetRectangle(shipPosition.x, shipPosition.y, shipRectangle.width * PLAYER_SCALE, shipRectangle.height * PLAYER_SCALE),
            shipCenter,
            shipRotation,
            RAYWHITE);

    int textX = MeasureText(TextFormat("Score: %i", score), 20);
    DrawText(TextFormat("Score: %i", score), Engine::screenWidthHalf - (textX / 2), 0, 20, uiColor);
    //DrawCircle(Engine::screenWidth, Engine::screenHeightHalf, 5.0f, RAYWHITE); // this here helped me visualize bounds :sunglasses:
    EntityManager::EntityDraw();
}
void Game::ProcessInput()
{
    if (IsKeyDown(KEY_W))
    {
        shipMove = 1;
    }
    if (IsKeyDown(KEY_LEFT))
    {
        shipPosition.x -= 2.5f;
    }
    if (IsKeyDown(KEY_S))
    {
        shipMove = -1;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        shipPosition.x += 2.5f;
    }

    if (IsKeyDown(KEY_A))
    {
        shipRotation -= 2.5f;
    }
    if (IsKeyDown(KEY_D))
    {
        shipRotation += 2.5f;
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        //EntityManager::CreateEntity(BulletT, 100.0f, 100.0f, 20.0f, 1.0f, 2.0f, 0.0f);
        EntityManager::CreateEntity(BulletT, shipPosition.x, shipPosition.y, shipRotation, 1.0f, 2.0f, 0.0f);
    }

    // DEBUG
    if (IsKeyPressed(KEY_TAB))
    {
        if (!displayDebug)
            displayDebug = true;
        else if (displayDebug)
            displayDebug = false;
    }

    shipPosition.x += (shipSpeed.x * shipMove);
    shipPosition.y -= (shipSpeed.y * shipMove);

    shipMove = 0;
}
void Game::OnUpdate(float deltaTime)
{
    shipSpeed.x = sin(shipRotation*DEG2RAD)*PLAYER_SPEED;
    shipSpeed.y = cos(shipRotation*DEG2RAD)*PLAYER_SPEED;

    //Vector2 centeredShipPosition = RayDefs::GetCenteredVec2(shipPosition.x, shipPosition.y, Engine::screenWidthHalf, Engine::screenHeightHalf);

    // LEFT TO RIGHT
    if (shipPosition.x > Engine::screenWidth)
    {
        shipPosition = RayDefs::GetVector2(-shipPosition.x + Engine::screenWidth, shipPosition.y);
    }
    // RIGHT TO LEFT
    else if (shipPosition.x < 0.0f) 
    {
        shipPosition = RayDefs::GetVector2(shipPosition.x + Engine::screenWidth, shipPosition.y);
    }
    // BOTTOM TO TOP
    else if (shipPosition.y < 0.0f) // why is Y flipped?
    {
        shipPosition = RayDefs::GetVector2(shipPosition.x, shipPosition.y + Engine::screenHeight);
    }
    // TOP TO BOTTOM
    else if (shipPosition.y > Engine::screenHeight)
    {
        shipPosition = RayDefs::GetVector2(shipPosition.x, -shipPosition.y + Engine::screenHeight);
    }

    timeElapsed += deltaTime;

    if(timeElapsed >= 69)
        PIERCING = true;

    timeElapsedSinceAsteroid += deltaTime;
    if (timeElapsedSinceAsteroid > ASTEROID_SPAWN_RATE * asteroidSpawnRateScale)
    {
        EntityManager::CreateEntity(AsteroidT, GetRandomValue(-randomAsteroidPosition.x, randomAsteroidPosition.y), GetRandomValue(randomAsteroidPosition.z/2, -randomAsteroidPosition.w/2), GetRandomValue(0, 360), 1.0f, GetRandomValue(ASTEROID_SPEED_MIN, ASTEROID_SPEED_MAX), 0.0f); // scale and bullet lifetime don't matter
        timeElapsedSinceAsteroid = 0.0f; //add this so you don't get a million asteroids in the world (looks cool though)
    }
    asteroidSpawnRateScale -= ASTEROID_SPAWN_MULTIPLIER * deltaTime;
    if (asteroidSpawnRateScale <= ASTEROID_MAX_SPAWN_RATE)
    {
        asteroidSpawnRateScale = ASTEROID_MAX_SPAWN_RATE;
    }

    EntityManager::EntityUpdate(deltaTime);
}
void Game::OnStart()
{
    playerShip = LoadTexture("../sprites/ship.png");
    shipRectangle = RayDefs::GetRectangle(0, 0, playerShip.width, playerShip.height);
    shipCenter = RayDefs::GetVector2((playerShip.width / 2) / PLAYER_SCALE_DIVISION, (playerShip.height / 2) / PLAYER_SCALE_DIVISION);
    /*Texture2D playerShip = LoadTexture("../sprites/ship.png");
    Rectangle shipRectangle = {0, 0, playerShip.width, playerShip.height};
    Vector2 shipCenter = {(playerShip.width / 2) / PLAYER_SCALE_DIVISION, (playerShip.height / 2) / PLAYER_SCALE_DIVISION};

    Texture2D playerBulletTex = LoadTexture("../sprites/bullet.png");
    Rectangle bulletRectangle = {0, 0, playerBulletTex.width, playerBulletTex.height};
    Vector2 bulletCenter = {(playerBulletTex.width / 2), (playerBulletTex.height / 2)};*/

    EntityManager::EntityStart();
}
void Game::OnEnd()
{
    UnloadTexture(playerShip);

    EntityManager::EntityEnd();
}
void Game::OnAwake()
{
    //m_entitySystem = std::make_unique<EntitySystem>();
    PIERCING = false;
    
    EntityManager::EntityAwake();
}

void Game::ResetScene()
{
    EntityManager::DeleteAll();

    PIERCING = false;

    shipPosition = RayDefs::GetVector2(Engine::screenWidthHalf, Engine::screenHeightHalf);
    shipRotation = 0.0f;

    timeElapsed = 0.0f;
    timeElapsedSinceAsteroid = 0.0f;

    score = 0;
}