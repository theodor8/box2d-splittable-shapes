#include "common.h"

#include <vector>

#include "box2d/box2d.h"

extern b2World* world;

b2Vec2 rotateVec2(const b2Vec2& v, float a)
{
    return b2Vec2(cosf(a) * v.x - sinf(a) * v.y, sinf(a) * v.x + cosf(a) * v.y);
}

b2Body* CreateBox(float x, float y, float w, float h, float a, float vx, float vy, float density, float friction, float restitution)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.angle = a;
    bodyDef.position.Set(x + w / 2, y + h / 2);
    b2Body* body = world->CreateBody(&bodyDef);
    body->SetLinearVelocity({vx, vy});

    b2FixtureDef fixtureDef;
    b2PolygonShape shape;
    shape.SetAsBox(w / 2, h / 2); 
    fixtureDef.shape = &shape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
    body->CreateFixture(&fixtureDef);
    return body;
}

b2Body* CreateCircle(float x, float y, float r, float vx, float vy, float density, float friction, float restitution)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    b2Body* body = world->CreateBody(&bodyDef);
    body->SetLinearVelocity({vx, vy});

    b2FixtureDef fixtureDef;
    b2CircleShape shape;
    shape.m_radius = r;
    fixtureDef.shape = &shape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
    body->CreateFixture(&fixtureDef);
    return body;
}

b2Body* CreateEdge(float x1, float y1, float x2, float y2)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(0.0f, 0.0f);
    b2Body* body = world->CreateBody(&bodyDef);

    b2FixtureDef fixtureDef;
    b2EdgeShape shape;
    shape.SetTwoSided({x1, y1}, {x2, y2});
    fixtureDef.shape = &shape;
    fixtureDef.friction = 1.0f;
    body->CreateFixture(&fixtureDef);
    return body;
}

std::vector<b2PolygonShape> SplitFixture(b2Fixture* fixture, const b2Vec2& splitPt1, const b2Vec2& splitPt2)
{

    b2Shape::Type shapeType = fixture->GetType();
    if (shapeType != b2Shape::e_polygon) return {};
    b2PolygonShape* polygonShape = (b2PolygonShape*)fixture->GetShape();

    b2Body* body = fixture->GetBody();
    if (!body) return {};


    // Set up input
    b2RayCastInput inputEntry;
    inputEntry.p1 = splitPt1;
    inputEntry.p2 = splitPt2;
    inputEntry.maxFraction = 1;
    b2RayCastInput inputExit;
    inputExit.p1 = splitPt2;
    inputExit.p2 = splitPt1;
    inputExit.maxFraction = 1;

    b2RayCastOutput outputEntry;
    b2RayCastOutput outputExit;

    if (!fixture->RayCast(&outputEntry, inputEntry, 0)) return {};
    if (!fixture->RayCast(&outputExit, inputExit, 0)) return {};

    b2Vec2 entryPoint = inputEntry.p1 + outputEntry.fraction * (inputEntry.p2 - inputEntry.p1);
    b2Vec2 exitPoint = inputExit.p1 + outputExit.fraction * (inputExit.p2 - inputExit.p1);

    if ((entryPoint - exitPoint).LengthSquared() < 0.01f) return {};

    b2Vec2 entryLocalPoint = body->GetLocalPoint(entryPoint);
    b2Vec2 exitLocalPoint = body->GetLocalPoint(exitPoint);

    b2Vec2 pointsA[8] = {entryLocalPoint, exitLocalPoint};
    b2Vec2 pointsB[8] = {entryLocalPoint, exitLocalPoint};
    int pointsAIndex = 2;
    int pointsBIndex = 2;

    for (int i = 0; i < polygonShape->m_count; i++)
    {
        b2Vec2 v = polygonShape->m_vertices[i];
        float cross = (exitLocalPoint.x - entryLocalPoint.x)*(v.y - entryLocalPoint.y) - (exitLocalPoint.y - entryLocalPoint.y)*(v.x - entryLocalPoint.x);
        if (cross >= 0)
        {
            pointsA[pointsAIndex] = v;
            pointsAIndex++;
        }
        else
        {
            pointsB[pointsBIndex] = v;
            pointsBIndex++;
        }
    }

    b2PolygonShape newShape1;
    b2PolygonShape newShape2;
    newShape1.Set(pointsA, pointsAIndex);
    newShape2.Set(pointsB, pointsBIndex);
    std::vector<b2PolygonShape> newShapes = {newShape1, newShape2};

    return newShapes;
}