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

#include "planetdetailsbinder.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Utils;

PlanetDetailsBinder::PlanetDetailsBinder( dsreal p_planetRay, dsreal p_atmoThickness ) :
m_planet_node( NULL),
m_ocean_details_alt( 1.0010 ),
m_terrain_bump_factor( TERRAIN_BUMP_FACTOR )
{

	m_mirror_mode = false;

	m_skyfromspace_ESun = 8.7; //9.0;
	m_skyfromatmo_ESun = 70.0; //50.0;
	m_groundfromspace_ESun = 24.0;
	m_groundfromatmo_ESun = 12.0;

	m_innerRadius = p_planetRay;
	m_outerRadius = p_planetRay + p_atmoThickness;

	m_waveLength[0] = 0.650;
	m_waveLength[1] = 0.570;
	m_waveLength[2] = 0.475;
	m_kr = 0.0025;
	m_km = 0.0010;
	m_scaleDepth = 0.25;

	m_atmo_scattering_flags0[0] = m_outerRadius;
	m_atmo_scattering_flags0[1] = m_innerRadius;
	m_atmo_scattering_flags0[2] = m_outerRadius * m_outerRadius;
	m_atmo_scattering_flags0[3] = m_innerRadius * m_innerRadius;

	m_atmo_scattering_flags1[0] = m_scaleDepth;
	m_atmo_scattering_flags1[1] = 1.0 / m_scaleDepth;
	m_atmo_scattering_flags1[2] = 1.0 / (m_outerRadius - m_innerRadius);
	m_atmo_scattering_flags1[3] = m_atmo_scattering_flags1[2] / m_scaleDepth;

	m_atmo_scattering_flags2[0] = 1.0 / pow(m_waveLength[0], 4.0);
	m_atmo_scattering_flags2[1] = 1.0 / pow(m_waveLength[1], 4.0);
	m_atmo_scattering_flags2[2] = 1.0 / pow(m_waveLength[2], 4.0);

	m_atmo_scattering_flags3[0] = m_kr;
	m_atmo_scattering_flags3[1] = m_km;
	m_atmo_scattering_flags3[2] = 4.0 * m_kr * 3.1415927;
	m_atmo_scattering_flags3[3] = 4.0 * m_km * 3.1415927;

	m_atmo_scattering_flags4[0] = m_skyfromspace_ESun;
	m_atmo_scattering_flags4[1] = m_skyfromatmo_ESun;
	m_atmo_scattering_flags4[2] = m_groundfromspace_ESun;
	m_atmo_scattering_flags4[3] = m_groundfromatmo_ESun;

	m_atmo_scattering_flags5[0] = ATMO_SCATTERING_SPACE_GROUND_LIMIT; // altitude limite de transition entre xxxfromspace_atmo_scattering et xxxfromatmo_atmo_scattering
	m_atmo_scattering_flags5[1] = FOG_ALT_LIMIT; // altitude debut d'apparition du fog "sol"
	m_atmo_scattering_flags5[2] = FOG_DENSITY; // intensite fog "sol"
	m_atmo_scattering_flags5[3] = ATMO_SCATTERING_ALPHA_ALT_VIEWER; // altitude limite pour prise en compte facteur altitude camera pour le calcul de l'alpha

	// couleurs fog "sol"    
	m_atmo_scattering_flags6[0] = 0.45;
	m_atmo_scattering_flags6[1] = 0.63;
	m_atmo_scattering_flags6[2] = 0.78;
	m_atmo_scattering_flags6[3] = 1.0;


	m_lights[0].m_enable = true;
	m_lights[0].m_color[0] = 1.0;
	m_lights[0].m_color[1] = 0.99;
	m_lights[0].m_color[2] = 0.99;
	m_lights[0].m_color[3] = 1.0;
	m_lights[0].m_dir[0] = -0.6;
	m_lights[0].m_dir[1] = 0.0;
	m_lights[0].m_dir[2] = 0.6; // 1.0
	m_lights[0].m_dir[3] = 1.0;

	m_lights[0].m_dir.Normalize();

	m_lights[1].m_enable = false;
	m_lights[1].m_color[0] = 1.0;
	m_lights[1].m_color[1] = 0.9;
	m_lights[1].m_color[2] = 0.9;
	m_lights[1].m_color[3] = 1.0;
	m_lights[1].m_dir[0] = -1.0;
	m_lights[1].m_dir[1] = 0.2;
	m_lights[1].m_dir[2] = 0.0;
	m_lights[1].m_dir[3] = 1.0;

	m_lights[1].m_dir.Normalize();

	m_lights[2].m_enable = false;
	m_lights[2].m_color[0] = 1.0;
	m_lights[2].m_color[1] = 0.5;
	m_lights[2].m_color[2] = 0.5;
	m_lights[2].m_color[3] = 1.0;
	m_lights[2].m_dir[0] = 0.0;
	m_lights[2].m_dir[1] = -1.0;
	m_lights[2].m_dir[2] = 0.0;
	m_lights[2].m_dir[3] = 1.0;

	m_lights[2].m_dir.Normalize();

	m_ambient = false;
	m_ambient_color[0] = 0.1;
	m_ambient_color[1] = 0.1;
	m_ambient_color[2] = 0.1;
	m_ambient_color[3] = 1.0;

}

