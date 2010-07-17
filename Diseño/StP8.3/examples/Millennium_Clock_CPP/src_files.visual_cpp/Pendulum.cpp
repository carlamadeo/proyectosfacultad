#include <iostream.h>  
#include "Pendulum.h"

//#ACD# M(UDIF) includes

    // user defined code to be added here ...

//#end ACD#



//------------------------------------------------------------------------------
// Constructors and destructors
//
//------------------------------------------------------------------------------
Pendulum::Pendulum() : ActiveInstance(Pendulum::NotMoving)
{
    //#ACD# M(UDCC) Constructor Code

		this->_clockwork = new Clockwork();
		this->_clockwork->takeEvent(Clockwork::objEvent::setTime);

    //#end ACD#
}

Pendulum::~Pendulum()
{
    //#ACD# M(UDDC) Destructor Code

        // user defined code to be added here ...

    //#end ACD#
}



//------------------------------------------------------------------------------
// S T A T E  M A C H I N E  S E C T I O N
//------------------------------------------------------------------------------

//--Class Event-----------------------------------------------------------------
Pendulum * Pendulum::classEvent (const Pendulum::objEvent& ev)
{
    Pendulum * newInstance = 0;

    switch(ev) {
        case Pendulum::move:
            {
                newInstance = new Pendulum;
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
void Pendulum::takeEvent (const Pendulum::objEvent& ev)
{
    switch(currentState) 
    {   
        case Pendulum::NotMoving: 
            switch(ev) { 
                case Pendulum::AutomaticTransition: 
                    break; 
                case Pendulum::move:     
                { 
                        _clockwork->takeEvent(Clockwork::objEvent::tick); 
                        currentState = IsLeft; 
                        IsLeft_Action();  
                } 
                break; 
            
                default: 
                    cout << "illegal event " << ev << " in state " << currentState << endl; 
                    break; 
            
            } // end of switch event on NotMoving 
            break; 
             
        case Pendulum::IsLeft: 
            switch(ev) { 
                case Pendulum::AutomaticTransition: 
                    break; 
                case Pendulum::move:     
                { 
                        _clockwork->takeEvent(Clockwork::objEvent::tick); 
                        currentState = IsRight; 
                        IsRight_Action();  
                } 
                break; 
            
                default: 
                    cout << "illegal event " << ev << " in state " << currentState << endl; 
                    break; 
            
            } // end of switch event on IsLeft 
            break; 
             
        case Pendulum::IsRight: 
            switch(ev) { 
                case Pendulum::AutomaticTransition: 
                    break; 
                case Pendulum::move:     
                { 
                        _clockwork->takeEvent(Clockwork::objEvent::tick); 
                        currentState = IsLeft; 
                        IsLeft_Action();  
                } 
                break; 
            
                default: 
                    cout << "illegal event " << ev << " in state " << currentState << endl; 
                    break; 
            
            } // end of switch event on IsRight 
            break; 
              
        default:
            cout << "illegal state " << currentState <<endl;
            break;

    } // end of switch current state
}

//--Action Procedures-----------------------------------------------------------
//------------------------------------------------------------------------------
//
// State :      NotMoving
//
//------------------------------------------------------------------------------
void Pendulum::NotMoving_Action ()
{
    //#ACD# M(UDAC::UID_98A70153-AC4F-11d4-BEDC-0060971B5EE0) Action Code

        // user defined code to be added here ...

    //#end ACD#
    takeEvent(AutomaticTransition);
}

//------------------------------------------------------------------------------
//
// State :      IsLeft
//
//------------------------------------------------------------------------------
void Pendulum::IsLeft_Action ()
{
    //#ACD# M(UDAC::UID_98A70156-AC4F-11d4-BEDC-0060971B5EE0) Action Code

        // user defined code to be added here ...

    //#end ACD#
    takeEvent(AutomaticTransition);
}

//------------------------------------------------------------------------------
//
// State :      IsRight
//
//------------------------------------------------------------------------------
void Pendulum::IsRight_Action ()
{
    //#ACD# M(UDAC::UID_98A70159-AC4F-11d4-BEDC-0060971B5EE0) Action Code

        // user defined code to be added here ...

    //#end ACD#
    takeEvent(AutomaticTransition);
}

