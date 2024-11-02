#pragma once
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <vector>

// See https://github.com/gabime/spdlog

#define LOG_TRACE(...) SPDLOG_LOGGER_TRACE(logger::instance().get_root_logger(), __VA_ARGS__);
#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(logger::instance().get_root_logger(), __VA_ARGS__);
#define LOG_INFO(...) SPDLOG_LOGGER_INFO(logger::instance().get_root_logger(), __VA_ARGS__);
#define LOG_WARN(...) SPDLOG_LOGGER_WARN(logger::instance().get_root_logger(), __VA_ARGS__);
#define LOG_WARNING(...) SPDLOG_LOGGER_WARN(logger::instance().get_root_logger(), __VA_ARGS__);
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(logger::instance().get_root_logger(), __VA_ARGS__);
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(logger::instance().get_root_logger(), __VA_ARGS__);


//See https://stackoverflow.com/questions/1008019/how-do-you-implement-the-singleton-design-pattern
// re the singleton pattern
 
class logger
{
public:
    const std::string default_log_pattern = "%H:%M:%S.%f [%L] %05t:%s:%-4# - %v";

    static logger& instance()
    {
        static logger instance("logging.log"); // Instantiated on first use.
        return instance;
    }

    // To stop cloning etc
    logger(logger const&) = delete;
    void operator=(logger const&) = delete;
    
    // Creates a brand new logger with it's own sink (which is also in the worker thread pool)
    // Handy for example to create a threaded csv logger.
    std::shared_ptr<spdlog::logger> new_logger(const std::string& filename)
    {
        auto logger = spdlog::basic_logger_mt<spdlog::async_factory>(filename, "logs/"+filename);
        logger->set_pattern("%H:%M:%S.%f, %v"); // an example, can be changed
        return logger;
    }
    
    // Gets a child logger of the default root logger    
    std::shared_ptr<spdlog::logger> get_logger(const std::string& name)
    {
        auto logger = spdlog::get(name);
        // If the logger does not exists, create it inheriting settings from default
        if (nullptr == logger)
        {
            auto sinks = _root_logger->sinks();
            logger = std::make_shared<spdlog::async_logger>(
                name, begin(sinks), end(sinks),
                spdlog::thread_pool(), spdlog::async_overflow_policy::overrun_oldest);
            logger->set_level(_root_logger->level());
            logger->flush_on(_root_logger->flush_level());
            spdlog::register_logger(logger);
        }
        return logger;
    }
    
    std::shared_ptr<spdlog::logger> get_root_logger()
    {
        return _root_logger;
    }
    
private:
    logger(const std::string& logfile = "logs/main.log") 
    {
        // Sets up multithreaded logging where logs can be called from 
        // any thread and all sinks run in a single worker thread.
		auto on_thread_start = []() {
			//WIN32: SetThreadPriority(GetCurrentThread(), -1);
            //POSIX: pthread_setschedparam(thread.native_handle(), policy, {priority});
		};
        
        // Create a threadpool with 1 thread and queue size of 8192
        spdlog::init_thread_pool(8192, 1, on_thread_start);  
        
        // Create the sinks
        std::shared_ptr<spdlog::sinks::sink> fileSink;
        fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            logfile, 1024*1024*10, 5);
        fileSink->set_level(spdlog::level::trace);
        fileSink->set_pattern(default_log_pattern);
        std::shared_ptr<spdlog::sinks::sink> stdoutSink;
        stdoutSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        stdoutSink->set_level(spdlog::level::info);
        stdoutSink->set_pattern(default_log_pattern);

        // Create a log
        std::vector<spdlog::sink_ptr> sinks = { fileSink, stdoutSink };
        _root_logger = std::make_shared<spdlog::async_logger>(
            "", begin(sinks), end(sinks), spdlog::thread_pool(), spdlog::async_overflow_policy::overrun_oldest);
        _root_logger->set_level(spdlog::level::trace);
        _root_logger->flush_on(spdlog::level::trace);
        // Cannot register an unnamed logger
        //spdlog::register_logger(_root_logger);  
    }
    
private:
	std::shared_ptr<spdlog::logger> _root_logger;
};







