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

#ifndef __BasicSceneMainFrame__
#define __BasicSceneMainFrame__


#include "panel.h"
#include "drawspace.h"
#include "scripting.h"
#include "BasicSceneObjectPropertiesDialog.h"

#define DFRONT_ICON_DIM                     22

#define CAMERA_ICON_INDEX                   0
#define CHUNK_ICON_INDEX                    1
#define COLLIDER_ICON_INDEX                 2
#define INERTBODY_ICON_INDEX                3
#define MOVEMENT_ICON_INDEX                 4
#define ORBIT_ICON_INDEX                    5
#define ORBITER_ICON_INDEX                  6
#define PLANET_ICON_INDEX                   7
#define ROCKET_ICON_INDEX                   8
#define SPACEBOX_ICON_INDEX                 9
#define TRANSFO_ICON_INDEX                  10
#define SCENEGRAPH_ICON_INDEX               11
#define DRAWSPACE_ICON_INDEX                12


#define CAMERA_MASK                         1
#define CHUNK_MASK                          2
#define COLLIDER_MASK                       3
#define INERTBODY_MASK                      4
#define MOVEMENT_MASK                       5
#define ORBIT_MASK                          6
#define ORBITER_MASK                        7
#define PLANET_MASK                         8
#define ROCKET_MASK                         9
#define SPACEBOX_MASK                       10
#define TRANSFO_MASK                        11
#define SCENEGRAPH_MASK                     12
#define DRAWSPACE_MASK                      13


#define CONTEXTMENU_NEWSCENENODEGRAPH       2000

#define CONTEXTMENU_NEWSPACEBOX             2010
#define CONTEXTMENU_NEWCHUNK                2011
#define CONTEXTMENU_NEWINERTBODY            2012
#define CONTEXTMENU_NEWCOLLIDER             2013
#define CONTEXTMENU_NEWROCKET               2014
#define CONTEXTMENU_NEWORBIT                2015
#define CONTEXTMENU_NEWORBITER              2016
#define CONTEXTMENU_NEWPLANET               2017
#define CONTEXTMENU_NEWTRANSFO              2018
#define CONTEXTMENU_NEWCAMERA               2019
#define CONTEXTMENU_NEWLINEARMVT            2020
#define CONTEXTMENU_NEWCIRCULARMVT          2021
#define CONTEXTMENU_NEWFPSMVT               2022
#define CONTEXTMENU_NEWFREEMVT              2023
#define CONTEXTMENU_NEWHEADMVT              2024
#define CONTEXTMENU_NEWLONGLATMVT           2025
#define CONTEXTMENU_NEWSPECTATORMVT         2026
#define CONTEXTMENU_SEPARATOR               2080
#define CONTEXTMENU_EDIT                    2081

//////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DIALOG_TEXTURE_PROPS_TITLE          "Texture properties"      


//////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DIALOG_DECLARE( _title_ ) \
    BasicSceneObjectPropertiesDialog* dialog = new BasicSceneObjectPropertiesDialog( this, _title_ ); \
    wxPropertyGrid* propertygrid = dialog->GetPropertyGrid(); \
    dialog->RegisterApplyButtonHandler( m_applybutton_clicked_cb ); \
    dialog->RegisterSpecificButton0Handler( m_specificbutton0_clicked_cb ); \
    dialog->RegisterSpecificButton1Handler( m_specificbutton1_clicked_cb ); \


#define DIALOG_GETGRID wxPropertyGrid* propertygrid = p_dialog->GetPropertyGrid();

#define DIALOG_SHOW dialog->Show(); propertygrid->ResetColumnSizes(); propertygrid->CollapseAll();

#define DIALOG_FINALIZE propertygrid->ResetColumnSizes(); propertygrid->CollapseAll();


