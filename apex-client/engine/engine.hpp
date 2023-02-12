#pragma once

#include <memory>
#include <vector>
#include "../driver/driver.hpp"
#include "structures/structures.hpp"

#include <mutex>

#include <optional>

constexpr auto M_PI = 3.14159265358979323846f;

namespace engine::offsets::core
{
	constexpr auto cl_entitylist = 0x1a75038;
	constexpr auto local_player = 0x1e25418;
	constexpr auto view_render = 0x7599758;
	constexpr auto view_matrix = 0x11a210;
	constexpr auto level_name = 0x13a17b8;
};

//0x7544150

namespace engine::offsets::base_entity
{
	constexpr const auto signifier_name = 0x580;
	constexpr const auto m_bConstrainBetweenEndpoints = 0x0f50;

	constexpr const auto m_iHealth = 0x438;
	constexpr const auto m_iMaxHealth = 0x578;

	constexpr const auto m_iTeamNum = 0x448;

	constexpr const auto view_angles = 0x24f4 - 0x14; // m_ammoPoolCapacity - 0x14

	constexpr const auto breath_angles = view_angles - 0x10;

	constexpr const auto aim_punch = 0x23f8;	    // m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle

	constexpr const auto camera_position = 0x1ea0; //CPlayer!camera_origin

	constexpr const auto last_visible_time = 0x1a44;

	//constexpr const auto glow_context = 0x3C8;

	//constexpr const auto glow_color = 0x1d0; //m_highlightParams + 24 (0x18)

	//constexpr const auto glow_start = 0x388; //m_highlightServerFadeEndTimes + 8

	//constexpr const auto glow_end = 0x3B4; //Script_Highlight_SetFarFadeDist / m_highlightServerFadeEndTimes + 52(0x34)

	//constexpr const auto glow_visibility_type = 0x3D0;//OK Script_Highlight_SetVisibilityType / m_highlightTeamBits + 12(C)

	//constexpr const auto m_HighlightFunctionBits = 0x2C0;

	//constexpr const auto distance = 0x3B4; //OK Script_Highlight_SetFarFadeDist / m_highlightServerFadeEndTimes + 52(0x34)

	constexpr const auto m_skydiveState = 0x45cc;

	constexpr const auto m_latestPrimaryWeapons = 0x19ec;

	constexpr const auto velocity = 0x140; // m_vecAbsVelocity

	constexpr const auto weapon_ammo = 0x1624;						// m_ammoInClip

	constexpr const auto shield = 0x170;
	constexpr const auto max_shield = 0x174;
}

namespace engine::offsets::weapon
{
	constexpr const auto next_primary_attack = 0x162c;

	constexpr const auto bullet_scale = 0x1ED8;
	constexpr const auto bullet_speed = 0x1ED0;
}

namespace engine::structures
{
	struct base_entity_t
	{
		std::uintptr_t address;

		bool is( const char* type ) const;
	};

	struct player_t : base_entity_t
	{
		engine::structures::vector3_t get_bone_pos( const engine::structures::bones_t bone_id ) const;

		std::pair< int, int > get_health( ) const ;

		std::int32_t get_team( ) const;

		float get_last_visible_time( ) const;

		bool is_skydiving( );

		std::uintptr_t get_primary_weapon( );
	};

	struct item_t : base_entity_t
	{

	};
}

namespace engine::values
{
	extern std::uintptr_t game_base;

	extern std::mutex lock;

	extern std::vector< engine::structures::player_t > players;

	extern std::mutex lock_items;

	extern std::vector< engine::structures::item_t > items;
}

namespace engine::functions
{
	void initialize( );

	bool verify_in_game( );

	std::vector< engine::structures::player_t > get_players( );

	std::vector< engine::structures::item_t > get_items( );

	void populate_vectors( );

	std::optional < engine::structures::vector2_t > world_to_screen( const engine::structures::vector3_t& position );

	static void normalize_angles( engine::structures::vector3_t& angle )
	{
		while ( angle.x > 89.0f )
			angle.x -= 180.f;

		while ( angle.x < -89.0f )
			angle.x += 180.f;

		while ( angle.y > 180.f )
			angle.y -= 360.f;

		while ( angle.y < -180.f )
			angle.y += 360.f;
	}

	static void vector_angles( const engine::structures::vector3_t& forward, engine::structures::vector3_t& angles )
	{
		if ( forward.y == 0 && forward.x == 0 )
		{
			angles.x = forward.z > 0.f ? 270.f : 90.f;
			angles.y = 0;
		}

		else
		{
			angles.x = ( atan2( -forward.z, forward.Length2D(  ) ) * 180 / M_PI );
			if ( angles.x < 0 )
				angles.x += 360;

			angles.y = ( atan2( forward.y, forward.x ) * 180 / M_PI );
			if ( angles.y < 0 )
				angles.y += 360;
		}

		angles.z = 0;
	}

	static engine::structures::vector3_t calc_angle( engine::structures::vector3_t src, engine::structures::vector3_t dst )
	{
		auto angles = engine::structures::vector3_t{};
		const auto delta = dst - src;

		vector_angles( delta, angles );

		angles = angles.Normalized(  ).Clamped(  );

		return angles;
	}

	static engine::structures::vector3_t smooth_angle( engine::structures::vector3_t orig, engine::structures::vector3_t dest, const float& smooth )
	{
		auto delta = engine::structures::vector3_t{};
		delta = dest - orig;

		delta = delta.Normalized(  ).Clamped(  ); 

		dest = orig + delta / smooth;

		return dest;
	}
}