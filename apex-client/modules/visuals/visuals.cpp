#include "visuals.hpp"

#include "../../engine/engine.hpp"

#include "../../renderer/renderer.hpp"
#include "../../renderer/menu/config/config.hpp"

#include <cmath>

//std::list< engine::structures::bones_t > upper_part = { engine::structures::bones_t::head, engine::structures::bones_t::chest };
//std::list< engine::structures::bones_t > right_arm = { engine::structures::bones_t::chest, engine::structures::bones_t::right_shoulder, engine::structures::bones_t::right_bicep, engine::structures::bones_t::right_elbow, engine::structures::bones_t::right_forearm, engine::structures::bones_t::right_wrist };
//std::list< engine::structures::bones_t > left_arm = { engine::structures::bones_t::chest, engine::structures::bones_t::left_shoulder, engine::structures::bones_t::left_bicep, engine::structures::bones_t::left_elbow, engine::structures::bones_t::left_forearm, engine::structures::bones_t::left_wrist };
//std::list< engine::structures::bones_t > spine = { engine::structures::bones_t::chest, engine::structures::bones_t::torso, engine::structures::bones_t::stomach, engine::structures::bones_t::root };
//std::list< engine::structures::bones_t > lower_right = { engine::structures::bones_t::root, engine::structures::bones_t::right_hip, engine::structures::bones_t::right_knee, engine::structures::bones_t::right_ankle };
//std::list< engine::structures::bones_t > lower_left = { engine::structures::bones_t::root, engine::structures::bones_t::left_hip, engine::structures::bones_t::left_knee, engine::structures::bones_t::left_ankle }; // Bones r dif per model ffs
//
//std::list< std::list< engine::structures::bones_t > > skeleton = { upper_part, right_arm, left_arm, spine, lower_right, lower_left };

