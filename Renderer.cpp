#include "Renderer.h"

#include <string>

#include "SDL2/SDL.h"
#include "box2d/box2d.h"

#include "common.h"
#include "Camera.h"


extern Camera camera;

Renderer::Renderer()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Renderer::Clear()
{
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderClear(renderer);
}

void Renderer::Present()
{
    SDL_RenderPresent(renderer);
}

void Renderer::DrawLine(const b2Vec2& v1, const b2Vec2& v2, const b2Color& color)
{
    b2Vec2 v1Screen = camera.worldToScreen(v1);
    b2Vec2 v2Screen = camera.worldToScreen(v2);
    SDL_SetRenderDrawColor(renderer, color.r * 255.0f, color.g * 255.0f, color.b * 255.0f, color.a * 255.0f);
    SDL_RenderDrawLineF(renderer, v1Screen.x, v1Screen.y, v2Screen.x, v2Screen.y);
}

void Renderer::DrawFilledTriangle(const b2Vec2& v1, const b2Vec2& v2, const b2Vec2& v3, const b2Color& color)
{
    SDL_Color fillColor = {
        (Uint8)(color.r * 255.0f),
        (Uint8)(color.g * 255.0f),
        (Uint8)(color.b * 255.0f),
        (Uint8)(color.a * 255.0f)
    };

    b2Vec2 v1Screen = camera.worldToScreen(v1);
    b2Vec2 v2Screen = camera.worldToScreen(v2);
    b2Vec2 v3Screen = camera.worldToScreen(v3);
    SDL_Vertex vs[] = {
        {{v1Screen.x, v1Screen.y}, fillColor},
        {{v2Screen.x, v2Screen.y}, fillColor},
        {{v3Screen.x, v3Screen.y}, fillColor},
    };

    SDL_RenderGeometry(renderer, nullptr, vs, 3, nullptr, 0);
}

void Renderer::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
    const float k_segments = 32.0f;
    const float k_increment = 2.0f * b2_pi / k_segments;
    float sinInc = sinf(k_increment);
    float cosInc = cosf(k_increment);
    b2Vec2 r1(1.0f, 0.0f);
    b2Vec2 v1 = center + radius * r1;
    for (int32 i = 0; i < k_segments; ++i)
    {
        b2Vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
        b2Vec2 v2 = center + radius * r2;
        DrawLine(v1, v2, color);
        r1 = r2;
        v1 = v2;
    }
}

void Renderer::DrawFilledCircle(const b2Vec2& center, float radius, const b2Color& color)
{
    const float k_segments = 32.0f;
    const float k_increment = 2.0f * b2_pi / k_segments;
    float sinInc = sinf(k_increment);
    float cosInc = cosf(k_increment);
    b2Vec2 v0 = center;
    b2Vec2 r1(cosInc, sinInc);
    b2Vec2 v1 = center + radius * r1;
    for (int32 i = 0; i < k_segments; ++i)
    {
        // Perform rotation to avoid additional trigonometry.
        b2Vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
        b2Vec2 v2 = center + radius * r2;
        DrawFilledTriangle(v0, v1, v2, color);
        r1 = r2;
        v1 = v2;
    }
}

SDL_Texture* Renderer::LoadTexture(const char* fileName)
{
    std::string path = "src/assets/";
    path += fileName;
    SDL_Surface* surface = IMG_Load(path.c_str());
    
    // if(!surface)
    //     return EXIT_FAILURE;
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void Renderer::DrawTexture(SDL_Texture* texture, const b2Vec2& pos, const b2Vec2& size, float angle)
{
    b2Vec2 screenPos = camera.worldToScreen(pos);
    b2Vec2 screenSize(size.x * camera.zoom, size.y * camera.zoom);
    SDL_Rect dsrect;
    dsrect.x = screenPos.x; dsrect.y = screenPos.y; dsrect.w = screenSize.x; dsrect.h = screenSize.y;
    SDL_RenderCopyEx(renderer, texture, NULL, &dsrect, angle * 180.0f * M_1_PI, NULL, SDL_FLIP_NONE);
}