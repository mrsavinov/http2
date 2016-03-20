
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "HttpRequestHandler.hpp"
#include "HttpRequestParser.hpp"

namespace etu {
namespace http {

    RequestHandler::RequestHandler(base::Config config) {}

    HandleResult RequestHandler::handle(base::BufferPtr bufferIn, base::BufferPtr bufferOut) {
        m_parser.parse(m_request, bufferIn->data(), bufferIn->data() + bufferIn->size());
        
    }

    bool url_decode(const std::string& in, std::string& out) {
        out.clear();
        out.reserve(in.size());
        for (std::size_t i = 0; i < in.size(); ++i) {
            if (in[i] == '%') {
                if (i + 3 <= in.size()) {
                    int value = 0;
                    std::istringstream is(in.substr(i + 1, 2));
                    if (is >> std::hex >> value) {
                        out += static_cast<char> (value);
                        i += 2;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            } else if (in[i] == '+') {
                out += ' ';
            } else {
                out += in[i];
            }
        }
        return true;
    }
}
}
