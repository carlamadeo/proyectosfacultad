with ACD_Runtime.Queued_Signals;
with Clockwork_pkg;
with ACD_Runtime.Signals;
package Clockwork_pkg.Clockwork_Signals is

    package Signals renames ACD_Runtime.Signals;
    package Queued_Signals renames ACD_Runtime.Queued_Signals;


    type Signal (Event : Clockwork_Event_Type) is new Queued_Signals.Queued_Signal with private;
    procedure Send_setTime_Event(acc_This : Clockwork_cptr; Priority : Signals.Priority_Type := Signals.Normal);
    procedure Send_tick_Event(acc_This : Clockwork_cptr; Priority : Signals.Priority_Type := Signals.Normal);

    procedure Dispatch (S : Signal);

    function Image (S : Signal) return String;

 
private



    type Signal (Event : Clockwork_Event_Type) is new Queued_Signals.Queued_Signal with
        record
            This : Clockwork_cptr;
            case Event is
            when setTime_Event => null;
            when tick_Event => null;
            end case;
        end record;
      
end Clockwork_pkg.Clockwork_Signals;
