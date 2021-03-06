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

#include "lod_layer.h"
#include "lod_body.h"
#include "lod_config.h"

using namespace DrawSpace;
using namespace LOD;

Layer::Layer(Config* p_config, Body* p_body, Layer::SubPassCreationHandler* p_handler, int p_index) :
m_config(p_config),
m_body(p_body),
m_handler(p_handler),
m_hot(false),
m_current_lod(-1),
m_patch_update_cb(this, &Layer::on_patchupdate)
{
    m_planetray = 1000.0 * m_config->m_layers_descr[p_index].ray;
    p_body->Initialize();
    p_body->RegisterPatchUpdateHandler(&m_patch_update_cb);
}

Body* Layer::GetBody(void) const
{
    return m_body;
}

bool Layer::GetHostState(void) const
{
    return m_hot;
}

Layer::SubPassCreationHandler* Layer::GetSubPassCreationHandler(void) const
{
    return m_handler;
}

int Layer::GetCurrentLOD(void) const
{
    return m_current_lod;
}

void Layer::SetHotState(bool p_hotstate)
{
    m_hot = p_hotstate;
    m_body->SetHotState(m_hot);
}

void Layer::UpdateRelativeAlt(dsreal p_alt)
{
    m_body->UpdateRelativeAlt(p_alt);
}

void Layer::UpdateInvariantViewerPos(const DrawSpace::Utils::Vector& p_pos)
{
    m_body->UpdateInvariantViewerPos( p_pos );
}

void Layer::UpdateHotPoint(const DrawSpace::Utils::Vector& p_vector)
{
    m_body->UpdateHotPoint( p_vector );
}

void Layer::Compute(void)
{
    m_body->Compute();
}

void Layer::on_patchupdate(Patch* p_patch, int p_patch_lod)
{
    m_current_lod = p_patch_lod;
}
