#include "server.hpp"

#include <iostream>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>

#include "enums.hpp"

Server::LIST_OF_CLIENTS::iterator Server::disconnect(Server::LIST_OF_CLIENTS::iterator _iter)
{
	Client& ref = **_iter;

	std::cout << ref.m_socket -> getRemoteAddress() << ":" << ref.m_socket -> getRemotePort() << " disconnected." << std::endl;
			
	m_selector.remove(*ref.m_socket);

	return m_clients.erase(_iter);
}

void Server::handle_connections()
{
	auto client = std::move(std::make_unique<Client>());

	if (m_listener.accept(*client -> m_socket) == sf::Socket::Done) // other cases are handled by sfml
	{
		if (m_clients.size() < m_max_clients)
		{
			std::cout << client -> m_socket -> getRemoteAddress() << ":" << client -> m_socket -> getRemotePort() << " connected." << std::endl;

			m_selector.add(*client -> m_socket);
			m_clients.push_back(std::move(client));

			/*
			 * Dear Me,
			 * If you will again add something after std::move, then please jump out of window (24 level = good flight).
			 * std::move + std::unique_ptr -> get it in your fucking head ! (sexting)
			 * https://www.youtube.com/watch?v=GAKgOkkDxys
			 */
		}
		else
		{
			std::cout << "Disconneting client: " << client -> m_socket -> getRemoteAddress() << ":" << client -> m_socket -> getRemotePort() << std::endl;
			client -> m_socket -> disconnect();
		}
	}
}

void Server::handle_packets()
{
	auto iter = m_clients.begin();

	while (iter != m_clients.end())
	{
		Client &ref = **iter;

		if (m_selector.isReady(*ref.m_socket))
		{
			sf::Packet packet;

			switch (ref.m_socket -> receive(packet))
			{
				if (ref.m_socket -> getRemoteAddress() == sf::IpAddress::None)
				{
					iter = disconnect(iter);
					return;
				}

				case sf::TcpSocket::Done:
				{
					sf::Uint8 flag = enums::Packet::INVALID;

					packet >> flag;

					/*switch (flag)
					{

					}*/

					break;
				}

				case sf::TcpSocket::Disconnected:
				{
					iter = disconnect(iter);

					break;
				}
			}
		}
	}
}

void Server::run()
{
	while (m_running)
	{
		if (m_selector.wait(sf::seconds(1.f)))
		{
			if (m_selector.isReady(m_listener))
				handle_connections();
			else
				handle_packets();
		}
	}
}

Server::Server()
{
	m_max_clients = 1u;
	m_running = false;
}

void Server::initialize(const sf::Uint16 _port, const sf::Uint8 _max)
{
	m_max_clients = _max;

	m_listener.setBlocking(false);

	if (m_listener.listen(_port) != sf::Socket::Done)
		throw "Server::run() != sf::Socket::Done";

	m_selector.add(m_listener);

	std::cout
	<< "# IP Lan: " << sf::IpAddress::getLocalAddress() << std::endl
	<< "# IP Pub: " << sf::IpAddress::getPublicAddress(sf::seconds(1.f)) << std::endl
	<< "# Port: " << m_listener.getLocalPort() << std::endl <<
	std::endl;

	m_running = true;

	run();
}

void Server::deinitialize()
{
	printf("Disconnecting all clients...\n");

	m_running = false;

	for (auto &ptr : m_clients)
		ptr -> m_socket -> disconnect();
}