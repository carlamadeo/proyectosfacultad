#include <iostream.h>  
#include "Owl.h"

//#ACD# M(UDIF) includes

    // user defined code to be added here ...

//#end ACD#



//------------------------------------------------------------------------------
// Constructors and destructors
//
//------------------------------------------------------------------------------
Owl::Owl() 
{
    //#ACD# M(UDCC) Constructor Code

        // user defined code to be added here ...

    //#end ACD#
}

Owl::~Owl()
{
    //#ACD# M(UDDC) Destructor Code

        // user defined code to be added here ...

    //#end ACD#
}


//------------------------------------------------------------------------------
// Operation :      cry 
// 
// Description :    
//
//------------------------------------------------------------------------------
void Owl::cry()      
{
    //#ACD# M(UDOI::UID_EB902209-601C-11d5-AEEC-00C0DFC00963) Operation Implementation

		this->_display->showBirdCry("Ouhouu ");

    //#end ACD#
}


