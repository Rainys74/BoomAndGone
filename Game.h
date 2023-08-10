#pragma once

#include <iostream>
#include "EntityManager.h"
#include "memory"
#include <sstream>

class Game
{
public:
    static std::vector<EntityManager> entities;

    static bool PIERCING;
    static Vector2 shipPosition;

    static int score;

    static float timeElapsed;

    static void OnRender(float deltaTime);
    static void ProcessInput();
    static void OnUpdate(float deltaTime);
    static void OnStart();
    static void OnEnd();
    static void OnAwake();

    static void ResetScene();
};