#define DIALOG_APPENDROOT_STRING( _label_, _value_ )    propertygrid->Append( new wxStringProperty( _label_, wxPG_LABEL, _value_ ) );
#define DIALOG_APPENDROOT_INTEGER( _label_, _value_ )   propertygrid->Append( new wxIntProperty( _label_, wxPG_LABEL, _value_ ) );
#define DIALOG_APPENDROOT_FLOAT( _label_, _value_ )     propertygrid->Append( new wxFloatProperty( _label_, wxPG_LABEL, _value_ ) );
#define DIALOG_APPENDROOT_ENUM( _label_, _value_ )      propertygrid->Append( new wxEnumProperty( _label_, wxPG_LABEL, _value_ ) );
#define DIALOG_APPENDROOT_BOOL( _label_, _value_ )      propertygrid->Append( new wxBoolProperty( _label_, wxPG_LABEL, _value_ ) );

#define DIALOG_APPENDROOT_NODE( _label_, _var_name_ )   wxPGProperty* _var_name_ = propertygrid->Append( new wxStringProperty( _label_, wxPG_LABEL, "<composed>" ) );


#define DIALOG_APPENDNODE_NODE( _parent_, _label_, _var_name_ ) wxPGProperty* _var_name_ = propertygrid->AppendIn( _parent_, new wxStringProperty( _label_, wxPG_LABEL, "<composed>" ) );

#define DIALOG_APPENDNODE_STRING( _parent_, _label_, _value_ )      propertygrid->AppendIn( _parent_, new wxStringProperty( _label_, wxPG_LABEL, _value_ ) );
#define DIALOG_APPENDNODE_INTEGER( _parent_, _label_, _value_ )     propertygrid->AppendIn( _parent_, new wxIntProperty( _label_, wxPG_LABEL, _value_ ) );
#define DIALOG_APPENDNODE_FLOAT( _parent_, _label_, _value_ )       propertygrid->AppendIn( _parent_, new wxFloatProperty( _label_, wxPG_LABEL, _value_ ) );
#define DIALOG_APPENDNODE_ENUM( _parent_, _label_, _value_ )        propertygrid->AppendIn( _parent_, new wxEnumProperty( _label_, wxPG_LABEL, _value_ ) );
#define DIALOG_APPENDNODE_BOOL( _parent_, _label_, _value_ )        propertygrid->AppendIn( _parent_, new wxBoolProperty( _label_, wxPG_LABEL, _value_ ) );


#define DIALOG_APPENDNODE_ITERATE( _parent_, _value_, _func_, _labels_ ) \
    for( size_t i = 0; i < _labels_.size(); i++ ) \
    { \
        wxString current = _labels_[i]; \
        _func_( _parent_, current, _value_ )\
    } \

#define DIALOG_APPENDNODE_ITERATE_NODE_BEGIN( _parent_, _counter_, _list_, _var_name_ ) \
    for( size_t _counter_ = 0; i < _list_.size(); _counter_++ ) \
    { \
        DIALOG_APPENDNODE_NODE( _parent_, _list_[_counter_], _var_name_ ) \


#define DIALOG_APPENDNODE_ITERATE_NODE_END \
    } \

#define DIALOG_BUILD_LABELS( _count_, _format_, _var_name_ ) \
    wxArrayString _var_name_; \
    for( int i = 0; i < _count_; i++ ) \
    { \
        char comment[128]; \
        sprintf( comment, _format_, i ); \
        wxString curr = comment; \
        _var_name_.Add( curr ); \
    } \

#define DIALOG_APPLY                                            dialog->EnableApplyButton();
#define DIALOG_SPECIFIC0( _text_ )                              dialog->EnableSpecificButton0( _text_ );
#define DIALOG_SPECIFIC1( _text_ )                              dialog->EnableSpecificButton1( _text_ );

#define DIALOG_TITLE                                            p_dialog->GetTitle()

#define DIALOG_SPECIFIC0_LABEL( _format_, _var_name_ ) \
    char comment[128]; \
    sprintf( comment, _format_, p_dialog->GetSpecific0Counter() ); \
    wxString _var_name_ = comment; \

#define DIALOG_SPECIFIC1_LABEL( _format_, _var_name_ ) \
    char comment[128]; \
    sprintf( comment, _format_, p_dialog->GetSpecific1Counter() ); \
    wxString _var_name_ = comment; \


