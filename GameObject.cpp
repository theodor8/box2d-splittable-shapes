#include "GameObject.h"

#include "box2d/box2d.h"
#include "SDL2/SDL.h"

#include "common.h"
#include "GameActor.h"

#include <iostream>


extern b2World* world;

void GameObject::Step()
{
    if (isDead || !body->GetFixtureList())
    {
        world->DestroyBody(body);
        DestroyGameActor(this);
    }

    GameActor::Step();
}

void GameObject::PostSolve(b2Body* other, b2Contact* contact, const b2ContactImpulse* impulse) {}