void PlanetDetailsBinder::Bind( void )
{

	Vector flags6(16.0, 1.095, 1.0040, m_ocean_details_alt);

	Vector flags_lights;
	flags_lights[0] = (m_ambient ? 1.0 : 0.0);
	flags_lights[1] = (m_lights[0].m_enable ? 1.0 : 0.0);
	flags_lights[2] = (m_lights[1].m_enable ? 1.0 : 0.0);
	flags_lights[3] = (m_lights[2].m_enable ? 1.0 : 0.0);


	m_renderer->SetFxShaderParams(0, 32, m_atmo_scattering_flags0);
	m_renderer->SetFxShaderParams(0, 33, m_atmo_scattering_flags1);
	m_renderer->SetFxShaderParams(0, 34, m_atmo_scattering_flags2);
	m_renderer->SetFxShaderParams(0, 35, m_atmo_scattering_flags3);
	m_renderer->SetFxShaderParams(0, 36, m_atmo_scattering_flags4);
	m_renderer->SetFxShaderParams(0, 37, m_atmo_scattering_flags5);
	m_renderer->SetFxShaderParams(0, 38, m_atmo_scattering_flags6);

	m_renderer->SetFxShaderParams(0, 40, flags_lights);
	m_renderer->SetFxShaderParams(0, 41, m_ambient_color);

	m_renderer->SetFxShaderParams(0, 42, m_lights[0].m_local_dir);
	m_renderer->SetFxShaderParams(0, 43, m_lights[0].m_dir);
	m_renderer->SetFxShaderParams(0, 44, m_lights[0].m_color);

	m_renderer->SetFxShaderParams(0, 45, m_lights[1].m_local_dir);
	m_renderer->SetFxShaderParams(0, 46, m_lights[1].m_dir);
	m_renderer->SetFxShaderParams(0, 47, m_lights[1].m_color);

	m_renderer->SetFxShaderParams(0, 48, m_lights[2].m_local_dir);
	m_renderer->SetFxShaderParams(0, 49, m_lights[2].m_dir);
	m_renderer->SetFxShaderParams(0, 50, m_lights[2].m_color);

	Vector mirror_flag;

	mirror_flag[0] = (m_mirror_mode ? 1.0 : 0.0);
	mirror_flag[1] = m_innerRadius;

	m_renderer->SetFxShaderParams(0, 51, mirror_flag);

	m_renderer->SetFxShaderParams(1, 6, flags6);

	m_renderer->SetFxShaderParams(1, 7, flags_lights);
	m_renderer->SetFxShaderParams(1, 8, m_ambient_color);

	m_renderer->SetFxShaderParams(1, 9, m_lights[0].m_local_dir);
	m_renderer->SetFxShaderParams(1, 10, m_lights[0].m_dir);
	m_renderer->SetFxShaderParams(1, 11, m_lights[0].m_color);

	m_renderer->SetFxShaderParams(1, 12, m_lights[1].m_local_dir);
	m_renderer->SetFxShaderParams(1, 13, m_lights[1].m_dir);
	m_renderer->SetFxShaderParams(1, 14, m_lights[1].m_color);

	m_renderer->SetFxShaderParams(1, 15, m_lights[2].m_local_dir);
	m_renderer->SetFxShaderParams(1, 16, m_lights[2].m_dir);
	m_renderer->SetFxShaderParams(1, 17, m_lights[2].m_color);

	m_renderer->SetFxShaderParams(1, 18, m_atmo_scattering_flags0);
	m_renderer->SetFxShaderParams(1, 19, m_atmo_scattering_flags1);
	m_renderer->SetFxShaderParams(1, 20, m_atmo_scattering_flags2);
	m_renderer->SetFxShaderParams(1, 21, m_atmo_scattering_flags3);
	m_renderer->SetFxShaderParams(1, 22, m_atmo_scattering_flags4);
	m_renderer->SetFxShaderParams(1, 23, m_atmo_scattering_flags5);
	m_renderer->SetFxShaderParams(1, 24, m_atmo_scattering_flags6);

	m_planet_final_transform_rots.Transpose(); // faire comme dans le plugin
	m_renderer->SetFxShaderMatrix(1, 25, m_planet_final_transform_rots);


	Vector terrain_bump_flag(m_terrain_bump_factor, 0.0, 0.0, 0.0);
	m_renderer->SetFxShaderParams(1, 31, terrain_bump_flag);

	MultiFractalBinder::Bind();
}

void PlanetDetailsBinder::Unbind( void )
{
	MultiFractalBinder::Unbind();
}

void PlanetDetailsBinder::SetPlanetNode( DrawSpace::Core::SceneNode<DrawSpace::SphericalLOD::Root>* p_planet_node )
{
	m_planet_node = p_planet_node;
}

void PlanetDetailsBinder::Update( void )
{
	Matrix planet_final_transform;

	m_planet_node->GetFinalTransform(planet_final_transform);

	Vector planet_pos;
	planet_pos[0] = - planet_final_transform(3, 0);
	planet_pos[1] = - planet_final_transform(3, 1);
	planet_pos[2] = - planet_final_transform(3, 2);
	planet_pos[3] = 1.0;

	planet_pos.Normalize();


	m_lights[0].m_dir.Normalize();

	planet_final_transform.ClearTranslation();
	m_planet_final_transform_rots = planet_final_transform;



	planet_final_transform.Transpose();

	for (long i = 0; i < 3; i++)
	{
		if (m_lights[i].m_enable)
		{
			planet_final_transform.Transform( &m_lights[i].m_dir, &m_lights[i].m_local_dir );

			m_lights[i].m_local_dir.Normalize();
		}
	}
}