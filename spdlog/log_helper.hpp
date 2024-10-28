#pragma once
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"

// See https://github.com/gabime/spdlog


#define LOG_TRACE(...) SPDLOG_LOGGER_TRACE(root_logger, __VA_ARGS__);
#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(root_logger, __VA_ARGS__);
#define LOG_INFO(...) SPDLOG_LOGGER_INFO(root_logger, __VA_ARGS__);
#define LOG_WARN(...) SPDLOG_LOGGER_WARN(root_logger, __VA_ARGS__);
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(root_logger, __VA_ARGS__);
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(root_logger, __VA_ARGS__);
extern std::shared_ptr<spdlog::logger> root_logger;


void configure_logging(const std::string& logfile = "logs/main.log");

std::shared_ptr<spdlog::logger> get_logger(const std::string& name, spdlog::level::level_enum log_level = spdlog::level::debug);

void end_logging();




