#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>

class ResourceManager;
class Module;
class Dot;

class Game
{
public:
    Game(ResourceManager& rm);   

    bool Update();
    void Clear();

    template <class T>
    typename T* GetModule(const std::string& name) const
    {
        return (T*)modules.at(name);
    }

    ResourceManager& RM;
    mutable bool quit = false;

private:
    std::map<std::string, Module*> modules;

};
