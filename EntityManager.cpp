#include "EntityManager.h"
#include "Engine.h"
#include "RaylibWrapper.h"
#include "Game.h"
#include "main.h"

#define BULLET_DIRECTIONAL_SPEED 7.5f
#define BULLET_LIFETIME 5.0f //0.5f

#define ASTEROID_SCALE            1
#define ASTEROID_SCALE_DIVISION   1

#define SMALL_ASTEROID_SCALE            0.75f
#define SMALL_ASTEROID_SCALE_DIVISION   1.5

#define MINI_ASTEROID_SCALE            0.5f // mini asteroids once were 0.25f...
#define MINI_ASTEROID_SCALE_DIVISION   2

#define ASTEROID_DIRECTIONAL_SPEED 2.0f
#define ASTEROID_LIFETIME          75.0f

#define ASTEROID_OFFSET_MULTIPLIER 10.0f

//#define PIERCING                   false

// GAME.CPP DOESN'T MATTER!
#define SMALL_ASTEROID_SPEED_MIN 1.25f
#define SMALL_ASTEROID_SPEED_MAX 2.25f
#define MINI_ASTEROID_SPEED_MIN 2.5f // their speed was 3-4...
#define MINI_ASTEROID_SPEED_MAX 3.0f

Texture2D playerBulletTex;
Rectangle bulletRectangle;
Vector2 bulletCenter;

Texture2D asteroidTex;
Rectangle asteroidRectangle;
Vector2 asteroidCenter;
Vector2 smallAsteroidCenter;
Vector2 miniAsteroidCenter;

