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

#ifndef _LUACLASS_GLOBALS_H_
#define _LUACLASS_GLOBALS_H_

#include "luna.h"

class LuaClass_Globals
{
public:
	LuaClass_Globals( lua_State* p_L );
	~LuaClass_Globals( void );

    int LUA_quit( lua_State* p_L );
    int LUA_clearconsole( lua_State* p_L );
    int LUA_print( lua_State* p_L );
    int LUA_dofile( lua_State* p_L );
    int LUA_dumpmem( lua_State* p_L );
    int LUA_totalmem( lua_State* p_L );
    int LUA_log(lua_State* p_L);
    
    int LUA_addappruncb( lua_State* p_L );
    int LUA_removeappruncb( lua_State* p_L );

    int LUA_addkeydowncb( lua_State* p_L );
    int LUA_removekeydowncb( lua_State* p_L );

    int LUA_addkeyupcb( lua_State* p_L );
    int LUA_removekeyupcb( lua_State* p_L );

    int LUA_addoncharcb( lua_State* p_L );
    int LUA_removeoncharcb( lua_State* p_L );

    int LUA_addmousemovecb( lua_State* p_L );
    int LUA_removemousemovecb( lua_State* p_L );

    int LUA_addmouseleftbuttondowncb( lua_State* p_L );
    int LUA_removemouseleftbuttondowncb( lua_State* p_L );

    int LUA_addmouseleftbuttonupcb( lua_State* p_L );
    int LUA_removemouseleftbuttonupcb( lua_State* p_L );

    int LUA_addmouserightbuttondowncb( lua_State* p_L );
    int LUA_removemouserightbuttondowncb( lua_State* p_L );

    int LUA_addmouserightbuttonupcb( lua_State* p_L );
    int LUA_removemouserightbuttonupcb( lua_State* p_L );

    int LUA_showmousecursor( lua_State* p_L );
    int LUA_setmousecursorcircularmode( lua_State* p_L );

    int LUA_reset( lua_State* p_L );

    int LUA_sleep( lua_State* p_L );

    int LUA_signalrenderscenebegin( lua_State* p_L );
    int LUA_signalrendersceneend( lua_State* p_L );

    int LUA_settexturesrootpath( lua_State* p_L );
    int LUA_setshadersrootpath( lua_State* p_L );
    int LUA_setmeshesrootpath( lua_State* p_L );
    int LUA_setscriptsrootpath( lua_State* p_L );

    int LUA_setvirtualfs( lua_State* p_L );

    int LUA_dsexception( lua_State* p_L );

    static const char className[];
    static const Luna<LuaClass_Globals>::RegType methods[];
};

#endif
