#pragma once
#include "raylib.h"
#include <iostream>
#include <vector>
#include <string>
#include <math.h>

typedef struct Transformation
{
    Vector2 position;
    float rotation;
    float scale;
} Transformation;

enum EntityType 
{
    BulletT = 0,
    AsteroidT = 1,
    SmallAsteroidT = 2,
    MiniAsteroidT = 3,
};

// specific entities
typedef struct Bullet
{
    Transformation transform;
    float speed;
    float lifetime;
} Bullet;
typedef struct Asteroid
{
    Transformation transform;
    float speed;
    float time; // lifetime
} Asteroid;
typedef struct SmallAsteroid
{
    Transformation transform;
    float speed;
    float time;
} SmallAsteroid;
typedef struct MiniAsteroid
{
    Transformation transform;
    float speed;
    float time;
} MiniAsteroid;

class EntityManager
{
public:
    static std::vector<Bullet> bulletEntities;
    static std::vector<Asteroid> asteroidEntities;
    static std::vector<SmallAsteroid> smallAsteroidEntities;
    static std::vector<MiniAsteroid> miniAsteroidEntities;

    EntityManager(){}
    ~EntityManager(){}

    static void CreateEntity(EntityType type, float x, float y, float rotation, float scale, float speed, float bulletLifetime);
    static void EntityUpdate(float deltaTime);
    static void EntityDraw();
    static void EntityAwake();
    static void EntityEnd();
    static void EntityStart();

    static void DeleteAll();

    static Bullet ReturnBullet(float x, float y, float rot, float scale, float speed, float lifetime);
    static bool CheckCollisionBullet(Transformation bulletTransform, Transformation asteroidTransform);
    static float CheckDistance(Transformation firstTransform, Transformation secondTransform);

    //friend class Game;
    friend class ReleaseEntity;
};
class ReleaseEntity
{
public:
    static void RemoveBullet(size_t bulletToRemove);
    static void RemoveAsteroid(size_t asteroidToRemove);
    static void RemoveSmallAsteroid(size_t smallAsteroidToRemove);
    static void RemoveMiniAsteroid(size_t miniAsteroidToRemove);

    friend class EntityManager;
};
class ReturnAsteroid
{
private:
    float xPos = 0.0f;
    float yPos = 0.0f;
    float rotation = 0.0f;
    float scaleFloat = 0.0f;
    float speedAmount = 0.0f;
    float lifetime = 0.0f;
public:
    ReturnAsteroid(float x, float y, float rot, float scale, float speed, float time)
    {
        xPos = x;
        yPos = y;
        rotation = rot;
        scaleFloat = scale;
        speedAmount = speed;
        lifetime = time;
    }
    operator Asteroid()
    {
        return {{{xPos, yPos}, rotation, scaleFloat}, speedAmount, lifetime};
    }
    operator SmallAsteroid()
    {
        return {{{xPos, yPos}, rotation, scaleFloat}, speedAmount, lifetime};
    }
    operator MiniAsteroid()
    {
        return {{{xPos, yPos}, rotation, scaleFloat}, speedAmount, lifetime};
    }
};
