#ifndef _ClockBird_h
#define _ClockBird_h

#include "classes.h"
#include "baseclasses.h"

//#ACD# M(UDIF) includes

    // user defined code to be added here ...

//#end ACD#

class ClockBird : public InactiveInstance
{
    public:
        ClockBird();
        virtual ~ClockBird();

    
    //--Attributes------------------------------------------------------------------

    //#ACD# M(UDA) Attributes

        // user defined code to be added here ...

    //#end ACD#


    //--Getters and setters------------------------------------------------------------------
    public:
    
    protected:


    //--Associations----------------------------------------------------------------
    public:
        Door  *_door;
        Display  *_display;


    //--Operations------------------------------------------------------------------
    public:
        void work(int hour); 
        void setDoor(Door *door); 
        void setDisplay(Display *display); 
    protected:
        virtual void cry(); 
    private:
        int isDoorOpened(); 

    //#ACD# M(UDO) Operations

        // user defined code to be added here ...

    //#end ACD#


};


//----------------------------------------------------------------------------------
// Inline operations
//
//----------------------------------------------------------------------------------

#include "Door.h"
#include "Display.h"

#endif
