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

#include "mediator.h"

using namespace DrawSpace;
using namespace DrawSpace::Core;
using namespace DrawSpace::Utils;

Mediator* Mediator::m_instance = NULL;


Mediator::Mediator( void ) : m_nb_handles( 0 )
{
}


Mediator::~Mediator( void )
{

}

/*
Mediator::Event* Mediator::CreateEvent( const dsstring& p_eventname )
{
    Event evt;
    evt.system_event = ::CreateEventA( NULL, TRUE, FALSE, p_eventname.c_str() );
    evt.args = _DRAWSPACE_NEW_( PropertyPool, PropertyPool );
    evt.name = p_eventname;

    m_events_by_name[p_eventname] = evt;
    m_events_by_handle[evt.system_event] = evt;

    m_handles[m_nb_handles] = evt.system_event;
    m_nb_handles++;

    return &m_events_by_name[p_eventname];
}

void Mediator::Notify( const dsstring& p_eventname )
{
    if( m_events_by_name.count( p_eventname ) > 0 )
    {
        SetEvent( m_events_by_name[p_eventname].system_event );
    }
}

Mediator::Event* Mediator::Wait( void )
{
    DWORD wait = WaitForMultipleObjects( m_nb_handles, m_handles, false, INFINITE );
    if( WAIT_FAILED == wait || WAIT_TIMEOUT == wait )
    {
        return NULL;
    }
    wait -= WAIT_OBJECT_0;

    ResetEvent( m_events_by_handle[m_handles[wait]].system_event );

    return &m_events_by_handle[m_handles[wait]];
}


PropertyPool* Mediator::GetEventPropertyPool( const dsstring& p_eventname )
{
    if( m_events_by_name.count( p_eventname ) > 0 )
    {
        return m_events_by_name[p_eventname].args;
    }
    return NULL;
}

*/

Mediator::MessageQueue* Mediator::CreateMessageQueue( const dsstring& p_messagequeueid )
{
    HANDLE evt = ::CreateEventA( NULL, TRUE, FALSE, p_messagequeueid.c_str() );
    Mediator::MessageQueue* mq = _DRAWSPACE_NEW_( Mediator::MessageQueue, Mediator::MessageQueue( p_messagequeueid, evt ) );

    m_messages_by_name[p_messagequeueid] = mq;
    m_messages_by_handle[evt] = mq;

    m_handles[m_nb_handles] = evt;
    m_nb_handles++;

    return mq;
}

Mediator::MessageQueue* Mediator::Wait( void )
{
    DWORD wait = WaitForMultipleObjects( m_nb_handles, m_handles, false, INFINITE );
    if( WAIT_FAILED == wait || WAIT_TIMEOUT == wait )
    {
        return NULL;
    }
    wait -= WAIT_OBJECT_0;

    ResetEvent( m_messages_by_handle[m_handles[wait]]->m_system_event );

    return m_messages_by_handle[m_handles[wait]];
}
