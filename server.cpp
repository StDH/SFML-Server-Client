#include "server.hpp"

#include <iostream>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpListener.hpp>

#include "enums.hpp"

void Server::run(const sf::Uint16 _port, const sf::Uint8 _max)
{
	sf::TcpListener m_listener;

	if (m_listener.listen(_port) != sf::Socket::Done)
		throw "Server::run() != sf::Socket::Done";

	m_selector.add(m_listener);

	std::cout
	<< "# IP Lan: " << sf::IpAddress::getLocalAddress() << std::endl
	<< "# IP Pub: " << sf::IpAddress::getPublicAddress(sf::seconds(1.f)) << std::endl
	<< "# Port: " << m_listener.getLocalPort() << std::endl <<
	std::endl;

	while (true)
	{
		if (m_selector.wait(sf::seconds(1.f)))
		{
			if (m_selector.isReady(m_listener))
			{
				auto client = std::make_shared<Client>();

				if (m_listener.accept(*client -> m_socket) == sf::Socket::Done) // other cases are handled by sfml
				{
					if (m_clients.size() < _max)
					{
						m_clients.push_back(client);
						m_selector.add(*client -> m_socket);

						std::cout << client -> m_socket -> getRemoteAddress() << ":" << client -> m_socket -> getRemotePort() << " connected." << std::endl;
					}
					else
					{
						std::cout << "Disconneting client: " << client -> m_socket -> getRemoteAddress() << ":" << client -> m_socket -> getRemotePort() << std::endl;
						client -> m_socket -> disconnect();
					}
				}
			}
			else
			{
				for (auto &ptr : m_clients)
				{
					if (m_selector.isReady(*ptr -> m_socket))
					{
						sf::Packet packet;

						sf::TcpSocket::Status socket_state = ptr -> m_socket -> receive(packet);

						switch (socket_state)
						{
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
								std::cout << ptr -> m_socket -> getRemoteAddress() << ":" << ptr -> m_socket -> getRemotePort() << " disconnected." << std::endl;
			
								m_selector.remove(*ptr -> m_socket);
								m_clients.remove(ptr);

								break;
							}
						}
					}
				}
			}
		}
	}
}