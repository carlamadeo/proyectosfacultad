with Door_pkg;
with Display_pkg;
with Unchecked_Deallocation;
--#ACD# M(UDIF) includes
   -- user defined code to be added here ...
--#end ACD#

package body ClockBird_pkg is

    --#ACD# M(UDBFD) package body first declarations
       -- user defined code to be added here ...
    --#end ACD#

    --=============================================================================
    -- fixed part
    --=============================================================================

    
    --Relation operations-----------------------------------------------------------

    function getdoor_asc(this : ClockBird'class) return Door_pkg.Door_cptr is
    begin
        return this.door_asc;

    end getdoor_asc;
    procedure setdoor_asc(acc_this : ClockBird_cptr; item : Door_pkg.Door_cptr) is
    begin
        acc_this.door_asc := Item;

    end setdoor_asc;

    function getdisplay_asc(this : ClockBird'class) return Display_pkg.Display_cptr is
    begin
        return this.display_asc;

    end getdisplay_asc;
    procedure setdisplay_asc(acc_this : ClockBird_cptr; item : Display_pkg.Display_cptr) is
    begin
        acc_this.display_asc := Item;

    end setdisplay_asc;



    --=============================================================================
    -- user extensible part
    --=============================================================================
    

    
    --Constructor Operations--------------------------------------------------------
    procedure Initialize (acc_this : ClockBird_cptr) is
        --#ACD# M(UDID::UID_91DD92B2-6EAE-11d4-8044-00104BF16798) Initialization Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDIB::UID_91DD92B2-6EAE-11d4-8044-00104BF16798) Initialization Body
        acc_This.door_asc := Door_pkg.Door_cptr (Door_pkg.Create );
    
        acc_This.display_asc := Display_pkg.Display_cptr (Display_pkg.Create );
        --#end ACD#
    end Initialize;

    function Create return ClockBird_ptr is
        acc_this : ClockBird_ptr;
        --#ACD# M(UDCD::UID_91DD92B2-6EAE-11d4-8044-00104BF16798) Create Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        acc_this := new ClockBird;
            Initialize (ClockBird_cptr (acc_this));
            --#ACD# M(UDCB::UID_91DD92B2-6EAE-11d4-8044-00104BF16798) Create Body
               -- user defined code to be added here ...
            --#end ACD#
        return acc_this;
    end Create;

    
    --Destructor Operations---------------------------------------------------------

    procedure Finalize (acc_this : in out ClockBird) is
    -- acc_this is NOT an access, but we keep the name for backward compatibility
        --#ACD# M(UDFID::UID_91DD92B2-6EAE-11d4-8044-00104BF16798) Finalize Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDFIB::UID_91DD92B2-6EAE-11d4-8044-00104BF16798) Finalize Body
        null;   -- user defined code to be added here ...
        --#end ACD#
    end Finalize;

    procedure Free (acc_this : in out ClockBird_cptr) is
        procedure Deallocate is new Unchecked_Deallocation (ClockBird'class, ClockBird_cptr);
        --#ACD# M(UDFRD::UID_91DD92B2-6EAE-11d4-8044-00104BF16798) Free Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDFRB::UID_91DD92B2-6EAE-11d4-8044-00104BF16798) Free Body
           -- user defined code to be added here ...
        --#end ACD#

        if acc_this = NULL then return; end if;
        Finalize (acc_this.all);
        Deallocate (acc_this);
    end Free;

    --Operations--------------------------------------------------------------------

    
    --------------------------------------------------------------------------------
    -- Operation :        cry
    --
    -- Description :    
    --
    --------------------------------------------------------------------------------
    procedure cry(acc_this : access ClockBird; hour : IN Integer) is
    --#ACD# M(UDOI::UID_98D34F51-6EAE-11d4-8044-00104BF16798) procedure code
    begin
	null;   -- user defined code to be added here ...
    --#end ACD#
    end cry;



    
    --------------------------------------------------------------------------------
    -- Operation :        isDoorOpened
    --
    -- Description :    
    --
    --------------------------------------------------------------------------------
    function isDoorOpened(acc_this : access ClockBird) return Boolean is
    begin
       return door_pkg.getdoorStatus (acc_this.door_asc.all);
    end isDoorOpened;



    
    --------------------------------------------------------------------------------
    -- Operation :        cry
    --
    -- Description :    
    --
    --------------------------------------------------------------------------------
    procedure cry(acc_this : access ClockBird) is
    --#ACD# M(UDOI::UID_91DD92BD-6EAE-11d4-8044-00104BF16798) procedure code
    begin
	null;   -- user defined code to be added here ...
    --#end ACD#
    end cry;




    --#ACD# M(UDBLD) package body last declarations
       -- user defined code to be added here ...
    --#end ACD#

--#ACD# M(UDBSP) package body statement part
begin
	null;   -- user defined code to be added here ...
--#end ACD#    
end ClockBird_pkg;
