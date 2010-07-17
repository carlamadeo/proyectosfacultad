with ACD_Runtime.Queued_Signals;
with Pendulum_pkg;
with ACD_Runtime.Signals;
package Pendulum_pkg.Pendulum_Signals is

    package Signals renames ACD_Runtime.Signals;
    package Queued_Signals renames ACD_Runtime.Queued_Signals;


    type Signal (Event : Pendulum_Event_Type) is new Queued_Signals.Queued_Signal with private;
    procedure Send_move_Event(acc_This : Pendulum_cptr; Priority : Signals.Priority_Type := Signals.Normal);

    procedure Dispatch (S : Signal);

    function Image (S : Signal) return String;

 
private



    type Signal (Event : Pendulum_Event_Type) is new Queued_Signals.Queued_Signal with
        record
            This : Pendulum_cptr;
            case Event is
            when move_Event => null;
            end case;
        end record;
      
end Pendulum_pkg.Pendulum_Signals;
