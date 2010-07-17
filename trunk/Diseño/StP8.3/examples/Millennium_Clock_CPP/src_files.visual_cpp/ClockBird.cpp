#include <iostream.h>  
#include "ClockBird.h"

//#ACD# M(UDIF) includes

    // user defined code to be added here ...

//#end ACD#



//------------------------------------------------------------------------------
// Constructors and destructors
//
//------------------------------------------------------------------------------
ClockBird::ClockBird() 
{
    //#ACD# M(UDCC) Constructor Code

        // user defined code to be added here ...

    //#end ACD#
}

ClockBird::~ClockBird()
{
    //#ACD# M(UDDC) Destructor Code

        // user defined code to be added here ...

    //#end ACD#
}


//------------------------------------------------------------------------------
// Operation :      work 
// 
// Description :    
//
//------------------------------------------------------------------------------
void ClockBird::work(int hour)      
{
    //#ACD# M(UDOI::UID_5B03EE55-5F22-11d5-AEEB-00C0DFC00963) Operation Implementation

        int i,limit;
		if(hour>12){
			limit = hour-12;
		}else{
			limit = hour;
		}
		if(limit == 0) limit =12;
		if(this->_door->getdoorStatus()){
			for(i=0;i<limit;i++){
				this->cry();
			}
		}else{
			this->_display->showBirdCry("\nOutsch\n");
		}

    //#end ACD#
}

//------------------------------------------------------------------------------
// Operation :      setDoor 
// 
// Description :    
//
//------------------------------------------------------------------------------
void ClockBird::setDoor(Door *door)      
{
    //#ACD# M(UDOI::UID_3AB1ACEF-6010-11d5-AEEC-00C0DFC00963) Operation Implementation

        this->_door = door;

    //#end ACD#
}

//------------------------------------------------------------------------------
// Operation :      setDisplay 
// 
// Description :    
//
//------------------------------------------------------------------------------
void ClockBird::setDisplay(Display *display)      
{
    //#ACD# M(UDOI::UID_3AB1ADA5-6010-11d5-AEEC-00C0DFC00963) Operation Implementation

        this->_display = display;

    //#end ACD#
}

//------------------------------------------------------------------------------
// Operation :      cry 
// 
// Description :    
//
//------------------------------------------------------------------------------
void ClockBird::cry()      
{
    //#ACD# M(UDOI::UID_378C31FD-544B-11d4-8778-00C04F8DEC45) Operation Implementation

        // user defined code to be added here ...

    //#end ACD#
}

//------------------------------------------------------------------------------
// Operation :      isDoorOpened 
// 
// Description :    
//
//------------------------------------------------------------------------------
int ClockBird::isDoorOpened()      
{
    int returnValue;

    //#ACD# M(UDOI::UID_066C8550-544E-11d4-8778-00C04F8DEC45) Operation Implementation

        returnValue=_door->getdoorStatus();

    //#end ACD#
    return returnValue;
}


