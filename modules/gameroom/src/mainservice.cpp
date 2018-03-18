/* -*-LIC_BEGIN-*- */
/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2018                        
*                                                                          
* This file is part of DrawSpace.                                          
*                                                                          
*    DrawSpace is free software: you can redistribute it and/or modify     
*    it under the terms of the GNU General Public License as published by  
*    the Free Software Foundation, either version 3 of the License, or     
*    (at your option) any later version.                                   
*                                                                          
*    DrawSpace is distributed in the hope that it will be useful,          
*    but WITHOUT ANY WARRANTY; without even the implied warranty of        
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
*    GNU General Public License for more details.                          
*                                                                          
*    You should have received a copy of the GNU General Public License     
*    along with DrawSpace.  If not, see <http://www.gnu.org/licenses/>.    
*
*/
/* -*-LIC_END-*- */

#include "mainservice.h"
#include "component.h"
#include "luacontext.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Aspect;
using namespace DrawSpace::AspectImplementations;
using namespace DrawSpace::Utils;

_DECLARE_DS_LOGGER( logger, "gameroom_mainservice", NULL )

MainService::MainService( void ) :
m_console_active( false ),
m_console_current_line( 0 ),
m_meshe_import( NULL )
{
    m_console_texts.push_back( "Command input ready" );
    m_console_texts.push_back( ">" );
    m_console_current_line++;
}

MainService::~MainService( void )
{
    if( m_meshe_import )
    {
        _DRAWSPACE_DELETE_( m_meshe_import );
    }
}

