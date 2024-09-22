#pragma once

#include <box2d/box2d.h>

extern b2World* world;


b2Body* CreateBox(float x, float y, float w, float h, float a = 0.0f, float vx = 0.0f, float vy = 0.0f, float density = 1.0f, float friction = 0.3f, float restitution = 0.5f)
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

b2Body* CreateCircle(float x, float y, float r, float vx = 0.0f, float vy = 0.0f, float density = 1.0f, float friction = 0.3f, float restitution = 0.5f)
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