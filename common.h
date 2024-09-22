#pragma once

#define WIDTH 900
#define HEIGHT 600

#define VELOCITY_ITERATIONS 8
#define POSITION_ITERATIONS 3

#define GRAVITY_Y 10.0f
#define GRAVITY_X 0.0f

#include <vector>

#include "box2d/box2d.h"

b2Vec2 rotateVec2(const b2Vec2& v, float a);

b2Body* CreateBox(float x, float y, float w, float h, float a = 0.0f, float vx = 0.0f, float vy = 0.0f, float density = 1.0f, float friction = 0.3f, float restitution = 0.5f);

b2Body* CreateCircle(float x, float y, float r, float vx = 0.0f, float vy = 0.0f, float density = 1.0f, float friction = 0.3f, float restitution = 0.5f);

b2Body* CreateEdge(float x1, float y1, float x2, float y2);

// Returns list of new shapes
std::vector<b2PolygonShape> SplitFixture(b2Fixture* fixture, const b2Vec2& splitPt1, const b2Vec2& splitPt2);