#define DIALOG_PROPERTIES_VARS \
        wxStringProperty* string_prop; \
        wxFloatProperty* float_prop; \
        wxIntProperty* int_prop; \
        wxBoolProperty* bool_prop; \
        wxEnumProperty* enum_prop; \
        wxCharBuffer buffer; \
        wxAny value; \

#define DIALOG_CHECK_PROPERTY( _name_ ) propertygrid->GetProperty( _name_ )

#define DIALOG_GET_STRING_PROPERTY( _name_, _var_name_ ) \
        wxString _var_name_; \
        string_prop = static_cast<wxStringProperty*>( propertygrid->GetProperty( _name_ ) ); \
        value = string_prop->GetValue(); \
        value.GetAs<wxString>( &_var_name_ ); \

#define DIALOG_GET_INT_PROPERTY( _name_, _var_name_ ) \
        int _var_name_; \
        int_prop = static_cast<wxIntProperty*>( propertygrid->GetProperty( _name_ ) ); \
        value = int_prop->GetValue(); \
        value.GetAs<int>( &_var_name_ ); \

#define DIALOG_GET_BOOL_PROPERTY( _name_, _var_name_ ) \
        bool _var_name_; \
        bool_prop = static_cast<wxBoolProperty*>( propertygrid->GetProperty( _name_ ) ); \
        value = bool_prop->GetValue(); \
        value.GetAs<bool>( &_var_name_ );
        
#define DIALOG_GET_FLOAT_PROPERTY( _name_, _var_name_ ) \
        dsreal _var_name_;\
        float_prop = static_cast<wxFloatProperty*>( propertygrid->GetProperty( _name_ ) ); \
        value = float_prop->GetValue(); \
        value.GetAs<dsreal>( &_var_name_ );
        
#define DIALOG_GET_ENUM_PROPERTY( _name_, _var_name_ ) \
        wxString _var_name_; \
        enum_prop = static_cast<wxEnumProperty*>( propertygrid->GetProperty( _name_ ) ); \
        _var_name_ = enum_prop->GetValueAsString(); \

#define DIALOG_WXSTRING_TO_DSSTRING( _src_var_name_, _dst_var_name_ ) \
        dsstring _dst_var_name_; \
        buffer = _src_var_name_.ToAscii(); \
        _dst_var_name_ = buffer.data(); \


#define DIALOG_EXPLORE_NODES_BEGIN( _root_, _format_, _counter_, _curr_label_ ) \
        long _counter_ = 0;\
        while( 1 ) \
        { \
            dsstring _curr_label_; \
            char comment[128]; \
            sprintf( comment, _format_, _counter_++ ); \
            dsstring final; \
            if( _root_ != "" ) \
            { \
                final = _root_ + dsstring( "." ) + comment;\
            } \
            else \
            { \
                final = comment; \
            } \
            if( NULL == DIALOG_CHECK_PROPERTY( final ) )\
            { \
                break;\
            } \
            _curr_label_ = final; \
  

#define DIALOG_EXPLORE_NODES_END \
        } \

#define DIALOG_INCREMENT_STRING( _src_, _delta_ ) \
        dsstring( _src_ + dsstring( "." ) + _delta_ ) \


//////////////////////////////////////////////////////////////////////////////////////////////////////////


class BasicSceneMainFrame : public MainFrame
{
public:

    typedef struct
    {
        int         id;
        dsstring    text;

    } PopupMenuEntry;

    

    typedef enum
    {
        TRANSFORMATIONSOURCE_MATRIXSTACK,
        TRANSFORMATIONSOURCE_MOVEMENT,
        TRANSFORMATIONSOURCE_BODY

    } TransformationSourceType;

    typedef enum
    {
        TRANSFORMATIONMATRIX_IDENTITY,
        TRANSFORMATIONMATRIX_SCALE,
        TRANSFORMATIONMATRIX_TRANSLATION,
        TRANSFORMATIONMATRIX_ROTATION

    } TransformationMatrixOperation;

    typedef struct
    {
        dsstring    var_alias;
        dsreal      value;          // si var_alias = ""

    } TransformationMatrixValueLinkage;

