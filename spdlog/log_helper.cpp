#include "log_helper.hpp"

#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>


//const std::string default_log_pattern = "%H:%M:%S.%f [%L] %05t:%n:%04# - %v\n";
const std::string default_log_pattern = "%H:%M:%S.%f [%L] %05t:%s:%-4# - %v";

static std::shared_ptr<spdlog::sinks::sink> fileSink;
static std::shared_ptr<spdlog::sinks::sink> stdoutSink;
//static std::shared_ptr<spdlog::sinks::sink> stderrSink;

std::shared_ptr<spdlog::logger> root_logger;


/**
 * Implementation for multithreaded logging where loggers dispatch (post) logs to one thread which 
 * writes them to a log file and the console.
 */
void configure_logging(const std::string& logfile)
{
    //
    // Create the sinks
    //
    spdlog::init_thread_pool(8192, 1);  // Create a threadpool with 1 thread and queue size of 8192
    fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        logfile, 1024*1024*10, 5);
    fileSink->set_level(spdlog::level::trace);
    fileSink->set_pattern(default_log_pattern);
    stdoutSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    stdoutSink->set_level(spdlog::level::info);
    stdoutSink->set_pattern(default_log_pattern);

    //
    // Create a log
    //
    std::vector<spdlog::sink_ptr> sinks = { fileSink, stdoutSink };
    root_logger = std::make_shared<spdlog::async_logger>(
        "", begin(sinks), end(sinks), spdlog::thread_pool(), spdlog::async_overflow_policy::overrun_oldest);
	root_logger->set_level(spdlog::level::trace);
	root_logger->flush_on(spdlog::level::trace);
    spdlog::register_logger(root_logger);
}

std::shared_ptr<spdlog::logger> get_logger(const std::string& name, spdlog::level::level_enum log_level)
{
    auto logger = spdlog::get(name);
    // If the logger does not exists, create it inheriting settings from default
    if (nullptr == logger)
    {
		auto sinks = root_logger->sinks();
		logger = std::make_shared<spdlog::async_logger>(
			name, begin(sinks), end(sinks),
			spdlog::thread_pool(), spdlog::async_overflow_policy::overrun_oldest);
		logger->set_level(root_logger->level());
		logger->flush_on(root_logger->flush_level());
        spdlog::register_logger(logger);
    }
    return logger;
}

void end_logging()
{
    // To avoid dead lock VS runtime when using threadpools
    spdlog::shutdown();
}