bool MainService::Init( void )
{
    //////////////recup params du service //////////////////

    ComponentList<DrawSpace::Logger::Configuration*> logconfs;
    m_owner->GetComponentsByType<DrawSpace::Logger::Configuration*>( logconfs );

    ComponentList<DrawSpace::Core::BaseCallback<void, bool>*> mouse_cbs;
    m_owner->GetComponentsByType<DrawSpace::Core::BaseCallback<void, bool>*>( mouse_cbs );

    ComponentList<DrawSpace::Core::BaseCallback<void, int>*> app_cbs;
    m_owner->GetComponentsByType<DrawSpace::Core::BaseCallback<void, int>*>( app_cbs );

    DrawSpace::Core::BaseCallback<void, bool>* mousecircularmode_cb;
    DrawSpace::Core::BaseCallback<void, bool>* mousevisible_cb;

    mousecircularmode_cb = mouse_cbs[0]->getPurpose();
    mousevisible_cb = mouse_cbs[1]->getPurpose();

    DrawSpace::Logger::Configuration* logconf = logconfs[0]->getPurpose();

    DrawSpace::Core::BaseCallback<void, int>* closeapp_cb;
    closeapp_cb = app_cbs[0]->getPurpose();


    m_mousecircularmode_cb = mousecircularmode_cb;
    m_closeapp_cb = closeapp_cb;

    ////////////////////////////////////////////////////////

        
    logconf->RegisterSink( &logger );
    logger.SetConfiguration( logconf );

    logconf->RegisterSink( MemAlloc::GetLogSink() );
    MemAlloc::GetLogSink()->SetConfiguration( logconf );

    /////////////////////////////////////////////////////////////////////////////////

    m_renderer = DrawSpace::Core::SingletonPlugin<DrawSpace::Interface::Renderer>::GetInstance()->m_interface;

    m_renderer->GetDescr( m_pluginDescr );

    m_renderer->SetRenderState( &DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETCULLING, "cw" ) );
    m_renderer->SetRenderState( &DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETTEXTUREFILTERTYPE, "linear" ) );
    m_renderer->SetRenderState( &DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "false" ) );


    m_meshe_import = _DRAWSPACE_NEW_( DrawSpace::Utils::AC3DMesheImport, DrawSpace::Utils::AC3DMesheImport );

    /////////////////////////////////////////////////////////////////////////////////

    LuaContext::GetInstance()->Startup();

    LuaContext::GetInstance()->Execute( "g=Globals()" );
    LuaContext::GetInstance()->Execute( "renderer=Renderer()" );
    LuaContext::GetInstance()->Execute( "rg=RenderPassNodeGraph('rg')" );
    LuaContext::GetInstance()->Execute( "rg:create_root('final_pass')" );

    LuaContext::GetInstance()->Execute( "eg=EntityNodeGraph('eg')" );
    LuaContext::GetInstance()->Execute( "root_entity=Entity()" );
    LuaContext::GetInstance()->Execute( "root_entity:add_renderingaspect()" );
    LuaContext::GetInstance()->Execute( "root_entity:add_timeaspect()" );
    LuaContext::GetInstance()->Execute( "root_entity:connect_renderingaspect_rendergraph(rg)" );
    LuaContext::GetInstance()->Execute( "eg:set_root('root', root_entity )" );


    // creation cote lua de l'enum RenderState::Operation
    LuaContext::GetInstance()->Execute( "RENDERSTATE_OPE_NONE=0" );
    LuaContext::GetInstance()->Execute( "RENDERSTATE_OPE_SETCULLING=1" );
    LuaContext::GetInstance()->Execute( "RENDERSTATE_OPE_ENABLEZBUFFER=2" );
    LuaContext::GetInstance()->Execute( "RENDERSTATE_OPE_SETTEXTUREFILTERTYPE=3" );
    LuaContext::GetInstance()->Execute( "RENDERSTATE_OPE_SETVERTEXTEXTUREFILTERTYPE=4" );
    LuaContext::GetInstance()->Execute( "RENDERSTATE_OPE_SETFILLMODE=5" );
    LuaContext::GetInstance()->Execute( "RENDERSTATE_OPE_ALPHABLENDENABLE=6" );
    LuaContext::GetInstance()->Execute( "RENDERSTATE_OPE_ALPHABLENDOP=7" );
    LuaContext::GetInstance()->Execute( "RENDERSTATE_OPE_ALPHABLENDFUNC=8" );
    LuaContext::GetInstance()->Execute( "RENDERSTATE_OPE_ALPHABLENDDEST=9" );
    LuaContext::GetInstance()->Execute( "RENDERSTATE_OPE_ALPHABLENDSRC=10" );

    // args loading shaders
    LuaContext::GetInstance()->Execute( "SHADER_COMPILED=1");
    LuaContext::GetInstance()->Execute( "SHADER_NOT_COMPILED=0");
    

    /////////////////////////////////////////////////////////////////////////////////
    
    
    //m_finalpass = m_rendergraph.CreateRoot( "final_pass" );

    /*
    m_finalpass.CreateViewportQuad();

    m_finalpass.GetViewportQuad()->SetFx( _DRAWSPACE_NEW_( Fx, Fx ) );

    RenderStatesSet finalpass_rss;
    finalpass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETTEXTUREFILTERTYPE, "point" ) );
    finalpass_rss.AddRenderStateOut( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::SETTEXTUREFILTERTYPE, "linear" ) );


    m_finalpass.GetViewportQuad()->GetFx()->SetRenderStates( finalpass_rss );


    m_finalpass.GetViewportQuad()->GetFx()->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.vso", true ) ) );
    m_finalpass.GetViewportQuad()->GetFx()->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.pso", true ) ) );

    m_finalpass.GetViewportQuad()->GetFx()->GetShader( 0 )->LoadFromFile();
    m_finalpass.GetViewportQuad()->GetFx()->GetShader( 1 )->LoadFromFile();
    */

    /*
    m_finalpass.GetRenderingQueue()->EnableDepthClearing( false );
    m_finalpass.GetRenderingQueue()->EnableTargetClearing( true );
    m_finalpass.GetRenderingQueue()->SetTargetClearingColor( 26, 27, 28, 255 );
    */

    /*
    m_render_passes["final_pass"] = m_rendergraph.CreateRoot( "final_pass" );

    m_render_passes["final_pass"].GetRenderingQueue()->EnableDepthClearing( false );
    m_render_passes["final_pass"].GetRenderingQueue()->EnableTargetClearing( true );
    m_render_passes["final_pass"].GetRenderingQueue()->SetTargetClearingColor( 26, 27, 28, 255 );
    */

    /*
    m_texturepass = m_finalpass.CreateChild( "texture_pass", 0 );    
    m_texturepass.GetRenderingQueue()->EnableDepthClearing( true );
    m_texturepass.GetRenderingQueue()->EnableTargetClearing( false );
    */

    ////////////////////////////////////////////////////////////////////////////////////

    //RenderingAspect* rendering_aspect = m_rootEntity.AddAspect<RenderingAspect>();

    DrawSpace::Core::Entity* root_entity = m_entitygraphs["eg"]->GetEntityGraph().GetRoot();
    DrawSpace::EntityGraph::EntityNode& root_entity_node = m_entitygraphs["eg"]->GetEntityNode( "root" );

    RenderingAspect* rendering_aspect = root_entity->GetAspect<RenderingAspect>();

    //rendering_aspect->AddImplementation( &m_passesRender );
    //m_passesRender.SetRendergraph( &m_rendergraph );


    //rendering_aspect->AddImplementation( &m_rendergraphs["rg"]->GetPassesRenderAspectImpl() );
    rendering_aspect->AddImplementation( &m_textRender );
    
    //rendering_aspect->AddComponent<TextRenderingAspectImpl::TextDisplay>( "fps", 10, 20, 255, 100, 100, "..." );
    rendering_aspect->AddComponent<std::vector<TextRenderingAspectImpl::TextDisplay>>( "console_lines" );

    /*
    TimeAspect* time_aspect = m_rootEntity.AddAspect<TimeAspect>();

    time_aspect->AddComponent<TimeManager>( "time_manager" );
    time_aspect->AddComponent<TimeAspect::TimeScale>( "time_scale", TimeAspect::NORMAL_TIME );
    time_aspect->AddComponent<dsstring>( "output_formated_datetime", "..." );
    time_aspect->AddComponent<dstime>( "time", 0 );
    time_aspect->AddComponent<int>( "output_fps" );
    time_aspect->AddComponent<int>( "output_world_nbsteps" );

    time_aspect->AddComponent<dsreal>( "output_time_factor" );
    */

    //m_fps_yaw = time_aspect->TimeAngleFactory( 0.0 );
    //m_fps_pitch = time_aspect->TimeAngleFactory( 0.0 );


    //m_rootEntityNode = m_entitygraph.SetRoot( &m_rootEntity );

    /*
    //////////////////////////////////////////////////////////////////////////////////////

    PhysicsAspect* physic_aspect = m_world1Entity.AddAspect<PhysicsAspect>();

    physic_aspect->AddComponent<bool>( "gravity_state", true );    
    physic_aspect->AddComponent<Vector>( "gravity", Vector( 0.0, -9.81, 0.0, 0.0 ) );

    m_World1EntityNode = m_rootEntityNode.AddChild( &m_world1Entity );
    */

    /////////////////////////////////////////////////////////////////////////////////////
    /*
    create_camera();
    create_skybox();
    create_ground();
    */

    create_console_quad();

    /*
    // ajouter la skybox a la scene
    m_skyboxEntityNode = m_rootEntityNode.AddChild( &m_skyboxEntity );                                                                      
    m_skyboxRender->RegisterToRendering( m_rendergraph );



    // ajouter la cameras a la scene
    m_cameraEntityNode = m_rootEntityNode.AddChild( &m_cameraEntity );

    // designer la camera courante    
    m_entitygraph.SetCurrentCameraEntity( &m_cameraEntity );

    // ajouter le ground a la scene
    m_groundEntityNode = m_World1EntityNode.AddChild( &m_groundEntity );
    m_groundRender.RegisterToRendering( m_rendergraph );
    */

    // ajout du quad console a la scene


    
    //m_quadEntityNode = m_rootEntityNode.AddChild( &m_quadEntity );
    m_quadEntityNode = root_entity_node.AddChild( &m_quadEntity );

    m_quadRender.RegisterToRendering( m_rendergraphs["rg"]->GetRenderGraph() );
    m_rendergraphs["rg"]->GetRenderGraph().PushSignal_UpdatedRenderingQueues();

    /////////////////////////////////////////////////////////////////////////////////

    //m_systemsHub.Init( &m_entitygraph );

    m_systemsHub.Init( &m_entitygraphs["eg"]->GetEntityGraph() );

    //m_rendergraph.PushSignal_UpdatedRenderingQueues();
    //m_entitygraph.PushSignal_RenderSceneBegin();

    m_entitygraphs["eg"]->GetEntityGraph().PushSignal_RenderSceneBegin();

    //set_mouse_circular_mode( true );

    _DSDEBUG( logger, dsstring("MainService : startup...") );

    return true;
}


