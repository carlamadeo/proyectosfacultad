with ACD_Runtime.Inactive_Base_Class;
with Display_pkg;
--#ACD# M(UDIF) includes
   -- user defined code to be added here ...
--#end ACD#
package Door_pkg is

    --#ACD# M(UDUFPD) package first public declarations
       -- user defined code to be added here ...
    --#end ACD#

    
    type Door is new ACD_Runtime.Inactive_Base_Class.InactiveInstance with private;
    type Door_cptr is access all Door'class;
    type Door_ptr is access all Door;


    --#ACD# M(UDUSPD) package second public declarations
       -- user defined code to be added here ...
    --#end ACD#
    

    --=============================================================================
    -- Class Door
    --=============================================================================
    
    
    --Constructor Operations--------------------------------------------------------
    procedure Initialize (acc_this : Door_cptr);
        -- pragma inline(Initialize);

    function Create return Door_ptr;
        -- pragma inline(Create);
    
    --Destructor Operations---------------------------------------------------------

    procedure Finalize (Acc_This : in out Door);
        -- pragma inline(Finalize);

    procedure Free (acc_this : in out Door_cptr);
        -- pragma inline(Free);

    --Operations--------------------------------------------------------------------
    procedure close(acc_this : access Door);
    procedure open(acc_this : access Door);
    --Attribute operations----------------------------------------------------------
    function getdoorStatus(this : Door'class) return Boolean;
        -- pragma inline(getdoorStatus);
    procedure setdoorStatus(acc_this : Door_cptr; value : Boolean);
        -- pragma inline(setdoorStatus);
    --Relation operations-----------------------------------------------------------

    function getdisplay_asc(this : Door'class) return Display_pkg.Display_cptr;
        -- pragma inline(getdisplay_asc);
    procedure setdisplay_asc(acc_this : Door_cptr; item : Display_pkg.Display_cptr);
        -- pragma inline(setdisplay_asc);
    function Get_display_asc(this : Door'class) return Display_pkg.Display_cptr
        renames getdisplay_asc;
    procedure Set_display_asc(acc_this : Door_cptr; item : Display_pkg.Display_cptr)
        renames setdisplay_asc;

    --#ACD# M(UDULPD) package last public declarations
       -- user defined code to be added here ...
    --#end ACD#
private

    --#ACD# M(UDPFPD) package first private declarations
       -- user defined code to be added here ...
    --#end ACD#
    

    --=============================================================================
    --Class Door
    --=============================================================================
    
    
    --Class Record------------------------------------------------------------------
    type Door is new ACD_Runtime.Inactive_Base_Class.InactiveInstance with
        record
            --Attributes--------------------------------------------------------------------
            doorStatus : Boolean;
            --Relations---------------------------------------------------------------------
            display_asc : Display_pkg.Display_cptr;
            --#ACD# M(UDCOMP::UID_91DD92B6-6EAE-11d4-8044-00104BF16798) User defined components
               -- user defined code to be added here ...
            --#end ACD#
        end record;


    --#ACD# M(UDPLPD) package last private declarations
       -- user defined code to be added here ...
    --#end ACD#

end Door_pkg;
    
