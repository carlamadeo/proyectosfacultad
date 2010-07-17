
with Weight;
with System;

package Main_Pkg is

    The_Weight : aliased Weight.Weight;
    Shutdown : Boolean := False; 
     
    DebuggerActive : constant Boolean := False;

    procedure Start_Queue_Dispatcher;

end Main_Pkg;


with ACD_Runtime.Queued_Signals;

with Text_IO;
with Ada.Exceptions;

package body Main_Pkg is

    package Queued_Signals renames ACD_Runtime.Queued_Signals;


    task Queue_Dispatcher is
        pragma priority (system.priority'last);
        entry Start;
    end Queue_Dispatcher;


    task body Queue_Dispatcher is
    begin
        accept Start;
        while not Shutdown loop
            begin
                Queued_Signals.Dispatch_Signals;
            exception
                when e:others =>       
                    text_io.put_line ("Exception in Dispatch_Signals:" & Ada.Exceptions.Exception_Name (e));
            end;
        end loop;
    exception
        when e:others =>       
            text_io.put_line ("Exception in Queue_Dispatcher:" & Ada.Exceptions.Exception_Name (e));
            Shutdown := True;
    end Queue_Dispatcher;

    procedure Start_Queue_Dispatcher is
    begin
        Queue_Dispatcher.Start;
    end Start_Queue_Dispatcher;
 
end Main_Pkg;

with Pendulum_pkg;
with Display_pkg;
with Weight;
with ClockBird_pkg.Coocoo_pkg;
with ClockBird_pkg.Owl_pkg;
with Door_pkg;
with ClockBird_pkg;
with Clockwork_pkg;

--#ACD# M(UDIF) additional context clauses
   -- user defined code to be added here ...
with Ada.Text_IO;
with Ada.Exceptions;
with Weight.Weight_Signals;
--#end ACD#
with Main_Pkg;

procedure Main is
    --#ACD# M(UDSD) StartUp Declarations
       -- user defined code to be added here ...
     use Main_Pkg;
    --#end ACD#
begin
    --#ACD# M(UDSC) StartUp Code
    Start_Queue_Dispatcher;
    Weight.Initialize (The_Weight'access,Weight.Raise_Event);
    delay 1.0;
    Weight.Weight_Signals.Send_Sink_Event (The_Weight'access);
    begin
        while not Shutdown loop
           delay 1.0;
--           if not Stp_Calls.DebuggerActive then 
              Weight.Weight_Signals.Send_Sink_Event (The_Weight'access);
--           end if;
        end loop;  
    exception
       when E:others =>       
          Ada.Text_Io.Put_Line ("Exception in Main Loop:" & Ada.Exceptions.Exception_Name (E));
          Shutdown := True;
    end;  
    --#end ACD#
end Main;
