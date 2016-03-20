
#include "connection.hpp"
#include <utility>
#include <vector>
#include "ConnectionManager.hpp"
#include "RequestHandler.hpp"

namespace etu {
namespace net {
static const int DEFAULT_BUFFER_SIZE = 1024;
void resizeBuffer(base::BufferPtr& ptr) {
    if (!ptr) {
        ptr = base::Buffer::make(DEFAULT_BUFFER_SIZE);
    } else {
        ptr->resize(ptr->size() * 2);
    }
}
Сonnection::Сonnection(Socket socket,
                       ConnectionManager& manager, RequestHandler& handler)
    : m_socket(std::move(socket)),
    m_connectionManager(manager),
    m_requestHandler(handler) {}

void Сonnection::start() {
    doRead();
}

void Сonnection::stop() {
    m_socket.close();
}

void Сonnection::doRead() {
    auto self(shared_from_this());
    m_socket.async_read_some(boost::asio::buffer(m_bufferIn->data(), m_bufferIn->size()),
                             [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
        if (!ec) {
            m_bufferIn->addSize(bytes_transferred);
            auto result = m_requestHandler.handle(m_bufferIn, m_bufferOut);

            if (result == decltype(result)::NEED_WRITE) {
                doWrite();
            } else if(result == decltype(result)::NEED_READ_MORE) {
                doRead();
            }
        } else if (ec != boost::asio::error::operation_aborted) {
            m_connectionManager.stop(shared_from_this());
        }
    });
}

void Сonnection::doWrite() {
    auto self(shared_from_this());
    boost::asio::async_write(m_socket, boost::asio::buffer(m_bufferOut->data(), m_bufferOut->size()),
                             [this, self](boost::system::error_code ec, std::size_t) {
        if (!ec) {
            // Initiate graceful Сonnection closure.
            boost::system::error_code ignored_ec;
            m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                              ignored_ec);
        }

        if (ec != boost::asio::error::operation_aborted) {
            m_connectionManager.stop(shared_from_this());
        }
    });
}

}
}
