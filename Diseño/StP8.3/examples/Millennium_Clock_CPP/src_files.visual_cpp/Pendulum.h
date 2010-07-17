#ifndef _Pendulum_h
#define _Pendulum_h

#include "classes.h"
#include "baseclasses.h"

//#ACD# M(UDIF) includes

    // user defined code to be added here ...

//#end ACD#

class Pendulum : public ActiveInstance
{
    public:
        Pendulum();
        virtual ~Pendulum();

    
    //--Attributes------------------------------------------------------------------

    //#ACD# M(UDA) Attributes

        // user defined code to be added here ...

    //#end ACD#


    //--Getters and setters------------------------------------------------------------------
    public:
    
    protected:


    //--Associations----------------------------------------------------------------
    public:
        Clockwork  *_clockwork;
        Weight  *_Weight;


    //--Operations------------------------------------------------------------------

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
            NotMoving,
            IsLeft,
            IsRight
        };

        /// Event taker functions /////////////////////////////////////
        void takeEvent (const Pendulum::objEvent&);
        static Pendulum * classEvent (const Pendulum::objEvent&);

    protected:
        /// State action functions ////////////////////////////////////
        void NotMoving_Action(); 
        void IsLeft_Action(); 
        void IsRight_Action();  

};


//----------------------------------------------------------------------------------
// Inline operations
//
//----------------------------------------------------------------------------------

#include "Clockwork.h"
#include "Weight.h"

#endif