void modules::visuals::callback( )
{
    if (!driver.read(engine::values::game_base + engine::offsets::core::level_name))
        return;

    if (driver.read_string(engine::values::game_base + engine::offsets::core::level_name, false) == xs("mp_lobby"))
        return;

    auto local_player = driver.read< engine::structures::player_t >( engine::values::game_base + engine::offsets::core::local_player );

    if ( !local_player.address )
        return;

    if ( local_player.is_skydiving( ) )
        return;

	std::unique_lock locker( engine::values::lock );

    const auto players_clone = engine::values::players;

    locker.unlock();

	for ( const auto player : players_clone)
	{

        if( !player.address )
			continue;

        if ( std::get< bool >( menu::config::config_map["enable_esp_teamcheck"] ) && local_player.get_team( ) == player.get_team( ) )
            continue;

        auto head_world_position = player.get_bone_pos( engine::structures::bones_t::head ); auto head_position = engine::functions::world_to_screen( head_world_position );

        auto draw_list = ImGui::GetBackgroundDrawList( );

		auto root_position = engine::functions::world_to_screen( driver.read< engine::structures::vector3_t>( player.address + 0x14C ) );

        if ( root_position.has_value( ) && head_position.has_value( ) )
        {
            auto root_screen_position = root_position.value( );
            auto head_screen_position = head_position.value( );

            const float width = std::fabs( head_screen_position.y - root_screen_position.y ) / 4.0f;
            const float height = std::fabs( head_screen_position.y - root_screen_position.y ) * 1.2f;

            if (isnan(static_cast<float>(width)) || isnan(static_cast<float>(height)))
                continue;

            const auto line_len_width = width / 2.5f;

            auto [current_health, max_health] = player.get_health( );

            if (isnan(static_cast<float>(current_health)) || isnan(static_cast<float>(max_health)))
                continue;

            if ( current_health <= 0 )
                continue;

            const auto new_width_health = ( width ) / max_health * current_health;
            const auto new_height_health = ( height ) / max_health * current_health;

            if (isnan(static_cast<float>(new_width_health)))
                continue;

            if ( std::get< bool >( menu::config::config_map["enable_tracers"] ) )
            {
                const auto w = GetSystemMetrics( SM_CXSCREEN ), h = GetSystemMetrics( SM_CYSCREEN );

                draw_list->AddLine(  { w / 2.f, static_cast<float>( h ) }, { root_screen_position.x, root_screen_position.y }, ImColor{ std::get< engine::structures::vector4_t >( menu::config::config_map["tracers_color"] ).color[0], std::get< engine::structures::vector4_t >( menu::config::config_map["tracers_color"] ).color[1], std::get< engine::structures::vector4_t >( menu::config::config_map["tracers_color"] ).color[2] } );
            }

            //if ( std::get< bool >( menu::config::config_map["display_skeleton"] ) )
            //{
            //    for ( const auto part : skeleton )
            //    {
            //        auto prev_bone = engine::structures::bones_t::invalid;

            //        for ( const auto bone : part )
            //        {
            //            if ( prev_bone == engine::structures::bones_t::invalid )
            //            {
            //                prev_bone = bone;

            //                continue;
            //            }

            //            auto prev_bone_world = player.get_bone_pos( prev_bone ); auto prev_bone_pos = engine::functions::world_to_screen( prev_bone_world );

            //            auto bone_world = player.get_bone_pos( bone ); auto bone_pos = engine::functions::world_to_screen( bone_world );

            //            if ( prev_bone_pos.has_value( ) && bone_pos.has_value( ) )
            //            {

            //                auto a = prev_bone_pos.value( );
            //                auto b = bone_pos.value( );

            //                draw_list->AddLine( { a.x, a.y }, { b.x, b.y }, ImColor{ std::get< engine::structures::vector4_t >( menu::config::config_map["skeleton_color"] ).color[0], std::get< engine::structures::vector4_t >( menu::config::config_map["skeleton_color"] ).color[1], std::get< engine::structures::vector4_t >( menu::config::config_map["skeleton_color"] ).color[2] } );

            //                prev_bone = bone;

            //            }
            //        }
            //    }
            //}

            if ( std::get< bool >( menu::config::config_map["display_health"] ) )
            {
                if (isnan(static_cast<float>(current_health)) || isnan(static_cast<float>(new_width_health)))
                    continue;

                std::string to_draw; to_draw.append( std::to_string( static_cast<int>( current_health ) ) ); to_draw.append( "HP" );

                {					
					draw_list->AddRectFilled({ root_screen_position.x - width - (width / 10.f) / 4.0f - 5, root_screen_position.y - 2 }, { root_screen_position.x - width - (width / 10.f) - 7, root_screen_position.y - new_height_health }, ImColor{ std::get< engine::structures::vector4_t >( menu::config::config_map[ "health_color" ] ).color[ 0 ], std::get< engine::structures::vector4_t >( menu::config::config_map[ "health_color" ] ).color[ 1 ], std::get< engine::structures::vector4_t >( menu::config::config_map[ "health_color" ] ).color[ 2 ], std::get< engine::structures::vector4_t >( menu::config::config_map[ "health_color" ] ).color[ 3 ] } );
				}

				{
					draw_list->AddRect({ root_screen_position.x - width - (width / 10.f) / 4.0f - 5 - 1, root_screen_position.y - 1 }, { root_screen_position.x - width - (width / 10.f) - 7 - 1, root_screen_position.y - height - 1 }, ImColor{ 1, 1, 1 } );
				}

				draw_list->AddOutlinedText(renderer::imgui::elements::fonts::pro_font_windows, 9.2f, { (root_screen_position.x - width - (width / 10.f) / 4.5f - 5 - 1) - 35.5f, root_screen_position.y - new_height_health }, ImColor{ std::get< engine::structures::vector4_t >( menu::config::config_map[ "health_color" ] ).color[ 0 ], std::get< engine::structures::vector4_t >( menu::config::config_map[ "health_color" ] ).color[ 1 ], std::get< engine::structures::vector4_t >( menu::config::config_map[ "health_color" ] ).color[ 2 ], std::get< engine::structures::vector4_t >( menu::config::config_map[ "health_color" ] ).color[ 3 ] }, to_draw.c_str( ) );
            }

            if ( std::get< bool >( menu::config::config_map["display_shield"] ) ) 
            {
                const auto new_height_shield = ( height ) / driver.read<std::uint32_t>( player.address + engine::offsets::base_entity::max_shield ) * driver.read<std::uint32_t>( player.address + engine::offsets::base_entity::shield );

                if (isnan(static_cast<float>(new_height_shield)))
                    continue;

                if ( new_height_shield > 0 )
                {
                    {
                        draw_list->AddRectFilled( { ( ( root_screen_position.x + width ) + ( width / 10.f ) * 2.f ) + .5f, root_screen_position.y }, { ( ( root_screen_position.x + width ) + ( width / 10.f ) ) + .5f,  root_screen_position.y - new_height_shield }, ImColor{ std::get< engine::structures::vector4_t >( menu::config::config_map["shield_color"] ).color[0], std::get< engine::structures::vector4_t >( menu::config::config_map["shield_color"] ).color[1], std::get< engine::structures::vector4_t >( menu::config::config_map["shield_color"] ).color[2], std::get< engine::structures::vector4_t >( menu::config::config_map["shield_color"] ).color[3] } );
                    }

                    {
                        draw_list->AddRect( { ( ( root_screen_position.x + width ) + ( width / 10.f ) * 2.f ) + .9f, root_screen_position.y }, { ( ( root_screen_position.x + width ) + ( width / 10.f ) ) + .9f,  root_screen_position.y - new_height_shield }, ImColor{ 1, 1, 1 } );
                    }
                }
            }

            if ( std::get< bool >( menu::config::config_map["display_bounding_box"] ) )
            {
                {
                    draw_list->AddRect( { root_screen_position.x - width, root_screen_position.y }, { root_screen_position.x + width, root_screen_position.y - height }, ImColor{ std::get< engine::structures::vector4_t >( menu::config::config_map["bounding_box_color"] ).color[0], std::get< engine::structures::vector4_t >( menu::config::config_map["bounding_box_color"] ).color[1], std::get< engine::structures::vector4_t >( menu::config::config_map["bounding_box_color"] ).color[2] } );
                }

                {
                    draw_list->AddRect( { root_screen_position.x - width + 1, root_screen_position.y + 1 }, { root_screen_position.x + width + 1, root_screen_position.y - height + 1 }, ImColor{ 0, 0, 0 } );
                }
            }

            if ( std::get< bool >( menu::config::config_map["display_filled_box"] ) )
            {
                {
                    draw_list->AddRectFilled( { root_screen_position.x - width, root_screen_position.y }, { root_screen_position.x + width, root_screen_position.y - height }, ImColor{ std::get< engine::structures::vector4_t >( menu::config::config_map["filled_box_color"] ).color[0], std::get< engine::structures::vector4_t >( menu::config::config_map["filled_box_color"] ).color[1], std::get< engine::structures::vector4_t >( menu::config::config_map["filled_box_color"] ).color[2], std::get< engine::structures::vector4_t >( menu::config::config_map["filled_box_color"] ).color[3] } );
                }
            }

            if ( std::get< bool >( menu::config::config_map["display_corner_box"] ) && !std::get< bool >( menu::config::config_map["display_bounding_box"] ) )
            {
                {
                    draw_list->AddLine( { root_screen_position.x - width, root_screen_position.y - height }, { root_screen_position.x - width + line_len_width, root_screen_position.y - height }, ImColor{ std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[0], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[1], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[2], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[3] } );
                    draw_list->AddLine( { root_screen_position.x - width, root_screen_position.y - height }, { root_screen_position.x - width, ( root_screen_position.y - height ) + line_len_width }, ImColor{ std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[0], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[1], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[2], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[3] } );

                    draw_list->AddLine( { root_screen_position.x + width, root_screen_position.y - height }, { root_screen_position.x + width - line_len_width, root_screen_position.y - height }, ImColor{ std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[0], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[1], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[2], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[3] } );
                    draw_list->AddLine( { root_screen_position.x + width, root_screen_position.y - height }, { root_screen_position.x + width, ( root_screen_position.y - height ) + line_len_width }, ImColor{ std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[0], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[1], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[2], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[3] } );

                    draw_list->AddLine( { root_screen_position.x - width, root_screen_position.y }, { root_screen_position.x - width + line_len_width, root_screen_position.y }, ImColor{ std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[0], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[1], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[2], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[3] } );
                    draw_list->AddLine( { root_screen_position.x - width, root_screen_position.y }, { root_screen_position.x - width, root_screen_position.y - line_len_width }, ImColor{ std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[0], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[1], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[2], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[3] } );

                    draw_list->AddLine( { root_screen_position.x + width, root_screen_position.y }, { root_screen_position.x + width - line_len_width, root_screen_position.y }, ImColor{ std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[0], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[1], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[2], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[3] } );
                    draw_list->AddLine( { root_screen_position.x + width, root_screen_position.y }, { root_screen_position.x + width, root_screen_position.y - line_len_width }, ImColor{ std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[0], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[1], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[2], std::get< engine::structures::vector4_t >( menu::config::config_map["corner_box_color"] ).color[3] } );
                }

                {
                    draw_list->AddLine( { root_screen_position.x - width + 1, root_screen_position.y - height + 1 }, { root_screen_position.x - width + line_len_width, root_screen_position.y - height + 1 }, ImColor{ 0,0,0 } );
                    draw_list->AddLine( { root_screen_position.x - width + 1, root_screen_position.y - height + 1 }, { root_screen_position.x - width + 1, ( root_screen_position.y - height ) + line_len_width }, ImColor{ 0,0,0 } );

                    draw_list->AddLine( { root_screen_position.x + width - 1, root_screen_position.y - height + 1 }, { root_screen_position.x + width - line_len_width, root_screen_position.y - height + 1 }, ImColor{ 0,0,0 } );
                    draw_list->AddLine( { root_screen_position.x + width - 1, root_screen_position.y - height + 1 }, { root_screen_position.x + width - 1, ( root_screen_position.y - height ) + line_len_width }, ImColor{ 0,0,0 } );

                    draw_list->AddLine( { root_screen_position.x - width + 1, root_screen_position.y - 1 }, { root_screen_position.x - width + line_len_width, root_screen_position.y - 1 }, ImColor{ 0,0,0 } );
                    draw_list->AddLine( { root_screen_position.x - width + 1, root_screen_position.y - 1 }, { root_screen_position.x - width + 1, root_screen_position.y - line_len_width - 1 }, ImColor{ 0,0,0 } );

                    draw_list->AddLine( { root_screen_position.x + width - 1, root_screen_position.y - 1 }, { root_screen_position.x + width - line_len_width, root_screen_position.y - 1 }, ImColor{ 0,0,0 } );
                    draw_list->AddLine( { root_screen_position.x + width - 1, root_screen_position.y - 1 }, { root_screen_position.x + width - 1, root_screen_position.y - line_len_width - 1 }, ImColor{ 0,0,0 } );
                }
            }

            if ( std::get< bool >( menu::config::config_map["display_head_hitbox"] ) )
            {
                {
                    draw_list->AddRect( { head_screen_position.x, head_screen_position.y }, { head_screen_position.x + 5.f, head_screen_position.y + 5.f }, ImColor{ std::get< engine::structures::vector4_t >( menu::config::config_map["hitbox_color"] ).color[0], std::get< engine::structures::vector4_t >( menu::config::config_map["hitbox_color"] ).color[1], std::get< engine::structures::vector4_t >( menu::config::config_map["hitbox_color"] ).color[2], std::get< engine::structures::vector4_t >( menu::config::config_map["hitbox_color"] ).color[3] } );
                }

                {
                    draw_list->AddRect( { head_screen_position.x - 1, head_screen_position.y - 1 }, { ( head_screen_position.x + 5.f ) - 1, ( head_screen_position.y + 5.f ) - 1 }, ImColor{ 0, 0, 0 }, 0, 0, 0.5f );
                }
            }
        }
	}
}
