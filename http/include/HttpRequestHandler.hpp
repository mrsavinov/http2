#pragma once

#include "Config.hpp"
#include "RequestHandler.hpp"

#include "HttpRequestParser.hpp"
#include "HttpRequest.hpp"

namespace etu {
namespace http {

	struct Request;

	class RequestHandler : public net::RequestHandler
	{
	public:
		RequestHandler(const RequestHandler&) = delete;
		RequestHandler& operator=(const RequestHandler&) = delete;

        explicit RequestHandler(base::Config config);

        virtual HandleResult handle(base::BufferPtr bufferIn, base::BufferPtr bufferOut) override;
	private:
	private:
        RequestParser m_parser;
        Request m_request;
	};

} // namespace http
}

