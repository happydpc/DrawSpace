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

#include "luacontext.h"
#include "luaclass_rendering.h"
#include "luaclass_module.h"
#include "luaclass_renderpassnodegraph.h"
#include "luaclass_renderconfig.h"
#include "luaclass_rendercontext.h"
#include "luaclass_texturesset.h"
#include "luaclass_fxparams.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Utils;
using namespace DrawSpace::Aspect;


const char LuaClass_Rendering::className[] = "Rendering";
const Luna<LuaClass_Rendering>::RegType LuaClass_Rendering::methods[] =
{    
    { "instanciate_renderingimpl", &LuaClass_Rendering::LUA_instanciateRenderingImpl },    
    { "trash_renderingimpl", &LuaClass_Rendering::LUA_trashRenderingImpl },
    { "attach_toentity", &LuaClass_Rendering::LUA_attachtoentity },
    { "detach_fromentity", &LuaClass_Rendering::LUA_detachfromentity },
    { "configure", &LuaClass_Rendering::LUA_configure },
    { "release", &LuaClass_Rendering::LUA_release },
    { "register_to_rendering", &LuaClass_Rendering::LUA_registertorendering },
    { "unregister_from_rendering", &LuaClass_Rendering::LUA_unregisterfromrendering },
    { "set_shaderrealvector", &LuaClass_Rendering::LUA_setshaderrealvector },
    
	{ 0, 0 }
};

LuaClass_Rendering::LuaClass_Rendering( lua_State* p_L ):
m_rendering_impl( NULL )
{
}

LuaClass_Rendering::~LuaClass_Rendering( void )
{
}

int LuaClass_Rendering::LUA_instanciateRenderingImpl( lua_State* p_L )
{
    
	int argc = lua_gettop( p_L );
	if( argc < 2 )
	{		
        LUA_ERROR( "Rendering::instanciate_renderingimpl : argument(s) missing" );
	}

    LuaClass_Module* lua_mod = Luna<LuaClass_Module>::check( p_L, 1 );
    dsstring implementation_id = luaL_checkstring(p_L, 2);

    m_rendering_impl = lua_mod->GetModuleRoot()->InstanciateRenderingAspectImpls( implementation_id );
    
    return 0;
}

int LuaClass_Rendering::LUA_trashRenderingImpl( lua_State* p_L )
{
    
	int argc = lua_gettop( p_L );
	if( argc < 1 )
	{		
        LUA_ERROR( "Rendering::trash_renderingimpl : argument(s) missing" );
	}

    LuaClass_Module* lua_mod = Luna<LuaClass_Module>::check( p_L, 1 );

    lua_mod->GetModuleRoot()->TrashRenderingAspectImpls( m_rendering_impl );
    
    return 0;
}

int LuaClass_Rendering::LUA_attachtoentity( lua_State* p_L )
{
    
	int argc = lua_gettop( p_L );
	if( argc < 1 )
	{		
        LUA_ERROR( "Rendering::attach_toentity : argument(s) missing" );
	}

    LuaClass_Entity* lua_ent = Luna<LuaClass_Entity>::check( p_L, 1 );

    DrawSpace::Core::Entity& entity = lua_ent->GetEntity();
    RenderingAspect* rendering_aspect = entity.GetAspect<RenderingAspect>();

    if( NULL == rendering_aspect )
    {
        LUA_ERROR( "Rendering::attach_toentity : entity has no rendering aspect!" );
    }

    m_entity_rendering_aspect = rendering_aspect;
    m_entity = &entity;

    m_entity_rendering_aspect->AddImplementation( m_rendering_impl );
    
    return 0;
}

int LuaClass_Rendering::LUA_detachfromentity( lua_State* p_L )
{
    
    if( NULL == m_entity )
    {
        LUA_ERROR( "Rendering::detach_fromentity : argument(s) missing" );
    }

    LUA_TRY
    {
        m_entity_rendering_aspect->RemoveImplementation( m_rendering_impl );

    } LUA_CATCH;

    m_entity_rendering_aspect = NULL;
    m_entity = NULL;
    
    return 0;
}

