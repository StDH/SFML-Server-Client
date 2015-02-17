#include "server.hpp"

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>

#include "enums.hpp"

void Server::handlePacket(pClient _client)
{
	sf::Packet packet;

	if (_client -> m_socket -> receive(packet) == sf::Socket::Done)
	{
		sf::Uint8 flag = enums::Packet::INVALID;

		packet >> flag;

		/*switch (flag)
		{

		}*/
	}
}

void Server::run(const sf::Uint16 _port, const sf::Uint8 _max)
{
	m_clients.reserve(_max);

	sf::TcpListener		m_listener;
	sf::SocketSelector	m_selector;

	if (m_listener.listen(_port) != sf::Socket::Done)
		throw "Server::run() != sf::Socket::Done";

	m_selector.add(m_listener);

	printf("# IP %s - %s\n# Port: %u\n# Max clients: %u\n", sf::IpAddress::getLocalAddress().toString().c_str(), sf::IpAddress::getPublicAddress(sf::seconds(1.f)).toString().c_str(), m_listener.getLocalPort(), _max);

	while (true)
	{
		if (m_selector.wait(sf::seconds(1.f)))
		{
			if (m_selector.isReady(m_listener))
			{
				pClient client = std::make_shared<Client>();

				if (m_listener.accept(*client -> m_socket) == sf::Socket::Done) // other cases are handled by sfml
				{
					if (m_clients.size() < _max)
					{
						m_clients.push_back(client);
						m_selector.add(*client -> m_socket);
					
						//handlePacket(client);

						printf("New client connected: %s:%u\n", client -> m_socket -> getRemoteAddress().toString().c_str(), client -> m_socket -> getRemotePort());
					}
					else
					{
						printf("Disconneting client: %s:%u\n", client -> m_socket -> getRemoteAddress().toString().c_str(), client -> m_socket -> getRemotePort());
						client -> m_socket -> disconnect();
					}
				}
			}
			else
			{
				for (auto &ptr : m_clients)
					if (m_selector.isReady(*ptr -> m_socket))
						handlePacket(ptr);
			}
		}
	}
}