void MainService::Run( void )
{
    //m_systemsHub.Run( &m_entitygraph );

    m_systemsHub.Run( &m_entitygraphs["eg"]->GetEntityGraph() );
    
    /*
    TimeAspect* time_aspect = m_rootEntity.GetAspect<TimeAspect>();
    char comment[256];
    sprintf( comment, "%d fps - %s", time_aspect->GetComponent<int>( "output_fps" )->getPurpose(), m_pluginDescr.c_str() );
    */

    //RenderingAspect* rendering_aspect = m_rootEntity.GetAspect<RenderingAspect>();

    DrawSpace::Core::Entity* root_entity = m_entitygraphs["eg"]->GetEntityGraph().GetRoot();
    RenderingAspect* rendering_aspect = root_entity->GetAspect<RenderingAspect>();


    //rendering_aspect->GetComponent<TextRenderingAspectImpl::TextDisplay>( "fps" )->getPurpose().m_text = comment;

    
    rendering_aspect->GetComponent<std::vector<TextRenderingAspectImpl::TextDisplay>>( "console_lines" )->getPurpose().clear();
    if( m_console_active )
    {
        print_console_content();
    }
}

void MainService::Release( void )
{
    _DSDEBUG( logger, dsstring("MainService : shutdown...") );

    //m_systemsHub.Release( &m_entitygraph );

    m_systemsHub.Release( &m_entitygraphs["eg"]->GetEntityGraph() );


    LuaContext::GetInstance()->Shutdown();
}

