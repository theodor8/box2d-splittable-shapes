#pragma once

#include <memory>
#include <vector>

#include "SDL2/SDL.h"
#include "box2d/box2d.h"

#include "GameActor.h"


class GameObject : public GameActor // GameActor for bodies
{
public:
    b2Body* body;
    bool isDead = false;

    void Step() override;

    virtual void PostSolve(b2Body* other, b2Contact* contact, const b2ContactImpulse* impulse);
};


extern std::vector<std::unique_ptr<GameActor>> gameActors;

template <typename T, typename... Args> // Add subclass to vector
T* CreateGameObject(b2Body* body, Args&&... args)
{
    auto gameObject = CreateGameActor<T>(args...);

    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(gameObject);
    gameObject->body = body;
    
    return gameObject;
}