int LuaClass_Rendering::LUA_configure( lua_State* p_L )
{
    
	int argc = lua_gettop( p_L );
	if( argc < 1 )
	{		
        LUA_ERROR( "Rendering::configure : argument(s) missing" );
	}
    LuaClass_RenderConfig* lua_renderconfig = Luna<LuaClass_RenderConfig>::check( p_L, 1 );

    if( m_entity_rendering_aspect )
    {
        LUA_TRY
        {
            int rc_list_size = lua_renderconfig->GetRenderContextListSize();

            ////////////// noms des passes

            std::vector<dsstring> passes;            
            for( int i = 0; i < rc_list_size; i++ )
            {
                LuaClass_RenderContext::Data render_context = lua_renderconfig->GetRenderContext( i );

                dsstring pass_name = render_context.passname;
                
                passes.push_back( pass_name );
            }
            m_entity_rendering_aspect->AddComponent<std::vector<dsstring>>( "passes", passes );
            
            ///////////////// jeux de textures pour chaque passes

            for( int i = 0; i < rc_list_size; i++ )
            {
                LuaClass_RenderContext::Data render_context = lua_renderconfig->GetRenderContext( i );
                dsstring pass_name = render_context.passname;

                int textures_set_size = render_context.textures_sets.size();

                ////////////// les N jeux de 32 textures stages
                std::vector<std::array<Texture*,RenderingNode::NbMaxTextures>> textures;

                for( int texture_face_index = 0; texture_face_index < textures_set_size; texture_face_index++ )
                {
                    std::array<Texture*, RenderingNode::NbMaxTextures> textures_set = { NULL };

                    LuaClass_TexturesSet::Data txts_set = render_context.textures_sets[texture_face_index];
                                        
                    for( int texture_stage_index = 0; texture_stage_index < RenderingNode::NbMaxTextures; texture_stage_index++ )
                    {
                        dsstring texture_name = txts_set.textures[texture_stage_index];
                        if( texture_name != "" )
                        {
                            Texture* texture = _DRAWSPACE_NEW_( Texture, Texture( texture_name ) );

                            bool status = texture->LoadFromFile();

                            status = texture->LoadFromFile();
                            if( !status )
                            {                                
                                cleanup_resources( p_L );
                                LUA_ERROR( "Rendering::configure : texture loading operation failed" );
                            }
                            else
                            {
                                textures_set[texture_stage_index] = texture;
                            }
                        }                        
                    }

                    textures.push_back( textures_set );
                }

                dsstring component_name = "renderingimpl_textures/" + pass_name;

                m_entity_rendering_aspect->AddComponent<std::vector<std::array<Texture*,RenderingNode::NbMaxTextures>>>( component_name, textures );
            }

            ////////////////// fx pour chaque passes

            for( int i = 0; i < rc_list_size; i++ )
            {
                LuaClass_RenderContext::Data render_context = lua_renderconfig->GetRenderContext( i );
                dsstring pass_name = render_context.passname;

                if( render_context.fxparams.size() < 1 )
                {
                    cleanup_resources( p_L );
                    LUA_ERROR( "Rendering::configure : missing fx parameters description" );                                   
                }
                LuaClass_FxParams::Data fx_params = render_context.fxparams[0];

                Fx* fx = _DRAWSPACE_NEW_( Fx, Fx );
              
                fx->SetRenderStates( fx_params.rss );

                for( size_t j = 0; j < fx_params.shaders.size(); j++ )
                {
                    std::pair<dsstring,bool> shader_file_infos = fx_params.shaders[j];
                    Shader* shader = _DRAWSPACE_NEW_( Shader, Shader( shader_file_infos.first, shader_file_infos.second ) );

                    bool status = shader->LoadFromFile();

                    if( !status )
                    {
                        cleanup_resources( p_L );
                        LUA_ERROR( "Rendering::configure : shader loading operation failed" );
                    }
                    else
                    {
                        fx->AddShader( shader );
                    }
                }

                dsstring component_name = "renderingimpl_fx/" + pass_name;

                m_entity_rendering_aspect->AddComponent<Fx*>( component_name, fx );
            }

            //////////////// parametres de shaders
            for( int i = 0; i < rc_list_size; i++ )
            {
                std::vector<std::pair<dsstring, RenderingNode::ShadersParams>> texturepass_shaders_params;

                LuaClass_RenderContext::Data render_context = lua_renderconfig->GetRenderContext( i );
                dsstring pass_name = render_context.passname;

                for( size_t j = 0; j < render_context.shaders_params.size(); j++ )
                {
                    LuaClass_RenderContext::NamedShaderParam param = render_context.shaders_params[j];
                    texturepass_shaders_params.push_back( param );
                }

                dsstring component_name = "renderingimpl_shaders_params/" + pass_name;

                m_entity_rendering_aspect->AddComponent<std::vector<std::pair<dsstring, RenderingNode::ShadersParams>>>( component_name, texturepass_shaders_params );
            }

            ///////////////// rendering order

            for( int i = 0; i < rc_list_size; i++ )
            {
                LuaClass_RenderContext::Data render_context = lua_renderconfig->GetRenderContext( i );
                dsstring pass_name = render_context.passname;

                dsstring component_name = "renderingimpl_ro/" + pass_name;
                m_entity_rendering_aspect->AddComponent<int>( component_name, render_context.rendering_order );
            }

        } LUA_CATCH; 
    }
    else
    {
        LUA_ERROR( "Rendering::configure : not attached to an entity" );
    }
    
    return 0;
}

