/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2016                              
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

#ifndef _SKYBOXSERVICE_H_
#define _SKYBOXSERVICE_H_

#include "module_service.h"
#include "spacebox.h"

class SkyboxService;


class SceneNodeGraphParam : public DrawSpace::Module::KeySink<DrawSpace::Core::SceneNodeGraph*>
{
protected:
    SkyboxService* m_owner;

public:
    SceneNodeGraphParam( const dsstring& p_id, SkyboxService* p_owner );
    virtual void OnUpdated( DrawSpace::Core::SceneNodeGraph* p_val );
};

class TexturePassParam : public DrawSpace::Module::KeySink<DrawSpace::IntermediatePass*>
{
protected:
    SkyboxService* m_owner;

public:
    TexturePassParam( const dsstring& p_id, SkyboxService* p_owner );
    virtual void OnUpdated( DrawSpace::IntermediatePass* p_val );
};

class TextureMirrorPassParam : public DrawSpace::Module::KeySink<DrawSpace::IntermediatePass*>
{
protected:
    SkyboxService* m_owner;

public:
    TextureMirrorPassParam( const dsstring& p_id, SkyboxService* p_owner );
    virtual void OnUpdated( DrawSpace::IntermediatePass* p_val );
};

class ReflectorNormaleParam : public DrawSpace::Module::KeySink<DrawSpace::Utils::Vector>
{
protected:
    SkyboxService* m_owner;

public:
    ReflectorNormaleParam( const dsstring& p_id, SkyboxService* p_owner );
    virtual void OnUpdated( DrawSpace::Utils::Vector p_val );
};

class SkyboxService : public DrawSpace::Interface::Module::Service
{
protected:

    DrawSpace::Interface::Renderer*         m_renderer;
    DrawSpace::Utils::TimeManager           m_tm;
    dsstring                                m_device;

    SceneNodeGraphParam*                    m_scparam;
    TexturePassParam*                       m_texturepassparam;
    TextureMirrorPassParam*                 m_texturemirrorpassparam;
    ReflectorNormaleParam*                  m_reflectornormaleparam;

    DrawSpace::Core::SceneNodeGraph*        m_scenenodegraph;

    DrawSpace::IntermediatePass*            m_texturepass;
    DrawSpace::IntermediatePass*            m_texturemirrorpass;

    DrawSpace::Spacebox*                    m_spacebox;

public:
    SkyboxService( const dsstring& p_id );
    ~SkyboxService( void );

    virtual void                            GetKeys( std::vector<DrawSpace::Module::KeySinkBase*>& p_keys );
    virtual void                            Init( DrawSpace::Logger::Configuration* p_logconf, DrawSpace::Core::BaseCallback<void, bool>* p_mousecircularmode_cb );
    virtual void                            Run( void );
    virtual void                            Release( void );
    virtual DrawSpace::Core::BaseSceneNode* GetSceneNode( void );

    virtual void                            OnSceneNodeGraphUpdated( DrawSpace::Core::SceneNodeGraph* p_val );
    virtual void                            OnTexturePassUpdate( DrawSpace::IntermediatePass* p_val );
    virtual void                            OnTextureMirrorPassUpdate( DrawSpace::IntermediatePass* p_val );
    virtual void                            OnReflectorNormaleUpdate( const DrawSpace::Utils::Vector& p_normale );


    virtual void                            OnKeyPress( long p_key );
    virtual void                            OnEndKeyPress( long p_key );
    virtual void                            OnKeyPulse( long p_key );
    virtual void                            OnMouseMove( long p_xm, long p_ym, long p_dx, long p_dy );
    virtual void                            OnMouseLeftButtonDown( long p_xm, long p_ym );
    virtual void                            OnMouseLeftButtonUp( long p_xm, long p_ym );
    virtual void                            OnMouseRightButtonDown( long p_xm, long p_ym );
    virtual void                            OnMouseRightButtonUp( long p_xm, long p_ym );
    virtual void                            OnAppEvent( WPARAM p_wParam, LPARAM p_lParam );
};

#endif