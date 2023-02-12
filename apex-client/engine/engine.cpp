#include "engine.hpp"

#include <cmath>

std::uintptr_t engine::values::game_base = 0;
std::mutex engine::values::lock;
std::vector< engine::structures::player_t > engine::values::players;

std::mutex engine::values::lock_items;
std::vector< engine::structures::item_t > engine::values::items;


void engine::functions::initialize( )
{
	engine::values::game_base = driver.base( );
}

bool engine::functions::verify_in_game( )
{
	if (!driver.read(engine::values::game_base + engine::offsets::core::level_name))
		return false;

	return driver.read_string( engine::values::game_base + engine::offsets::core::level_name, false ) != xs( "mp_lobby");
}

std::vector< engine::structures::player_t > engine::functions::get_players( )
{
	std::vector< engine::structures::player_t > players;

	for ( auto i = 0u; i < 100; ++i )
	{
		const auto player = driver.read< engine::structures::player_t > ( engine::values::game_base + engine::offsets::core::cl_entitylist + 32 * i );

		if ( !player.address )
			continue;

		auto local_player = driver.read( engine::values::game_base + engine::offsets::core::local_player );

		if ( !local_player )
			continue;

		if ( player.is( xs( "player" ) ) )
		{
			if ( player.address != local_player && player.address )
				players.push_back( player );
		}
	}

	return players;
}

std::vector<engine::structures::item_t> engine::functions::get_items( )
{
	std::vector< engine::structures::item_t > items;

	for ( auto i = 0u; i < 10000; ++i )
	{
		const auto item = driver.read< engine::structures::item_t >( engine::values::game_base + engine::offsets::core::cl_entitylist + 32 * i );

		if ( !item.address )
			continue;

		if ( item.is( xs( "prop_survival" ) ) )
		{
			if ( item.address )
				items.push_back( item );
		}
	}

	return items;
}

void engine::functions::populate_vectors( )
{
	auto plrs = get_players();

	std::unique_lock locker( engine::values::lock );

	engine::values::players = plrs;

	locker.unlock();
	//std::unique_lock locker2( engine::values::lock_items ); engine::values::items = get_items( );
}

bool engine::structures::base_entity_t::is( const char* type ) const
{
	if ( !address || !driver.read( address + engine::offsets::base_entity::signifier_name ) )
		return false;

	return !strcmp( driver.read_string( address + engine::offsets::base_entity::signifier_name ).c_str( ), type );
}

#define OFFSET_BONES				0x0e88 + 0x48 //m_nForceBone + 0x48
#define OFFSET_STUDIOHDR            0x10d8

engine::structures::vector3_t engine::structures::player_t::get_bone_pos( const engine::structures::bones_t bone_id ) const
{
	if (!address)
		return { 0, 0, 0 };

	vector3_t origin = driver.read< engine::structures::vector3_t>(address + 0x14C);;

	//BoneByHitBox
	uint64_t Model = driver.read<uint64_t>(address + OFFSET_STUDIOHDR);

	//get studio hdr
	uint64_t StudioHdr = driver.read<uint64_t>(Model + 0x8);

	//get hitbox array
	int HitBoxsArray_set = driver.read<int>(StudioHdr + 0xB4);
	uint64_t HitBoxsArray = StudioHdr + HitBoxsArray_set;

	int HitboxIndex = driver.read<int>(HitBoxsArray + 0x8);

	int Bone = driver.read<int>(HitBoxsArray + HitboxIndex + (static_cast<std::uint16_t>(bone_id) * 0x2C));

	if (Bone < 0 || Bone > 255)
		return engine::structures::vector3_t();

	//hitpos
	uint64_t BoneArray = driver.read<uint64_t>(address + OFFSET_BONES);

	matrix3x4_t Matrix = driver.read<matrix3x4_t>(BoneArray + Bone * sizeof(matrix3x4_t));

	return vector3_t(Matrix.matrix3x4[0][3] + origin.x, Matrix.matrix3x4[1][3] + origin.y, Matrix.matrix3x4[2][3] + origin.z);

	//const auto bones = driver.read< std::uintptr_t >( address + engine::offsets::base_entity::m_bConstrainBetweenEndpoints );

	//if(!bones)
	//	return { 0, 0, 0 };

	//const auto bone = bones + ( static_cast< std::uint16_t >( bone_id ) * static_cast< std::uint16_t >( 0x30 ) );

	//if ( !bone )
	//	return { 0, 0, 0 };

	//const auto origin = driver.read< engine::structures::vector3_t>( address + 0x14C );

	//if ( !origin.x || !origin.y || !origin.z )
	//	return { 0, 0, 0 };

	//engine::structures::vector3_t bone_offsets;

	//bone_offsets.x = driver.read< float >( bone + 0xCC);
	//bone_offsets.y = driver.read< float >( bone + 0xDC);
	//bone_offsets.z = driver.read< float >( bone + 0xEC);

	//return bone_offsets + origin;
}

std::pair< int, int > engine::structures::player_t::get_health() const
{
	return std::make_pair< int, int > ( driver.read<int>( address + engine::offsets::base_entity::m_iHealth ), driver.read<int>(address + engine::offsets::base_entity::m_iMaxHealth) );
}

std::int32_t engine::structures::player_t::get_team( ) const
{
	return driver.read< std::int32_t >( address + engine::offsets::base_entity::m_iTeamNum );
}

float engine::structures::player_t::get_last_visible_time( ) const
{
	return driver.read< float >( address + engine::offsets::base_entity::last_visible_time );
}

bool engine::structures::player_t::is_skydiving( )
{
	return driver.read< std::uint32_t >( address + engine::offsets::base_entity::m_skydiveState ) != 0;
}

std::uintptr_t engine::structures::player_t::get_primary_weapon( )
{
	const auto primary_weapon_index = driver.read< std::uintptr_t >( address + engine::offsets::base_entity::m_latestPrimaryWeapons ) & 0xFFFF;

	if ( !primary_weapon_index )
		return 0;

	return driver.read( engine::values::game_base + engine::offsets::core::cl_entitylist + 32 * primary_weapon_index );
}

std::optional < engine::structures::vector2_t > engine::functions::world_to_screen( const engine::structures::vector3_t& position )
{
	const auto width = GetSystemMetrics( SM_CXSCREEN ), height = GetSystemMetrics( SM_CYSCREEN );

	if (const auto view_render = driver.read(engine::values::game_base + engine::offsets::core::view_render))
	{
		if (const auto matrix_ptr = driver.read(view_render + engine::offsets::core::view_matrix))
		{

			const auto view_matrix = driver.read< engine::structures::view_matrix_t >(matrix_ptr);

			auto matrix = view_matrix.m;

			engine::structures::vector2_t out;

			out.x = matrix[0][0] * position.x + matrix[0][1] * position.y + matrix[0][2] * position.z + matrix[0][3];
			out.y = matrix[1][0] * position.x + matrix[1][1] * position.y + matrix[1][2] * position.z + matrix[1][3];

			const auto w = matrix[3][0] * position.x + matrix[3][1] * position.y + matrix[3][2] * position.z + matrix[3][3];

			if (w < 0.001f)
				return std::nullopt;

			out.x /= w;
			out.y /= w;

			out.x = (width / 2.0f) + (out.x * width) / 2.0f;
			out.y = (height / 2.0f) - (out.y * height) / 2.0f;


			return std::optional< engine::structures::vector2_t >(out);
		}
	}
}