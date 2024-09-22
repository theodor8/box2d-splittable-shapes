#include "DebugDraw.h"

extern Renderer renderer;

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{

}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    b2Color fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);

    for (int32 i = 1; i < vertexCount - 1; ++i)
    {
        renderer.DrawFilledTriangle(vertices[0], vertices[i], vertices[i + 1], fillColor);
    }

    b2Vec2 p1 = vertices[vertexCount - 1];
    for (int32 i = 0; i < vertexCount; ++i)
    {
        b2Vec2 p2 = vertices[i];
        renderer.DrawLine(p1, p2, color);
        p1 = p2;
    }
}

void DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{

}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
    b2Color fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
    renderer.DrawFilledCircle(center, radius, fillColor);

    renderer.DrawCircle(center, radius, color);

    // Draw a line fixed in the circle to animate rotation.
    b2Vec2 p = center + radius * axis;
    renderer.DrawLine(center, p, color);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    renderer.DrawLine(p1, p2, color);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{

}

void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
    
}