with ACD_Runtime.Signal_Queue;
with System;
package body Clockwork_pkg.Clockwork_Signals is

    Dots : constant string := "...";
    
    package Signal_Queue renames ACD_Runtime.Signal_Queue;

    use Queued_Signals;

    procedure Send_setTime_Event(acc_This : Clockwork_cptr; Priority : Signals.Priority_Type := Signals.Normal) is
    begin
        Send (Signal'(Queued_Signal with Event => setTime_Event, This => Clockwork_cptr (acc_This)), Priority);
    end Send_setTime_Event;
    
    procedure Send_tick_Event(acc_This : Clockwork_cptr; Priority : Signals.Priority_Type := Signals.Normal) is
    begin
        Send (Signal'(Queued_Signal with Event => tick_Event, This => Clockwork_cptr (acc_This)), Priority);
    end Send_tick_Event;
    

    procedure Dispatch (S : Signal) is
    begin
        --T text_io.put_line ("Clockwork Event " & Clockwork_Event_Type'image (S.Event)); --T
        Clockwork_pkg.Take_Event (    
            acc_This    => S.This.all'access,
            Event        => S.Event);
    end Dispatch;

    function Image (S : Signal) return String is
        StpCalls_Inst_And_St : constant String := System.Image (S.This.all'address);
    begin
        return "Clockwork::" & Clockwork_Event_Type'image (S.Event)
           & " -> " & StpCalls_Inst_And_St;
    end Image;


end Clockwork_pkg.Clockwork_Signals;
