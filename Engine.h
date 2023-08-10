#pragma once
#include <iostream>
using namespace std;

class Engine
{
public:
    static const int screenWidth = 1200; //800
    static const int screenHeight = 675; //450

    static const int targetFPS = 60;

    static constexpr char TITLE[12] = "Boom & Gone";

    static const int screenWidthHalf = screenWidth / 2;
    static const int screenHeightHalf = screenHeight / 2;

    static void InitializeWindow();
    static void Print(string preMessage, string message);
    static void PrintError(string errorMessage);
    static void PrintLog(string log);
};

class Renderer
{
public:
    static void Update(float deltaTime);
};