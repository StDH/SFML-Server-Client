#ifndef SERVER_HPP
#define SERVER_HPP

#include <list>			// std::list
#include <memory>		// std::unique_ptr, std::make_unique
#include <string>		// std::string
#include <type_traits>	// std::move

#include <SFML/Config.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

struct Client
{
	std::unique_ptr<sf::TcpSocket> m_socket;

	const sf::Uint64 m_uid;

	Client()
		: m_socket(std::move(std::make_unique<sf::TcpSocket>()))
		, m_uid((sf::Uint64)m_socket.get())
	{
	}
};

class Server
{
	private:
		sf::SocketSelector	m_selector;
		sf::TcpListener		m_listener;

		typedef std::list<std::unique_ptr<Client>> LIST_OF_CLIENTS;

		LIST_OF_CLIENTS m_clients;

		sf::Uint8 m_max_clients;

		bool m_running;

		LIST_OF_CLIENTS::iterator disconnect(LIST_OF_CLIENTS::iterator);

		void handle_connections();
		void handle_packets();

		void run();

	public:
		Server();

		void initialize(const sf::Uint16, const sf::Uint8); // port(65535), max-clients(255)
		void deinitialize();
};

#endif