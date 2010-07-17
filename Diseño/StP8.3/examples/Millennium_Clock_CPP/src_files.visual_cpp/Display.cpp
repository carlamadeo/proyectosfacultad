#include <iostream.h>  
#include "Display.h"

//#ACD# M(UDIF) includes

    // user defined code to be added here ...

//#end ACD#



//------------------------------------------------------------------------------
// Constructors and destructors
//
//------------------------------------------------------------------------------
Display::Display() 
{
    //#ACD# M(UDCC) Constructor Code

        // user defined code to be added here ...

    //#end ACD#
}

Display::~Display()
{
    //#ACD# M(UDDC) Destructor Code

        // user defined code to be added here ...

    //#end ACD#
}


//------------------------------------------------------------------------------
// Operation :      showTime 
// 
// Description :    
//
//------------------------------------------------------------------------------
void Display::showTime(int hour,int minute)      
{
    //#ACD# M(UDOI::UID_066C8551-544E-11d4-8778-00C04F8DEC45) Operation Implementation

        cout << hour << ":" << minute;
        if (minute == 0) {
			cout << "0\n";
        } else {
            cout << " ";
        }

    //#end ACD#
}

//------------------------------------------------------------------------------
// Operation :      showBirdCry 
// 
// Description :    
//
//------------------------------------------------------------------------------
void Display::showBirdCry(char * s)      
{
    //#ACD# M(UDOI::UID_066C8552-544E-11d4-8778-00C04F8DEC45) Operation Implementation

        cout << s;

    //#end ACD#
}

//------------------------------------------------------------------------------
// Operation :      showDoorState 
// 
// Description :    
//
//------------------------------------------------------------------------------
void Display::showDoorState(char * s)      
{
    //#ACD# M(UDOI::UID_066C8553-544E-11d4-8778-00C04F8DEC45) Operation Implementation

        cout << s;

    //#end ACD#
}