void LuaClass_Rendering::cleanup_resources( lua_State* p_L )
{
    
    if( m_entity_rendering_aspect )
    {
        std::vector<dsstring> passes_list = m_entity_rendering_aspect->GetComponent<std::vector<dsstring>>( "passes" )->getPurpose();

        int rc_list_size = passes_list.size();

        //////////////// textures
        for( int i = 0; i < rc_list_size; i++ )
        {
            dsstring pass_name = passes_list[i];            
            dsstring component_name = "renderingimpl_textures/" + pass_name;

            std::vector<std::array<Texture*,RenderingNode::NbMaxTextures>> textures = m_entity_rendering_aspect->GetComponent<std::vector<std::array<Texture*,RenderingNode::NbMaxTextures>>>( component_name )->getPurpose();

            for( size_t texture_face_index = 0; texture_face_index < textures.size(); texture_face_index++ )
            {
                std::array<Texture*,RenderingNode::NbMaxTextures> texture_set = textures[texture_face_index];
            
                for( size_t texture_stage_index = 0; texture_stage_index < texture_set.size(); texture_stage_index++ )
                {
                    Texture* texture = texture_set[texture_stage_index];
                    if( texture )
                    {
                        _DRAWSPACE_DELETE_( texture );
                    }
                }
            }

            LUA_TRY
            {
                m_entity_rendering_aspect->RemoveComponent<std::vector<std::array<Texture*,RenderingNode::NbMaxTextures>>>( component_name );

            } LUA_CATCH;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        //////////////// fx
        for( int i = 0; i < rc_list_size; i++ )
        {
            dsstring pass_name = passes_list[i];            
            dsstring component_name = "renderingimpl_fx/" + pass_name;

            Fx* fx = m_entity_rendering_aspect->GetComponent<Fx*>( component_name )->getPurpose();

            for( int j = 0; j < fx->GetShadersListSize(); j++ )
            {
                Shader* shader = fx->GetShader( j );
                _DRAWSPACE_DELETE_( shader );
            }
            _DRAWSPACE_DELETE_( fx );

            LUA_TRY
            {
                m_entity_rendering_aspect->RemoveComponent<Fx*>( component_name );

            } LUA_CATCH;
        }
        ///////////////////////////////////////////////////////////////////////////////////////////
        //////////////// args shaders
        for( int i = 0; i < rc_list_size; i++ )
        {
            dsstring pass_name = passes_list[i];            
            dsstring component_name = "renderingimpl_shaders_params/" + pass_name;

            m_entity_rendering_aspect->RemoveComponent<std::vector<std::pair<dsstring, RenderingNode::ShadersParams>>>( component_name );
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        //////////////// rendering orders
        for( int i = 0; i < rc_list_size; i++ )
        {
            dsstring pass_name = passes_list[i];            
            dsstring component_name = "renderingimpl_ro/" + pass_name;

            m_entity_rendering_aspect->RemoveComponent<int>( component_name );
        }

        ///////////////////////////////////////////////////////////////////////////////////////////

        LUA_TRY
        {
            m_entity_rendering_aspect->RemoveComponent<std::vector<dsstring>>( "passes" );

        } LUA_CATCH;

    }
    else
    {
        LUA_ERROR( "Rendering::cleanup_resources : no rendering aspect" );
    }
    
}

int LuaClass_Rendering::LUA_release( lua_State* p_L )
{
    cleanup_resources( p_L );
    return 0;
}

int LuaClass_Rendering::LUA_registertorendering( lua_State* p_L )
{
    
    if( NULL == m_rendering_impl )
    {
        LUA_ERROR( "Rendering::register_to_rendering : no rendering aspect impl created" );
    }

	int argc = lua_gettop( p_L );
	if( argc < 1 )
	{		
        LUA_ERROR( "Rendering::register_to_rendering : argument(s) missing" );
	}

    LuaClass_RenderPassNodeGraph* lua_rg = Luna<LuaClass_RenderPassNodeGraph>::check( p_L, 1 );

    m_rendering_impl->RegisterToRendering( lua_rg->GetRenderGraph() );
    
    return 0;
}

int LuaClass_Rendering::LUA_unregisterfromrendering( lua_State* p_L )
{
    
    if( NULL == m_rendering_impl)
    {
        LUA_ERROR( "Rendering::unregister_from_rendering : no rendering aspect impl created" );
    }

	int argc = lua_gettop( p_L );
	if( argc < 1 )
	{		
        LUA_ERROR( "Rendering::unregister_from_rendering : argument(s) missing" );
	}

    LuaClass_RenderPassNodeGraph* lua_rg = Luna<LuaClass_RenderPassNodeGraph>::check( p_L, 1 );

    m_rendering_impl->UnregisterFromRendering( lua_rg->GetRenderGraph() );
    
    return 0;
}

int LuaClass_Rendering::LUA_setshaderrealvector( lua_State* p_L )
{
    
	int argc = lua_gettop( p_L );
	if( argc < 6 )
	{		
        LUA_ERROR( "Rendering::set_shaderrealvector : argument(s) missing" );
	}

    dsstring pass_id = luaL_checkstring( p_L, 1 );
    dsstring param_id = luaL_checkstring( p_L, 2 );
    dsreal valx = luaL_checknumber( p_L, 3 );
    dsreal valy = luaL_checknumber( p_L, 4 );
    dsreal valz = luaL_checknumber( p_L, 5 );
    dsreal valw = luaL_checknumber( p_L, 6 );

    dsstring component_name = "renderingimpl_shaders_params/" + pass_id;
    
    LUA_TRY
    {
        std::vector<std::pair<dsstring, RenderingNode::ShadersParams>> texturepass_shaders_params = m_entity_rendering_aspect->GetComponent<std::vector<std::pair<dsstring, RenderingNode::ShadersParams>>>( component_name )->getPurpose();
        for( auto it = texturepass_shaders_params.begin(); it != texturepass_shaders_params.end(); ++it )
        {
            if( it->first == param_id )
            {
                it->second.vector = true;
                it->second.param_values[0] = valx;
                it->second.param_values[1] = valy;
                it->second.param_values[2] = valz;
                it->second.param_values[3] = valw;

                m_entity_rendering_aspect->GetComponent<std::vector<std::pair<dsstring, RenderingNode::ShadersParams>>>( component_name )->getPurpose() = texturepass_shaders_params;

                break;
            }
        }

    } LUA_CATCH;
    
    return 0;
}