#include <iostream>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "box2d/box2d.h"

#include "DebugDraw.h"
#include "ContactListener.h"
#include "Renderer.h"
#include "Camera.h"
#include "common.h"

#include "GameActor.h"
#include "GameObject.h"
#include "BreakableObject.h"
#include "SoftBody.h"
#include "Particle.h"


b2World* world;
Renderer renderer;
Camera camera(0.0f, -12.0f, 18.0f);
DebugDraw debugDraw;
ContactListener contactListener;


std::vector<std::unique_ptr<GameActor>> gameActors;





int main()
{
    srand(time(NULL));

    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    world = new b2World({GRAVITY_X, GRAVITY_Y}); // gravity
    world->SetDebugDraw(&debugDraw);
    world->SetContactListener(&contactListener);
    debugDraw.SetFlags(b2Draw::e_shapeBit);
    // x, y 0 i mitten, y ökar ner
    


    b2Body* groundBody;
    {
        b2BodyDef bobyDef;
        bobyDef.type = b2_staticBody;
        groundBody = world->CreateBody(&bobyDef);

        b2ChainShape shape;
        float w = 40.0f, h = 25.0f;
        b2Vec2 vs[] = {
            {w / 2, -h},
            {-w / 2, -h},
            {-w / 2, 0.0f},
            {w / 2, 0.0f},
        };
        shape.CreateLoop(vs, 4);
        groundBody->CreateFixture(&shape, 1.0f);
    }

    // tower
    float x = 5.0f;
    float y = 0.0f;
    float w = 10.0f;
    float h = 2.0f;
    float thickness = 0.9f;
    float strength = 13.0f;
    for (int i = 0; i < 8; i++)
    {
        // CreateGameObject<BreakableObject>(CreateBox(7.5f, -2.0f - 2.5 * i, 0.5f, 2.0f));
        // CreateGameObject<BreakableObject>(CreateBox(7.5f, -2.5f - 2.5 * i, 3.0f, 0.5f));
        // CreateGameObject<BreakableObject>(CreateBox(10.0f, -2.0f - 2.5 * i, 0.5f, 2.0f));
        CreateGameObject<BreakableObject>(CreateBox(x, y - h * (i + 1) + thickness, thickness, h - thickness), strength);
        CreateGameObject<BreakableObject>(CreateBox(x + w - thickness, y - h * (i + 1) + thickness, thickness, h - thickness), strength);
        CreateGameObject<BreakableObject>(CreateBox(x, y - h * (i + 1), w, thickness), strength);
    }

    auto box = CreateGameObject<BreakableObject>(CreateBox(-10.0f, -10.0f, 2.0f, 5.0f), 10.0f);
    box->body->SetAngularVelocity(0.5f);


    // auto softBody = CreateGameActor<SoftBody>(0.005);
    // softBody->MakeCircle({9.0f, -20.0f}, 3.0f, 10);
    // softBody->MakeRect({7.0f, -15.0f}, 4, 3, 3.0f);





    auto bullet = CreateGameObject<BreakableObject>(CreateCircle(-12.0f, -15.0f, 0.8f, 25.0f), 10.0f);
    bullet->body->SetBullet(true);
    

    bool isHolding = false;
    b2Vec2 mouseHoldStart;


    float timeStep = 1.0f / 60.0f;
    Uint32 delay = timeStep * 1000;
    SDL_bool quit = SDL_FALSE;
    while (!quit)
    {
        SDL_Delay(delay);
    
        int mouseScreenX, mouseScreenY;
        Uint32 mouseButtons { SDL_GetMouseState(&mouseScreenX, &mouseScreenY) };
        b2Vec2 mouseWorldPos = camera.screenToWorld({(float)mouseScreenX, (float)mouseScreenY});


        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    quit = SDL_TRUE;
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            quit = SDL_TRUE;
                            break;
                            
                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }
        if (keyboardState[SDL_SCANCODE_W]) camera.pos.y -= 0.2f;
        if (keyboardState[SDL_SCANCODE_S]) camera.pos.y += 0.2f;
        if (keyboardState[SDL_SCANCODE_A]) camera.pos.x -= 0.2f;
        if (keyboardState[SDL_SCANCODE_D]) camera.pos.x += 0.2f;
        if (keyboardState[SDL_SCANCODE_Q]) camera.zoom -= 0.2f;
        if (keyboardState[SDL_SCANCODE_E]) camera.zoom += 0.2f;
        renderer.Clear();


        // split shapes by holding left mouse and dragging cursor over them
        if (mouseButtons & SDL_BUTTON_LMASK)
        {
            if (!isHolding)
            {
                mouseHoldStart = mouseWorldPos;
                isHolding = true;
            }
            renderer.DrawLine(mouseHoldStart, mouseWorldPos, {1.0f, 0.0f, 0.0f});
        }
        else if (isHolding)
        {
            isHolding = false;

            // Split all shapes
            for (int i = 0; i < gameActors.size(); i++)
            {
                auto gameActor = gameActors[i].get();
                if (typeid(*gameActor) != typeid(BreakableObject)) continue;
                auto breakableObject = (BreakableObject*)gameActor;
                breakableObject->splitQueue.push_back({mouseHoldStart, mouseWorldPos});
            }
        }
        
        // if (keyboardState[SDL_SCANCODE_SPACE])
        // {
        //     CreateGameObject<GameObject>(CreateCircle(mouseWorldPos.x, mouseWorldPos.y, 0.1f));
        // }





        world->Step(timeStep, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
        world->DebugDraw();

        

        for (int i = gameActors.size() - 1; i >= 0; i--)
        {
            gameActors[i]->Step();
        }

        //std::cout << gameActors.size() << std::endl;
        
        renderer.Present();
    }


    

    delete world;
    SDL_Quit();
    return EXIT_SUCCESS;
}