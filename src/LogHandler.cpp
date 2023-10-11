#include "LogHandler.h"

namespace MiniFPS
{
    LogHandler::LogHandler()
    {
        // Create the empty log file
        std::ofstream outfile(LOG_FILENAME, std::ios_base::out);
        outfile.close();

        // Open file in append mode
        m_outfile = std::ofstream(LOG_FILENAME, std::ios_base::app);
    }

    LogHandler::~LogHandler()
    {
        m_outfile.close();
    }

    LogHandler& LogHandler::GetInstance()
    {
        if (s_instance == nullptr)
        {
            if (!s_initialized) {
                s_instance = new LogHandler();
                s_initialized = true;
            }
            else {
                std::cerr << "Attempting to reinitialize LogHandler" << std::endl;
            }
        }

        return *s_instance;
    }

    void LogHandler::Update()
    {
        if (!m_msgQueue.empty())
        {
            while (!m_msgQueue.empty())
            {
                m_outfile << m_msgQueue.front().msg << std::endl;
                m_msgQueue.pop();
            }
        }
    }

    void LogHandler::Log(const char* msg)
    {
        m_msgQueue.push({LOG_INFO, msg});
    }

    void LogHandler::LogWarning(const char* msg)
    {
        m_msgQueue.push({LOG_WARNING, msg});
    }

    void LogHandler::LogError(const char* msg)
    {
        m_msgQueue.push({LOG_ERROR, msg});
    }

    LogHandler* LogHandler::s_instance = nullptr;
    bool LogHandler::s_initialized = false;
}