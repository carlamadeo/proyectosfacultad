#include <iostream.h>  
#include "Clockwork.h"

//#ACD# M(UDIF) includes

    // user defined code to be added here ...

//#end ACD#



//------------------------------------------------------------------------------
// Constructors and destructors
//
//------------------------------------------------------------------------------
Clockwork::Clockwork() : ActiveInstance(Clockwork::NotMoving)
{
    //#ACD# M(UDCC) Constructor Code

	    this->hour = 0;
		this->minute = 0;

		this->_display = new Display();
        this->_door = new Door();
		this->_door->setDisplay(this->_display);
        this->_owl = new Owl();
		this->_owl->setDisplay(this->_display);
		this->_owl->setDoor(this->_door);
        this->_cuckoo = new Cuckoo();
		this->_cuckoo->setDisplay(this->_display);
		this->_cuckoo->setDoor(this->_door);

    //#end ACD#
}

Clockwork::~Clockwork()
{
    //#ACD# M(UDDC) Destructor Code

        // user defined code to be added here ...

    //#end ACD#
}


//------------------------------------------------------------------------------
// Operation :      addMinute 
// 
// Description :    
//
//------------------------------------------------------------------------------
void Clockwork::addMinute(int m)      
{
    //#ACD# M(UDOI::UID_FD39C160-544D-11d4-8778-00C04F8DEC45) Operation Implementation

        if (minute+m >= 60) {
			minute = 0;
            if (hour + 1 == 24){
				hour = 0;
			}else{
				++hour;
			}
		}else{
			minute += m;
        }

    //#end ACD#
}

//------------------------------------------------------------------------------
// Operation :      showTime 
// 
// Description :    
//
//------------------------------------------------------------------------------
void Clockwork::showTime()      
{
    //#ACD# M(UDOI::UID_FD39C161-544D-11d4-8778-00C04F8DEC45) Operation Implementation

        _display->showTime(this->gethour(),this->getminute());

    //#end ACD#
}

//------------------------------------------------------------------------------
// Operation :      solidHourAtDay 
// 
// Description :    
//
//------------------------------------------------------------------------------
int Clockwork::solidHourAtDay()      
{
    int returnValue;

    //#ACD# M(UDOI::UID_FD39C162-544D-11d4-8778-00C04F8DEC45) Operation Implementation

        returnValue = false;
		if(this->getminute()==0 && (this->gethour()>6 && this->gethour()<22) ){
			returnValue = true;
		}

    //#end ACD#
    return returnValue;
}

//------------------------------------------------------------------------------
// Operation :      solidHourAtNight 
// 
// Description :    
//
//------------------------------------------------------------------------------
int Clockwork::solidHourAtNight()      
{
    int returnValue;

    //#ACD# M(UDOI::UID_FD39C163-544D-11d4-8778-00C04F8DEC45) Operation Implementation

        returnValue = false;
		if(this->getminute()==0 && (this->gethour()<=6 || this->gethour()>=22) ){
			returnValue = true;
		}

    //#end ACD#
    return returnValue;
}

//------------------------------------------------------------------------------
// Operation :      notSolidHour 
// 
// Description :    
//
//------------------------------------------------------------------------------
int Clockwork::notSolidHour()      
{
    int returnValue;

    //#ACD# M(UDOI::UID_FD39C164-544D-11d4-8778-00C04F8DEC45) Operation Implementation

        returnValue = false;
		if(this->getminute()!=0){
			returnValue = true;
		}

    //#end ACD#
    return returnValue;
}

//------------------------------------------------------------------------------
// Operation :      driveBird 
// 
// Description :    
//
//------------------------------------------------------------------------------
void Clockwork::driveBird(int birdType)      
{
    //#ACD# M(UDOI::UID_DE187CD0-5FEB-11d5-AEEC-00C0DFC00963) Operation Implementation

		_door->open();
		if(birdType == 0){
			this->_owl->work(this->gethour());
		}else{
			this->_cuckoo->work(this->gethour());
		}
		_door->close();

    //#end ACD#
}


//------------------------------------------------------------------------------
// S T A T E  M A C H I N E  S E C T I O N
//------------------------------------------------------------------------------