void MainService::print_console_content( void )
{
    
    //RenderingAspect* rendering_aspect = m_rootEntity.GetAspect<RenderingAspect>();
    DrawSpace::Core::Entity* root_entity = m_entitygraphs["eg"]->GetEntityGraph().GetRoot();
    RenderingAspect* rendering_aspect = root_entity->GetAspect<RenderingAspect>();

    if( m_console_texts.size() <= m_console_max_lines_display )
    {
        for( size_t i = 0; i < m_console_texts.size(); i++ )
        {
            TextRenderingAspectImpl::TextDisplay myline( 15, m_console_y_pos + ( i * 15 ), 10, 70, 10, m_console_texts[i] );
            rendering_aspect->GetComponent<std::vector<TextRenderingAspectImpl::TextDisplay>>( "console_lines" )->getPurpose().push_back( myline );
        }
    }
    else
    {
        for( size_t i = 0; i < m_console_max_lines_display; i++ )
        {
            TextRenderingAspectImpl::TextDisplay myline( 15, m_console_y_pos + ( i * 15 ), 10, 70, 10, m_console_texts[m_console_texts.size() - m_console_max_lines_display + i] );

            rendering_aspect->GetComponent<std::vector<TextRenderingAspectImpl::TextDisplay>>( "console_lines" )->getPurpose().push_back( myline );
        }
    }
    
}

