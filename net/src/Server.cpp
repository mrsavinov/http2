#include "server.hpp"
#include <signal.h>
#include <utility>

namespace etu {
namespace net {

Server::Server(const std::string & address, const std::string & port, RequestHandler * handler)
  : m_ioService(),
    m_signals(m_ioService),
    m_acceptor(m_ioService),
    m_connectionManager(),
    m_socket(m_ioService),
    m_requestHandler(handler)
{
  // Register to handle the signals that indicate when the server should exit.
  // It is safe to register for the same signal multiple times in a program,
  // provided all registration for the specified signal is made through Asio.
  m_signals.add(SIGINT);
  m_signals.add(SIGTERM);
#if defined(SIGQUIT)
  m_signals.add(SIGQUIT);
#endif // defined(SIGQUIT)

  doAwaitStop();

  // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
  boost::asio::ip::tcp::resolver resolver(m_ioService);
  boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, port});
  m_acceptor.open(endpoint.protocol());
  m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  m_acceptor.bind(endpoint);
  m_acceptor.listen();

  doAccept();
}

void Server::run()
{
  // The io_service::run() call will block until all asynchronous operations
  // have finished. While the server is running, there is always at least one
  // asynchronous operation outstanding: the asynchronous accept call waiting
  // for new incoming connections.
  m_ioService.run();
}

void Server::doAccept()
{
  m_acceptor.async_accept(m_socket,
      [this](boost::system::error_code ec)
      {
        // Check whether the server was stopped by a signal before this
        // completion handler had a chance to run.
        if (!m_acceptor.is_open())
        {
          return;
        }

        if (!ec)
        {
          m_connectionManager.start(std::make_shared<Сonnection>(
              std::move(m_socket), m_connectionManager, *m_requestHandler));
        }

        doAccept();
      });
}

void Server::doAwaitStop()
{
  m_signals.async_wait(
      [this](boost::system::error_code /*ec*/, int /*signo*/)
      {
        // The server is stopped by cancelling all outstanding asynchronous
        // operations. Once all operations have finished the io_service::run()
        // call will exit.
        m_acceptor.close();
        m_connectionManager.stopAll();
      });
}

}
}
