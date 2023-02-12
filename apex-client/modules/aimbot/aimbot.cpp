#define NOMINMAX

#include <map>

#include "aimbot.hpp"

#include "../../engine/engine.hpp"

#include "../../renderer/renderer.hpp"

#include "../../renderer/menu/config/config.hpp"

#include "../../dependencies/xorstr/xorstr.hpp"
#include <unordered_set>

static HWND game;

auto bone_map = std::map< int, engine::structures::bones_t >
{
    { 0, engine::structures::bones_t::head },
    { 1, engine::structures::bones_t::head },
    { 2, engine::structures::bones_t::head },
    { 3, engine::structures::bones_t::head },
};

auto distance_from_cursor( const engine::structures::vector2_t& target )
{
    const auto w = GetSystemMetrics( SM_CXSCREEN ), h = GetSystemMetrics( SM_CYSCREEN );

    if ( !game )
        game = FindWindowA( nullptr, xs( "Apex Legends" ) );

    return std::sqrt( std::pow( target.x - w / 2, 2 ) + std::pow( target.y - h / 2, 2 ) );
}

auto modules::aimbot::draw_fov( ) -> void
{
    if ( std::get< bool >( menu::config::config_map["assist_show_fov"] ) && std::get< int >( menu::config::config_map["assist_fov"] ) > 0 )
    {
        const auto w = GetSystemMetrics( SM_CXSCREEN ), h = GetSystemMetrics( SM_CYSCREEN );

        const auto draw_list = ImGui::GetBackgroundDrawList( );

        draw_list->AddCircle( { w / 2.f, h / 2.f }, std::get< int >( menu::config::config_map["assist_fov"] ) * 10, ImColor{ std::get< engine::structures::vector4_t >( menu::config::config_map["fov_color"] ).color[0], std::get< engine::structures::vector4_t >( menu::config::config_map["fov_color"] ).color[1], std::get< engine::structures::vector4_t >( menu::config::config_map["fov_color"] ).color[2], std::get< engine::structures::vector4_t >( menu::config::config_map["fov_color"] ).color[3] }, std::get< int >( menu::config::config_map["assist_fov_segments"] ), 2.f );
    }
}

