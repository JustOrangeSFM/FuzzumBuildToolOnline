#pragma once

// === НАСТРОЙКИ (можно менять до включения заголовка) ===
#ifndef LOGGER_ENABLED
#define LOGGER_ENABLED 1
#endif

#if !LOGGER_ENABLED
    // Полностью выключаем логгер в релизе
#define LOG_TRACE(...)      ((void)0)
#define LOG_DEBUG(...)      ((void)0)
#define LOG_INFO(...)       ((void)0)
#define LOG_WARN(...)       ((void)0)
#define LOG_ERROR(...)      ((void)0)
#define LOG_CRITICAL(...)   ((void)0)
#define LoggerConfig(...)   ((void)0)
#else

#include <string>
#include <cstdio>
#include <mutex>
#include <cstdarg>

// Платформа
#if defined(_WIN32)
#define LOGGER_PLATFORM_WINDOWS 1
#elif defined(__ANDROID__)
#define LOGGER_PLATFORM_ANDROID 1
#elif defined(__linux__)
#define LOGGER_PLATFORM_LINUX 1
#elif defined(__APPLE__)
#define LOGGER_PLATFORM_APPLE 1
#else
#error "Unsupported platform"
#endif

// Уровни
enum class LogLevel {
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warn = 3,
    Error = 4,
    Critical = 5
};

class Logger {
public:
    // Singleton
    static Logger& Get();

    // Настройки (вызывать ДО первого лога!)
    void SetLogToFile(bool Enable);
    void SetLogPath(const char* Path);              // по умолчанию: ./logs/app.log
    void SetMaxFileSize(size_t Bytes);              // по умолчанию: 20 MB
    void SetMinLogLevel(LogLevel Level);            // по умолчанию: Debug
    void SetFlushOnWrite(bool Flush);               // сразу сбрасывать на диск

    // Прямой вызов (редко нужен)
    void Log(LogLevel Level, const char* Format, ...);

    Logger();
    ~Logger();

    void WriteLog(LogLevel Level, const char* Message);
    void EnsureDirectory(const std::string& Path);
    std::string GetTimestamp();
    std::string LevelToString(LogLevel Level) const;
    bool ShouldLog(LogLevel Level) const;

    bool LogToFile = true;
    bool FlushOnWrite = true;
    std::string LogFilePath;
    size_t MaxFileSize = 20 * 1024 * 1024; // 20 MB
    LogLevel MinLevel = LogLevel::Debug;
    FILE* FileHandle = nullptr;
    mutable std::mutex Mutex;
};

// Макросы — безопасны, быстры, не вычисляют аргументы если уровень ниже
#define LOG_IMPL(LEVEL, ...) \
    Logger::Get().Log(LogLevel::LEVEL, __VA_ARGS__)

#define LOG_TRACE(...)      LOG_IMPL(Trace, __VA_ARGS__)
#define LOG_DEBUG(...)      LOG_IMPL(Debug, __VA_ARGS__)
#define LOG_INFO(...)       LOG_IMPL(Info, __VA_ARGS__)
#define LOG_WARN(...)       LOG_IMPL(Warn, __VA_ARGS__)
#define LOG_ERROR(...)      LOG_IMPL(Error, __VA_ARGS__)
#define LOG_CRITICAL(...)   LOG_IMPL(Critical, __VA_ARGS__)

#define LoggerConfig(ToFile, Path, Level, MaxSize) \
    do { \
        auto& logger = Logger::Get(); \
        logger.SetLogToFile(ToFile); \
        if (Path) logger.SetLogPath(Path); \
        logger.SetMinLogLevel(Level); \
        logger.SetMaxFileSize(MaxSize); \
    } while(0)
#endif // LOGGER_ENABLED