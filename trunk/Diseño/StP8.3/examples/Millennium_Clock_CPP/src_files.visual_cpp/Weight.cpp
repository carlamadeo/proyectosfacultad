#include <iostream.h>  
#include "Weight.h"

//#ACD# M(UDIF) includes

    // user defined code to be added here ...

//#end ACD#



//------------------------------------------------------------------------------
// Constructors and destructors
//
//------------------------------------------------------------------------------
Weight::Weight() : ActiveInstance(Weight::hasNoEnergy)
{
    //#ACD# M(UDCC) Constructor Code

		this->setEnergy(0);
		this->_pend = new Pendulum();

    //#end ACD#
}

Weight::~Weight()
{
    //#ACD# M(UDDC) Destructor Code

        // user defined code to be added here ...

    //#end ACD#
}


//------------------------------------------------------------------------------
// Operation :      decrease 
// 
// Description :    
//
//------------------------------------------------------------------------------
int Weight::decrease(int e)      
{
    int returnValue;

    //#ACD# M(UDOI::UID_378C3200-544B-11d4-8778-00C04F8DEC45) Operation Implementation

		if(this->getEnergy()>e){
			this->setEnergy(this->getEnergy()-e);
		}else{
			this->setEnergy(0);
		}
		returnValue = this->getEnergy();

    //#end ACD#
    return returnValue;
}

//------------------------------------------------------------------------------
// Operation :      energize 
// 
// Description :    
//
//------------------------------------------------------------------------------
void Weight::energize(int e)      
{
    //#ACD# M(UDOI::UID_378C31FF-544B-11d4-8778-00C04F8DEC45) Operation Implementation

		this->setEnergy(e);

    //#end ACD#
}

//------------------------------------------------------------------------------
// Operation :      has_energy 
// 
// Description :    
//
//------------------------------------------------------------------------------
int Weight::has_energy(int e)      
{
    int returnValue;

    //#ACD# M(UDOI::UID_066C8556-544E-11d4-8778-00C04F8DEC45) Operation Implementation

    	returnValue = false;
		if(this->getEnergy()>e){
			returnValue = true;
		}

    //#end ACD#
    return returnValue;
}


//------------------------------------------------------------------------------
// S T A T E  M A C H I N E  S E C T I O N
//------------------------------------------------------------------------------

//--Class Event-----------------------------------------------------------------
Weight * Weight::classEvent (const Weight::objEvent& ev)
{
    Weight * newInstance = 0;

    switch(ev) {
        case Weight::raise:
            {
                newInstance = new Weight;
                newInstance->takeEvent(ev);
            }
            break;
        default:
            cout << "illegal create event\n"; 
            break;
    }
    return newInstance;
}

//--Take Event------------------------------------------------------------------
void Weight::takeEvent (const Weight::objEvent& ev)
{
    switch(currentState) 
    {   
        case Weight::hasNoEnergy: 
            switch(ev) { 
                case Weight::AutomaticTransition: 
                    break; 
                case Weight::raise:     
                { 
                    int e = 1000; 
                        energize(e); 
                        currentState = hasEnergy; 
                        hasEnergy_Action();  
                } 
                break; 
            
                default: 
                    cout << "illegal event " << ev << " in state " << currentState << endl; 
                    break; 
            
            } // end of switch event on hasNoEnergy 
            break; 
             
        case Weight::hasEnergy: 
            switch(ev) { 
                case Weight::AutomaticTransition: 
                    break; 
                case Weight::sink:     
                { 
                    int e = 10; 
                    if (has_energy(e)) { 
                        decrease(e); 
                        _pend->takeEvent(Pendulum::objEvent::move); 
                        currentState = hasEnergy; 
                        hasEnergy_Action();  
                    } 
                } 
                break; 
            
                default: 
                    cout << "illegal event " << ev << " in state " << currentState << endl; 
                    break; 
            
            } // end of switch event on hasEnergy 
            break; 
              
        default:
            cout << "illegal state " << currentState <<endl;
            break;

    } // end of switch current state
}

//--Action Procedures-----------------------------------------------------------
//------------------------------------------------------------------------------
//
// State :      hasEnergy
//
//------------------------------------------------------------------------------
void Weight::hasEnergy_Action ()
{
    //#ACD# M(UDAC::UID_98A70172-AC4F-11d4-BEDC-0060971B5EE0) Action Code

		this->takeEvent(sink);

    //#end ACD#
    takeEvent(AutomaticTransition);
}

//------------------------------------------------------------------------------
//
// State :      hasNoEnergy
//
//------------------------------------------------------------------------------
void Weight::hasNoEnergy_Action ()
{
    //#ACD# M(UDAC::UID_98A7016F-AC4F-11d4-BEDC-0060971B5EE0) Action Code

        // user defined code to be added here ...

    //#end ACD#
    takeEvent(AutomaticTransition);
}

