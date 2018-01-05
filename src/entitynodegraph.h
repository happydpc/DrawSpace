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

#ifndef _ENTITYNODEGRAPH_H_
#define _ENTITYNODEGRAPH_H_

#include "entitynode.h"
namespace DrawSpace
{

namespace Interface
{
class System;
}

namespace EntityGraph
{
class EntityNodeGraph sealed
{
public:
	using EntityTree = st_tree::tree<Core::Entity*>;
 
private:
	mutable EntityTree                                          m_tree;
    std::vector<EntityNode::EventsHandler*>                     m_nodesevt_handlers;

    std::map<Core::Entity*, EntityNode::EntityTree::node_type*> m_entity_to_node;

public:
	EntityNodeGraph(void);
	~EntityNodeGraph(void);

	EntityNode SetRoot( Core::Entity* p_entity );
	void Erase(void);
    
    void AcceptSystemLeafToRoot( DrawSpace::Interface::System* p_system );
    void AcceptSystemRootToLeaf( DrawSpace::Interface::System* p_system );

    void RegisterNodesEvtHandler( EntityNode::EventsHandler* p_handler );

    void GetEntityAncestorsList( Core::Entity* p_entity, std::vector<Core::Entity*>& p_ancestors ) const;

    virtual void OnSceneRenderBegin( void );
    virtual void OnSceneRenderEnd( void );
    
    friend class EntityNode;
};
}
}

#endif