//engine::structures::player_t best_player = { NULL }; float distance = 9999.f;
//
//engine::structures::player_t second_best_player = { NULL };
//
//void modules::aimbot::callback( )
//{
//    if ( !std::get< bool >( menu::config::config_map["assist_enable"] ) )
//        return;
//
//    auto local_player = driver.read< engine::structures::player_t >(  engine::values::game_base + engine::offsets::core::local_player );
//
//    if ( !local_player.address )
//        return;
//
//    if ( local_player.is_skydiving( ) )
//        return;
//
//    std::unique_lock locker( engine::values::lock );
//
//    for ( const auto player : engine::values::players )
//    {
//        if ( !player.address )
//            continue;
//
//        if ( std::get< bool >( menu::config::config_map["enable_aimbot_teamcheck"] ) && local_player.get_team( ) == player.get_team( ) )
//            continue;
//
//        auto head_world_position = player.get_bone_pos( engine::structures::bones_t::head ); auto head_position = engine::functions::world_to_screen( head_world_position );
//
//        if ( head_position.has_value( ) )
//        {
//            auto head_screen_position = head_position.value( );
//
//            static const auto w = lf( GetSystemMetrics )( SM_CXSCREEN ), h = lf( GetSystemMetrics )( SM_CYSCREEN );
//
//            auto dist = std::sqrtf( std::powf( head_screen_position.x - w / 2, 2 ) + std::powf( head_screen_position.y - h / 2, 2 ) );
//
//            if ( dist > std::get< int >( menu::config::config_map["assist_fov"] ) * 10 )
//                continue;
//
//            if (std::get<bool>(menu::config::config_map["enable_wallcheck"]))
//            {
//                const auto vis_time = player.get_last_visible_time();
//                const auto is_vis = vis_time > last_vis_time || vis_time < 0 && last_vis_time > 0;
//
//                last_vis_time = vis_time;
//
//                if (!is_vis)
//                    continue;
//            }
//
//            if ( dist < distance )
//            {
//                distance = dist;
//
//                if ( best_player.address )
//                {
//                    second_best_player = best_player;
//                }
//
//                best_player = player;
//            }
//        }
//    }
//
//
//    auto run = [&]( auto best_player )
//    {
//
//        auto [current_health, max_health] = best_player.get_health( );
//
//        if ( current_health <= 0 )
//            return false;
//
//        auto head_world_position = best_player.get_bone_pos( bone_map[std::get< int >( menu::config::config_map["assist_bone"] )] );
//
//        auto lp_bone_position = local_player.get_bone_pos( engine::structures::bones_t::head );
//
//        auto local_player_weapon = local_player.get_primary_weapon( );
//
//        if ( !local_player_weapon )
//            return false;
//
//        //auto enemy_velocity = driver.read<engine::structures::vector3_t>( best_player.address + 0x140 );
//
//        //const auto bullet_speed = driver.read<float>( local_player_weapon + 0x1ef0 );
//        //const auto bullet_gravity = driver.read<float>( local_player_weapon + 0x1ef8 ) * 750.0f;
//
//        //auto vertical_time = head_world_position.dist_to( lp_bone_position ) / bullet_speed;
//        //head_world_position.z += ( bullet_gravity * 0.5f ) * ( vertical_time * vertical_time );
//        //auto horizontal_time = head_world_position.dist_to( lp_bone_position ) / bullet_speed;
//        //head_world_position += ( enemy_velocity * horizontal_time );
//
//        auto angle = engine::functions::calc_angle( driver.read<engine::structures::vector3_t>( local_player.address + engine::offsets::base_entity::camera_position ), head_world_position );
//
//        auto curr_angles = driver.read<engine::structures::vector3_t>( local_player.address + ( engine::offsets::base_entity::view_angles ) );
//
//        auto punch_angles = driver.read<engine::structures::vector3_t>( local_player.address + ( engine::offsets::base_entity::aim_punch ) );
//
//        if ( std::get< bool >( menu::config::config_map["enable_no_recoil"] ) )
//        {
//            angle -= ( driver.read<engine::structures::vector3_t>( local_player.address + ( engine::offsets::base_entity::breath_angles ) ) );
//
//            angle -= ( punch_angles * 0.005 );
//
//            angle += curr_angles;
//        }
//
//        if ( std::get< bool >( menu::config::config_map["enable_smoothing"] ) )
//            angle = engine::functions::smooth_angle( curr_angles, angle, static_cast<float>( std::get< int >( menu::config::config_map["assist_smoothness"] ) ) > 5 ? static_cast<float>( std::get< int >( menu::config::config_map["assist_smoothness"] ) ) : 5 );
//
//        angle.Clamp( );
//
//        angle.Normalize( );
//
//        if ( !GetAsyncKeyState( std::get< int >( menu::config::config_map["assist_hotkey"] ) ) )
//            return false;
//
//        driver.write<engine::structures::vector3_t>( local_player.address + ( engine::offsets::base_entity::view_angles ), angle );
//
//        return true;
//    };
//
//    if ( best_player.address )
//    {
//
//        if ( !run( best_player ) && second_best_player.address )
//            run( second_best_player );
//    }
//}

constexpr auto min_health = 0;
constexpr auto max_health = 120;

auto locked = false;
engine::structures::player_t closest_player = { NULL };

std::unordered_map<std::uintptr_t, float> vis_map;

auto last_vis_time = 0.f;

auto picked_bone = false;

