#include "action.hpp"

#include <boost/asio/io_context.hpp>
#include <sdbusplus/asio/object_server.hpp>

#include <optional>
#include <string>

constexpr const char* demoAppName = "xyz.openbmc_project.demoApp";

enum class ModeList
{
    starting,
    stopping,
    working,
    stopped
};

std::string getModeString(ModeList modeToFind);
std::optional<ModeList> getModeEnum(const std::string& nameToFind);

class Application
{
  public:
    Application() = delete;
    Application(boost::asio::io_context& ioc, sdbusplus::asio::connection& bus,
                sdbusplus::asio::object_server& objServer);

  private:
    boost::asio::io_context& ioc;
    sdbusplus::asio::connection& bus;
    sdbusplus::asio::object_server& objServer;
    std::shared_ptr<PeriodicTimer> timer;
    bool timerStop = false;

    std::unique_ptr<sdbusplus::asio::dbus_interface> demo;
    std::string status = "Ready";
    ModeList mode = ModeList::stopped;
};

size_t getVectorSize(int selection);
