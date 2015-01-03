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

#ifndef __BasicSceneObjectPropertiesDialog__
#define __BasicSceneObjectPropertiesDialog__


#include "panel.h"
#include "drawspace.h"

class BasicSceneObjectPropertiesDialog : public ObjectPropertiesDialog
{
protected:

    DrawSpace::Core::BaseCallback<void, BasicSceneObjectPropertiesDialog*>*     m_applybutton_handler;
    DrawSpace::Core::BaseCallback<void, BasicSceneObjectPropertiesDialog*>*     m_specificbutton0_handler;
    DrawSpace::Core::BaseCallback<void, BasicSceneObjectPropertiesDialog*>*     m_specificbutton1_handler;
    std::map<dsstring, void*>                                                   m_datas;


    virtual void OnCloseButtonClicked( wxCommandEvent& event );
    virtual void OnApplyButtonClicked( wxCommandEvent& event );
    virtual void OnSpecificButton0Clicked( wxCommandEvent& event );
    virtual void OnSpecificButton1Clicked( wxCommandEvent& event );

public:

    BasicSceneObjectPropertiesDialog( wxWindow* parent, const wxString& title );
    wxPropertyGrid* GetPropertyGrid( void );

    void EnableApplyButton( void );
    void EnableSpecificButton0( const dsstring& p_label );
    void EnableSpecificButton1( const dsstring& p_label );
    void RegisterApplyButtonHandler( DrawSpace::Core::BaseCallback<void, BasicSceneObjectPropertiesDialog*>* p_handler );
    void RegisterSpecificButton0Handler( DrawSpace::Core::BaseCallback<void, BasicSceneObjectPropertiesDialog*>* p_handler );
    void RegisterSpecificButton1Handler( DrawSpace::Core::BaseCallback<void, BasicSceneObjectPropertiesDialog*>* p_handler );

    void SetData( const dsstring& p_id, void* p_data );
    void* GetData( const dsstring& p_id );
};

#endif // __BasicSceneObjectPropertiesDialog__
