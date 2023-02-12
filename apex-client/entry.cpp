#include <thread>

#include "engine/engine.hpp"
#include "io/io.hpp"

#include "modules/visuals/glow/glow.hpp"

#include "renderer/menu/menu.hpp"
#include "renderer/renderer.hpp"

#include "modules/aimbot/aimbot.hpp"
#include "integrity/integrity.hpp"

int main( )
{
	if ( integrity::check( ) )
	{
		io::initiate( "apex" );

		while ( !FindWindowA( nullptr, xs( "Apex Legends" ) ) )
			std::this_thread::sleep_for( std::chrono::seconds( 15 ) );

		driver.initiate( xs( "Apex Legends" ) );

		std::thread( []( )
			{
				while ( true )
				{
					engine::functions::initialize( );

					std::this_thread::sleep_for( std::chrono::seconds( 5 ) );
				}
			} ).detach( );

		std::thread{

			[]( )
			{
				while ( true )
				{
					if (!driver.read(engine::values::game_base + engine::offsets::core::level_name))
						 continue;

					if( driver.read_string(engine::values::game_base + engine::offsets::core::level_name, false) != xs("mp_lobby"))
						engine::functions::populate_vectors( );
					else
					{
						std::unique_lock locker( engine::values::lock );

						engine::values::players.clear( );

						locker.unlock();

					}

					std::this_thread::sleep_for( std::chrono::seconds( 4 ) );
				}
			}

		}.detach( );

		//std::thread{

		//	[]( )
		//	{
		//		while ( true )
		//		{
		//			if ( engine::functions::verify_in_game( ) )
		//			{
		//				modules::visuals::glow::callback( );
		//			}
		//			std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
		//		}
		//	}

		//}.detach( );

		//std::thread{

		//	[]( )
		//	{
		//		while ( true )
		//		{
		//			if ( engine::functions::verify_in_game( ) )				
		//				modules::visuals::glow::callback_items( );		

		//			std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
		//		}
		//	}

		//}.detach( );

		std::thread{

			[]( )
			{
				while ( true )
				{
					if (!driver.read(engine::values::game_base + engine::offsets::core::level_name))
						continue;

					if (driver.read_string(engine::values::game_base + engine::offsets::core::level_name, false) != xs("mp_lobby"))
						modules::aimbot::callback( );
					
					std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
				}
			}

		}.detach( );

		std::thread([]()
			{ renderer::start(xs("Apex Legends")); }).detach();
	}
	else
		exit( NULL );

	return 0;
}

bool DllMain( void*, const std::uint32_t reason )
{
	if ( reason == DLL_PROCESS_ATTACH )
	{
		std::thread{ main }.detach( );
	}

	return true;
}