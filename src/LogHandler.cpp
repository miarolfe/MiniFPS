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
        delete s_instance;
    }

    LogHandler& LogHandler::GetInstance()
    {
        if (s_instance == nullptr)
        {
            if (!s_initialized) {
                s_instance = new LogHandler();
                s_initialized = true;
            }
            else
            {
                // This should never happen
            }
        }

        return *s_instance;
    }

    void LogHandler::Update()
    {
        LogHandler::GetInstance();

        if (!LogHandler::GetInstance().m_msgQueue.empty())
        {
            while (!LogHandler::GetInstance().m_msgQueue.empty())
            {
                LogMsg logMsg = LogHandler::GetInstance().m_msgQueue.front();
                switch (logMsg.type)
                {
                    case LOG_INFO:
                    {
                        LogHandler::GetInstance().m_outfile << "[INFO   ] ";
                        std::cout << "[INFO   ] " << LogHandler::GetInstance().m_msgQueue.front().msg << std::endl;
                        break;
                    }
                    case LOG_WARNING:
                    {
                        LogHandler::GetInstance().m_outfile << "[WARNING] ";
                        std::cout << "[WARNING] " << LogHandler::GetInstance().m_msgQueue.front().msg << std::endl;
                        break;
                    }
                    case LOG_ERROR:
                    {
                        LogHandler::GetInstance().m_outfile << "[ERROR  ] ";
                        std::cerr << "[ERROR  ] " << LogHandler::GetInstance().m_msgQueue.front().msg << std::endl;
                        break;
                    }
                }
                LogHandler::GetInstance().m_outfile << LogHandler::GetInstance().m_msgQueue.front().msg << std::endl;
                LogHandler::GetInstance().m_msgQueue.pop();
            }
        }
    }

    void LogHandler::Log(const char* msg)
    {
        GetInstance().m_msgQueue.push({LOG_INFO, msg});
    }

    void LogHandler::LogWarning(const char* msg)
    {
        GetInstance().m_msgQueue.push({LOG_WARNING, msg});
    }

    void LogHandler::LogError(const char* msg)
    {
        GetInstance().m_msgQueue.push({LOG_ERROR, msg});
    }

    LogHandler* LogHandler::s_instance = nullptr;
    bool LogHandler::s_initialized = false;
}