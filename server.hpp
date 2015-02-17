#ifndef SERVER_HPP
#define SERVER_HPP

#include <list>		// std::list
#include <memory>	// std::unique_ptr, std::make_unique, std::shared_ptr, std::make_shared
#include <string>	// std::string

#include <SFML/Config.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpSocket.hpp>

struct Client
{
		std::unique_ptr<sf::TcpSocket> m_socket;

		sf::Uint64 m_uid; // why should i use counter, when memory is the best "random generator" =D

		Client()
			: m_socket(std::make_unique<sf::TcpSocket>())
			, m_uid((sf::Uint64)m_socket.get())
		{
		}
};

class Server
{
	private:
		sf::SocketSelector m_selector;

		std::list<std::shared_ptr<Client>> m_clients;

	public:
		void run(const sf::Uint16, const sf::Uint8); // port(65535), max-clients(255)
};

#endif