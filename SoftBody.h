#pragma once

#include <vector>

#include "box2d/box2d.h"

#include "GameActor.h"

struct Edge
{
    b2Body* bodyA;
    b2Body* bodyB;
    float length;
};


class SoftBody : public GameActor
{
public:
    std::vector<Edge> edges;
    float strength;

    SoftBody(float strength)
        : strength(strength) {}

    void Step() override;

    void AddEdge(b2Body* bodyA, b2Body* bodyB);

    void ConnectVertices(const b2Vec2* vs, int count);

    void DrawEdges();

    void MakeCircle(const b2Vec2& pos, float radius, int numVertices);

    void MakeRect(const b2Vec2& pos, int rows, int cols, float edgeLen);

    void DrawPolygon();

    void Split(const b2Vec2& v1, const b2Vec2& v2);
};