/*
void MainService::create_skybox( void )
{
    DrawSpace::Interface::Module::Root* sbmod_root;

    if( !DrawSpace::Utils::PILoad::LoadModule( "skyboxmod", "skybox", &sbmod_root ) )
    {
        _DSEXCEPTION( "fail to load skyboxmod module root" )
    }
    m_skyboxRender = sbmod_root->InstanciateRenderingAspectImpls( "skyboxRender" );


    RenderingAspect* rendering_aspect = m_skyboxEntity.AddAspect<RenderingAspect>();
    
    rendering_aspect->AddImplementation( m_skyboxRender );

    ////////////// noms des passes

    std::vector<dsstring> skybox_passes;

    skybox_passes.push_back( "texture_pass" );

    rendering_aspect->AddComponent<std::vector<dsstring>>( "skybox_passes", skybox_passes );

    ////////////// jeu de 6 textures par slot pass

    std::vector<Texture*> skybox_textures;
    skybox_textures.push_back( _DRAWSPACE_NEW_( Texture, Texture( "sb0.bmp" ) ) );
    skybox_textures.push_back( _DRAWSPACE_NEW_( Texture, Texture( "sb2.bmp" ) ) );
    skybox_textures.push_back( _DRAWSPACE_NEW_( Texture, Texture( "sb3.bmp" ) ) );
    skybox_textures.push_back( _DRAWSPACE_NEW_( Texture, Texture( "sb1.bmp" ) ) );
    skybox_textures.push_back( _DRAWSPACE_NEW_( Texture, Texture( "sb4.bmp" ) ) );
    skybox_textures.push_back( _DRAWSPACE_NEW_( Texture, Texture( "sb4.bmp" ) ) );

    for( int i = 0; i < 6; i++ )
    {
        skybox_textures[i]->LoadFromFile();    
    }

    rendering_aspect->AddComponent<std::vector<Texture*>>( "skybox_textures", skybox_textures );

    /////////////// les FX pour chaque slot pass

    Fx* skybox_texturepass_fx = _DRAWSPACE_NEW_( Fx, Fx );

    skybox_texturepass_fx->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.vso", true ) ) );
    skybox_texturepass_fx->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.pso", true ) ) );

    skybox_texturepass_fx->GetShader( 0 )->LoadFromFile();
    skybox_texturepass_fx->GetShader( 1 )->LoadFromFile();

    RenderStatesSet skybox_texturepass_rss;
    skybox_texturepass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "false" ) );
    skybox_texturepass_rss.AddRenderStateOut( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "false" ) );

    skybox_texturepass_fx->SetRenderStates( skybox_texturepass_rss );

    rendering_aspect->AddComponent<Fx*>( "skybox_fx", skybox_texturepass_fx );
       
    /////////// params shaders
    
    //std::vector<std::vector<std::pair<dsstring, RenderingNode::ShadersParams>>> skybox_shaders_params;

    std::vector<std::pair<dsstring, RenderingNode::ShadersParams>> skybox_texturepass_shaders_params;

        
    rendering_aspect->AddComponent<std::vector<std::pair<dsstring, RenderingNode::ShadersParams>>>( "skybox_texturepass_shaders_params", skybox_texturepass_shaders_params );

    //////////////// valeur du rendering order pour chaque slot pass
    rendering_aspect->AddComponent<int>( "skybox_ro", -1000 );


    TransformAspect* transform_aspect = m_skyboxEntity.AddAspect<TransformAspect>();

    transform_aspect->SetImplementation( &m_skybox_transformer );

    transform_aspect->AddComponent<Matrix>( "skybox_scaling" );

    transform_aspect->GetComponent<Matrix>( "skybox_scaling" )->getPurpose().Scale( 100.0, 100.0, 100.0 );
        
}

void MainService::create_ground( void )
{
    RenderingAspect* rendering_aspect = m_groundEntity.AddAspect<RenderingAspect>();

    rendering_aspect->AddImplementation( &m_groundRender );

    rendering_aspect->AddComponent<MesheRenderingAspectImpl::PassSlot>( "texturepass_slot", "texture_pass" );

    
    RenderingNode* ground_texturepass = rendering_aspect->GetComponent<MesheRenderingAspectImpl::PassSlot>( "texturepass_slot" )->getPurpose().GetRenderingNode();
    ground_texturepass->SetFx( _DRAWSPACE_NEW_( Fx, Fx ) );

    ground_texturepass->GetFx()->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.vso", true ) ) );
    ground_texturepass->GetFx()->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "texture.pso", true ) ) );

    ground_texturepass->GetFx()->GetShader( 0 )->LoadFromFile();
    ground_texturepass->GetFx()->GetShader( 1 )->LoadFromFile();

    RenderStatesSet ground_texturepass_rss;
    ground_texturepass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "true" ) );
    ground_texturepass_rss.AddRenderStateOut( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "false" ) );

    ground_texturepass->GetFx()->SetRenderStates( ground_texturepass_rss );

    ground_texturepass->SetTexture( _DRAWSPACE_NEW_( Texture, Texture( "002b2su2.jpg" ) ), 0 );
    ground_texturepass->GetTexture( 0 )->LoadFromFile();


    ground_texturepass->SetMeshe( _DRAWSPACE_NEW_( Meshe, Meshe ) );
    ground_texturepass->GetMeshe()->SetImporter( m_meshe_import );
    ground_texturepass->GetMeshe()->LoadFromFile( "water.ac", 0 );

    ground_texturepass->SetOrderNumber( -500 );


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TransformAspect* transform_aspect = m_groundEntity.AddAspect<TransformAspect>();    

    BodyAspect* body_aspect = m_groundEntity.AddAspect<BodyAspect>();
    body_aspect->AddComponent<BodyAspect::BoxCollisionShape>( "shape", Vector( 100.0, 0.0, 100.0, 1.0 ) );

    Matrix ground_attitude;
    
    ground_attitude.Translation( 0.0, -3.0, 0.0 );
    body_aspect->AddComponent<Matrix>( "attitude", ground_attitude );

    body_aspect->AddComponent<BodyAspect::Mode>( "mode", BodyAspect::COLLIDER );

    body_aspect->AddComponent<bool>( "enable", true );
    body_aspect->AddComponent<bool>( "contact_state", false );

    transform_aspect->SetImplementation( body_aspect->GetTransformAspectImpl() );

}

void MainService::create_camera( void )
{
    TransformAspect* transform_aspect = m_cameraEntity.AddAspect<TransformAspect>();

    transform_aspect->SetImplementation( &m_fps_transformer );
    transform_aspect->AddComponent<dsreal>( "yaw", 0.0 );
    transform_aspect->AddComponent<dsreal>( "pitch", 0.0 );

    transform_aspect->AddComponent<Vector>( "speed" );
    transform_aspect->AddComponent<Matrix>( "pos" );

    transform_aspect->GetComponent<Matrix>( "pos" )->getPurpose().Translation( Vector( 0.0, 2.0, 10.0, 1.0 ) );

    transform_aspect->AddComponent<bool>( "ymvt", true );

    CameraAspect* camera_aspect = m_cameraEntity.AddAspect<CameraAspect>();

    camera_aspect->AddComponent<Matrix>( "camera_proj" );

    DrawSpace::Interface::Renderer::Characteristics characteristics;
    m_renderer->GetRenderCharacteristics( characteristics );
    camera_aspect->GetComponent<Matrix>( "camera_proj" )->getPurpose().Perspective( characteristics.width_viewport, characteristics.height_viewport, 1.0, 100000.0 );

}
*/
void MainService::OnKeyPress( long p_key )
{
    if( m_console_active )
    {
        return;
    }

    switch( p_key )
    {
        /*
        case 'Q':
        {
            TransformAspect* transform_aspect = m_cameraEntity.GetAspect<TransformAspect>();
            transform_aspect->GetComponent<Vector>( "speed" )->getPurpose()[2] = 12.0;    
        }
        break;

        case 'W':
        {
            TransformAspect* transform_aspect = m_cameraEntity.GetAspect<TransformAspect>();
            transform_aspect->GetComponent<Vector>( "speed" )->getPurpose()[2] = -12.0;
        }
        break;
        */
    }
}

