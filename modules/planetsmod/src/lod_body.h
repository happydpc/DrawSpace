/* -*-LIC_BEGIN-*- */
/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2019                     
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

#pragma once


#include "renderer.h"
#include "lod_face.h"
#include "timemanager.h"

namespace LOD
{
class Face;
struct Config;

class Body
{
public:
    using PatchUpdateHandler = DrawSpace::Core::BaseCallback2<void, Patch*, int>;
    using BodyTimerCb = DrawSpace::Core::CallBack<Body, void, DrawSpace::Utils::Timer*>;

    enum MesheType
    {
        LOWRES_MESHE,
        AVGRES_MESHE,
        HIRES_MESHE,
        LOWRES_SKIRT_MESHE,    
    };

protected:

    Config*                                                                     m_config;

    dsreal                                                                      m_diameter;    
    dsreal                                                                      m_hotpoint_altitud;
    
    Face*                                                                       m_faces[6];

    std::vector<PatchUpdateHandler*>                                            m_patchupdate_handlers;

    int                                                                         m_current_face;

    Patch*                                                                      m_current_patch;

    DrawSpace::Utils::TimeManager*                                              m_timemanager;

    double                                                                      m_relative_alt;

    bool                                                                        m_enable_cdlod;

    DrawSpace::Utils::Vector                                                    m_invariant_viewerpos; //relatif au centre planete

    void check_currentpatch_event( Patch* p_newvalue, int p_currentpatch_lod );

    static void build_meshe( long p_patch_resol, DrawSpace::Core::Meshe* p_meshe_dest, bool p_fastmode, bool p_skirt );

public:

    static DrawSpace::Core::Meshe*                                              m_patch_meshe; //patch terrains
    static DrawSpace::Core::Meshe*                                              m_patch2_meshe; // patch terrains haute resolution
    static DrawSpace::Core::Meshe*                                              m_patch3_meshe; // patch terrains moyenne resolution
    static DrawSpace::Core::Meshe*                                              m_skirt_meshe; //les jupes terrains


    Body( DrawSpace::Utils::TimeManager* p_time, Config* p_config, int p_layer_index,
            Patch::SubPassCreationHandler* p_handler, int p_nbLODRanges );

    virtual ~Body( void );

    DrawSpace::Core::Meshe*     GetPatcheMeshe(void) const;
    Patch*                      GetFaceCurrentLeaf(int p_faceid) const;
    Face*                       GetFace(int p_faceid) const;
    dsreal                      GetHotPointAltitud(void) const;
    int                         GetCurrentFace(void) const;
    void                        GetInvariantViewerPos(DrawSpace::Utils::Vector& p_pos) const;
    dsreal                      GetDiameter(void) const;

    
    void                        Initialize(void);
    void                        Compute( void );
    
    void                        RegisterPatchUpdateHandler( PatchUpdateHandler* p_handler );
    void                        UpdateHotPoint( const DrawSpace::Utils::Vector& p_hotpoint );
    void                        UpdateRelativeAlt(dsreal p_alt);
    void                        UpdateInvariantViewerPos(const DrawSpace::Utils::Vector& p_pos);

    void                        SetHotState( bool p_hotstate );
    
    void                        Reset( void ); // merge all

    static void                 BuildMeshes(void);
};
}
