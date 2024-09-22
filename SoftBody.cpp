#include "SoftBody.h"

#include "box2d/box2d.h"

#include "Renderer.h"
#include "DebugDraw.h"

extern b2World* world;
extern Renderer renderer;

void SoftBody::Step()
{
    for (auto& edge : edges)
    {
        b2Vec2 dir = edge.bodyB->GetPosition() - edge.bodyA->GetPosition();
        float dist = dir.LengthSquared();
        dir.Normalize();
        float difDist = edge.length - dist;
        dir *= difDist * strength;
        edge.bodyA->ApplyForceToCenter(-dir, true);
        edge.bodyB->ApplyForceToCenter(dir, true);
    }

    DrawEdges();
    //DrawPolygon();

    GameActor::Step();
}

void SoftBody::AddEdge(b2Body* bodyA, b2Body* bodyB)
{
    float length = (bodyA->GetPosition() - bodyB->GetPosition()).LengthSquared();
    edges.push_back({bodyA, bodyB, length});
}

void SoftBody::ConnectVertices(const b2Vec2* vs, int count)
{
    // Prepare
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.angularDamping = 1.0f;
    bodyDef.linearDamping = 0.8f;

    b2FixtureDef fixtureDef;
    b2CircleShape shape;
    shape.m_radius = 0.02f;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.5f;
    fixtureDef.shape = &shape;

    b2Body* bodies[count];

    for (int i = 0; i < count; i++)
    {
        bodyDef.position = vs[i];
        b2Body* body = world->CreateBody(&bodyDef);
        body->CreateFixture(&fixtureDef);
        bodies[i] = body;
    }


    for (int i = 0; i < count; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            AddEdge(bodies[i], bodies[j]);
        }
    }
}

void SoftBody::DrawEdges()
{
    for (auto& edge : edges)
    {
        renderer.DrawLine(edge.bodyA->GetPosition(), edge.bodyB->GetPosition(), {1.0f, 0.0f, 0.0f});
    }
}

void SoftBody::MakeCircle(const b2Vec2& pos, float radius, int numVertices)
{
    float da = 2.0f * M_PI / (float)numVertices;
    b2Vec2 vs[numVertices];
    int count;
    float a = 0.0f;
    for (count = 0; count < numVertices; count++)
    {
        vs[count] = b2Vec2(cosf(a) * radius, sinf(a) * radius) + pos;
        a += da;
    }
    ConnectVertices(vs, count);
}

void SoftBody::MakeRect(const b2Vec2& pos, int rows, int cols, float edgeLen)
{
    b2Vec2 vs[rows * cols];
    int count = 0;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            vs[count] = {c * edgeLen + pos.x, r * edgeLen + pos.y};
            count++;
        }
    }
    ConnectVertices(vs, count);
}

void SoftBody::DrawPolygon()
{
    if (!edges.size()) return;
    b2Vec2 v1 = edges[0].bodyA->GetPosition();
    for (auto& edge : edges)
    {
        renderer.DrawFilledTriangle(v1, edge.bodyA->GetPosition(), edge.bodyB->GetPosition(), {1.0f, 0.0f, 0.0f});
    }
}

float ccw(const b2Vec2& A, const b2Vec2& B, const b2Vec2& C)
{
    return (C.y-A.y) * (B.x-A.x) > (B.y-A.y) * (C.x-A.x);
}

// Return true if line segments AB and CD intersect
bool intersect( const b2Vec2& A, const b2Vec2& B, const b2Vec2& C, const b2Vec2& D)
{
    return ccw(A,C,D) != ccw(B,C,D) && ccw(A,B,C) != ccw(A,B,D);
}
    

void SoftBody::Split(const b2Vec2& v1, const b2Vec2& v2)
{
    for (int i = edges.size() - 1; i >= 0; i--)
    {
        if (intersect(v1, v2, edges[i].bodyA->GetPosition(), edges[i].bodyB->GetPosition()))
        {
            b2Body* bodyA = edges[i].bodyA;
            b2Body* bodyB = edges[i].bodyB;
            edges.erase(edges.begin() + i);

            // Destroy bodies if no edges has them
            bool destroyBodyA = true, destroyBodyB = true;
            for (auto& edge : edges)
            {
                if (edge.bodyA == bodyA || edge.bodyB == bodyA) destroyBodyA = false;
                if (edge.bodyA == bodyB || edge.bodyB == bodyB) destroyBodyB = false;
                if (!destroyBodyA && !destroyBodyB) break;
            }
            if (destroyBodyA) world->DestroyBody(bodyA);
            if (destroyBodyB) world->DestroyBody(bodyB);
        }
    }
}