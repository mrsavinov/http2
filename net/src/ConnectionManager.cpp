
#include "ConnectionManager.hpp"

namespace etu {
namespace net {

ConnectionManager::ConnectionManager()
{
}

void ConnectionManager::start(ConnectionPtr c)
{
  m_connections.insert(c);
  c->start();
}

void ConnectionManager::stop(ConnectionPtr c)
{
  m_connections.erase(c);
  c->stop();
}

void ConnectionManager::stopAll()
{
  for (auto c: m_connections)
    c->stop();
  m_connections.clear();
}

}
}
