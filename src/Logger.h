#ifndef LOGGER_H
#define LOGGER_H

#ifndef NDEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#endif
#include "spdlog/spdlog.h"
#include <memory>
#include "Macros.h"
// this space is deliberately here because of the spdlog order. Do not remove
#include "spdlog/sinks/stdout_color_sinks.h"

class Logger
{
    using consolLogger = spdlog::sinks::stdout_color_sink_st;

public:
    Logger(const Logger&) = delete;
    Logger(Logger&&)      = delete;

    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

    static Logger& GetInstance()
    {
        static Logger instance;
        return instance;
    }

	void Check_GL_ErrorCode();
    spdlog::logger& GetLogger()
    {
        return m_logger;
    }

private:
    Logger()
    {
#ifndef NDEBUG
        m_logger.set_level(spdlog::level::trace);
#else
        m_logger.set_level(spdlog::level::info);
#endif
    }
    spdlog::logger m_logger = spdlog::logger("Console", std::make_shared<consolLogger>());
};


#define CHECK_GL_ERROR_(expr) \
{							\
	expr;						\
	Logger::GetInstance().Check_GL_ErrorCode();\
}

#endif
