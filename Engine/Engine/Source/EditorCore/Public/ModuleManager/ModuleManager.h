#pragma once
#include <unordered_map>
#include <memory>
#include "IModule.h"

class ModuleManager
{
public:
    static ModuleManager& Get();

    void LoadModulesFromDirectory(const std::string& dirPath);
    void StartupAll();  
    void ShutdownAll();

    
    template<typename T>
    T* GetModule(const char* name) const
    {
        static_assert(std::is_base_of_v<IModule, T>, "T must inherit from IModule");
        auto it = moduleMap_.find(name);
        return it != moduleMap_.end() ? static_cast<T*>(it->second) : nullptr;
    }
	
	 size_t GetModuleCount() const { return modules_.size(); }

private:
    void TopologicalStartup();  
    struct LoadedModule
    {
        std::unique_ptr<class DynamicLibrary> library;
        IModule* instance = nullptr;
    };

    std::vector<LoadedModule> modules_;
    std::unordered_map<std::string, IModule*> moduleMap_;
};