void MainService::OnEndKeyPress( long p_key )
{
    if( m_console_active )
    {
        return;
    }

    switch( p_key )
    {
        /*
        case 'Q':
        case 'W':
        {
            TransformAspect* transform_aspect = m_cameraEntity.GetAspect<TransformAspect>();
            transform_aspect->GetComponent<Vector>( "speed" )->getPurpose()[2] = 0.0; 
        }
        break;
        */
    }
}

void MainService::OnKeyPulse( long p_key )
{
}

void MainService::OnChar( long p_char, long p_scan )
{
    if( 178 == p_char )
    {
        if( m_console_active )
        {
            m_console_active = false;       
        }
        else
        {
            m_console_active = true;            
        }

        RenderingAspect* rendering_aspect = m_quadEntity.GetAspect<RenderingAspect>();
        RenderingNode* quad_pass = rendering_aspect->GetComponent<QuadRenderingAspectImpl::PassSlot>( "finalpass_slot" )->getPurpose().GetRenderingNode();
        quad_pass->SetDrawingState( m_console_active );
     
        return;
    }

    if( m_console_active )
    {
        if( 8 == p_char )
        {        
            if( m_console_texts[m_console_current_line].size() > 1 )
            {
                char comment[4096];

                strcpy( comment, m_console_texts[m_console_current_line].c_str() );
                comment[m_console_texts[m_console_current_line].size()-1] = 0;

                m_console_texts[m_console_current_line] = comment;
            }        
        }
        else if( 13 == p_char )
        {
            char comment[4096];

            strcpy( comment, m_console_texts[m_console_current_line].c_str() );
            char* cmd = comment;
            cmd++; // sauter le '>' 
            if( strcmp( cmd, "" ) )
            {
                process_console_command( cmd );
            }
            m_console_texts.push_back( ">" );
            m_console_current_line++;    
        }
        else
        {
            m_console_texts[m_console_current_line] += p_char;
        }
    }
}

