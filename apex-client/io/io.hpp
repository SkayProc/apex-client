#pragma once

#include <Windows.h>
#include <cstdio>
#include "../settings/settings.hpp"

namespace io
{
	void initiate( const char* const console_title );

	//void log( const char* const message, const auto&... arguments )
	//{
	//	if constexpr ( settings::debug )
	//		std::printf( message, arguments... );
	//}
}