    class TransformationMatrixArg
    {
    public:
        //DrawSpace::Utils::Vector                translation;
        TransformationMatrixValueLinkage        translation_vals_link[3];

        //DrawSpace::Utils::Vector                rotation;
        TransformationMatrixValueLinkage        rotation_vals_link[3];

        //dsreal                                  angle;
        TransformationMatrixValueLinkage        angle_val_link;

        //DrawSpace::Utils::Vector                scale;
        TransformationMatrixValueLinkage        scale_vals_link[3];

     public:
        TransformationMatrixArg( void )
        {
            translation_vals_link[0].value = 0.0;
            translation_vals_link[1].value = 0.0;
            translation_vals_link[2].value = 0.0;
            translation_vals_link[0].var_alias = "...";
            translation_vals_link[1].var_alias = "...";
            translation_vals_link[2].var_alias = "...";

            rotation_vals_link[0].value = 0.0;
            rotation_vals_link[1].value = 0.0;
            rotation_vals_link[2].value = 0.0;
            rotation_vals_link[0].var_alias = "...";
            rotation_vals_link[1].var_alias = "...";
            rotation_vals_link[2].var_alias = "...";

            scale_vals_link[0].value = 0.0;
            scale_vals_link[1].value = 0.0;
            scale_vals_link[2].value = 0.0;
            scale_vals_link[0].var_alias = "...";
            scale_vals_link[1].var_alias = "...";
            scale_vals_link[2].var_alias = "...";

            angle_val_link.value = 0.0;
            angle_val_link.var_alias = "...";
        }
    };

    typedef struct
    {
        TransformationMatrixOperation   ope;
        TransformationMatrixArg         arg;

    } TransformationMatrixDescriptor;
    

    typedef struct
    {
        DrawSpace::Core::TransformNode*                 node;
        TransformationSourceType                        transformation_source_type;

        // les sources de transfo autorisees pour ce node
        bool                                            propose_matrixstack;
        bool                                            propose_movement;
        bool                                            propose_body;
        
        //DrawSpace::Utils::Transformation                matrix_stack;
        std::vector<TransformationMatrixDescriptor>     matrix_stack_descr;
        DrawSpace::Core::Movement*                      movement;
        DrawSpace::Dynamics::Body*                      body;


    } MetadataScenegraphEntry;



    typedef struct
    {
        dsstring                            name;
        DrawSpace::Core::SceneNodeGraph*    scenenodegraph;        
        wxTreeItemId                        treeitemid;

    } SceneNodeGraphEntry;


    typedef struct
    {

        dsstring                                                        name;
        DrawSpace::Core::SceneNode<DrawSpace::Core::Transformation>*    transformation;
        wxTreeItemId                                                    treeitemid;

    } TransformationNodeEntry;




    typedef enum
    {
        MOVEMENTCONTROLSOURCE_KEYBMOUSE,
        MOVEMENTCONTROLSOURCE_REGISTER

    } MovementControlSource;


    typedef struct
    {
        DrawSpace::Core::Movement*  movement;

        MovementControlSource       speed_control_source;
        dsstring                    speed_control_register;

        MovementControlSource       yaw_control_source;
        dsstring                    yaw_control_register;

        MovementControlSource       pitch_control_source;
        dsstring                    pitch_control_register;

        MovementControlSource       roll_control_source;
        dsstring                    roll_control_register;

        dsreal                      theta_pos_mouse;
        dsreal                      phi_pos_mouse;

    } MovementEntry;


    typedef enum
    {
        REGISTER_CONSTANT,
        REGISTER_VARIABLE

    } RegisterMode;

    typedef enum
    {
        REGISTER_VARIABLE_TRANSLATION_SIMPLE,
        REGISTER_VARIABLE_TRANSLATION_ROUNDTRIP,
        REGISTER_VARIABLE_ANGULAR_SIMPLE,
        REGISTER_VARIABLE_ANGULAR_ROUNDTRIP,

    } RegisterVariableMode;


