#ifndef _Cuckoo_h
#define _Cuckoo_h

#include "classes.h"
#include "ClockBird.h"

//#ACD# M(UDIF) includes

    // user defined code to be added here ...

//#end ACD#

class Cuckoo : public ClockBird
{
    public:
        Cuckoo();
        virtual ~Cuckoo();

    
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
