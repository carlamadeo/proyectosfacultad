with Display_pkg;
with Unchecked_Deallocation;
--#ACD# M(UDIF) includes
   -- user defined code to be added here ...
--#end ACD#

package body Door_pkg is

    --#ACD# M(UDBFD) package body first declarations
       -- user defined code to be added here ...
    --#end ACD#

    --=============================================================================
    -- fixed part
    --=============================================================================

    
    --Attribute operations----------------------------------------------------------
    function getdoorStatus(this : Door'class) return Boolean is
    begin
        return this.doorStatus;

    end getdoorStatus;

    procedure setdoorStatus(acc_this : Door_cptr; value : Boolean) is
    begin
        acc_this.doorStatus := value;
    end setdoorStatus;

    --Relation operations-----------------------------------------------------------

    function getdisplay_asc(this : Door'class) return Display_pkg.Display_cptr is
    begin
        return this.display_asc;

    end getdisplay_asc;
    procedure setdisplay_asc(acc_this : Door_cptr; item : Display_pkg.Display_cptr) is
    begin
        acc_this.display_asc := Item;

    end setdisplay_asc;



    --=============================================================================
    -- user extensible part
    --=============================================================================
    

    
    --Constructor Operations--------------------------------------------------------
    procedure Initialize (acc_this : Door_cptr) is
        --#ACD# M(UDID::UID_91DD92B6-6EAE-11d4-8044-00104BF16798) Initialization Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDIB::UID_91DD92B6-6EAE-11d4-8044-00104BF16798) Initialization Body
        acc_This.display_asc := Display_pkg.Display_cptr (Display_pkg.Create );
        --#end ACD#
    end Initialize;

    The_Door : aliased Door;
    The_Door_Ref : aliased Door_ptr;
    function Create return Door_ptr is
        acc_this : Door_ptr;
        --#ACD# M(UDCD::UID_91DD92B6-6EAE-11d4-8044-00104BF16798) Create Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        if The_Door_Ref /= NULL then
            return The_Door_Ref;
        else            
            The_Door_Ref := The_Door'access;
            acc_this := The_Door_Ref;
            Initialize (Door_cptr (acc_this));
            --#ACD# M(UDCB::UID_91DD92B6-6EAE-11d4-8044-00104BF16798) Create Body
               -- user defined code to be added here ...
            --#end ACD#
        end if;
        return acc_this;
    end Create;

    
    --Destructor Operations---------------------------------------------------------

    procedure Finalize (acc_this : in out Door) is
    -- acc_this is NOT an access, but we keep the name for backward compatibility
        --#ACD# M(UDFID::UID_91DD92B6-6EAE-11d4-8044-00104BF16798) Finalize Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDFIB::UID_91DD92B6-6EAE-11d4-8044-00104BF16798) Finalize Body
        null;   -- user defined code to be added here ...
        --#end ACD#
    end Finalize;

    procedure Free (acc_this : in out Door_cptr) is
        procedure Deallocate is new Unchecked_Deallocation (Door'class, Door_cptr);
        --#ACD# M(UDFRD::UID_91DD92B6-6EAE-11d4-8044-00104BF16798) Free Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDFRB::UID_91DD92B6-6EAE-11d4-8044-00104BF16798) Free Body
           -- user defined code to be added here ...
        --#end ACD#

        if acc_this = NULL then return; end if;
        Finalize (acc_this.all);
        Deallocate (acc_this);
    end Free;

    --Operations--------------------------------------------------------------------

    
    --------------------------------------------------------------------------------
    -- Operation :        close
    --
    -- Description :    
    --
    --------------------------------------------------------------------------------
    procedure close(acc_this : access Door) is
    begin
       Display_pkg.showDoorState(acc_this.display_asc, ASCII.cr & ASCII.lf & "Door closed" & ASCII.cr & ASCII.lf & ASCII.lf);
       acc_this.doorStatus := false;
    end close;



    
    --------------------------------------------------------------------------------
    -- Operation :        open
    --
    -- Description :    
    --
    --------------------------------------------------------------------------------
    procedure open(acc_this : access Door) is
    begin
       Display_pkg.showDoorState(acc_this.display_asc, "Door opened" & ASCII.cr & ASCII.lf);
       acc_this.doorStatus := true;
    end open;




    --#ACD# M(UDBLD) package body last declarations
       -- user defined code to be added here ...
    --#end ACD#

--#ACD# M(UDBSP) package body statement part
begin
	null;   -- user defined code to be added here ...
--#end ACD#    
end Door_pkg;