    typedef struct
    {
        long                    id;
        RegisterMode            mode;
        dsreal                  const_value;

        RegisterVariableMode    variable_mode;
        dsreal                  variable_initial_value;
        dsreal                  variable_speed;
        dsreal                  variable_range_sup;
        dsreal                  variable_range_inf;

        bool                    state;

        dsreal                  current_value;

       
        bool                    variable_roundtrip_back;
        bool                    discontinuity;

    } RegisterEntry;


protected:

    typedef DrawSpace::Core::CallBack<BasicSceneMainFrame, void, const dsstring&>                   TimerCallback;
    typedef DrawSpace::Core::CallBack<BasicSceneMainFrame, void, const dsstring&>                   ScriptingErrorCallback;

    typedef DrawSpace::Core::CallBack<BasicSceneMainFrame, void, DrawSpace::Core::PropertyPool&>    ScriptingCallsCallback;


    typedef DrawSpace::Core::CallBack<BasicSceneMainFrame, void, BasicSceneObjectPropertiesDialog*> DialogButtonCallback;


    void on_timer( const dsstring& p_timername );
    void on_scripting_error( const dsstring& p_error );
    void on_scripting_calls( DrawSpace::Core::PropertyPool& p_propertypool );

    void compute_scenegraph_transforms( void );
    void compute_regs( void );
    void compute_movements( void );

    void compute_transformnodes( void );

    bool set_var_alias( const dsstring& p_source, dsstring& p_dest );

    void build_popupmenu( int p_level, wxMenu& p_menu );

    wxArrayString get_passes_list( void );
    wxArrayString get_finalpasses_list( void );
    wxArrayString get_intermediatepasses_list( void );
    wxArrayString get_fx_list( void );
    wxArrayString get_textures_list( void );
    wxArrayString get_shaders_list( void );
    wxArrayString get_meshes_list( void );
    wxArrayString get_fonts_list( void );
    wxArrayString insert_void_choice( const wxArrayString& p_array );



    

    long                                                    m_w_width;
    long                                                    m_w_height;

    bool                                                    m_glready;
    DrawSpace::Utils::TimeManager                           m_timer;
    std::map<dsstring, MovementEntry>                       m_movements;

    DrawSpace::Scenegraph                                   m_scenegraph;
    std::map<dsstring, MetadataScenegraphEntry>             m_metada_scenegraph;


    //////////////////////////////////////////////////////////////////////////////////

    std::map<void*, SceneNodeGraphEntry>                    m_scenenodegraphs;

    std::map<void*, DrawSpace::Core::BaseSceneNode*>        m_tree_nodes;

    std::map<void*, TransformationNodeEntry>                m_transformation_nodes;


    




    //////////////////////////////////////////////////////////////////////////////////

    std::vector<DrawSpace::Core::Configurable*>             m_ordered_configs;
    long                                                    m_scenegraphlistctrl_currentindex;
    long                                                    m_mvtslistctrl_currentindex;
    long                                                    m_cameraslistctrl_currentindex;
    long                                                    m_regslistctrl_currentindex;

    std::map<dsstring, RegisterEntry>                       m_registers;

    TimerCallback*                                          m_timercb;

    wxCoord                                                 m_last_xmouse;
    wxCoord                                                 m_last_ymouse;

    DrawSpace::Dynamics::CameraPoint*                       m_current_camera;

    Scripting*                                              m_scripting;


    ScriptingErrorCallback*                                 m_scripting_error_cb;
    ScriptingCallsCallback*                                 m_scripting_calls_cb;

    bool                                                    m_display_framerate;
    bool                                                    m_display_currentcamera;

    wxFont                                                  m_console_font;

    wxTreeItemId                                            m_scenegraphs_root_item;


    std::map<int, std::vector<PopupMenuEntry>>              m_scenegraphs_masks;

    wxTreeItemId                                            m_last_clicked_treeitem;


    DialogButtonCallback*                                   m_applybutton_clicked_cb;
    DialogButtonCallback*                                   m_specificbutton0_clicked_cb;
    DialogButtonCallback*                                   m_specificbutton1_clicked_cb;



