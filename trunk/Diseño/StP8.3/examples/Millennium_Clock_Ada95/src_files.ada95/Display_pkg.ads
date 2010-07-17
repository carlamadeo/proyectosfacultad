with ACD_Runtime.Inactive_Base_Class;
--#ACD# M(UDIF) includes
   -- user defined code to be added here ...
--#end ACD#
package Display_pkg is

    --#ACD# M(UDUFPD) package first public declarations
       -- user defined code to be added here ...
    --#end ACD#

    
    type Display is new ACD_Runtime.Inactive_Base_Class.InactiveInstance with private;
    type Display_cptr is access all Display'class;
    type Display_ptr is access all Display;


    --#ACD# M(UDUSPD) package second public declarations
       -- user defined code to be added here ...
    --#end ACD#
    

    --=============================================================================
    -- Class Display
    --=============================================================================
    
    
    --Constructor Operations--------------------------------------------------------
    procedure Initialize (acc_this : Display_cptr);
        -- pragma inline(Initialize);

    function Create return Display_ptr;
        -- pragma inline(Create);
    
    --Destructor Operations---------------------------------------------------------

    procedure Finalize (Acc_This : in out Display);
        -- pragma inline(Finalize);

    procedure Free (acc_this : in out Display_cptr);
        -- pragma inline(Free);

    --Operations--------------------------------------------------------------------
    procedure showTime(acc_this : access Display; hour : IN Integer; minute : IN Integer);
    procedure showBirdCry(acc_this : access Display; s : IN String);
    procedure showDoorState(acc_this : access Display; s : IN String);

    --#ACD# M(UDULPD) package last public declarations
       -- user defined code to be added here ...
    --#end ACD#
private

    --#ACD# M(UDPFPD) package first private declarations
       -- user defined code to be added here ...
    --#end ACD#
    

    --=============================================================================
    --Class Display
    --=============================================================================
    
    
    --Class Record------------------------------------------------------------------
    type Display is new ACD_Runtime.Inactive_Base_Class.InactiveInstance with
        record
            --#ACD# M(UDCOMP::UID_91DD9312-6EAE-11d4-8044-00104BF16798) User defined components
            null; -- Delete this line if you add your own components   -- user defined code to be added here ...
            --#end ACD#
        end record;


    --#ACD# M(UDPLPD) package last private declarations
       -- user defined code to be added here ...
    --#end ACD#

end Display_pkg;
    
