#include "main.h"
#include "raylib.h"
#include "Engine.h"
#include "Game.h"
#include "EntityManager.h"

// this fixed a reference error...
std::vector<Bullet> EntityManager::bulletEntities;
std::vector<Asteroid> EntityManager::asteroidEntities;
std::vector<SmallAsteroid> EntityManager::smallAsteroidEntities;
std::vector<MiniAsteroid> EntityManager::miniAsteroidEntities;

bool windowShouldClose = false;
bool Main::closingWindow = false;

int main()
{
    Game::OnAwake();

    Engine::InitializeWindow();
    
    Game::OnStart();

    while (!windowShouldClose)
    {
        if (WindowShouldClose() || Main::closingWindow)
            windowShouldClose = true;

        Renderer::Update(GetFrameTime());
    }

    Game::OnEnd();
    CloseWindow();

    return 0;
}