    void on_applybutton_clicked( BasicSceneObjectPropertiesDialog* p_dialog );
    void on_specificbutton0_clicked( BasicSceneObjectPropertiesDialog* p_dialog );
    void on_specificbutton1_clicked( BasicSceneObjectPropertiesDialog* p_dialog );


    virtual void OnClose( wxCloseEvent& p_event );
    virtual void OnIdle( wxIdleEvent& p_event );
	virtual void OnKeyDown( wxKeyEvent& p_event );
	virtual void OnKeyUp( wxKeyEvent& p_event );
	virtual void OnMouseMotion( wxMouseEvent& p_event );
    virtual void OnAssetsListItemActivated( wxListEvent& p_event );
    virtual void OnConfigsListItemActivated( wxListEvent& p_event );
    virtual void OnPassesListItemActivated( wxListEvent& p_event );
    virtual void OnPassesListItemSelected( wxListEvent& p_event );
    virtual void OnShadersListItemActivated( wxListEvent& p_event );
    virtual void OnCreateMvtButtonClicked( wxCommandEvent& p_event );
    virtual void OnMvtsListItemActivated( wxListEvent& p_event );
    virtual void OnMvtsListDeleteAllItems( wxListEvent& p_event );
    virtual void OnMvtsListItemSelected( wxListEvent& p_event );
    virtual void OnCreateCameraButtonClicked( wxCommandEvent& p_event );
    virtual void OnCamerasListItemActivated( wxListEvent& p_event );
    virtual void OnCamerasListDeleteAllItems( wxListEvent& p_event );
    virtual void OnCamerasListItemSelected( wxListEvent& p_event );
    virtual void OnScenegraphItemActivated( wxListEvent& p_event );
    virtual void OnScenegraphListItemSelected( wxListEvent& p_event );
    virtual void OnCreateDrawableButtonClicked( wxCommandEvent& p_event );
    virtual void OnTransfTypeButtonClicked( wxCommandEvent& p_event );
	virtual void OnTransfoEditButtonClicked( wxCommandEvent& p_event );
    virtual void OnScenegraphListItemDeselected( wxListEvent& p_event );
    virtual void OnScenegraphListDeleteItem( wxListEvent& p_event );
    virtual void OnScenegraphListDeleteAllItems( wxListEvent& p_event );

    virtual void OnPopupClick(wxCommandEvent& p_evt);
    virtual void OnSceneNodeGraphsListRightClick( wxTreeEvent& p_event );

	virtual void OnControlButtonClicked( wxCommandEvent& p_event );
    virtual void OnMouseKeyboardOutputCombobox( wxCommandEvent& p_event );
    virtual void OnCreateRegButtonClicked( wxCommandEvent& p_event );
    virtual void OnCameraEditButtonClicked( wxCommandEvent& p_event );
    virtual void OnRegistersListItemActivated( wxListEvent& p_event );
	virtual void OnRegsListDeleteAllItems( wxListEvent& p_event );
	virtual void OnRegsListItemSelected( wxListEvent& p_event );
	virtual void OnModRegButtonClicked( wxCommandEvent& p_event );
    virtual void OnResetRegButtonClicked( wxCommandEvent& p_event );
    virtual void OnRegOnButtonClicked( wxCommandEvent& p_event );
    virtual void OnRegOffButtonClicked( wxCommandEvent& p_event );
    virtual void OnConsoleInSendButtonClicked( wxCommandEvent& p_event );
    virtual void OnConsoleInClearButtonClicked( wxCommandEvent& p_event );
    virtual void OnConsoleInLoadButtonClicked( wxCommandEvent& p_event );


public:
	BasicSceneMainFrame( wxWindow* parent );

    void SetGLReady( void );
    void Update( void );
    wxNotebook* GetNoteBook( void );

    void SetWindowDims( long p_w_width, long p_w_height );


    void ExecStartupScript( const dsstring& p_scriptfilepath );
    void PrintOutputConsole( const dsstring& p_text );

    bool RegisterMovement( const dsstring& p_name, const MovementEntry& p_movement );
};

#endif // __BasicSceneMainFrame__
