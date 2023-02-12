#include "menu.hpp"

#include <array>
#include <functional>

#include "../renderer.hpp"

#include "../../engine/engine.hpp"

#include "../../dependencies/rawdata/person.hpp"
#include "../../dependencies/rawdata/skeleton.hpp"
#include "../../dependencies/libs/imgui_tricks.hpp"

#include "../../dependencies/xorstr/xorstr.hpp"

#include "config/config.hpp"

#include "../../modules/visuals/visuals.hpp"
#include "../../modules/aimbot/aimbot.hpp"

static auto current_tab = 0;

void Draw_Main( float, float )
{
    ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { 0,0 } );

    ImGui::SetCursorPos( { 24, 235 } );
    ImGui::BeginChild( xs( "Main" ), { 337, 530 }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove );
    {
        switch ( current_tab )
        {
        case 0:
            renderer::imgui::elements::functions::checkbox( xs( "Aimbot" ), xs( "enables aimbot" ), std::get< bool >( menu::config::config_map[ "assist_enable" ] ) );

            renderer::imgui::elements::functions::hotkey( "##1", &std::get< int >( menu::config::config_map[ "assist_hotkey" ] ) );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Wallcheck" ), xs( "enables wallcheck" ), std::get< bool >( menu::config::config_map[ "enable_wallcheck" ] ) );

            renderer::imgui::elements::functions::separator( );
            
            renderer::imgui::elements::functions::information( xs( "Smoothness" ), xs( "sets the smoothness of the aimbot" ), std::get< int >( menu::config::config_map[ "assist_smoothness" ] ), "%i" );

            renderer::imgui::elements::functions::slider_int( "##2", &std::get< int >( menu::config::config_map[ "assist_smoothness" ] ), 1, 100, "" );

            renderer::imgui::elements::functions::separator( );
            
            renderer::imgui::elements::functions::checkbox( xs( "Ignore Teammates" ), xs( "ignores teammates" ), std::get< bool >( menu::config::config_map[ "enable_aimbot_teamcheck" ] ) );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::information( xs( "Bone" ), xs( "assigns the aiming bone" ), std::get< int >( menu::config::config_map[ "assist_bone" ] ), "%i" );

            renderer::imgui::elements::functions::combo( "##3", &std::get< int >( menu::config::config_map[ "assist_bone" ] ), "head\0\rchest\0\rtorso\0\rstomach\0\0", 1 );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Show FOV" ), xs( "shows circular FOV" ), std::get< bool >( menu::config::config_map[ "assist_show_fov" ] ) );

            renderer::imgui::elements::functions::information( xs( "FOV" ), xs( "sets the aimbot fov" ), std::get< int >( menu::config::config_map[ "assist_fov" ] ), "%i" );

            renderer::imgui::elements::functions::slider_int( "##5", &std::get< int >( menu::config::config_map[ "assist_fov" ] ), 0, 100, "" );

            renderer::imgui::elements::functions::color_picker( "##6", std::get< engine::structures::vector4_t >( menu::config::config_map[ "fov_color" ] ).color );

            renderer::imgui::elements::functions::information( xs( "FOV Segments" ), xs( "adjusts the aimbot fov shape" ), std::get< int >( menu::config::config_map[ "assist_fov_segments" ] ), "%i" );

            renderer::imgui::elements::functions::slider_int( "##7", &std::get< int >( menu::config::config_map[ "assist_fov_segments" ] ), 0, 10, "" );
            break;
        case 1:
            renderer::imgui::elements::functions::checkbox( xs( "No Recoil" ), xs( "based off aim smoothness value" ), std::get< bool >( menu::config::config_map[ "enable_no_recoil" ] ) );
            break;
        case 2:               
            //renderer::imgui::elements::functions::checkbox( xs( "Glow Items" ), xs( "glow for items" ), std::get< bool >( menu::config::config_map[ "enable_glow_items" ] ) );

            //renderer::imgui::elements::functions::separator( );

            //renderer::imgui::elements::functions::checkbox( xs( "Glow" ), xs( "glow for players" ), std::get< bool >( menu::config::config_map[ "enable_glow" ] ) );

            //renderer::imgui::elements::functions::color_picker( "##1", std::get< engine::structures::vector4_t >( menu::config::config_map[ "glow_color" ] ).color );

            //if ( std::get< bool >( menu::config::config_map[ "enable_glow" ] ) )
            //    renderer::imgui::elements::functions::checkbox( xs( "Ignore Teammates" ), xs( "ignores teammates" ), std::get< bool >( menu::config::config_map[ "glow_ignore_teammates" ] ) );

            //renderer::imgui::elements::functions::separator( );

            //renderer::imgui::elements::functions::checkbox( xs( "Glow Teamcheck" ), xs( "glow teammates a different color" ), std::get< bool >( menu::config::config_map[ "enable_glow_teamcheck" ] ) );
            break;
        case 3:
            renderer::imgui::elements::functions::information( xs( "Max Distance" ), xs( "sets the max visual distance" ), std::get< int >( menu::config::config_map[ "display_max_distance" ] ), "%i" );

            renderer::imgui::elements::functions::slider_int( "##1", &std::get< int >( menu::config::config_map[ "display_max_distance" ] ), 0, 1000, "" );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Ignore Teammates" ), xs( "ignores teammates" ), std::get< bool >( menu::config::config_map[ "enable_esp_teamcheck" ] ) );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Bounding Box" ), xs( "draws a rectangular box on entities" ), std::get< bool >( menu::config::config_map[ "display_bounding_box" ] ) );

            renderer::imgui::elements::functions::color_picker( "##2", std::get< engine::structures::vector4_t >( menu::config::config_map[ "bounding_box_color" ] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Tracers" ), xs( "draws a line to entities" ), std::get< bool >( menu::config::config_map[ "enable_tracers" ] ) );

            renderer::imgui::elements::functions::color_picker( "##9", std::get< engine::structures::vector4_t >( menu::config::config_map[ "tracers_color" ] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Filled Box" ), xs( "draws a filled rectangular box on entities" ), std::get< bool >( menu::config::config_map[ "display_filled_box" ] ) );

            renderer::imgui::elements::functions::color_picker( "##3", std::get< engine::structures::vector4_t >( menu::config::config_map[ "filled_box_color" ] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Corner Box" ), xs( "draws a cornered box on entities" ), std::get< bool >( menu::config::config_map[ "display_corner_box" ] ) );

            renderer::imgui::elements::functions::color_picker( "##4", std::get< engine::structures::vector4_t >( menu::config::config_map[ "corner_box_color" ] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Head Hitbox" ), xs( "draws a head square on entities" ), std::get< bool >( menu::config::config_map[ "display_head_hitbox" ] ) );

            renderer::imgui::elements::functions::color_picker( "##5", std::get< engine::structures::vector4_t >( menu::config::config_map[ "hitbox_color" ] ).color );

            //renderer::imgui::elements::functions::checkbox( xs( "Skeleton" ), xs( "expiremental" ), std::get< bool >( menu::config::config_map[ "display_skeleton" ] ) );

            //renderer::imgui::elements::functions::color_picker( "##6", std::get< engine::structures::vector4_t >( menu::config::config_map[ "skeleton_color" ] ).color );

            //renderer::imgui::elements::functions::separator( );

            //renderer::imgui::elements::functions::checkbox( xs( "Distance" ), xs( "draws distance of entity" ), std::get< bool >( menu::config::config_map[ "display_distance" ] ) );

            //renderer::imgui::elements::functions::color_picker( "##7", std::get< engine::structures::vector4_t >( menu::config::config_map[ "distance_color" ] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Health" ), xs ( "draws health of entity" ), std::get< bool >( menu::config::config_map[ "display_health" ] ) );

            renderer::imgui::elements::functions::color_picker( "##8", std::get< engine::structures::vector4_t >( menu::config::config_map[ "health_color" ] ).color );

            renderer::imgui::elements::functions::checkbox( xs( "Shield" ), xs( "draws shield of entity" ), std::get< bool >( menu::config::config_map[ "display_shield" ] ) );

            renderer::imgui::elements::functions::color_picker( "##15", std::get< engine::structures::vector4_t >( menu::config::config_map[ "shield_color" ] ).color );
            break;
        case 4:
            renderer::imgui::elements::functions::button( xs( "Save Config" ), { 328, 35 }, renderer::imgui::elements::button_flags::BUTTON_PURPLE );

            ImGui::NewLine( );

            renderer::imgui::elements::functions::button( xs( "Load Config" ), { 328, 35 }, renderer::imgui::elements::button_flags::BUTTON_GRAY );
            break;
        default:
            break;
        }
    }
    ImGui::EndChild( );

    ImGui::PopStyleVar( );
}

void renderer::menu::draw( )
{
   modules::aimbot::draw_fov( );
   
   if ( renderer::values::renderer->is_menu_open )
   {
        ImGui::SetNextWindowSize( { 375, 770 } );

        ImGui::Begin( xs( "Apex" ), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove );
        {
            auto DrawList = ImGui::GetWindowDrawList( );
            auto Position = ImGui::GetWindowPos( );

            DrawList->AddText( renderer::imgui::elements::fonts::druk_wide_bold, 30, { Position.x + 24, Position.y + 36 }, ImColor{ 255, 255, 255 }, xs( "Apex" ) );

            DrawList->AddRectFilled( { Position.x + 24, Position.y + 73 }, { Position.x + 24 + 187, Position.y + 73 + 2 }, ImColor{ 255, 255, 255, 10 } );
            DrawList->AddRectFilled( { Position.x + 24, Position.y + 73 }, { Position.x + 24 + 88, Position.y + 73 + 2 }, ImColor{ 127, 128, 246 } );

            ImGui::SetCursorPos( { 24, 107 } );

            ImGui::BeginGroup( );
            {
                renderer::imgui::elements::functions::tab( xs( "LEGIT" ), (const char*)ICON_FA_SHIELD_CHECK, current_tab, 0 );

                ImGui::SameLine( );

                renderer::imgui::elements::functions::tab( xs( "RAGE" ), (const char*)ICON_FA_SHIELD, current_tab, 1 );

                ImGui::SameLine( );

                renderer::imgui::elements::functions::tab( xs( "MISC" ), (const char*)ICON_FA_FOLDER, current_tab, 2 );

                ImGui::SameLine( );

                renderer::imgui::elements::functions::tab( xs( "VISUALS" ), (const char*)ICON_FA_BOLT, current_tab, 3 );

                ImGui::SameLine( );

                renderer::imgui::elements::functions::tab( xs( "CFG" ), (const char*)ICON_FA_ARCHIVE, current_tab, 4 );

            }
            ImGui::EndGroup( );

            Draw_Main( 0.f, 0.f );
        }
        ImGui::End( );
    }
    else
    {
        ImGui::Begin( xs( "Watermark" ), nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove );
        { 
            auto DrawList = ImGui::GetWindowDrawList( );

            ImGui::SetWindowSize( { 220, 40 } );

            ImGui::SetWindowPos( { 0, 0 } );

            DrawList->AddText( renderer::imgui::elements::fonts::druk_wide_bold, 25, { 15, 5 }, ImColor{ 255, 255, 255, 70 }, xs( "loaded" ) );
        }
        ImGui::End( );
    }
}