void modules::aimbot::callback()
{
    auto local_player = driver.read< engine::structures::player_t >(engine::values::game_base + engine::offsets::core::local_player);

    if (!local_player.address)
        return;

    if (local_player.is_skydiving())
        return;

    if (GetAsyncKeyState(std::get< int >(menu::config::config_map["assist_hotkey"])) && std::get< bool >(menu::config::config_map["assist_enable"]))
    {
        auto lowest_distance = 1000.f;

        const auto view_angles = driver.read<engine::structures::vector3_t>(local_player.address + (engine::offsets::base_entity::view_angles));
        auto punch_angles = driver.read<engine::structures::vector3_t>(local_player.address + (engine::offsets::base_entity::aim_punch));
        const auto camera_pos = driver.read<engine::structures::vector3_t>(local_player.address + engine::offsets::base_entity::camera_position);

        auto sway_angles = driver.read<engine::structures::vector3_t>(local_player.address + (engine::offsets::base_entity::breath_angles));

        std::unique_lock locker(engine::values::lock);

        const auto players_clone = engine::values::players;

        locker.unlock();

        for (const auto player : players_clone)
        {
            if (!player.address || (std::get< bool >(menu::config::config_map["enable_aimbot_teamcheck"]) && local_player.get_team() == player.get_team()))
                continue;

            auto [npc_health, maximum_health] = player.get_health();

            if (npc_health <= min_health || npc_health > max_health)
                continue;

            auto origin = player.get_bone_pos(engine::structures::bones_t::head);

            if (std::get< bool >(menu::config::config_map["enable_aimbot_teamcheck"]) && origin.dist_to(local_player.get_bone_pos(engine::structures::bones_t::head)) > 9999999999)
                continue;

            auto angle = engine::functions::calc_angle(camera_pos, origin);
            auto delta = angle - view_angles;

            const auto distance = delta.Clamped().Length();

            if (distance < lowest_distance)
            {
                lowest_distance = distance;
                closest_player = player;
            }
        }

        if (closest_player.address)
        {
            locked = true;
            engine::structures::vector3_t bone;

            //const auto bone_type = bone_map[std::get< int >(menu::config::config_map["assist_bone"])];

            bone = closest_player.get_bone_pos(engine::structures::bones_t::head);

            auto enemy_velocity = driver.read<engine::structures::vector3_t>(closest_player.address + engine::offsets::base_entity::velocity);
            const auto local_player_weapon = local_player.get_primary_weapon();
            const auto lp_bone = local_player.get_bone_pos(engine::structures::bones_t::head);
            const auto bullet_speed = driver.read<float>(local_player_weapon + engine::offsets::weapon::bullet_speed);
            const auto bullet_gravity = driver.read<float>(local_player_weapon + engine::offsets::weapon::bullet_scale) * 750.0f;

            engine::structures::vector3_t bone_sp;

            auto head_world_position = closest_player.get_bone_pos( engine::structures::bones_t::head ); auto head_position = engine::functions::world_to_screen( head_world_position );

            if (head_position.has_value())
            {
                auto head_screen_position = head_position.value();

                const auto w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);

                auto dist = std::sqrtf(std::powf(head_screen_position.x - w / 2, 2) + std::powf(head_screen_position.y - h / 2, 2));

                if (dist > std::get< int >(menu::config::config_map["assist_fov"]) * 10)
                {
                    closest_player = { 0ul };

                    return;
                }
            }
            else
            {
                closest_player = { 0ul };

                return;
            }

            if (std::get<bool>(menu::config::config_map["enable_wallcheck"]))
            {
                const auto iter = vis_map.find(closest_player.address);

                if (iter == vis_map.end())
                {
                    vis_map.insert({ closest_player.address, closest_player.get_last_visible_time() });
                }
                else
                {

                    const auto vis_time = closest_player.get_last_visible_time();

                    const auto is_vis = vis_time > iter->second && iter->second > 0;

                    iter->second = vis_time;

                    if (!is_vis)
                        return;
                }
            }

            /*if (local_player_weapon && bullet_speed > 0.0f)
            {

                auto vertical_time = bone.dist_to(lp_bone) / bullet_speed;
                bone.z += (bullet_gravity * 0.5f) * (vertical_time * vertical_time);
                auto horizontal_time = bone.dist_to(lp_bone) / bullet_speed;
                bone += (enemy_velocity * horizontal_time);
            }*/

            auto angle = engine::functions::calc_angle(camera_pos, bone);

            if (std::get< bool >(menu::config::config_map["enable_no_recoil"]))
            {
                angle -= ( driver.read<engine::structures::vector3_t>( local_player.address + ( engine::offsets::base_entity::breath_angles ) ) );

                angle -= ( punch_angles * 0.005 );

                angle += view_angles;
            }

            angle.Normalize();

            angle = engine::functions::smooth_angle( view_angles, angle, std::max(std::get< int >( menu::config::config_map["assist_smoothness"] ) , 4) );

            angle.Clamp();

            driver.write<engine::structures::vector3_t>(local_player.address + (engine::offsets::base_entity::view_angles), angle);
        }

        return;

    }
    else
        picked_bone = false;

    locked = false;

    closest_player = { 0 };
}