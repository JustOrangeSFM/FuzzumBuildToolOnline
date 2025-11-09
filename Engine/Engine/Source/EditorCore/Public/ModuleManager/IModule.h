#pragma once
#include <string>
#include <vector>

class IModule
{
public:
    virtual ~IModule() = default;
    virtual const char* GetName() const = 0;
    virtual std::vector<const char*> GetDependencies() const { return {}; }
    virtual void Startup() = 0;
    virtual void Shutdown() = 0;
};

extern "C" using CreateModuleFunc = IModule * (*)();