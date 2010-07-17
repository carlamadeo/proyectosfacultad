with ACD_Runtime.Queued_Signals;
with Weight;
with ACD_Runtime.Signals;
package Weight.Weight_Signals is

    package Signals renames ACD_Runtime.Signals;
    package Queued_Signals renames ACD_Runtime.Queued_Signals;


    type Signal (Event : Weight_Event_Type) is new Queued_Signals.Queued_Signal with private;
    procedure Send_raise_Event(acc_This : Weight_cptr; e : Integer := 1000; Priority : Signals.Priority_Type := Signals.Normal);
    procedure Send_sink_Event(acc_This : Weight_cptr; e : Integer := 10; Priority : Signals.Priority_Type := Signals.Normal);

    procedure Dispatch (S : Signal);

    function Image (S : Signal) return String;

 
private

    type raise_Event_Arg_Type is
        record
            e : Integer;
        end record;
    type sink_Event_Arg_Type is
        record
            e : Integer;
        end record;


    type Signal (Event : Weight_Event_Type) is new Queued_Signals.Queued_Signal with
        record
            This : Weight_cptr;
            case Event is
            when raise_Event =>
                raise_Event_Arg : raise_Event_Arg_Type;
            when sink_Event =>
                sink_Event_Arg : sink_Event_Arg_Type;
            end case;
        end record;
      
end Weight.Weight_Signals;
