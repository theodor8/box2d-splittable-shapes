#include "Camera.h"

Camera::Camera(float x, float y, float zoom)
    : pos(x, y), zoom(zoom) {}

b2Vec2 Camera::worldToScreen(const b2Vec2& worldPoint)
{
    float x = (worldPoint.x - pos.x) * zoom + WIDTH / 2;
    float y = (worldPoint.y - pos.y) * zoom + HEIGHT / 2;
    return {x, y};
}

b2Vec2 Camera::screenToWorld(const b2Vec2& screenPoint)
{
    float x = (screenPoint.x - WIDTH / 2) / zoom + pos.x;
    float y = (screenPoint.y - HEIGHT / 2) / zoom + pos.y;
    return {x, y};
}