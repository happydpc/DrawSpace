/* -*-LIC_BEGIN-*- */
/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2020                     
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

#include "skyboxrenderingaspectimpl.h"
#include "renderingaspect.h"
#include "transformaspect.h"


using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Aspect;
using namespace DrawSpace::AspectImplementations;
using namespace DrawSpace::RenderGraph;
using namespace DrawSpace::Utils;

#define SB_DIM 0.5

SkyboxRenderingAspectImpl::PassSlot::PassSlot( const dsstring& p_pass_name ) :
    m_pass_name( p_pass_name )
{
    m_renderer = DrawSpace::Core::SingletonPlugin<DrawSpace::Interface::Renderer>::GetInstance()->m_interface;

    Vertex v1, v2, v3, v4;

    for( long i = 0; i < 6; i++ )
    {
        m_meshes[i] = _DRAWSPACE_NEW_( Core::Meshe, Core::Meshe );
        switch( i )
        {
            case FrontQuad:

                v1.x = -SB_DIM;
                v1.y = SB_DIM;
                v1.z = -SB_DIM;
                v1.tu[0] = 0.0;
                v1.tv[0] = 0.0;

                v2.x = SB_DIM;
                v2.y = SB_DIM;
                v2.z = -SB_DIM;
                v2.tu[0] = 1.0;
                v2.tv[0] = 0.0;

                v3.x = SB_DIM;
                v3.y = -SB_DIM;
                v3.z = -SB_DIM;
                v3.tu[0] = 1.0;
                v3.tv[0] = 1.0;

                v4.x = -SB_DIM;
                v4.y = -SB_DIM;
                v4.z = -SB_DIM;
                v4.tu[0] = 0.0;
                v4.tv[0] = 1.0;

                m_meshes[i]->SetPath( "Skybox FrontQuad" );

                break;

            case RearQuad:

                v1.x = SB_DIM;
                v1.y = SB_DIM;
                v1.z = SB_DIM;
                v1.tu[0] = 0.0;
                v1.tv[0] = 0.0;

                v2.x = -SB_DIM;
                v2.y = SB_DIM;
                v2.z = SB_DIM;
                v2.tu[0] = 1.0;
                v2.tv[0] = 0.0;

                v3.x = -SB_DIM;
                v3.y = -SB_DIM;
                v3.z = SB_DIM;
                v3.tu[0] = 1.0;
                v3.tv[0] = 1.0;

                v4.x = SB_DIM;
                v4.y = -SB_DIM;
                v4.z = SB_DIM;
                v4.tu[0] = 0.0;
                v4.tv[0] = 1.0;

                m_meshes[i]->SetPath( "Skybox RearQuad" );
                break;

            case LeftQuad:

                v1.x = -SB_DIM;
                v1.y = SB_DIM;
                v1.z = SB_DIM;
                v1.tu[0] = 0.0;
                v1.tv[0] = 0.0;

                v2.x = -SB_DIM;
                v2.y = SB_DIM;
                v2.z = -SB_DIM;
                v2.tu[0] = 1.0;
                v2.tv[0] = 0.0;

                v3.x = -SB_DIM;
                v3.y = -SB_DIM;
                v3.z = -SB_DIM;
                v3.tu[0] = 1.0;
                v3.tv[0] = 1.0;

                v4.x = -SB_DIM;
                v4.y = -SB_DIM;
                v4.z = SB_DIM;
                v4.tu[0] = 0.0;
                v4.tv[0] = 1.0;

                m_meshes[i]->SetPath( "Skybox LeftQuad" );
                break;

            case RightQuad:

                v1.x = SB_DIM;
                v1.y = SB_DIM;
                v1.z = -SB_DIM;
                v1.tu[0] = 0.0;
                v1.tv[0] = 0.0;

                v2.x = SB_DIM;
                v2.y = SB_DIM;
                v2.z = SB_DIM;
                v2.tu[0] = 1.0;
                v2.tv[0] = 0.0;

                v3.x = SB_DIM;
                v3.y = -SB_DIM;
                v3.z = SB_DIM;
                v3.tu[0] = 1.0;
                v3.tv[0] = 1.0;

                v4.x = SB_DIM;
                v4.y = -SB_DIM;
                v4.z = -SB_DIM;
                v4.tu[0] = 0.0;
                v4.tv[0] = 1.0;

                m_meshes[i]->SetPath( "Skybox RightQuad" );
                break;

            case TopQuad:

                v1.x = -SB_DIM;
                v1.y = SB_DIM;
                v1.z = SB_DIM;
                v1.tu[0] = 0.0;
                v1.tv[0] = 0.0;

                v2.x = SB_DIM;
                v2.y = SB_DIM;
                v2.z = SB_DIM;
                v2.tu[0] = 1.0;
                v2.tv[0] = 0.0;

                v3.x = SB_DIM;
                v3.y = SB_DIM;
                v3.z = -SB_DIM;
                v3.tu[0] = 1.0;
                v3.tv[0] = 1.0;

                v4.x = -SB_DIM;
                v4.y = SB_DIM;
                v4.z = -SB_DIM;
                v4.tu[0] = 0.0;
                v4.tv[0] = 1.0;

                m_meshes[i]->SetPath( "Skybox TopQuad" );
                break;

            case BottomQuad:

                v1.x = -SB_DIM;
                v1.y = -SB_DIM;
                v1.z = -SB_DIM;
                v1.tu[0] = 0.0;
                v1.tv[0] = 0.0;

                v2.x = SB_DIM;
                v2.y = -SB_DIM;
                v2.z = -SB_DIM;
                v2.tu[0] = 1.0;
                v2.tv[0] = 0.0;

                v3.x = SB_DIM;
                v3.y = -SB_DIM;
                v3.z = SB_DIM;
                v3.tu[0] = 1.0;
                v3.tv[0] = 1.0;

                v4.x = -SB_DIM;
                v4.y = -SB_DIM;
                v4.z = SB_DIM;
                v4.tu[0] = 0.0;
                v4.tv[0] = 1.0;

                m_meshes[i]->SetPath( "Skybox BottomQuad" );
                break;
        }

        m_meshes[i]->AddVertex( v1 );
        m_meshes[i]->AddVertex( v2 );
        m_meshes[i]->AddVertex( v3 );
        m_meshes[i]->AddVertex( v4 );

        m_meshes[i]->AddTriangle( Triangle( 0, 3, 1 ) );
        m_meshes[i]->AddTriangle( Triangle( 1, 3, 2 ) );
    }

    
    m_cb = _DRAWSPACE_NEW_( RenderingNodeDrawCallback, RenderingNodeDrawCallback( this, &PassSlot::on_renderingnode_draw ) );

    for( long i = 0; i < 6; i++ )
    {
        m_rendering_node[i] = _DRAWSPACE_NEW_( RenderingNode, RenderingNode );
        m_rendering_node[i]->RegisterHandler( m_cb );
        m_rendering_node[i]->SetMeshe( m_meshes[i] );

        m_rendering_node[i]->m_debug_id = "skybox_node";
    }

    m_world.Identity();
    
    m_view.Identity();
    m_proj.Identity();
}

