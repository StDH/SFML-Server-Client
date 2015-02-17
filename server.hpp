#ifndef SERVER_HPP
#define SERVER_HPP

#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

#include <SFML/Config.hpp>
#include <SFML/Network/TcpSocket.hpp>

struct Client
{
	std::unique_ptr<sf::TcpSocket> m_socket;
	const sf::Uint32 m_uid;
	std::string m_name;

	Client()
		: m_socket(std::make_unique<sf::TcpSocket>())
		, m_uid((sf::Uint32)m_socket.get())
	{
	}
};

class Server
{
	private:
		typedef std::shared_ptr<Client> pClient;

		std::vector<pClient> m_clients;

		void handlePacket(pClient);

	public:
		void run(const sf::Uint16, const sf::Uint8); // port(65535), max-clients(255)
};

#endif