#ifndef _Weight_h
#define _Weight_h

#include "classes.h"
#include "baseclasses.h"

//#ACD# M(UDIF) includes

    // user defined code to be added here ...

//#end ACD#

class Weight : public ActiveInstance
{
    public:
        Weight();
        virtual ~Weight();

    
    //--Attributes------------------------------------------------------------------
    private:
        int Energy;

    //#ACD# M(UDA) Attributes

        // user defined code to be added here ...

    //#end ACD#


    //--Getters and setters------------------------------------------------------------------
    public:
        inline int getEnergy() const;
    
    protected:
        inline void setEnergy (int const &);


    //--Associations----------------------------------------------------------------
    public:
        Pendulum  *_pend;


    //--Operations------------------------------------------------------------------
    private:
        int decrease(int e); 
        void energize(int e); 
        int has_energy(int e); 

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
            hasEnergy,
            hasNoEnergy
        };

        /// Event taker functions /////////////////////////////////////
        void takeEvent (const Weight::objEvent&);
        static Weight * classEvent (const Weight::objEvent&);

    protected:
        /// State action functions ////////////////////////////////////
        void hasEnergy_Action(); 
        void hasNoEnergy_Action();  

};


//----------------------------------------------------------------------------------
// Inline operations
//
//----------------------------------------------------------------------------------
//--Getters and Setters-------------------------------------------------------------
inline int Weight::getEnergy() const
{
    //#ACD# M(UDGA::UID_066C8554-544E-11d4-8778-00C04F8DEC45) Get Accessor

        // user defined code to be added here ...

    //#end ACD#
    return Energy ;
}

inline void Weight::setEnergy(int const &a)
{
    Energy = a;

    //#ACD# M(UDSA::UID_066C8554-544E-11d4-8778-00C04F8DEC45) Set Accessor

        // user defined code to be added here ...

    //#end ACD#
}

#include "Pendulum.h"

#endif
