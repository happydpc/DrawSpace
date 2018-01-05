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

#include "vector.h"

using namespace DrawSpace::Utils;

Vector::Vector( void )
{
    m_vector[0] = 0.0;
    m_vector[1] = 0.0;
    m_vector[2] = 0.0;
    m_vector[3] = 1.0;
}

Vector::Vector( dsreal p_x, dsreal p_y, dsreal p_z, dsreal p_w )
{
    m_vector[0] = p_x;
    m_vector[1] = p_y;
    m_vector[2] = p_z;
    m_vector[3] = p_w;
}

Vector::Vector( dsreal p_x, dsreal p_y )
{
    m_vector[0] = p_x;
    m_vector[1] = p_y;
    m_vector[2] = 0.0;
    m_vector[3] = 1.0;
}

Vector::~Vector( void )
{

}

dsreal* Vector::GetArray( void ) 
{ 
    return m_vector; 
};

dsreal Vector::LengthPow2( void )
{
    return ( ( m_vector[0] * m_vector[0] ) +
             ( m_vector[1] * m_vector[1] ) +
             ( m_vector[2] * m_vector[2] ) );
}

dsreal Vector::Length( void )
{
    return ( std::sqrt( LengthPow2() ) );
}

void Vector::Normalize( void )
{
    double len = Length();
    if( Length() > 0.0 )
    {
        double mag = 1.0 / Length();
        m_vector[0] *= mag;
        m_vector[1] *= mag;
        m_vector[2] *= mag;
    }
}

void Vector::Scale( dsreal p_scale )
{
    m_vector[0] *= p_scale;
    m_vector[1] *= p_scale;
    m_vector[2] *= p_scale;
}

dsreal operator* ( Vector p_vA, Vector p_vB )
{
    return ( ( p_vA[0] * p_vB[0] ) +
             ( p_vA[1] * p_vB[1] ) +
             ( p_vA[2] * p_vB[2] ) );				
}

Vector ProdVec( Vector p_vA, Vector p_vB )
{
    Vector res;

    res[0] = ( p_vA[1] * p_vB[2] ) - ( p_vA[2] * p_vB[1] );
    res[1] = ( p_vA[2] * p_vB[0] ) - ( p_vA[0] * p_vB[2] );
    res[2] = ( p_vA[0] * p_vB[1] ) - ( p_vA[1] * p_vB[0] );
    res[3] = 1.0;

    return res;
}

Vector operator+ ( Vector p_vA, Vector p_vB )
{
    Vector res;

    res[0] = p_vA[0] + p_vB[0];
    res[1] = p_vA[1] + p_vB[1];
    res[2] = p_vA[2] + p_vB[2];
    res[3] = 1.0;

    return res;
}
