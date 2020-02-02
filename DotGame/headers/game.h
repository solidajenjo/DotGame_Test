#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <globals.h>

class ResourceManager;
class Module;
class Dot;

class Game
{
public:

    enum class States 
    {
        INTRO,
        PLAYING,
        GAME_OVER
    };

    Game(ResourceManager& rm);   

    bool Update();
    void Clear();
    void SetTimer();

    template <class T>
    T* GetModule(const std::string& name) const
    {
        return (T*)modules.at(name);
    }

    ResourceManager& RM;
    States state = States::INTRO;

    mutable bool gameOver = false;

private:
    std::map<std::string, Module*> modules;
    float timer;
};