void MainService::OnMouseMove( long p_xm, long p_ym, long p_dx, long p_dy )
{
    /*
    TransformAspect* transform_aspect = m_cameraEntity.GetAspect<TransformAspect>();

    m_fps_yaw += - p_dx / 1.0;
    m_fps_pitch += - p_dy / 1.0;

    transform_aspect->GetComponent<dsreal>( "yaw" )->getPurpose() = m_fps_yaw.GetValue();
    transform_aspect->GetComponent<dsreal>( "pitch" )->getPurpose() = m_fps_pitch.GetValue();
    */
}

void MainService::OnMouseWheel( long p_delta )
{
}

void MainService::OnMouseLeftButtonDown( long p_xm, long p_ym )
{
}

void MainService::OnMouseLeftButtonUp( long p_xm, long p_ym )
{
}

void MainService::OnMouseRightButtonDown( long p_xm, long p_ym )
{
}

void MainService::OnMouseRightButtonUp( long p_xm, long p_ym )
{
}

void MainService::OnAppEvent( WPARAM p_wParam, LPARAM p_lParam )
{
}

void MainService::process_console_command( const dsstring& p_cmd )
{
    if( false == LuaContext::GetInstance()->Execute( p_cmd ) )
    {
        dsstring lua_err = LuaContext::GetInstance()->GetLastError();
        print_console_line( lua_err );
    }
}

void MainService::print_console_line( const dsstring& p_text )
{
    m_console_texts.push_back( p_text );
    m_console_current_line++;
}


void MainService::set_mouse_circular_mode( bool p_state )
{
    if( m_mousecircularmode_cb )
    {
        (*m_mousecircularmode_cb)( p_state );        
    }
}

