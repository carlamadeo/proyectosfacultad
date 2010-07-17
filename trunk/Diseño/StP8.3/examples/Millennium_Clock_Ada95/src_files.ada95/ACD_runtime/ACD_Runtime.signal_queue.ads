with ACD_Runtime.Signals;
package ACD_Runtime.Signal_Queue is

    package Signals renames ACD_Runtime.Signals;
    
    procedure Enqueue (S : Signals.Signal'class; Priority : Signals.Priority_Type);
    
    function Dequeue return Signals.Signal'class;

    function Image return String;

end ACD_Runtime.Signal_Queue;
