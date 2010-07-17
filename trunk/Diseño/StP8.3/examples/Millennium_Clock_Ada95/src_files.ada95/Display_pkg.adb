with Unchecked_Deallocation;
--#ACD# M(UDIF) includes
with Text_IO;
   -- user defined code to be added here ...
--#end ACD#

package body Display_pkg is

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
    procedure Initialize (acc_this : Display_cptr) is
        --#ACD# M(UDID::UID_91DD9312-6EAE-11d4-8044-00104BF16798) Initialization Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDIB::UID_91DD9312-6EAE-11d4-8044-00104BF16798) Initialization Body
        null;   -- user defined code to be added here ...
        --#end ACD#
    end Initialize;

    The_Display : aliased Display;
    The_Display_Ref : aliased Display_ptr;
    function Create return Display_ptr is
        acc_this : Display_ptr;
        --#ACD# M(UDCD::UID_91DD9312-6EAE-11d4-8044-00104BF16798) Create Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        if The_Display_Ref /= NULL then
            return The_Display_Ref;
        else            
            The_Display_Ref := The_Display'access;
            acc_this := The_Display_Ref;
            Initialize (Display_cptr (acc_this));
            --#ACD# M(UDCB::UID_91DD9312-6EAE-11d4-8044-00104BF16798) Create Body
               -- user defined code to be added here ...
            --#end ACD#
        end if;
        return acc_this;
    end Create;

    
    --Destructor Operations---------------------------------------------------------

    procedure Finalize (acc_this : in out Display) is
    -- acc_this is NOT an access, but we keep the name for backward compatibility
        --#ACD# M(UDFID::UID_91DD9312-6EAE-11d4-8044-00104BF16798) Finalize Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDFIB::UID_91DD9312-6EAE-11d4-8044-00104BF16798) Finalize Body
        null;   -- user defined code to be added here ...
        --#end ACD#
    end Finalize;

    procedure Free (acc_this : in out Display_cptr) is
        procedure Deallocate is new Unchecked_Deallocation (Display'class, Display_cptr);
        --#ACD# M(UDFRD::UID_91DD9312-6EAE-11d4-8044-00104BF16798) Free Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDFRB::UID_91DD9312-6EAE-11d4-8044-00104BF16798) Free Body
           -- user defined code to be added here ...
        --#end ACD#

        if acc_this = NULL then return; end if;
        Finalize (acc_this.all);
        Deallocate (acc_this);
    end Free;

    --Operations--------------------------------------------------------------------

    
    --------------------------------------------------------------------------------
    -- Operation :        showTime
    --
    -- Description :    
    --
    --------------------------------------------------------------------------------
    procedure showTime(acc_this : access Display; hour : IN Integer; minute : IN Integer) is
        begin		
              Text_io.put(Integer'image(Hour));
              Text_IO.put(':');
              if Minute = 0 then
                  Text_Io.Put_Line(" 00");
              else
                  Text_IO.Put (Integer'Image(Minute));
                  Text_io.put (", ");
               end if;
    end showTime;



    
    --------------------------------------------------------------------------------
    -- Operation :        showBirdCry
    --
    -- Description :    
    --
    --------------------------------------------------------------------------------
    procedure showBirdCry(acc_this : access Display; s : IN String) is
    begin
       text_io.put (s);
    end showBirdCry;



    
    --------------------------------------------------------------------------------
    -- Operation :        showDoorState
    --
    -- Description :    
    --
    --------------------------------------------------------------------------------
    procedure showDoorState(acc_this : access Display; s : IN String) is
    begin
       text_io.put (s);
    end showDoorState;




    --#ACD# M(UDBLD) package body last declarations
       -- user defined code to be added here ...
    --#end ACD#

--#ACD# M(UDBSP) package body statement part
begin
	null;   -- user defined code to be added here ...
--#end ACD#    
end Display_pkg;
