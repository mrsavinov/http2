#pragma once

#include "NetTypes.hpp"
#include "Buffer.hpp"

namespace etu {
namespace net {

	class RequestHandler {
	public:
		RequestHandler();
		virtual ~RequestHandler() {}
		virtual HandleResult handle(base::BufferPtr bufferIn, base::BufferPtr bufferOut) = 0;
	};

} // namespace http
}

