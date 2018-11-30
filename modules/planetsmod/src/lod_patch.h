#pragma once

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

#include "vector.h"
#include "quadtree.h"
#include "lod_subpass.h"
#include "pass.h"

//#define NB_LOD_RANGES           10//16

#define NB_LOD_RANGES_MAX       20
#define PATCH_RESOLUTION        15
#define PATCH_HIGH_RESOLUTION   256
#define PATCH_AVG_RESOLUTION    64

namespace LOD
{
struct Config;

class Patch : public SubPass
{
public:
    using SubPassCreationHandler = DrawSpace::Core::BaseCallback2<SubPass::EntryInfos, SubPass*, int>;

    static const int    NorthNeighbour      = 0;
    static const int    SouthNeighbour      = 1;
    static const int    WestNeighbour       = 2;
    static const int    EastNeighbour       = 3;
    static const int    NorthWestNeighbour  = 4;
    static const int    NorthEastNeighbour  = 5;
    static const int    SouthWestNeighbour  = 6;
    static const int    SouthEastNeighbour  = 7;

    static const int    FrontPlanetFace    = 0;
    static const int    RearPlanetFace     = 1;
    static const int    LeftPlanetFace     = 2;
    static const int    RightPlanetFace    = 3;
    static const int    TopPlanetFace      = 4;
    static const int    BottomPlanetFace   = 5;

protected:

    Patch*                                  m_parent;
    Config*                                 m_config;

    dsreal                                  m_sidelength;
    dsreal                                  m_xpos;
    dsreal                                  m_ypos;
    dsreal                                  m_ray;

    dsreal                                  m_global_ref_u1;
    dsreal                                  m_global_ref_v1;
    dsreal                                  m_global_ref_u2;
    dsreal                                  m_global_ref_v2;

    dsreal                                  m_global_u1;
    dsreal                                  m_global_v1;
    dsreal                                  m_global_u2;
    dsreal                                  m_global_v2;


    int                                     m_orientation;
    int                                     m_nodeid;
    DrawSpace::Utils::BaseQuadtreeNode*     m_owner;
    DrawSpace::Utils::BaseQuadtreeNode*     m_neighbours[8];
    int                                     m_lod_level;
    DrawSpace::IntermediatePass*            m_datatexture_pass;
    Patch*                                  m_texture_referent;

    Patch::SubPassCreationHandler*          m_subpasscreation_handler;

    EntryInfos                              m_subpass_entry_infos;
    bool                                    m_subpass_entry_infos_valid;

    bool                                    m_enable_datatexture;

    int                                     m_nbLODRanges;

    /////////////////////////////////////////////////////////////////////////////////////

    DrawSpace::IntermediatePass*            create_data_texture_pass( void );
    void                                    prepare_data_texture( Patch::SubPassCreationHandler* p_handler, int p_subpass_dest, int p_layer_index );
    void                                    destroy_color_texture( void );
    void                                    recurs_update_texture_referent( Patch* p_texture_referent );
    
public:
    Patch( dsreal p_ray, int p_orientation, Patch* p_parent, int p_nodeid, DrawSpace::Utils::BaseQuadtreeNode* p_owner, 
                Patch::SubPassCreationHandler* p_handler, Config* p_config, int p_layer_index, int p_nbLODRanges );

    virtual ~Patch( void );
   
    DrawSpace::Utils::BaseQuadtreeNode* GetNeighbour( int p_id ) const;
    dsreal                              GetSideLength( void ) const ;
    dsreal                              GetUnitSideLenght( void ) const ;
    int                                 GetOrientation( void ) const ;
    dsreal                              GetTriangleSideLength( void ) const;
    void                                GetGlobalRelUVCoords(DrawSpace::Utils::Vector& p_uv) const;
    void                                GetGlobalUVCoords(DrawSpace::Utils::Vector& p_uv) const ;
    void                                GetPos( dsreal& p_xpos, dsreal& p_ypos ) const;
    void                                GetUnitPos( dsreal& p_xpos, dsreal& p_ypos ) const;
    DrawSpace::Utils::BaseQuadtreeNode* GetOwner( void ) const;
    int                                 GetLodLevel( void ) const;
    DrawSpace::Core::Texture*           GetDataTexture( void ) const;
    Patch*                              GetTextureReferent( void ) const;
  
    void                                SetNeighbour(DrawSpace::Utils::BaseQuadtreeNode* p_patch, int p_id);
    void                                ProjectVertex( const DrawSpace::Utils::Vector& p_in, DrawSpace::Utils::Vector& p_out );
    bool                                IsCircleIntersection( dsreal p_centerx, dsreal p_centery, dsreal p_ray );

    virtual void                        SubPassDone( void );

    static void                         GetNormalVector(int p_orientation, DrawSpace::Utils::Vector& p_vector);

    static void                         CubeToSphere( const DrawSpace::Utils::Vector& p_in, DrawSpace::Utils::Vector& p_out );
    static void                         SphereToCube( const DrawSpace::Utils::Vector& p_in, DrawSpace::Utils::Vector& p_out );
    static void                         XYToXYZ( int p_orientation, dsreal p_x, dsreal p_y, DrawSpace::Utils::Vector& p_out );
    
    // change les coords d'un vecteur pour obtenir son equivalent dans le repere face front, ou x et y du repere 2D correspondent
    // bien au x et y du repere 3D
    static void                         ConvertVectorToFrontFaceCoords( int p_orientation, const DrawSpace::Utils::Vector& p_in, DrawSpace::Utils::Vector& p_out );       
};
}

