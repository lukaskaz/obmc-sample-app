#include "interfaces.hpp"

#include <boost/container/flat_map.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/property.hpp>

#include <iostream>

constexpr const char* demoObjectPath = "/xyz/demoApp";
constexpr const char* demoInterfaceName = "xyz.demoApp";
constexpr const char* propertyStatus = "Status";
constexpr const char* propertyMode = "Mode";

boost::container::flat_map<std::string, ModeList> ModeMap = {
    {"Starting", ModeList::starting},
    {"Stopping", ModeList::stopping},
    {"Working", ModeList::working},
    {"Stopped", ModeList::stopped}};

std::string getModeString(ModeList modeToFind)
{
    for (const auto& [name, mode] : ModeMap)
    {
        if (mode == modeToFind)
        {
            return name;
        }
    }
    return std::string();
}

std::optional<ModeList> getModeEnum(const std::string& nameToFind)
{
    if (ModeMap.contains(nameToFind))
    {
        return ModeMap[nameToFind];
    }
    return std::nullopt;
}

Application::Application(boost::asio::io_context& ioc,
                         sdbusplus::asio::connection& bus,
                         sdbusplus::asio::object_server& objServer) :
    ioc(ioc),
    bus(bus), objServer(objServer)
{
    demo = objServer.add_unique_interface(
        demoObjectPath, demoInterfaceName,
        [this](sdbusplus::asio::dbus_interface& demo) {
        demo.register_property_r<std::string>(
            propertyStatus, sdbusplus::vtable::property_::const_,
            [this](const auto&) { return status; });

        demo.register_property_rw<std::string>(
            propertyMode, sdbusplus::vtable::property_::emits_change,
            [this](const auto& newValue, const auto& oldValue) {
            if (newValue != oldValue)
            {
                if (auto tmp = getModeEnum(newValue); tmp.has_value())
                {
                    mode = *tmp;
                }
                else
                {
                    std::cout
                        << "New value not represent modes, current mode not changed"
                        << std::endl;
                }
            }
            else
            {
                std::cout << "New value same as current value, update skipped"
                          << std::endl;
            }
            return true;
            },
            [this](const auto&) { return getModeString(mode); });
        });

    // timer = std::make_shared<PeriodicTimer>(ioc, 2000ms, []() {
    //     std::cout << "REPEATING ACTION TRIGGERED PERIODIC TIMER 1"
    //               << std::endl;
    // });

    DetachedPeriodicTimer::create(
        ioc, 3000ms,
        []() { std::cout << "PERIODIC DETACHED TRIGGERED 1" << std::endl; },
        timerStop);

    DetachedOneShotTimer::create(ioc, 14000ms, [this]() {
        std::cout << "ONE SHOT DETACHED TRIGGERED 2" << std::endl;
        this->timerStop = true;
        // this->timer->stop();
    });
}

using vect = std::vector<std::string>;

size_t getVectorSize(int selection)
{
    std::unique_ptr<vect> ptr = nullptr;

    if (selection)
    {
        ptr = std::make_unique<vect>(vect{"hello", "you", "nerd"});
    }
    return ptr->size();
}

// std::optional<ModeList> getModeEnum(const std::string& nameToFind)
// {
//     if (ModeMap.contains(nameToFind))
//     {
//         return ModeMap[nameToFind];
//     }
//     return std::nullopt;
// }

// using vect = std::vector<std::string>;

// size_t getVectorSize(int selection)
// {
//     std::unique_ptr<vect> ptr = nullptr;

//     if (selection)
//     {
//         ptr = std::make_unique<vect>(vect{"hello", "you", "nerd"});
//     }
//     return ptr->size();
// }
