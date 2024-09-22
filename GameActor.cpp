#include "GameActor.h"

#include <vector>

#include "box2d/box2d.h"

extern b2World* world;
extern std::vector<std::unique_ptr<GameActor>> gameActors;


void GameActor::Step()
{
    
}


void DestroyGameActor(GameActor* gameActor)
{
    //if (gameActor->body) world->DestroyBody(gameActor->body);

    for (int i = 0; i < gameActors.size(); ++i)
    {
        if (gameActors[i].get() == gameActor)
        {
            std::swap(gameActors[i], gameActors[gameActors.size() - 1]);
            gameActors.pop_back();
            break;
        }
    }
}