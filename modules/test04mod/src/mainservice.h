/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2017                        
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

#ifndef _MAINSERVICE_H_
#define _MAINSERVICE_H_

#include "drawspace.h"
#include "serviceaspectimpl.h"

class MainService : public DrawSpace::Interface::AspectImplementations::ServiceAspectImpl
{
protected:

    DrawSpace::Interface::Renderer*                                                 m_renderer;
    dsstring                                                                        m_pluginDescr;

    DrawSpace::Interface::MesheImport*                                              m_meshe_import;

    DrawSpace::RenderGraph::RenderPassNodeGraph                                     m_rendergraph;
    DrawSpace::EntityGraph::EntityNodeGraph                                         m_entitygraph;

    DrawSpace::RenderGraph::RenderPassNode                                          m_texturepass;
    DrawSpace::RenderGraph::RenderPassNode                                          m_finalpass;    

    DrawSpace::Core::Entity                                                         m_rootEntity;
    DrawSpace::EntityGraph::EntityNode                                              m_rootEntityNode;

    DrawSpace::Core::Entity                                                         m_skyboxEntity;
    DrawSpace::EntityGraph::EntityNode                                              m_skyboxEntityNode;


    DrawSpace::Systems::RenderingSystem                                             m_renderingSystem;
    DrawSpace::Systems::TransformSystem                                             m_transformSystem;
    DrawSpace::Systems::PhysicsSystem                                               m_physicsSystem;
    DrawSpace::Systems::TimeSystem                                                  m_timeSystem;

    std::vector<DrawSpace::Interface::System*>                                      m_systems;

    DrawSpace::AspectImplementations::PassesRenderingAspectImpl                     m_passesRender;
    DrawSpace::Interface::AspectImplementations::RenderingAspectImpl*               m_skyboxRender;

    DrawSpace::AspectImplementations::RawTransformAspectImpl                        m_skybox_transformer;
    DrawSpace::AspectImplementations::FPSTransformAspectImpl                        m_fps_transformer;

    DrawSpace::Core::Entity                                                         m_cameraEntity;
    DrawSpace::EntityGraph::EntityNode                                              m_cameraEntityNode;


    void create_skybox( void );

public:
    bool Init( void );
    void Run( void );
    void Release( void );

    void OnKeyPress( long p_key );
    void OnEndKeyPress( long p_key );
    void OnKeyPulse( long p_key );
    void OnChar( long p_char, long p_scan );
    void OnMouseMove( long p_xm, long p_ym, long p_dx, long p_dy );
    void OnMouseWheel( long p_delta );
    void OnMouseLeftButtonDown( long p_xm, long p_ym );
    void OnMouseLeftButtonUp( long p_xm, long p_ym );
    void OnMouseRightButtonDown( long p_xm, long p_ym );
    void OnMouseRightButtonUp( long p_xm, long p_ym );
    void OnAppEvent( WPARAM p_wParam, LPARAM p_lParam );

};

#endif