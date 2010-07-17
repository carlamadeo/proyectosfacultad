with ACD_Runtime.Signal_Queue;
with System;
package body Pendulum_pkg.Pendulum_Signals is

    Dots : constant string := "...";
    
    package Signal_Queue renames ACD_Runtime.Signal_Queue;

    use Queued_Signals;

    procedure Send_move_Event(acc_This : Pendulum_cptr; Priority : Signals.Priority_Type := Signals.Normal) is
    begin
        Send (Signal'(Queued_Signal with Event => move_Event, This => Pendulum_cptr (acc_This)), Priority);
    end Send_move_Event;
    

    procedure Dispatch (S : Signal) is
    begin
        --T text_io.put_line ("Pendulum Event " & Pendulum_Event_Type'image (S.Event)); --T
        Pendulum_pkg.Take_Event (    
            acc_This    => S.This.all'access,
            Event        => S.Event);
    end Dispatch;

    function Image (S : Signal) return String is
        StpCalls_Inst_And_St : constant String := System.Image (S.This.all'address);
    begin
        return "Pendulum::" & Pendulum_Event_Type'image (S.Event)
           & " -> " & StpCalls_Inst_And_St;
    end Image;


end Pendulum_pkg.Pendulum_Signals;