void MainService::create_console_quad( void )
{
    RenderingAspect* rendering_aspect = m_quadEntity.AddAspect<RenderingAspect>();
    rendering_aspect->AddImplementation( &m_quadRender );

    rendering_aspect->AddComponent<QuadRenderingAspectImpl::PassSlot>( "finalpass_slot", "final_pass" );
    RenderingNode* quad_pass = rendering_aspect->GetComponent<QuadRenderingAspectImpl::PassSlot>( "finalpass_slot" )->getPurpose().GetRenderingNode();
    quad_pass->SetFx( _DRAWSPACE_NEW_( Fx, Fx ) );

    quad_pass->GetFx()->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "color.vso", true ) ) );
    quad_pass->GetFx()->AddShader( _DRAWSPACE_NEW_( Shader, Shader( "color.pso", true ) ) );

    quad_pass->GetFx()->GetShader( 0 )->LoadFromFile();
    quad_pass->GetFx()->GetShader( 1 )->LoadFromFile();

    quad_pass->AddShaderParameter( 1, "color", 0 );
    quad_pass->SetShaderRealVector( "color", Vector( 0.5, 0.5, 0.5, 0.85 ) );

    RenderStatesSet quadpass_rss;
    quadpass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "false" ) );
    
    quadpass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ALPHABLENDENABLE, "true" ) );
    quadpass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ALPHABLENDOP, "add" ) );
    quadpass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ALPHABLENDFUNC, "always" ) );
    quadpass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ALPHABLENDDEST, "invsrcalpha" ) );
    quadpass_rss.AddRenderStateIn( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ALPHABLENDSRC, "srcalpha" ) );


    quadpass_rss.AddRenderStateOut( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ENABLEZBUFFER, "false" ) );
    quadpass_rss.AddRenderStateOut( DrawSpace::Core::RenderState( DrawSpace::Core::RenderState::ALPHABLENDENABLE, "false" ) );

    quad_pass->GetFx()->SetRenderStates( quadpass_rss );

    quad_pass->SetTexture( _DRAWSPACE_NEW_( Texture, Texture( "mars.jpg" ) ), 0 );
    quad_pass->GetTexture( 0 )->LoadFromFile();

    quad_pass->SetOrderNumber( 11000 );


    TransformAspect* transform_aspect = m_quadEntity.AddAspect<TransformAspect>();

    transform_aspect->SetImplementation( &m_quadTransformer );

    
    transform_aspect->AddComponent<Matrix>( "quad_scaling" );
    transform_aspect->GetComponent<Matrix>( "quad_scaling" )->getPurpose().Scale( 2.0, 2.0, 1.0 );
    

    transform_aspect->AddComponent<Matrix>( "quad_pos" );
    transform_aspect->GetComponent<Matrix>( "quad_pos" )->getPurpose().Translation( 0.0, 0.0, -1.0 );

    quad_pass->SetDrawingState( m_console_active );

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainService::RegisterRenderGraph( const std::string& p_id, LuaClass_RenderPassNodeGraph* p_rg )
{
    m_rendergraphs[p_id] = p_rg;
}

void MainService::RegisterEntityGraph( const std::string& p_id, LuaClass_EntityNodeGraph* p_eg )
{
    m_entitygraphs[p_id] = p_eg;
}

void MainService::RequestClose( void )
{
    (*m_closeapp_cb)( 0 );
}

void MainService::RequestClearConsole( void )
{
    m_console_texts.clear();
    m_console_texts.push_back( "Command input ready" );
    m_console_current_line = 0;
}

void MainService::RequestConsolePrint( const dsstring& p_msg )
{
    print_console_line( p_msg );
}


void MainService::RequestLuaFileExec( const dsstring& p_path )
{
    if( false == LuaContext::GetInstance()->ExecuteFromFile( p_path ) )
    {
        dsstring lua_err = LuaContext::GetInstance()->GetLastError();
        print_console_line( lua_err );
    }
}

void MainService::RequestMemAllocDump( void )
{
    MemAlloc::GetInstance()->DumpContent();
}