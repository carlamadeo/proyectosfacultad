with ClockBird_pkg;
with Unchecked_Deallocation;
--#ACD# M(UDIF) includes
   -- user defined code to be added here ...
--#end ACD#

package body ClockBird_pkg.Coocoo_pkg is

    --#ACD# M(UDBFD) package body first declarations
       -- user defined code to be added here ...
    --#end ACD#

    --=============================================================================
    -- fixed part
    --=============================================================================

    



    --=============================================================================
    -- user extensible part
    --=============================================================================
    

    
    --Constructor Operations--------------------------------------------------------
    procedure Initialize (acc_this : Coocoo_cptr) is
        --#ACD# M(UDID::UID_91DD92BA-6EAE-11d4-8044-00104BF16798) Initialization Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        ClockBird_pkg.Initialize(ClockBird_pkg.ClockBird(acc_This.all)'access);

        --#ACD# M(UDIB::UID_91DD92BA-6EAE-11d4-8044-00104BF16798) Initialization Body
        null;   -- user defined code to be added here ...
        --#end ACD#
    end Initialize;

    function Create return Coocoo_ptr is
        acc_this : Coocoo_ptr;
        --#ACD# M(UDCD::UID_91DD92BA-6EAE-11d4-8044-00104BF16798) Create Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        acc_this := new Coocoo;
            Initialize (Coocoo_cptr (acc_this));
            --#ACD# M(UDCB::UID_91DD92BA-6EAE-11d4-8044-00104BF16798) Create Body
               -- user defined code to be added here ...
            --#end ACD#
        return acc_this;
    end Create;

    
    --Destructor Operations---------------------------------------------------------

    procedure Finalize (acc_this : in out Coocoo) is
    -- acc_this is NOT an access, but we keep the name for backward compatibility
        --#ACD# M(UDFID::UID_91DD92BA-6EAE-11d4-8044-00104BF16798) Finalize Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDFIB::UID_91DD92BA-6EAE-11d4-8044-00104BF16798) Finalize Body
        null;   -- user defined code to be added here ...
        --#end ACD#
        ClockBird_pkg.Finalize(ClockBird_pkg.ClockBird(Acc_This));

    end Finalize;

    procedure Free (acc_this : in out Coocoo_cptr) is
        procedure Deallocate is new Unchecked_Deallocation (Coocoo'class, Coocoo_cptr);
        --#ACD# M(UDFRD::UID_91DD92BA-6EAE-11d4-8044-00104BF16798) Free Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDFRB::UID_91DD92BA-6EAE-11d4-8044-00104BF16798) Free Body
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
    procedure cry(acc_this : access Coocoo; hour : IN Integer) is
       I : Integer;
    begin
       if isDoorOpened (ClockBird_cptr (acc_this)) then
          I := hour mod 12;
          if I = 0 then I := 12; end if;
          while I > 0 loop
             I := I - 1;
    	 Display_pkg.showBirdCry(acc_this.display_asc, "Cuckoo ");
          end loop;
       else
          Display_pkg.showBirdCry(acc_this.display_asc, ASCII.cr & ASCII.lf & "Outsch" & ASCII.cr & ASCII.lf);
       end if;
    end cry;




    --#ACD# M(UDBLD) package body last declarations
       -- user defined code to be added here ...
    --#end ACD#

--#ACD# M(UDBSP) package body statement part
begin
	null;   -- user defined code to be added here ...
--#end ACD#    
end ClockBird_pkg.Coocoo_pkg;