SkyboxRenderingAspectImpl::PassSlot::~PassSlot( void )
{    
    for( long i = 0; i < 6; i++ )
    {
        _DRAWSPACE_DELETE_( m_rendering_node[i] );
        _DRAWSPACE_DELETE_( m_meshes[i] );
    }
    _DRAWSPACE_DELETE_( m_cb );
}       

void SkyboxRenderingAspectImpl::PassSlot::on_renderingnode_draw( RenderingNode* p_rendering_node )
{
    m_renderer->DrawMeshe( m_world, m_view, m_proj );
}

SkyboxRenderingAspectImpl::SkyboxRenderingAspectImpl( void )
{
}


bool SkyboxRenderingAspectImpl::VisitRenderPassDescr( const dsstring& p_name, DrawSpace::Core::RenderingQueue* p_passqueue )
{
    bool updated_queue = false;

    for( size_t i = 0; i < m_pass_slots.size(); i++ )
    {
        if( m_pass_slots[i]->m_pass_name == p_name )
        {
            if( m_add_in_rendergraph )
            {
                // ajout du renderingnode dans la renderingqueue  

                for( int j = 0; j < 6; j++ )
                {
                    p_passqueue->Add( m_pass_slots[i]->m_rendering_node[j] );
                }
            }
            else
            {
                // suppression du renderingnode de la renderingqueue

                for( int j = 0; j < 6; j++ )
                {
                    p_passqueue->Remove( m_pass_slots[i]->m_rendering_node[j] );
                }
            }
            updated_queue = true;
        }
    }


    return updated_queue;
}

void SkyboxRenderingAspectImpl::RegisterToRendering( DrawSpace::RenderGraph::RenderPassNodeGraph& p_rendergraph )
{
    init_rendering_objects();

    m_add_in_rendergraph = true;
    p_rendergraph.Accept( this );
}

void SkyboxRenderingAspectImpl::UnregisterFromRendering( DrawSpace::RenderGraph::RenderPassNodeGraph& p_rendergraph )
{
    release_rendering_objects();

    m_add_in_rendergraph = false;
    p_rendergraph.Accept( this );
}

void SkyboxRenderingAspectImpl::Run( DrawSpace::Core::Entity* p_entity )
{
    TransformAspect* transform_aspect = p_entity->GetAspect<TransformAspect>();

    if( transform_aspect )
    {
        Matrix world;        
        transform_aspect->GetWorldTransform( world );
        world.ClearTranslation();

        Matrix view;
        transform_aspect->GetViewTransform( view );
        view.ClearTranslation();

        Matrix proj;
        transform_aspect->GetProjTransform( proj ); 

        for( size_t i = 0; i < m_pass_slots.size(); i++ )
        {
            m_pass_slots[i]->m_world = world;
            m_pass_slots[i]->m_view = view;
            m_pass_slots[i]->m_proj = proj;
        }
    }

    update_shader_params();

    ////////////////////////////////////////////////////////
}

