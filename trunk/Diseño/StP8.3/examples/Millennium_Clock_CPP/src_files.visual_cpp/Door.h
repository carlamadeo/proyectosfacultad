#ifndef _Door_h
#define _Door_h

#include "classes.h"
#include "baseclasses.h"

//#ACD# M(UDIF) includes

    // user defined code to be added here ...

//#end ACD#

class Door : public InactiveInstance
{
    public:
        Door();
        virtual ~Door();

    
    //--Attributes------------------------------------------------------------------
    private:
        int doorStatus;

    //#ACD# M(UDA) Attributes

        // user defined code to be added here ...

    //#end ACD#


    //--Getters and setters------------------------------------------------------------------
    public:
        inline int getdoorStatus() const;
    
    protected:
        inline void setdoorStatus (int const &);


    //--Associations----------------------------------------------------------------
    public:
        Clockwork  *_Clockwork;
        ClockBird  *_ClockBird;
        Display  *_display;


    //--Operations------------------------------------------------------------------
    public:
        void close(); 
        void open(); 
        void setDisplay(Display *display); 

    //#ACD# M(UDO) Operations

        // user defined code to be added here ...

    //#end ACD#


};


//----------------------------------------------------------------------------------
// Inline operations
//
//----------------------------------------------------------------------------------
//--Getters and Setters-------------------------------------------------------------
inline int Door::getdoorStatus() const
{
    //#ACD# M(UDGA::UID_378C3201-544B-11d4-8778-00C04F8DEC45) Get Accessor

        // user defined code to be added here ...

    //#end ACD#
    return doorStatus ;
}

inline void Door::setdoorStatus(int const &a)
{
    doorStatus = a;

    //#ACD# M(UDSA::UID_378C3201-544B-11d4-8778-00C04F8DEC45) Set Accessor

        // user defined code to be added here ...

    //#end ACD#
}

#include "Clockwork.h"
#include "ClockBird.h"
#include "Display.h"

#endif
