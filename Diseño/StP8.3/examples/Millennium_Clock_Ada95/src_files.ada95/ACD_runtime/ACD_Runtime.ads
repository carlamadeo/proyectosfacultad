
package ACD_Runtime is
    -- This is the root package for all stuff that is used implicitly from the ACD templates.
    -- This reduces the chance of namespace conflicts with the application.
    
    
    -- Report a fatal Exception (typically terminating a task).
    -- Call should be like 
    -- Report_Exception (Context   => "Task XXX",
    --                   Excp_Name => Ada.Exceptions.Exception_Name(Ex),
    --                   Excp_Info => Ada.Exceptions.Exception_Information(Ex);
    --
    procedure Report_Exception (Context, Excp_Name, Excp_Info : string);
    
end ACD_Runtime;

package ACD_Runtime.Active_Base_Class is

    type ActiveInstance is tagged private;
    type ActiveLimitedInstance is tagged limited private;

private

    type ActiveInstance is tagged 
        record
            --#ACD# M(UDAI) Active Instance Declarations
            null;   -- user defined code to be added here ...
            --#end ACD#
        end record;
    type ActiveLimitedInstance is tagged limited 
        record
            --#ACD# M(UDALI) Active Limited Instance Declarations
            null;   -- user defined code to be added here ...
            --#end ACD#
        end record;


end ACD_Runtime.Active_Base_Class;
package ACD_Runtime.Inactive_Base_Class is

    type InactiveInstance is tagged private;
    type InactiveLimitedInstance is tagged limited private;

private

    type InactiveInstance is tagged 
        record
            --#ACD# M(UDII) Inactive Instance Declarations
            null;   -- user defined code to be added here ...
            --#end ACD#
        end record;
    type InactiveLimitedInstance is tagged limited
        record
            --#ACD# M(UDILI) Inactive Limited Instance Declarations
            null;   -- user defined code to be added here ...
            --#end ACD#
        end record;

end ACD_Runtime.Inactive_Base_Class;
