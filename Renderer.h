#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "box2d/box2d.h"


class Renderer
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    Renderer();

    ~Renderer();

    void Clear();

    void Present();

    void DrawLine(const b2Vec2& v1, const b2Vec2& v2, const b2Color& color);

    void DrawFilledTriangle(const b2Vec2& v1, const b2Vec2& v2, const b2Vec2& v3, const b2Color& color);

    void DrawCircle(const b2Vec2& center, float radius, const b2Color& color);

    void DrawFilledCircle(const b2Vec2& center, float radius, const b2Color& color);

    SDL_Texture* LoadTexture(const char* fileName);

    void DrawTexture(SDL_Texture* texture, const b2Vec2& pos, const b2Vec2& size, float angle);
};

