with ACD_Runtime.Signal_Queue;
package body ACD_Runtime.Queued_Signals is
    
    procedure Send (S : Queued_Signal'class; Priority : Signals.Priority_Type := Signals.Normal) is
    begin
       ACD_Runtime.Signal_Queue.Enqueue (S, Priority);
    end Send;
    
    procedure Dispatch_Signals is
       S : Queued_Signal'class := Queued_Signal'class (Signal_Queue.Dequeue);
    begin
       Dispatch (S);
    end Dispatch_Signals;
        
end ACD_Runtime.Queued_Signals;