void SkyboxRenderingAspectImpl::init_rendering_objects( void )
{
    std::vector<std::vector<dsstring>> passes_names_layers = m_owner->GetComponent<std::vector<std::vector<dsstring>>>("passes")->getPurpose();

    // il n'ya qu'un seul layer de rendu dans ce module
    std::vector<dsstring> passes_names = passes_names_layers[0];

    std::vector<std::vector<std::vector<std::array<Texture*, RenderingNode::NbMaxTextures>>>> layers_textures = m_owner->GetComponent<std::vector<std::vector<std::vector<std::array<Texture*, RenderingNode::NbMaxTextures>>>>>("layers_textures")->getPurpose();

    // il n'ya qu'un seul layer de rendu dans ce module
    std::vector<std::vector<std::array<Texture*, RenderingNode::NbMaxTextures>>> passes_textures = layers_textures[0];

    std::vector<std::vector<Fx*>> layers_fx = m_owner->GetComponent<std::vector<std::vector<Fx*>>>("layers_fx")->getPurpose();

    // il n'ya qu'un seul layer de rendu dans ce module
    std::vector<Fx*> fxs = layers_fx[0];

    std::vector<std::vector<int>> layers_ro = m_owner->GetComponent<std::vector<std::vector<int>>>("layers_ro")->getPurpose();

    // il n'ya qu'un seul layer de rendu dans ce module
    std::vector<int> ros = layers_ro[0];


    for (size_t i = 0; i < passes_names.size(); i++)
    {
        dsstring pass_name;
        pass_name = passes_names[i];

        std::vector<std::array<Texture*, RenderingNode::NbMaxTextures>> textures = passes_textures[i];

        PassSlot* pass_slot = _DRAWSPACE_NEW_(PassSlot, PassSlot(pass_name));
        for (size_t j = 0; j < 6; j++)
        {
            pass_slot->GetRenderingNode(j)->SetOrderNumber(ros[i]);
            pass_slot->GetRenderingNode(j)->SetFx(fxs[i]);

            std::array<Texture*, RenderingNode::NbMaxTextures> textures_set = textures[j];

            for (size_t k = 0; k < RenderingNode::NbMaxTextures; k++)
            {
                pass_slot->GetRenderingNode(j)->SetTexture(textures_set[k], k);
            }
        }
        m_pass_slots.push_back(pass_slot);
    }
    
    update_shader_params();   
}

void SkyboxRenderingAspectImpl::release_rendering_objects( void )
{
    for( size_t i = 0; i < m_pass_slots.size(); i++ )
    {
        _DRAWSPACE_DELETE_( m_pass_slots[i] );
    }
    m_pass_slots.clear();
}

void SkyboxRenderingAspectImpl::update_shader_params( void ) // for all passes
{
    ////////////////////////////////////////////////////////
    //recup des params shaders

    std::vector<std::vector<std::vector<std::pair<dsstring, RenderingNode::ShadersParams>>>> layers_shaders_params = 
        m_owner->GetComponent< std::vector<std::vector<std::vector<std::pair<dsstring, RenderingNode::ShadersParams>>>>>( "layers_shaders_params" )->getPurpose();

    // il n'y a qu'un seul layer de rendu dans ce module
    std::vector<std::vector<std::pair<dsstring, RenderingNode::ShadersParams>>> shaders_params_passes = layers_shaders_params[0];

    for( size_t i = 0; i < m_pass_slots.size(); i++ )
    {
        // pour chaque passe
        PassSlot* curr_pass = m_pass_slots[i];

        std::vector<std::pair<dsstring, RenderingNode::ShadersParams>> shaders_params = shaders_params_passes[i];

        for( size_t k = 0; k < shaders_params.size(); k++ )
        {
            std::pair<dsstring, RenderingNode::ShadersParams> shader_params_pair = shaders_params[k];

            curr_pass->GetRenderingNode( PassSlot::FrontQuad )->UpdateShaderParams( shader_params_pair.first, shader_params_pair.second );
            curr_pass->GetRenderingNode( PassSlot::RearQuad )->UpdateShaderParams( shader_params_pair.first, shader_params_pair.second );
            curr_pass->GetRenderingNode( PassSlot::LeftQuad )->UpdateShaderParams( shader_params_pair.first, shader_params_pair.second );
            curr_pass->GetRenderingNode( PassSlot::RightQuad )->UpdateShaderParams( shader_params_pair.first, shader_params_pair.second );
            curr_pass->GetRenderingNode( PassSlot::TopQuad )->UpdateShaderParams( shader_params_pair.first, shader_params_pair.second );
            curr_pass->GetRenderingNode( PassSlot::BottomQuad )->UpdateShaderParams( shader_params_pair.first, shader_params_pair.second );
        }
    }
}
