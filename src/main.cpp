#include "interfaces.hpp"

#include <boost/asio/signal_set.hpp>
#include <boost/container/flat_map.hpp>
#include <nlohmann/json.hpp>

#include <iostream>
#include <string>

using json = nlohmann::json;

int main(void)
{
    boost::asio::io_context ioc;
    boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);

    signals.async_wait(
        [&ioc](const boost::system::error_code ec, const int& sig) {
        if (ec)
        {
            throw std::runtime_error("Signal should not be canceled");
        }
        ioc.stop();
    });

    auto bus = std::make_shared<sdbusplus::asio::connection>(ioc);
    auto objServer = std::make_unique<sdbusplus::asio::object_server>(bus);
    bus->request_name(demoAppName);

    Application app(ioc, *bus, *objServer);

    std::cout << "Hello, Lukasz!! :)" << std::endl;

    boost::container::flat_map<std::string, int> fmap{
        {"one", 1},
        {"two", 2},
        {"three", 3},
        {"four", 4},
    };

    fmap["five"] = 321;
    fmap["six"] = 654;

    std::cout << "List flatmap container elems:" << std::endl;
    for (const auto& elem : fmap)
    {
        std::cout << "Key: " << elem.first << ", value: " << elem.second
                  << std::endl;
    }

    json array = json::array({{"currency", "USD"}, {"value", 42.99}});
    std::string s = array.dump();

    std::cout << "serialization: " << s << std::endl;
    std::cout << "vector size 1: " << getVectorSize(5) << std::endl;
    // std::cout << "vector size 2: " << getVectorSize(0) << std::endl;
    std::cout << "vector size 3: " << getVectorSize(8) << std::endl;

    ioc.run();

    return 0;
}
