#include "Engine.h"
#include "raylib.h"
#include "Game.h"

void Engine::InitializeWindow()
{
    InitWindow(screenWidth, screenHeight, TITLE);
    SetTargetFPS(targetFPS);
}
void Renderer::Update(float deltaTime)
{
    Game::OnUpdate(deltaTime);
    Game::ProcessInput();

    BeginDrawing();

    Game::OnRender(deltaTime);

    EndDrawing();
}

void Engine::Print(string preMessage, string message)
{
    std::cout << preMessage << message << endl;
}
void Engine::PrintError(string errorMessage)
{
    std::cout << "[ERROR]: " << errorMessage << endl;
}

void Engine::PrintLog(string log)
{
    std::cout << "[Log]: " << log << endl;
}
