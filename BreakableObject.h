#pragma once

#include "box2d/box2d.h"
#include "GameObject.h"

class BreakableObject : public GameObject
{
public:
    float strength;
    std::vector<std::array<b2Vec2, 2>> splitQueue; // World points

    BreakableObject(float strength)
        : strength(strength) {}
    void Step() override;
    void PostSolve(b2Body* other, b2Contact* contact, const b2ContactImpulse* impulse) override;
};