//--Class Event-----------------------------------------------------------------
Clockwork * Clockwork::classEvent (const Clockwork::objEvent& ev)
{
    Clockwork * newInstance = 0;

    switch(ev) {
        case Clockwork::setTime:
            {
                newInstance = new Clockwork;
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
void Clockwork::takeEvent (const Clockwork::objEvent& ev)
{
    switch(currentState) 
    {   
        case Clockwork::NotMoving: 
            switch(ev) { 
                case Clockwork::AutomaticTransition: 
                    break; 
                case Clockwork::setTime:     
                { 
                        sethour(13); 
                        setminute(15); 
                        currentState = NotSolidHour; 
                        NotSolidHour_Action();  
                } 
                break; 
            
                default: 
                    cout << "illegal event " << ev << " in state " << currentState << endl; 
                    break; 
            
            } // end of switch event on NotMoving 
            break; 
             
        case Clockwork::SolidHourAtNight: 
            switch(ev) { 
                case Clockwork::AutomaticTransition: 
                    break; 
                case Clockwork::tick:     
                { 
                    if (notSolidHour()) { 
                        showTime(); 
                        addMinute(15); 
                        currentState = NotSolidHour; 
                        NotSolidHour_Action();  
                    } 
                } 
                break; 
            
                default: 
                    cout << "illegal event " << ev << " in state " << currentState << endl; 
                    break; 
            
            } // end of switch event on SolidHourAtNight 
            break; 
             
        case Clockwork::NotSolidHour: 
            switch(ev) { 
                case Clockwork::AutomaticTransition: 
                    break; 
                case Clockwork::tick:     
                { 
                    if (solidHourAtDay()) { 
                        showTime(); 
                        addMinute(15); 
                        driveBird(1); 
                        currentState = SolidHourAtDay; 
                        SolidHourAtDay_Action();  
                    } 
                } 
                { 
                    if (solidHourAtNight()) { 
                        showTime(); 
                        addMinute(15); 
                        driveBird(0); 
                        currentState = SolidHourAtNight; 
                        SolidHourAtNight_Action();  
                    } 
                } 
                { 
                    if (notSolidHour()) { 
                        showTime(); 
                        addMinute(15); 
                        currentState = NotSolidHour; 
                        NotSolidHour_Action();  
                    } 
                } 
                break; 
                default: 
                    cout << "illegal event " << ev << " in state " << currentState << endl; 
                    break; 
            
            } // end of switch event on NotSolidHour 
            break; 
             
        case Clockwork::SolidHourAtDay: 
            switch(ev) { 
                case Clockwork::AutomaticTransition: 
                    break; 
                case Clockwork::tick:     
                { 
                    if (notSolidHour()) { 
                        showTime(); 
                        addMinute(15); 
                        currentState = NotSolidHour; 
                        NotSolidHour_Action();  
                    } 
                } 
                break; 
            
                default: 
                    cout << "illegal event " << ev << " in state " << currentState << endl; 
                    break; 
            
            } // end of switch event on SolidHourAtDay 
            break; 
              
        default:
            cout << "illegal state " << currentState <<endl;
            break;

    } // end of switch current state
}

//--Action Procedures-----------------------------------------------------------
//------------------------------------------------------------------------------
//
// State :      SolidHourAtNight
//
//------------------------------------------------------------------------------
void Clockwork::SolidHourAtNight_Action ()
{
    //#ACD# M(UDAC::UID_98A70130-AC4F-11d4-BEDC-0060971B5EE0) Action Code

        // user defined code to be added here ...

    //#end ACD#
    takeEvent(AutomaticTransition);
}

//------------------------------------------------------------------------------
//
// State :      NotSolidHour
//
//------------------------------------------------------------------------------
void Clockwork::NotSolidHour_Action ()
{
    //#ACD# M(UDAC::UID_98A7012D-AC4F-11d4-BEDC-0060971B5EE0) Action Code

        // user defined code to be added here ...

    //#end ACD#
    takeEvent(AutomaticTransition);
}

//------------------------------------------------------------------------------
//
// State :      SolidHourAtDay
//
//------------------------------------------------------------------------------
void Clockwork::SolidHourAtDay_Action ()
{
    //#ACD# M(UDAC::UID_98A7012A-AC4F-11d4-BEDC-0060971B5EE0) Action Code

        // user defined code to be added here ...

    //#end ACD#
    takeEvent(AutomaticTransition);
}

//------------------------------------------------------------------------------
//
// State :      NotMoving
//
//------------------------------------------------------------------------------
void Clockwork::NotMoving_Action ()
{
    //#ACD# M(UDAC::UID_98A70127-AC4F-11d4-BEDC-0060971B5EE0) Action Code

        // user defined code to be added here ...

    //#end ACD#
    takeEvent(AutomaticTransition);
}