void EntityManager::CreateEntity(EntityType type, float x, float y, float rotation, float scale, float speed, float bulletLifetime)
{
    if (type == BulletT)
    {
        bulletEntities.push_back(ReturnBullet(x, y, rotation, scale, speed, bulletLifetime));
    }
    if (type == AsteroidT)
    {
        asteroidEntities.push_back(ReturnAsteroid(x, y, rotation, scale, speed, bulletLifetime));
    }
    if (type == SmallAsteroidT)
    {
        smallAsteroidEntities.push_back(ReturnAsteroid(x, y, rotation, scale, speed, bulletLifetime));
    }
    if (type == MiniAsteroidT)
    {
        miniAsteroidEntities.push_back(ReturnAsteroid(x, y, rotation, scale, speed, bulletLifetime));
    }
}
void EntityManager::EntityUpdate(float deltaTime)
{
    //Engine::PrintLog(std::to_string(smallAsteroidEntities.size()));

    for (size_t i = 0; i < bulletEntities.size(); i++)
    {
        // POSITION
        Transformation newTransform = bulletEntities[i].transform; 
        Vector2 bulletDirSpeed;

        bulletDirSpeed.x = sin(bulletEntities[i].transform.rotation*DEG2RAD)*BULLET_DIRECTIONAL_SPEED;
        bulletDirSpeed.y = cos(bulletEntities[i].transform.rotation*DEG2RAD)*BULLET_DIRECTIONAL_SPEED;

        newTransform.position.x += (bulletDirSpeed.x * bulletEntities[i].speed);
        newTransform.position.y -= (bulletDirSpeed.y * bulletEntities[i].speed);

        bulletEntities[i].transform = newTransform;

        // LIFETIME
        bulletEntities[i].lifetime += deltaTime;
        if (bulletEntities[i].lifetime > BULLET_LIFETIME)
            ReleaseEntity::RemoveBullet(i);
    }
    // ASTEROIDS
    for (size_t i = 0; i < asteroidEntities.size(); i++)
    {
        // POSITION
        Transformation newTransform = asteroidEntities[i].transform; 
        Vector2 asteroidDirSpeed;

        asteroidDirSpeed.x = sin(asteroidEntities[i].transform.rotation*DEG2RAD)*ASTEROID_DIRECTIONAL_SPEED;
        asteroidDirSpeed.y = cos(asteroidEntities[i].transform.rotation*DEG2RAD)*ASTEROID_DIRECTIONAL_SPEED;

        newTransform.position.x += (asteroidDirSpeed.x * asteroidEntities[i].speed);
        newTransform.position.y -= (asteroidDirSpeed.y * asteroidEntities[i].speed);

        asteroidEntities[i].transform = newTransform;

        // COPIED FROM PLAYER BOUNDS
        // LEFT TO RIGHT
        if (asteroidEntities[i].transform.position.x > Engine::screenWidth)
        {
            asteroidEntities[i].transform.position = RayDefs::GetVector2(RayDefs::ReverseFloat(asteroidEntities[i].transform.position.x) + Engine::screenWidth, asteroidEntities[i].transform.position.y);
        }
        // RIGHT TO LEFT
        else if (asteroidEntities[i].transform.position.x < 0.0f - (ASTEROID_SCALE * ASTEROID_OFFSET_MULTIPLIER)) 
        {
            asteroidEntities[i].transform.position = RayDefs::GetVector2(asteroidEntities[i].transform.position.x + Engine::screenWidth + (ASTEROID_SCALE * ASTEROID_OFFSET_MULTIPLIER), asteroidEntities[i].transform.position.y);
        }
        // BOTTOM TO TOP
        else if (asteroidEntities[i].transform.position.y < 0.0f - (ASTEROID_SCALE * ASTEROID_OFFSET_MULTIPLIER)) // why is Y flipped?
        {
            asteroidEntities[i].transform.position = RayDefs::GetVector2(asteroidEntities[i].transform.position.x, asteroidEntities[i].transform.position.y + Engine::screenHeight + (ASTEROID_SCALE * ASTEROID_OFFSET_MULTIPLIER));
        }
        // TOP TO BOTTOM
        else if (asteroidEntities[i].transform.position.y > Engine::screenHeight)
        {
            asteroidEntities[i].transform.position = RayDefs::GetVector2(asteroidEntities[i].transform.position.x, -asteroidEntities[i].transform.position.y + Engine::screenHeight);
        }

        // PLAYER COLLISION
        if(CheckCollisionCircles(Game::shipPosition, 20.0f, asteroidEntities[i].transform.position, ASTEROID_SCALE * 25))
        {
            //Main::CloseWindow();
            Game::ResetScene();
        }  

        // LIFETIME FOR OPTIMIZATION
        asteroidEntities[i].time += deltaTime;
        if (asteroidEntities[i].time > ASTEROID_LIFETIME)
            ReleaseEntity::RemoveAsteroid(i);
    }

    // SMALL ASTEROIDS
    for (size_t i = 0; i < smallAsteroidEntities.size(); i++)
    {
        // POSITION
        Transformation newTransform = smallAsteroidEntities[i].transform; 
        Vector2 asteroidDirSpeed;

        asteroidDirSpeed.x = sin(smallAsteroidEntities[i].transform.rotation*DEG2RAD)*ASTEROID_DIRECTIONAL_SPEED;
        asteroidDirSpeed.y = cos(smallAsteroidEntities[i].transform.rotation*DEG2RAD)*ASTEROID_DIRECTIONAL_SPEED;

        newTransform.position.x += (asteroidDirSpeed.x * smallAsteroidEntities[i].speed);
        newTransform.position.y -= (asteroidDirSpeed.y * smallAsteroidEntities[i].speed);

        smallAsteroidEntities[i].transform = newTransform;

        // COPIED FROM PLAYER BOUNDS
        // LEFT TO RIGHT
        if (smallAsteroidEntities[i].transform.position.x > Engine::screenWidth)
        {
            smallAsteroidEntities[i].transform.position = RayDefs::GetVector2(RayDefs::ReverseFloat(smallAsteroidEntities[i].transform.position.x) + Engine::screenWidth, smallAsteroidEntities[i].transform.position.y);
        }
        // RIGHT TO LEFT
        else if (smallAsteroidEntities[i].transform.position.x < 0.0f - (SMALL_ASTEROID_SCALE * ASTEROID_OFFSET_MULTIPLIER)) 
        {
            smallAsteroidEntities[i].transform.position = RayDefs::GetVector2(smallAsteroidEntities[i].transform.position.x + Engine::screenWidth + (SMALL_ASTEROID_SCALE * ASTEROID_OFFSET_MULTIPLIER), smallAsteroidEntities[i].transform.position.y);
        }
        // BOTTOM TO TOP
        else if (smallAsteroidEntities[i].transform.position.y < 0.0f - (SMALL_ASTEROID_SCALE * ASTEROID_OFFSET_MULTIPLIER)) // why is Y flipped?
        {
            smallAsteroidEntities[i].transform.position = RayDefs::GetVector2(smallAsteroidEntities[i].transform.position.x, smallAsteroidEntities[i].transform.position.y + Engine::screenHeight + (SMALL_ASTEROID_SCALE * ASTEROID_OFFSET_MULTIPLIER));
        }
        // TOP TO BOTTOM
        else if (smallAsteroidEntities[i].transform.position.y > Engine::screenHeight)
        {
            smallAsteroidEntities[i].transform.position = RayDefs::GetVector2(smallAsteroidEntities[i].transform.position.x, -smallAsteroidEntities[i].transform.position.y + Engine::screenHeight);
        }

        // PLAYER COLLISION
        if(CheckCollisionCircles(Game::shipPosition, 20.0f, smallAsteroidEntities[i].transform.position, SMALL_ASTEROID_SCALE * 30))
        {
            Game::ResetScene();
        }

        // NO LIFETIME
    }

    // MINI ASTEROIDS
    for (size_t i = 0; i < miniAsteroidEntities.size(); i++)
    {
        // POSITION
        Transformation newTransform = miniAsteroidEntities[i].transform; 
        Vector2 asteroidDirSpeed;

        asteroidDirSpeed.x = sin(miniAsteroidEntities[i].transform.rotation*DEG2RAD)*ASTEROID_DIRECTIONAL_SPEED;
        asteroidDirSpeed.y = cos(miniAsteroidEntities[i].transform.rotation*DEG2RAD)*ASTEROID_DIRECTIONAL_SPEED;

        newTransform.position.x += (asteroidDirSpeed.x * miniAsteroidEntities[i].speed);
        newTransform.position.y -= (asteroidDirSpeed.y * miniAsteroidEntities[i].speed);

        miniAsteroidEntities[i].transform = newTransform;

        // COPIED FROM PLAYER BOUNDS
        // LEFT TO RIGHT
        if (miniAsteroidEntities[i].transform.position.x > Engine::screenWidth)
        {
            miniAsteroidEntities[i].transform.position = RayDefs::GetVector2(RayDefs::ReverseFloat(miniAsteroidEntities[i].transform.position.x) + Engine::screenWidth, miniAsteroidEntities[i].transform.position.y);
        }
        // RIGHT TO LEFT
        else if (miniAsteroidEntities[i].transform.position.x < 0.0f - (MINI_ASTEROID_SCALE * ASTEROID_OFFSET_MULTIPLIER)) 
        {
            miniAsteroidEntities[i].transform.position = RayDefs::GetVector2(miniAsteroidEntities[i].transform.position.x + Engine::screenWidth + (MINI_ASTEROID_SCALE * ASTEROID_OFFSET_MULTIPLIER), miniAsteroidEntities[i].transform.position.y);
        }
        // BOTTOM TO TOP
        else if (miniAsteroidEntities[i].transform.position.y < 0.0f - (MINI_ASTEROID_SCALE * ASTEROID_OFFSET_MULTIPLIER)) // why is Y flipped?
        {
            miniAsteroidEntities[i].transform.position = RayDefs::GetVector2(miniAsteroidEntities[i].transform.position.x, miniAsteroidEntities[i].transform.position.y + Engine::screenHeight + (MINI_ASTEROID_SCALE * ASTEROID_OFFSET_MULTIPLIER));
        }
        // TOP TO BOTTOM
        else if (miniAsteroidEntities[i].transform.position.y > Engine::screenHeight)
        {
            miniAsteroidEntities[i].transform.position = RayDefs::GetVector2(miniAsteroidEntities[i].transform.position.x, -miniAsteroidEntities[i].transform.position.y + Engine::screenHeight);
        }

        // PLAYER COLLISION
        if(CheckCollisionCircles(Game::shipPosition, 20.0f, miniAsteroidEntities[i].transform.position, MINI_ASTEROID_SCALE * 35))
        {
            Game::ResetScene();
        } 

        // NO LIFETIME AS WELL
    }

    // COLLISION
    for (size_t bi = 0; bi < bulletEntities.size(); bi++)
    {
        for (size_t ai = 0; ai < asteroidEntities.size(); ai++)
        {
            if(CheckCollisionCircles(bulletEntities[bi].transform.position, bulletEntities[bi].transform.scale * 15, asteroidEntities[ai].transform.position, ASTEROID_SCALE * 25)) //CheckCollisionBullet(bulletEntities[bi].transform, asteroidEntities[ai].transform)
            {
                if (!Game::PIERCING)
                    ReleaseEntity::RemoveBullet(bi);

                for (size_t i = 0; i < 2; i++) 
                    EntityManager::CreateEntity(SmallAsteroidT, asteroidEntities[ai].transform.position.x + RayDefs::FRandom(-0.15f, 0.15f), asteroidEntities[ai].transform.position.x + RayDefs::FRandom(-0.25f, 0.25f), GetRandomValue(0, 360), SMALL_ASTEROID_SCALE, RayDefs::FRandom(SMALL_ASTEROID_SPEED_MIN, SMALL_ASTEROID_SPEED_MAX), ASTEROID_LIFETIME);

                Game::score += 10 * RayDefs::RoundToTen(Game::timeElapsed);
                ReleaseEntity::RemoveAsteroid(ai);
            }       
        }
        for (size_t sai = 0; sai < smallAsteroidEntities.size(); sai++)
        {
            if(CheckCollisionCircles(bulletEntities[bi].transform.position, bulletEntities[bi].transform.scale * 15, smallAsteroidEntities[sai].transform.position, SMALL_ASTEROID_SCALE * 30))
            {
                if (!Game::PIERCING)
                    ReleaseEntity::RemoveBullet(bi);

                for (size_t i = 0; i < 3; i++) 
                    EntityManager::CreateEntity(MiniAsteroidT, smallAsteroidEntities[sai].transform.position.x + RayDefs::FRandom(-0.05f, 0.05f), smallAsteroidEntities[sai].transform.position.x + RayDefs::FRandom(-0.1f, 0.1f), GetRandomValue(0, 360), MINI_ASTEROID_SCALE, RayDefs::FRandom(MINI_ASTEROID_SPEED_MIN, MINI_ASTEROID_SPEED_MAX), ASTEROID_LIFETIME);

                Game::score += 100 * RayDefs::RoundToTen(Game::timeElapsed);
                ReleaseEntity::RemoveSmallAsteroid(sai);
            } 
        }
        for (size_t mai = 0; mai < miniAsteroidEntities.size(); mai++)
        {         
            if(CheckCollisionCircles(bulletEntities[bi].transform.position, bulletEntities[bi].transform.scale * 15, miniAsteroidEntities[mai].transform.position, MINI_ASTEROID_SCALE * 35))
            {
                if (!Game::PIERCING)
                    ReleaseEntity::RemoveBullet(bi);

                Game::score += 1000 * RayDefs::RoundToTen(Game::timeElapsed);
                ReleaseEntity::RemoveMiniAsteroid(mai);
            }
        }     
    }
}
void EntityManager::EntityDraw()
{
    for (size_t i = 0; i < bulletEntities.size(); i++)
    {
        DrawTexturePro(playerBulletTex,
            bulletRectangle,
            RayDefs::GetRectangle(bulletEntities[i].transform.position.x, bulletEntities[i].transform.position.y, bulletRectangle.width, bulletRectangle.height),
            bulletCenter,
            bulletEntities[i].transform.rotation,
            RAYWHITE);
    }
    for (size_t i = 0; i < asteroidEntities.size(); i++)
    {
        DrawTexturePro(asteroidTex,
            asteroidRectangle,
            RayDefs::GetRectangle(asteroidEntities[i].transform.position.x, asteroidEntities[i].transform.position.y, asteroidRectangle.width * ASTEROID_SCALE, asteroidRectangle.height * ASTEROID_SCALE),
            asteroidCenter,
            asteroidEntities[i].transform.rotation,
            RAYWHITE);
    }
    for (size_t i = 0; i < smallAsteroidEntities.size(); i++)
    {
        DrawTexturePro(asteroidTex,
            asteroidRectangle,
            RayDefs::GetRectangle(smallAsteroidEntities[i].transform.position.x, smallAsteroidEntities[i].transform.position.y, asteroidRectangle.width * SMALL_ASTEROID_SCALE, asteroidRectangle.height * SMALL_ASTEROID_SCALE),
            asteroidCenter,
            smallAsteroidEntities[i].transform.rotation,
            RAYWHITE);
    }
    for (size_t i = 0; i < miniAsteroidEntities.size(); i++)
    {
        DrawTexturePro(asteroidTex,
            asteroidRectangle,
            RayDefs::GetRectangle(miniAsteroidEntities[i].transform.position.x, miniAsteroidEntities[i].transform.position.y, asteroidRectangle.width * MINI_ASTEROID_SCALE, asteroidRectangle.height * MINI_ASTEROID_SCALE),
            asteroidCenter,
            miniAsteroidEntities[i].transform.rotation,
            RAYWHITE);
    }
}
void EntityManager::EntityAwake()
{
    /*if (bulletEntities.empty())
    {
        bulletEntities.push_back(ReturnBullet(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
    }*/
}
void EntityManager::EntityStart()
{
    playerBulletTex = LoadTexture("../sprites/bullet.png");
    bulletRectangle = RayDefs::GetRectangle(0, 0, playerBulletTex.width, playerBulletTex.height);
    bulletCenter = RayDefs::GetVector2((playerBulletTex.width / 2), (playerBulletTex.height / 2));

    asteroidTex = LoadTexture("../sprites/asteroidone.png");
    asteroidRectangle = RayDefs::GetRectangle(0, 0, asteroidTex.width, asteroidTex.height);
    asteroidCenter = RayDefs::GetVector2((asteroidTex.width / 2) / ASTEROID_SCALE_DIVISION, (asteroidTex.height / 2) / ASTEROID_SCALE_DIVISION);

    smallAsteroidCenter = RayDefs::GetVector2((asteroidTex.width / 2) / SMALL_ASTEROID_SCALE_DIVISION, (asteroidTex.height / 2) / SMALL_ASTEROID_SCALE_DIVISION);
    miniAsteroidCenter = RayDefs::GetVector2((asteroidTex.width / 2) / MINI_ASTEROID_SCALE_DIVISION, (asteroidTex.height / 2) / MINI_ASTEROID_SCALE_DIVISION);
}
void EntityManager::EntityEnd()
{
    UnloadTexture(playerBulletTex);
    UnloadTexture(asteroidTex);
}
Bullet EntityManager::ReturnBullet(float x, float y, float rot, float scale, float speed, float lifetime)
{
    return {{{x, y}, rot, scale}, speed, lifetime};
}

