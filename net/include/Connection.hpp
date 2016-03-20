#pragma once

#include <array>
#include <memory>
#include "Socket.hpp"
#include "Buffer.hpp"

namespace etu {
namespace net {

	class ConnectionManager;

	/// Represents a single Сonnection from a client.
	class Сonnection
		: public std::enable_shared_from_this<Сonnection>
	{
	public:
		enum class Status {
			S_INIT,
			S_SEND_SWITCHING_PROTOCOL,
			S_HTTP2,
			S_NO_NEED_SWITCH
		};
	public:
		Сonnection(const Сonnection&) = delete;
		Сonnection& operator=(const Сonnection&) = delete;

		/// Construct a Сonnection with the given socket.
		explicit Сonnection(Socket socket,
			ConnectionManager& manager, RequestHandler& handler);

		void start();
		void stop();

	private:
		void doRead();
		void doWrite();
	private:
		Socket m_socket;
		ConnectionManager& m_connectionManager;
		RequestHandler& m_requestHandler;

		base::BufferPtr m_bufferIn;
		base::BufferPtr m_bufferOut;

	};

	typedef std::shared_ptr<Сonnection> ConnectionPtr;

}
}

