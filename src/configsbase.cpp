/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2014                                
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

#include "configsbase.h"

DrawSpace::Core::ConfigsBase* DrawSpace::Core::ConfigsBase::m_instance = NULL;

using namespace DrawSpace;
using namespace DrawSpace::Core;

ConfigsBase::ConfigsBase( void ) : m_configinstancereg_handler( NULL )
{
}

ConfigsBase::~ConfigsBase( void )
{
}

void ConfigsBase::RegisterConfigurableInstance( const dsstring& p_id, DrawSpace::Core::Configurable* p_conf )
{
    m_configurables_instances[p_id] = p_conf;
    if( m_configinstancereg_handler )
    {
        (*m_configinstancereg_handler)( p_conf );
    }
}

DrawSpace::Core::Configurable* ConfigsBase::GetConfigurableInstance( const dsstring& p_id )
{
    if( m_configurables_instances.count( p_id ) > 0 )
    {
        return m_configurables_instances[p_id];
    }
    return NULL;
}

void ConfigsBase::RegisterConfigurableInstanceRegistrationHandler( ConfigurableInstanceRegistrationHandler* p_handler )
{
    m_configinstancereg_handler = p_handler;
}

void ConfigsBase::RegisterConfigurableTextDescription( const dsstring& p_id, const dsstring& p_keyword, const dsstring& p_text )
{
    std::pair<dsstring, dsstring> descr( p_keyword, p_text );
    m_configurables_text[p_id] = descr;
}

void ConfigsBase::GetConfigurableText( const dsstring& p_id, dsstring& p_outkeyword, dsstring& p_outtext )
{
    if( m_configurables_text.count( p_id ) > 0 )
    {
        std::pair<dsstring, dsstring> descr = m_configurables_text[p_id];

        p_outtext = descr.second;
        p_outkeyword = descr.first;
    }
}

bool ConfigsBase::ConfigurableInstanceExists( const dsstring& p_id )
{
    if( m_configurables_instances.count( p_id ) > 0 )
    {
        return true;
    }
    return false;
}

bool ConfigsBase::ConfigurableTextExists( const dsstring& p_id )
{
    if( m_configurables_text.count( p_id ) > 0 )
    {
        return true;
    }
    return false;
}