void EntityManager::DeleteAll()
{
    bulletEntities.clear();
    asteroidEntities.clear();
    smallAsteroidEntities.clear();
    miniAsteroidEntities.clear();
}

bool EntityManager::CheckCollisionBullet(Transformation bulletTransform, Transformation asteroidTransform)
{
    float distance = CheckDistance(bulletTransform, asteroidTransform); //CheckDistance(bulletTransform, asteroidTransform);
    //printf("%f\n", distance);

    if (distance < bulletTransform.scale + asteroidTransform.scale)
        return true; 

    return false;
}

float EntityManager::CheckDistance(Transformation firstTransform, Transformation secondTransform)
{
    Vector2 vec2Distance; 
    vec2Distance.x = firstTransform.position.x -= secondTransform.position.x;
    vec2Distance.y = firstTransform.position.y -= secondTransform.position.y;

    float distance = vec2Distance.x + vec2Distance.y;

    if (distance < 0.0f)
        distance = distance * -1;

    return distance;
    //return sqrtf(firstTransform.position.x - secondTransform.position.x)*(firstTransform.position.x - secondTransform.position.x) + (firstTransform.position.y - secondTransform.position.y)*(firstTransform.position.y - secondTransform.position.y);
}

void ReleaseEntity::RemoveBullet(size_t bulletToRemove)
{   
    EntityManager::bulletEntities.erase(EntityManager::bulletEntities.begin() + bulletToRemove);
}
void ReleaseEntity::RemoveAsteroid(size_t asteroidToRemove)
{
    EntityManager::asteroidEntities.erase(EntityManager::asteroidEntities.begin() + asteroidToRemove);
}
void ReleaseEntity::RemoveSmallAsteroid(size_t smallAsteroidToRemove)
{
    EntityManager::smallAsteroidEntities.erase(EntityManager::smallAsteroidEntities.begin() + smallAsteroidToRemove);
}
void ReleaseEntity::RemoveMiniAsteroid(size_t miniAsteroidToRemove)
{
    EntityManager::miniAsteroidEntities.erase(EntityManager::miniAsteroidEntities.begin() + miniAsteroidToRemove);
}
