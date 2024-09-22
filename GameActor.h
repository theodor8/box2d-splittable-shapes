#pragma once

#include <vector>

class GameActor
{
public:

    virtual void Step(); // Override in subclass
    
};

extern std::vector<std::unique_ptr<GameActor>> gameActors;

template <typename T, typename... Args> // Add subclass to vector
T* CreateGameActor(Args&&... args)
{
    gameActors.emplace_back(std::unique_ptr<T>(new T(args...)));
    return (T*)gameActors.back().get();
}

void DestroyGameActor(GameActor* gameActor);