#include <iostream.h>  
#include "Door.h"

//#ACD# M(UDIF) includes

    // user defined code to be added here ...

//#end ACD#



//------------------------------------------------------------------------------
// Constructors and destructors
//
//------------------------------------------------------------------------------
Door::Door() : doorStatus(0)
{
    //#ACD# M(UDCC) Constructor Code

	this->setdoorStatus(false);

    //#end ACD#
}

Door::~Door()
{
    //#ACD# M(UDDC) Destructor Code

        // user defined code to be added here ...

    //#end ACD#
}


//------------------------------------------------------------------------------
// Operation :      close 
// 
// Description :    
//
//------------------------------------------------------------------------------
void Door::close()      
{
    //#ACD# M(UDOI::UID_378C31F8-544B-11d4-8778-00C04F8DEC45) Operation Implementation

		this->setdoorStatus(false);
		this->_display->showDoorState("\nDoor closed\n\n");

    //#end ACD#
}

//------------------------------------------------------------------------------
// Operation :      open 
// 
// Description :    
//
//------------------------------------------------------------------------------
void Door::open()      
{
    //#ACD# M(UDOI::UID_378C31F7-544B-11d4-8778-00C04F8DEC45) Operation Implementation

		this->setdoorStatus(true);
		this->_display->showDoorState("Door opened\n");

    //#end ACD#
}

//------------------------------------------------------------------------------
// Operation :      setDisplay 
// 
// Description :    
//
//------------------------------------------------------------------------------
void Door::setDisplay(Display *display)      
{
    //#ACD# M(UDOI::UID_3AB1AD9E-6010-11d5-AEEC-00C0DFC00963) Operation Implementation

        this->_display = display;

    //#end ACD#
}


