with ACD_Runtime.Signals;
package ACD_Runtime.Queued_Signals is

    package Signals renames ACD_Runtime.Signals;
    
    type Queued_Signal is abstract new Signals.Signal with private;
    
    procedure Send (S : Queued_Signal'class; Priority : Signals.Priority_Type := Signals.Normal);

    procedure Dispatch (S : Queued_Signal) is abstract;
    
    procedure Dispatch_Signals;

    function Image (S : Queued_Signal) return String is abstract;

    
private
    
    type Queued_Signal is abstract new Signals.Signal with null record;

end ACD_Runtime.Queued_Signals;
