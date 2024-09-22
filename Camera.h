#pragma once

#include "box2d/box2d.h"

#include "common.h"

class Camera
{
public:
    b2Vec2 pos;
    float zoom;

    Camera(float x, float y, float zoom);

    b2Vec2 worldToScreen(const b2Vec2& worldPoint);

    b2Vec2 screenToWorld(const b2Vec2& screenPoint);

};