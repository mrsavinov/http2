
#pragma once

#include <boost/asio.hpp>
#include <string>
#include "Connection.hpp"
#include "ConnectionManager.hpp"
#include "RequestHandler.hpp"

namespace etu {
namespace net {

/// The top-level class of the HTTP server.
class Server
{
public:
  Server(const Server&) = delete;
  Server& operator=(const Server&) = delete;

  /// Construct the server to listen on the specified TCP address and port, and
  /// serve up files from the given directory.
  explicit Server(const std::string& address, const std::string& port, RequestHandler* handler);

  /// Run the server's io_service loop.
  void run();

private:
  /// Perform an asynchronous accept operation.
  void doAccept();

  /// Wait for a request to stop the server.
  void doAwaitStop();

  /// The io_service used to perform asynchronous operations.
  boost::asio::io_service m_ioService;

  /// The signal_set is used to register for process termination notifications.
  boost::asio::signal_set m_signals;

  /// Acceptor used to listen for incoming connections.
  boost::asio::ip::tcp::acceptor m_acceptor;

  /// The Сonnection manager which owns all live connections.
  ConnectionManager m_connectionManager;

  /// The next socket to be accepted.
  boost::asio::ip::tcp::socket m_socket;

  /// The handler for all incoming requests.
  RequestHandler* m_requestHandler;
};

} 
}


