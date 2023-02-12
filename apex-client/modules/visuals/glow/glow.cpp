#define NOMINMAX

#include "glow.hpp"

#include "../../../engine/engine.hpp"

#include "../../../engine/structures/structures.hpp"

#include "../../../renderer/menu/config/config.hpp"

void modules::visuals::glow::callback(  )
{
	//std::unique_lock locker( engine::values::lock );

	//for ( const auto player : engine::values::players )
	//{
	//	if( !player.address )
	//		continue;

	//	if ( driver.read< bool >( player.address + engine::offsets::base_entity::glow_context ) && std::get< bool >( menu::config::config_map[ "enable_glow" ] ) )
	//		continue;

	//	auto local_player = driver.read< engine::structures::player_t >( engine::values::game_base + engine::offsets::core::local_player );

	//	if ( !local_player.address )
	//		continue;

	//	auto [ current_health, max_health ] = player.get_health( );

 //       if( current_health <= 0 )
	//		continue;

	//	if ( std::get< bool >( menu::config::config_map[ "glow_ignore_teammates" ] ) && local_player.get_team( ) == player.get_team( ) )
	//	{
	//		driver.write< bool >( player.address + engine::offsets::base_entity::glow_context, false );

	//		continue;
	//	}

	//	driver.write< bool >( player.address + engine::offsets::base_entity::glow_context, std::get< bool >( menu::config::config_map[ "enable_glow" ] ) ? 7 : 2 );

	//	driver.write< int >( player.address + engine::offsets::base_entity::glow_visibility_type, 2 );

	//	const auto color = engine::structures::vector3_t{ std::get< engine::structures::vector4_t >( menu::config::config_map[ "glow_color" ] ).color[ 0 ], std::get< engine::structures::vector4_t >( menu::config::config_map[ "glow_color" ] ).color[ 1 ], std::get< engine::structures::vector4_t >( menu::config::config_map[ "glow_color" ] ).color[ 2 ] };

	//	driver.write< engine::structures::vector3_t >( player.address + engine::offsets::core::glow_color, color );

	//	if ( std::get< bool >( menu::config::config_map[ "enable_glow_teamcheck" ] ) && local_player.get_team( ) == player.get_team( ) )
	//		driver.write< engine::structures::vector3_t >( player.address + engine::offsets::core::glow_color, { 0, 0 , 255 } );

	//	for ( auto i = engine::offsets::base_entity::glow_start; i < engine::offsets::base_entity::glow_end; i += sizeof( float ) )
	//		driver.write<float>( player.address + i, 5000.f );
	//}
}

constexpr auto loba_flag = 0x51408A89;

void modules::visuals::glow::callback_items( )
{
	/*std::unique_lock locker( engine::values::lock_items );

	for ( const auto item : engine::values::items )
	{
		if ( !item.address )
			continue;
		
		driver.write( item.address + engine::offsets::base_entity::m_HighlightFunctionBits, std::get< bool >( menu::config::config_map[ "enable_glow_items" ] ) ? loba_flag : 0 );

		if ( !std::get< bool >( menu::config::config_map[ "enable_glow_items" ] ) )
			continue;

		driver.write<float>( item.address + engine::offsets::base_entity::distance, std::numeric_limits<float>::max( ) );
	}*/
}
