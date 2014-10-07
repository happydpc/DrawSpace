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

#include "factory.h"
#include "assetsbase.h"
#include "configsbase.h"
#include "exceptions.h"

DrawSpace::Core::Factory* DrawSpace::Core::Factory::m_instance = NULL;

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Utils;

Factory::Factory( void )
{


}

Factory::~Factory( void )
{

}

DrawSpace::Core::Configurable* Factory::BuildConfigurableFromText( const dsstring& p_text )
{
    return NULL;
}

bool Factory::on_new_line( const dsstring& p_line, long p_line_num, std::vector<dsstring>& p_words )
{
    if( m_capture_asset_props )
    {        
        if( "end_asset" == p_words[0] )
        {
            m_capture_asset_props = false;
            
            // instanciate asset object

            if( m_assets_instanciationfuncs_bytext.count( m_asset_keyword ) > 0 )
            {
                Asset* asset = ( *m_assets_instanciationfuncs_bytext[m_asset_keyword] ) ();

                // call ParseProperties() method
                if( false == asset->ParseProperties( m_asset_properties ) )
                {
                    dsstring asset_parse_error;
                    asset->GetLastError( asset_parse_error );

                    m_lasterror = asset_parse_error;
                    return false;
                }

                // register in Assetsbase

                // check out if an name has been specified for asset
                dsstring assetname;
                asset->GetName( assetname );

                if( "" == assetname )
                {
                    _DSEXCEPTION( "asset has no name, and so cannot be registered in AssetsBase" );
                }

                if( AssetsBase::GetInstance()->AssetIdExists( assetname ) )
                {
                    _DSEXCEPTION( "asset with same id already registered in AssetsBase" );
                }

                AssetsBase::GetInstance()->RegisterAsset( assetname, asset );
            }
            else
            {
                _DSEXCEPTION( dsstring( "no registered instanciation function for the keyword : " ) +  m_asset_keyword );
            }
        }
        else
        {
            m_asset_properties += p_line;
            m_asset_properties += "\n";
        }
    }
    else if( m_capture_config_props )
    {
        if( "end_config" == p_words[0] )
        {
            m_capture_config_props = false;
            if( m_store_config_text_only )
            {
                // register text in Configsbase

                // check out if an name has been specified for config text

                if( ConfigsBase::GetInstance()->ConfigurableTextExists( m_config_name ) )
                {
                    _DSEXCEPTION( "config text with same id already registered in ConfigsBase" );
                }

                /*
                dsstring complete_text;
                complete_text = dsstring( "declare_config " ) + m_config_keyword + dsstring( " " ) + m_config_name + dsstring( "\n" );
                complete_text += m_config_properties + dsstring( "\n" );
                complete_text += dsstring( "end_config" );

                ConfigsBase::GetInstance()->RegisterConfigurableTextDescription( m_config_name, complete_text );
                */

                ConfigsBase::GetInstance()->RegisterConfigurableTextDescription( m_config_name, m_config_keyword, m_config_properties );

            }
            else
            {
                // instanciate config object

                if( m_configs_instanciationfuncs_bytext.count( m_config_keyword ) > 0 )
                {
                    Configurable* config = ( *m_configs_instanciationfuncs_bytext[m_config_keyword] ) ();

                    // call ParseProperties() method
                    if( false == config->ParseProperties( m_config_properties ) )
                    {
                        dsstring config_parse_error;
                        config->GetLastError( config_parse_error );

                        m_lasterror = config_parse_error;
                        return false;
                    }

                    // register in Configsbase

                    // check out if an name has been specified for config instance

                    if( ConfigsBase::GetInstance()->ConfigurableInstanceExists( m_config_name ) )
                    {
                        _DSEXCEPTION( "config instance with same id already registered in ConfigsBase" );
                    }

                    ConfigsBase::GetInstance()->RegisterConfigurableInstance( m_config_name, config );

                }
                else
                {
                    _DSEXCEPTION( dsstring( "no registered instanciation function for the keyword : " ) +  m_config_keyword );
                }
            }
        }
        else
        {
            m_config_properties += p_line;
            m_config_properties += "\n";
        }
    }
    else
    {
        if( "declare_asset_instance" == p_words[0] )
        {
            if( p_words.size() < 2 )
            {
                _PARSER_MISSING_ARG__
                return false;
            }
            m_capture_asset_props = true;

            m_asset_properties = "";
            m_asset_keyword = p_words[1];

        }
        else if( "declare_config_instance" == p_words[0] )
        {
            if( p_words.size() < 3 )
            {
                _PARSER_MISSING_ARG__
                return false;
            }
            m_capture_config_props = false;

            m_config_properties = "";
            m_config_keyword = p_words[1];
            m_config_name = p_words[2];

            m_store_config_text_only = false; 
        }
        else if( "declare_config" == p_words[0] )
        {
            if( p_words.size() < 3 )
            {
                _PARSER_MISSING_ARG__
                return false;
            }
            m_capture_config_props = false;

            m_config_properties = "";
            m_config_keyword = p_words[1];
            m_config_name = p_words[2];

            m_store_config_text_only = true;
        }
        else
        {
            _PARSER_UNEXPECTED_KEYWORD_
            return false;
        }
    }
    return true;
}

bool Factory::ExecuteFromTextFile( const dsstring& p_path )
{
    m_capture_asset_props = false;

    char seps[] = { 0x09, 0x020, 0x00 };
    // run parser
    return Run( p_path, seps );
}

bool Factory::ExecuteFromBinaryFile( const dsstring& p_path )
{
    // TODO
    return false;
}

bool Factory::ExecuteFromArchiveChunk( const DrawSpace::Utils::Archive& p_arc )
{
    // TODO
    return false;
}

bool Factory::ExecuteFromTextChunk( const dsstring& p_text )
{
    m_capture_asset_props = false;
    char seps[] = { 0x09, 0x020, 0x00 };

    return RunOnTextChunk( p_text, seps );
}

void Factory::RegisterAssetInstanciationFuncByText( const dsstring& p_keyword, Asset::InstanciateFunc p_func )
{
    m_assets_instanciationfuncs_bytext[p_keyword] = p_func;
}

void Factory::RegisterAssetInstanciationFuncByArc( short p_magic, Asset::InstanciateFunc p_func )
{
    m_assets_instanciationfuncs_byarc[p_magic] = p_func;
}

void Factory::RegisterConfigInstanciationFuncByText( const dsstring& p_keyword, Configurable::InstanciateFunc p_func )
{
    m_configs_instanciationfuncs_bytext[p_keyword] = p_func;
}

void Factory::RegisterConfigInstanciationFuncByArc( short p_magic, Configurable::InstanciateFunc p_func )
{
    m_configs_instanciationfuncs_byarc[p_magic] = p_func;
}
