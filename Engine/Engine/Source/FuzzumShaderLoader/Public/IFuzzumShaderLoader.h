#pragma once
#include <vector>
#include <cstdint>
#include "ModuleManager/IModule.h"

namespace Fuzzum {

struct FShaderBlob {
    std::vector<uint8_t> Data;
    bool IsValid() const { return !Data.empty(); }
};

struct FShaderProgram {
    FShaderBlob Vertex;
    FShaderBlob Pixel;
    bool IsValid() const { return Vertex.IsValid() && Pixel.IsValid(); }
};

// ИСПРАВЛЕНО: наследуемся от IModule
class IFuzzumShaderLoader : public IModule {
public:
    virtual ~IFuzzumShaderLoader() = default;
    virtual bool LoadFromFile(const char* FilePath) = 0;
    virtual FShaderProgram GetProgram(const char* ProgramName) = 0;
};

} // namespace Fuzzum