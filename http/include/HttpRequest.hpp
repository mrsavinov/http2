#pragma once
#include <string>
#include <vector>
#include "HttpHeader.hpp"

namespace etu {
namespace http {

/// A request received from a client.
struct Request {
    std::string method;
    std::string uri;
    int http_version_major;
    int http_version_minor;
    std::vector<Header> headers;
};

}
}


