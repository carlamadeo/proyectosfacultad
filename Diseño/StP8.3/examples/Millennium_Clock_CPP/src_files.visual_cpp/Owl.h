#ifndef _Owl_h
#define _Owl_h

#include "classes.h"
#include "ClockBird.h"

//#ACD# M(UDIF) includes

    // user defined code to be added here ...

//#end ACD#

class Owl : public ClockBird
{
    public:
        Owl();
        virtual ~Owl();

    
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


    //--Operations------------------------------------------------------------------
    private:
        void cry(); 

    //#ACD# M(UDO) Operations

        // user defined code to be added here ...

    //#end ACD#


};


//----------------------------------------------------------------------------------
// Inline operations
//
//----------------------------------------------------------------------------------

#include "Clockwork.h"

#endif
