#ifndef ENUMS_HPP
#define ENUMS_HPP

#include <SFML/Config.hpp>

namespace enums
{
	enum Packet : sf::Uint8
	{
		INVALID = 0u,
		UID_OF_CLIENT,
		MSG_TO_SERVER,
		MSG_TO_CLIENT,
		FTP,
		VOICE
	};
};

#endif