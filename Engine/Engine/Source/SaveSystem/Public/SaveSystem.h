#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <optional>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <type_traits>
#include <memory>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif
#include <windows.h>
#include <shlobj.h>
#endif

namespace Fuzzum
{

    // ============================================================================
    // Type traits
    // ============================================================================

    namespace detail
    {
        template<typename T>
        struct is_vector : std::false_type {};

        template<typename T, typename A>
        struct is_vector<std::vector<T, A>> : std::true_type {};

        template<typename T>
        struct is_map : std::false_type {};

        template<typename K, typename V, typename C, typename A>
        struct is_map<std::map<K, V, C, A>> : std::true_type {};
    }

    // ============================================================================
    // Forward declarations
    // ============================================================================

    class FSaveArchive;

    class FSaveGame
    {
    public:
        virtual ~FSaveGame() = default;
        virtual void Serialize(FSaveArchive& Ar) = 0;
    };

    template<typename T>
    typename std::enable_if_t<std::is_base_of_v<FSaveGame, T>, void>
        Serialize(FSaveArchive& Ar, T& Object);

    // ============================================================================
    // Archive
    // ============================================================================

    class FSaveArchive
    {
    public:
        enum class EMode { Save, Load };

        explicit FSaveArchive(const std::string& FilePath, EMode Mode)
            : Stream(), Mode(Mode)
        {
            auto flags = (Mode == EMode::Save)
                ? (std::ios::out | std::ios::binary)
                : (std::ios::in | std::ios::binary);
            Stream.open(FilePath, flags);
            if (!Stream.is_open() || !Stream.good())
                std::cout << "[FSS] Failed to open: " << FilePath << "\n";
        }

        ~FSaveArchive()
        {
            if (Stream.is_open()) Stream.close();
        }

        [[nodiscard]] bool IsValid() const { return Stream.is_open() && Stream.good(); }
        [[nodiscard]] bool IsSaving() const { return Mode == EMode::Save; }
        [[nodiscard]] bool IsLoading() const { return Mode == EMode::Load; }

        template<typename T>
        FSaveArchive& operator<<(T& Value)
        {
            using Raw = std::remove_cv_t<std::remove_reference_t<T>>;

            if constexpr (std::is_arithmetic_v<Raw>)
            {
                if (IsSaving())
                    Stream.write(reinterpret_cast<const char*>(&Value), sizeof(Raw));
                else
                    Stream.read(reinterpret_cast<char*>(&Value), sizeof(Raw));
            }
            else if constexpr (std::is_same_v<Raw, std::string>)
            {
                if (IsSaving())
                {
                    uint32_t len = static_cast<uint32_t>(Value.size());
                    (*this) << len;
                    if (len > 0)
                        Stream.write(reinterpret_cast<const char*>(Value.data()), static_cast<std::streamsize>(len));
                }
                else
                {
                    uint32_t len = 0;
                    (*this) << len;
                    Value.resize(len);
                    if (len > 0)
                        Stream.read(reinterpret_cast<char*>(Value.data()), static_cast<std::streamsize>(len));
                }
            }
            else if constexpr (detail::is_vector<Raw>::value)
            {
                if (IsSaving())
                {
                    uint32_t size = static_cast<uint32_t>(Value.size());
                    (*this) << size;
                    for (auto& item : Value) (*this) << item;
                }
                else
                {
                    uint32_t size = 0;
                    (*this) << size;
                    Value.resize(size);
                    for (auto& item : Value) (*this) << item;
                }
            }
            else if constexpr (detail::is_map<Raw>::value)
            {
                if (IsSaving())
                {
                    uint32_t size = static_cast<uint32_t>(Value.size());
                    (*this) << size;
                    for (const auto& pair : Value)
                    {
                        auto key = pair.first;
                        auto val = pair.second;
                        (*this) << key;
                        (*this) << val;
                    }
                }
                else
                {
                    uint32_t size = 0;
                    (*this) << size;
                    Value.clear();
                    for (uint32_t i = 0; i < size; ++i)
                    {
                        typename Raw::key_type k;
                        typename Raw::mapped_type v;
                        (*this) << k;
                        (*this) << v;
                        Value.emplace(std::move(k), std::move(v));
                    }
                }
            }
            else
            {
                Serialize(*this, Value);
            }

            return *this;
        }

    private:
        std::fstream Stream;
        EMode Mode;
    };

    // ============================================================================
    // Serialize implementation
    // ============================================================================

    template<typename T>
    typename std::enable_if_t<std::is_base_of_v<FSaveGame, T>, void>
        Serialize(FSaveArchive& Ar, T& Obj)
    {
        Obj.Serialize(Ar);
    }

    // ============================================================================
    // Save Handle
    // ============================================================================

