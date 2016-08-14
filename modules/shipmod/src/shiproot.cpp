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

#include "shiproot.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Utils;
using namespace DrawSpace::Interface::Module;

ShipRoot::ShipRoot( void )
{
}

ShipRoot::~ShipRoot( void )
{
}

dsstring ShipRoot::GetModuleName( void )
{
    return "ShipMod";
}

dsstring ShipRoot::GetModuleDescr( void )
{
    return "SpaceShip module";
}

std::vector<dsstring> ShipRoot::GetServicesList( void )
{
    std::vector<dsstring> list;
    return list;
}

Service* ShipRoot::InstanciateService( const dsstring& p_id )
{
    return NULL;
}