#ifndef _Clockwork_h
#define _Clockwork_h

#include "classes.h"
#include "baseclasses.h"

//#ACD# M(UDIF) includes

    // user defined code to be added here ...

//#end ACD#

class Clockwork : public ActiveInstance
{
    public:
        Clockwork();
        virtual ~Clockwork();

    
    //--Attributes------------------------------------------------------------------
    private:
        int hour;
        int minute;

    //#ACD# M(UDA) Attributes

        // user defined code to be added here ...

    //#end ACD#


    //--Getters and setters------------------------------------------------------------------
    public:
        inline int gethour() const;
        inline int getminute() const;
    
    protected:
        inline void sethour (int const &);
        inline void setminute (int const &);


    //--Associations----------------------------------------------------------------
    public:
        Door  *_door;
        Owl  *_owl;
        Pendulum  *_Pendulum;
        Cuckoo  *_cuckoo;
        Display  *_display;


    //--Operations------------------------------------------------------------------
    private:
        void addMinute(int m); 
        void showTime(); 
        int solidHourAtDay(); 
        int solidHourAtNight(); 
        int notSolidHour(); 
        void driveBird(int birdType); 

    //#ACD# M(UDO) Operations

        // user defined code to be added here ...

    //#end ACD#


    // --StateMachine Code ----------------------------------------------------
    public:
        enum objEvent {
            move,
            sink,
            raise,
            tick,
            setTime,
            AutomaticTransition
        };

        enum objStates {
            SolidHourAtNight,
            NotSolidHour,
            SolidHourAtDay,
            NotMoving
        };

        /// Event taker functions /////////////////////////////////////
        void takeEvent (const Clockwork::objEvent&);
        static Clockwork * classEvent (const Clockwork::objEvent&);

    protected:
        /// State action functions ////////////////////////////////////
        void SolidHourAtNight_Action(); 
        void NotSolidHour_Action(); 
        void SolidHourAtDay_Action(); 
        void NotMoving_Action();  

};


//----------------------------------------------------------------------------------
// Inline operations
//
//----------------------------------------------------------------------------------
//--Getters and Setters-------------------------------------------------------------
inline int Clockwork::gethour() const
{
    //#ACD# M(UDGA::UID_378C31F4-544B-11d4-8778-00C04F8DEC45) Get Accessor

        // user defined code to be added here ...

    //#end ACD#
    return hour ;
}

inline void Clockwork::sethour(int const &a)
{
    hour = a;

    //#ACD# M(UDSA::UID_378C31F4-544B-11d4-8778-00C04F8DEC45) Set Accessor

        // user defined code to be added here ...

    //#end ACD#
}
inline int Clockwork::getminute() const
{
    //#ACD# M(UDGA::UID_378C31F3-544B-11d4-8778-00C04F8DEC45) Get Accessor

        // user defined code to be added here ...

    //#end ACD#
    return minute ;
}

inline void Clockwork::setminute(int const &a)
{
    minute = a;

    //#ACD# M(UDSA::UID_378C31F3-544B-11d4-8778-00C04F8DEC45) Set Accessor

        // user defined code to be added here ...

    //#end ACD#
}

#include "Door.h"
#include "Owl.h"
#include "Pendulum.h"
#include "Cuckoo.h"
#include "Display.h"

#endif
