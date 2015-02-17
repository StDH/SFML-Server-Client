#include <stdio.h>
#include <string>

#include "server.hpp"

#include <SFML/Network/IpAddress.hpp>
#include <SFML/System/Time.hpp>

void main(int argc, const char* argv[])
{
	printf("%s\n", sf::IpAddress::getPublicAddress(sf::seconds(1.f)).toString().c_str());

	try
	{
		if (argc == 3)
		{
			Server server;
			server.run(static_cast<sf::Uint16>(std::atoi(argv[1])), static_cast<sf::Uint8>(std::atoi(argv[2])));
		}
		else
		{
			printf("ex: network.exe <port> <max-clients>\n");
			throw "";
		}
	}
	catch (std::string& _string)
	{
		printf("EXCEPTION: %s\n", _string.c_str());
	}
}