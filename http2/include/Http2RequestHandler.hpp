#pragma once

#include "Config.hpp"
#include "RequestHandler.hpp"
#include "Http2Types.hpp"

namespace etu {
namespace http2 {

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
	};

} // namespace http
}

