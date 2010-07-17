with ACD_Runtime.Signal_Queue;
with System;
package body Weight.Weight_Signals is

    Dots : constant string := "...";
    
    package Signal_Queue renames ACD_Runtime.Signal_Queue;

    use Queued_Signals;

    procedure Send_raise_Event(acc_This : Weight_cptr; e : Integer := 1000; Priority : Signals.Priority_Type := Signals.Normal) is
    begin
        Send (Signal'(    Queued_Signal with
                Event    => raise_Event,
                This    => Weight_cptr (acc_This),
                raise_Event_Arg => raise_Event_Arg_Type'(e => e)),
              Priority);
    end Send_raise_Event;
    
    procedure Send_sink_Event(acc_This : Weight_cptr; e : Integer := 10; Priority : Signals.Priority_Type := Signals.Normal) is
    begin
        Send (Signal'(    Queued_Signal with
                Event    => sink_Event,
                This    => Weight_cptr (acc_This),
                sink_Event_Arg => sink_Event_Arg_Type'(e => e)),
              Priority);
    end Send_sink_Event;
    

    procedure Dispatch (S : Signal) is
        acc_This : Weight_cptr renames S.This;
    begin
        --T text_io.put_line ("Class Weight Event " & Weight_Event_Type'image (S.Event)); --T
        case S.Event is
           when raise_Event =>
            Weight_State_Machine.Take_raise_Event(acc_This
                , e    => S.raise_Event_Arg.e
                );
           when sink_Event =>
            Weight_State_Machine.Take_sink_Event(acc_This
                , e    => S.sink_Event_Arg.e
                );
        end case;
    end Dispatch;

    function Image (S : Signal) return String is
        acc_This : Weight_cptr renames S.This;
        StpCalls_Inst_And_St : constant String := System.Image (S.This.all'address);
    begin
        case S.Event is
           when raise_Event =>
            return "Weight::raise_Event"
                & "(e => " & Integer'image(S.raise_Event_Arg.e)
                & ") -> " & StpCalls_Inst_And_St;
           when sink_Event =>
            return "Weight::sink_Event"
                & "(e => " & Integer'image(S.sink_Event_Arg.e)
                & ") -> " & StpCalls_Inst_And_St;
        end case;
    end Image;


end Weight.Weight_Signals;
