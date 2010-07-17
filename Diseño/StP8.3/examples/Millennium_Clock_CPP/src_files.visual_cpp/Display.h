#ifndef _Display_h
#define _Display_h

#include "classes.h"
#include "baseclasses.h"

//#ACD# M(UDIF) includes

    // user defined code to be added here ...

//#end ACD#

class Display : public InactiveInstance
{
    public:
        Display();
        virtual ~Display();

    
    //--Attributes------------------------------------------------------------------

    //#ACD# M(UDA) Attributes

        // user defined code to be added here ...

    //#end ACD#


    //--Getters and setters------------------------------------------------------------------
    public:
    
    protected:


    //--Associations----------------------------------------------------------------
    public:
        Clockwork  *_Clockwork;
        ClockBird  *_ClockBird;
        Door  *_Door;


    //--Operations------------------------------------------------------------------
    public:
        void showTime(int hour,int minute); 
        void showBirdCry(char * s); 
        void showDoorState(char * s); 

    //#ACD# M(UDO) Operations

        // user defined code to be added here ...

    //#end ACD#


};


//----------------------------------------------------------------------------------
// Inline operations
//
//----------------------------------------------------------------------------------

#include "Clockwork.h"
#include "ClockBird.h"
#include "Door.h"

#endif
