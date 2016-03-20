#pragma once

#include <string>

namespace etu {
namespace http {

struct Header {
    Header() = default;
    Header(std::string name, std::string value) : name(name), value(value) {}
    std::string name;
    std::string value;
};

}
}

