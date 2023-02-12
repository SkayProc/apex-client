#include "io.hpp"

void io::initiate( const char* const console_title )
{
	if constexpr ( settings::debug )
	{
		FILE* stream;

		AllocConsole( );

		freopen_s( &stream, "CONIN$", "r", stdin );
		freopen_s( &stream, "CONOUT$", "w", stdout );

		SetConsoleTitleA( console_title );
	}
}