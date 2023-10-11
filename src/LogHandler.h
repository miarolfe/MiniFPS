#pragma once

#include "Common.h"

namespace MiniFPS
{
    const char* const LOG_FILENAME = "log.txt";

    enum LogType
    {
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR
    };

    struct LogMsg
    {
        LogType type;
        const char* msg;
    };

    class LogHandler
    {
    private:
        LogHandler();
        ~LogHandler();

        std::ofstream m_outfile;
        std::queue<LogMsg> m_msgQueue;

        static LogHandler* s_instance;
        static bool s_initialized;

    public:
        static LogHandler& GetInstance();

        LogHandler(const LogHandler&) = delete; // No copy
        LogHandler& operator=(const LogHandler&) = delete; // No assignment
        LogHandler(LogHandler&& other) = delete; // No move
        LogHandler& operator=(LogHandler&& other) = delete; // No move assignment

        void Update();
        void Log(const char* msg);
        void LogWarning(const char* msg);
        void LogError(const char* msg);
    };
}