    template<typename T>
    class TSaveHandle
    {
    public:
        TSaveHandle(T Obj, std::string Path) : Obj_(std::move(Obj)), Path_(std::move(Path)) {}

        T* operator->() { return &Obj_; }
        const T* operator->() const { return &Obj_; }
        T& operator*() { return Obj_; }
        const T& operator*() const { return Obj_; }

        bool Save() const
        {
            std::filesystem::create_directories(std::filesystem::path(Path_).parent_path());
            FSaveArchive Ar(Path_, FSaveArchive::EMode::Save);
            if (!Ar.IsValid()) return false;
            Ar << const_cast<T&>(Obj_);
            return Ar.IsValid();
        }

    private:
        T Obj_;
        std::string Path_;
    };

    // ============================================================================
    // Вспомогательная функция: сборка полного пути
    // ============================================================================

    inline std::string MakeFullPath(const std::string& Directory, const std::string& FileName, const std::string& Extension = ".fzsave")
    {
        std::filesystem::path Dir(Directory);
        Dir.make_preferred();
        return (Dir / (FileName + Extension)).string();
    }

    // ============================================================================
    // Основной API: вы задаёте отдельно Directory и FileName
    // ============================================================================

    template<typename T>
    bool SaveToPath(const T& Obj, const std::string& Directory, const std::string& FileName)
    {
        static_assert(std::is_base_of_v<FSaveGame, T>, "T must inherit from FSaveGame");
        std::string FullPath = MakeFullPath(Directory, FileName);
        std::filesystem::create_directories(std::filesystem::path(FullPath).parent_path());
        FSaveArchive Ar(FullPath, FSaveArchive::EMode::Save);
        if (!Ar.IsValid()) return false;
        Ar << const_cast<T&>(Obj);
        return Ar.IsValid();
    }

    template<typename T>
    std::optional<T> LoadFromPath(const std::string& Directory, const std::string& FileName)
    {
        static_assert(std::is_base_of_v<FSaveGame, T>, "T must inherit from FSaveGame");
        std::string FullPath = MakeFullPath(Directory, FileName);
        if (!std::filesystem::exists(FullPath))
        {
            std::cout << "[FSS] File not found: " << FullPath << "\n";
            return std::nullopt;
        }
        FSaveArchive Ar(FullPath, FSaveArchive::EMode::Load);
        if (!Ar.IsValid()) return std::nullopt;
        T Obj{};
        Ar << Obj;
        return Obj;
    }

    template<typename T>
    TSaveHandle<T> LoadOrNewFromPath(const std::string& Directory, const std::string& FileName)
    {
        if (auto Loaded = LoadFromPath<T>(Directory, FileName))
            return TSaveHandle<T>(std::move(*Loaded), MakeFullPath(Directory, FileName));
        return TSaveHandle<T>(T{}, MakeFullPath(Directory, FileName));
    }

    // ============================================================================
    // Старый слотовый API (опционально)
    // ============================================================================

    namespace Private
    {
        inline std::string GetSaveDirectory()
        {
#ifdef _WIN32
            char PathBuffer[MAX_PATH];
            if (SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, PathBuffer)))
                return std::string(PathBuffer) + "\\Fuzzum\\Saved\\";
            return ".\\Saved\\";
#elif defined(__ANDROID__)
            return "/data/data/com.fuzzum.game/files/";
#elif defined(__linux__) || defined(__APPLE__)
            const char* Home = std::getenv("HOME");
            return Home ? (std::string(Home) + "/.fuzzum/saved/") : "./saved/";
#else
            return "./saved/";
#endif
        }

        inline std::string ConstructSaveFilePath(const std::string& Class, const std::string& Slot)
        {
            std::string Dir = GetSaveDirectory();
            std::filesystem::create_directories(Dir);
            return Dir + Class + "_" + Slot + ".fzsave";
        }
    }

    template<typename T>
    bool SaveToSlot(const T& Obj, const std::string& Class, const std::string& Slot)
    {
        return SaveToPath(Obj, Private::GetSaveDirectory(), Class + "_" + Slot);
    }

    template<typename T>
    std::optional<T> LoadFromSlot(const std::string& Class, const std::string& Slot)
    {
        return LoadFromPath<T>(Private::GetSaveDirectory(), Class + "_" + Slot);
    }

    template<typename T>
    TSaveHandle<T> LoadOrNew(const std::string& Class, const std::string& Slot)
    {
        return LoadOrNewFromPath<T>(Private::GetSaveDirectory(), Class + "_" + Slot);
    }

    inline bool DeleteSlot(const std::string& Class, const std::string& Slot)
    {
        std::string Path = Private::ConstructSaveFilePath(Class, Slot);
        std::error_code ec;
        return std::filesystem::remove(Path, ec);
    }

} // namespace Fuzzum