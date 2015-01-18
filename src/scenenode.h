/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2015                              
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

#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include "drawspace_commons.h"
#include "matrix.h"
#include "vsphere.h"
#include "lod.h"
#include "timemanager.h"


#define SCENENODE_FLAG_DLINKSCAN                            0x01
#define SCENENODE_FLAG_DLINKED                              0x02



namespace DrawSpace
{
namespace Core
{
class SceneNodeGraph;

class BaseSceneNode
{
protected:
    
    dsstring                    m_scenename;
    std::vector<BaseSceneNode*> m_children;
    DrawSpace::Utils::Matrix    m_finaltransform;
    unsigned long               m_flags;
    
public:

    BaseSceneNode( const dsstring& p_scenename ) :
    m_scenename( p_scenename ),
    m_flags( 0 )
    {


    }

    virtual void GetSceneName( dsstring& p_scenename )
    {
        p_scenename = m_scenename;
    }

    virtual void GetFinalTransform( DrawSpace::Utils::Matrix& p_mat )
    {
        p_mat = m_finaltransform;
    }

    virtual void ComputeTransformation( DrawSpace::Utils::TimeManager& p_timemanager ) = 0;
    virtual void OnRegister( SceneNodeGraph* p_scenegraph ) = 0;

    virtual void AddChild( BaseSceneNode* p_node )
    {
        m_children.push_back( p_node );
    }

    virtual void SetFlag( unsigned long p_flag )
    {
        m_flags |= p_flag;
    }

    virtual bool CheckFlag( unsigned long p_flag )
    {
        return ( m_flags & p_flag );
    }
    
    friend class SceneNodeGraph;
    
};

template <typename Base>
class SceneNode : public BaseSceneNode
{
protected:    
    BaseSceneNode*              m_parent;    
    DrawSpace::Utils::Matrix    m_globaltransform;
    Base*                       m_content;
    
public:

    SceneNode( const dsstring& p_scenename ) :
    BaseSceneNode( p_scenename ),
    m_content( NULL ),
    m_parent( NULL )
    {
    }

    virtual ~SceneNode( void )
    {
    }

    virtual void SetContent( Base* p_content )
	{
		m_content = p_content;
	}

    virtual void LinkTo( BaseSceneNode* p_node )
    {
        p_node->AddChild( this );
        m_parent = p_node;
    }

    virtual Base* GetContent( void )
    {
        return m_content;
    }

    virtual void ComputeTransformation( DrawSpace::Utils::TimeManager& p_timemanager )
    {
        if( !m_content )
        {
            return;
        }

        m_content->Update( p_timemanager );

        DrawSpace::Utils::Matrix base_mat;
        m_content->GetBaseTransform( base_mat );
       
        if( m_parent )
        {
            DrawSpace::Utils::Matrix parent_final_transform;
            m_parent->GetFinalTransform( parent_final_transform );

            m_finaltransform = base_mat * parent_final_transform;
        }
        else
        {
            m_finaltransform = base_mat;
        }
        m_content->SetFinalTransform( m_finaltransform );

        m_content->Update2( p_timemanager );

        // Update() children
        for( size_t i = 0; i < m_children.size(); i++ )
        {
            m_children[i]->ComputeTransformation( p_timemanager );
        }
    }

    void OnRegister( SceneNodeGraph* p_scenegraph )
    {
        if( m_content )
        {
            m_content->OnRegister( p_scenegraph, this );
        }
    }

    BaseSceneNode* GetParentNode( void )
    {
        return m_parent;
    }

    friend class SceneNodeGraph;
};

}
}
#endif
