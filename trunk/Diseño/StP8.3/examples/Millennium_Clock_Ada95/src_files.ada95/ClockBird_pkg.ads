with ACD_Runtime.Inactive_Base_Class;
with Door_pkg;
with Display_pkg;
--#ACD# M(UDIF) includes
   -- user defined code to be added here ...
--#end ACD#
package ClockBird_pkg is

    --#ACD# M(UDUFPD) package first public declarations
       -- user defined code to be added here ...
    --#end ACD#

    
    type ClockBird is new ACD_Runtime.Inactive_Base_Class.InactiveInstance with private;
    type ClockBird_cptr is access all ClockBird'class;
    type ClockBird_ptr is access all ClockBird;


    --#ACD# M(UDUSPD) package second public declarations
       -- user defined code to be added here ...
    --#end ACD#
    

    --=============================================================================
    -- Class ClockBird
    --=============================================================================
    
    
    --Constructor Operations--------------------------------------------------------
    procedure Initialize (acc_this : ClockBird_cptr);
        -- pragma inline(Initialize);

    function Create return ClockBird_ptr;
        -- pragma inline(Create);
    
    --Destructor Operations---------------------------------------------------------

    procedure Finalize (Acc_This : in out ClockBird);
        -- pragma inline(Finalize);

    procedure Free (acc_this : in out ClockBird_cptr);
        -- pragma inline(Free);

    --Operations--------------------------------------------------------------------
    procedure cry(acc_this : access ClockBird; hour : IN Integer);
    function isDoorOpened(acc_this : access ClockBird) return Boolean;
    procedure cry(acc_this : access ClockBird);
    --Relation operations-----------------------------------------------------------

    function getdoor_asc(this : ClockBird'class) return Door_pkg.Door_cptr;
        -- pragma inline(getdoor_asc);
    procedure setdoor_asc(acc_this : ClockBird_cptr; item : Door_pkg.Door_cptr);
        -- pragma inline(setdoor_asc);
    function Get_door_asc(this : ClockBird'class) return Door_pkg.Door_cptr
        renames getdoor_asc;
    procedure Set_door_asc(acc_this : ClockBird_cptr; item : Door_pkg.Door_cptr)
        renames setdoor_asc;

    function getdisplay_asc(this : ClockBird'class) return Display_pkg.Display_cptr;
        -- pragma inline(getdisplay_asc);
    procedure setdisplay_asc(acc_this : ClockBird_cptr; item : Display_pkg.Display_cptr);
        -- pragma inline(setdisplay_asc);
    function Get_display_asc(this : ClockBird'class) return Display_pkg.Display_cptr
        renames getdisplay_asc;
    procedure Set_display_asc(acc_this : ClockBird_cptr; item : Display_pkg.Display_cptr)
        renames setdisplay_asc;

    --#ACD# M(UDULPD) package last public declarations
       -- user defined code to be added here ...
    --#end ACD#
private

    --#ACD# M(UDPFPD) package first private declarations
       -- user defined code to be added here ...
    --#end ACD#
    

    --=============================================================================
    --Class ClockBird
    --=============================================================================
    
    
    --Class Record------------------------------------------------------------------
    type ClockBird is new ACD_Runtime.Inactive_Base_Class.InactiveInstance with
        record
            --Relations---------------------------------------------------------------------
            door_asc : Door_pkg.Door_cptr;
            display_asc : Display_pkg.Display_cptr;
            --#ACD# M(UDCOMP::UID_91DD92B2-6EAE-11d4-8044-00104BF16798) User defined components
               -- user defined code to be added here ...
            --#end ACD#
        end record;


    --#ACD# M(UDPLPD) package last private declarations
       -- user defined code to be added here ...
    --#end ACD#

end ClockBird_pkg;
    
