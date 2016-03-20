#include "Socket.h"

namespace etu {
namespace net {
		class Socket {
		public:
			Socket();
			Socket(const Socket& s) = delete;
			Socket(Socket&& s) = delete;
			Socket& operator = (const Socket& s) = delete;
			Socket& operator = (Socket&& s) = delete;
		};
}
}