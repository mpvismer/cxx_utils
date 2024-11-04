#include "logger.hpp"
#include "utils.hpp"
#include "udpserver.hpp"



int main(int argc, char** argv)
{
    LOG_INFO("Starting application...");
    sleep(10);
    LOG_TRACE("Test 10ms log");
    sleep(100);
    LOG_TRACE("Test 100ms log");


	udpserver server("127.0.0.1", 51000);
	udpserver client;

    auto server_thread_entry = [&server]()
    {
        SetThreadPriority(GetCurrentThread(), 2);
        server.run();
    };
    auto client_thread_entry = [&client]()
    {
        SetThreadPriority(GetCurrentThread(), 1);
        client.connect("127.0.0.1", 51000);
        for (uint32_t count=0; count < 100; count++)
        {
            sleep(100);
            LOG_INFO("Client sending...");
            client.send("hello", 5);
        }
        LOG_INFO("Client thread done.");
    };
    std::thread server_thread(server_thread_entry);
    std::thread client_thread(client_thread_entry);
    client_thread.join();
    LOG_INFO("Stopping server...");
    server.stop_running();
    server_thread.join();
    LOG_INFO("All done!");

    return 0;
}
