#include <iostream>
#include <memory>
#include <string>

#include "boost/container/flat_map.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using vect = std::vector<std::string>;

size_t getVectorSize(int selection) {
    std::unique_ptr<vect> ptr = nullptr;

    if (selection) {
        ptr = std::make_unique<vect>(vect{"hello", "you", "nerd"});
    }
    return ptr->size();
}

int main(void) {
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
    for (const auto& elem : fmap) {
        std::cout << "Key: " << elem.first << ", value: " << elem.second
                  << std::endl;
    }

    json array = json::array({{"currency", "USD"}, {"value", 42.99}});
    std::string s = array.dump();

    std::cout << "serialization: " << s << std::endl;
    std::cout << "vector size 1: " << getVectorSize(5) << std::endl;
    std::cout << "vector size 2: " << getVectorSize(0) << std::endl;
    std::cout << "vector size 3: " << getVectorSize(8) << std::endl;

    return 0;
}

