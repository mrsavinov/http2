#pragma once
#include <set>
#include "connection.hpp"

namespace etu {
namespace net {

	class ConnectionManager
	{
	public:
		ConnectionManager(const ConnectionManager&) = delete;
		ConnectionManager& operator=(const ConnectionManager&) = delete;

		ConnectionManager();

		void start(ConnectionPtr c);

		void stop(ConnectionPtr c);

		void stopAll();

	private:
		std::set<ConnectionPtr> m_connections;
	};

}
} // namespace etu

