with Pendulum_pkg;
with Text_IO;
with Unchecked_Deallocation;
--#ACD# M(UDIF) includes
   -- user defined code to be added here ...
--#end ACD#

package body Weight is

    --#ACD# M(UDBFD) package body first declarations
       -- user defined code to be added here ...
    --#end ACD#

    --=============================================================================
    -- fixed part
    --=============================================================================

    
    --Attribute operations----------------------------------------------------------
    function getEnergy(this : Weight'class) return Integer is
    begin
        return this.Energy;

    end getEnergy;

    procedure setEnergy(acc_this : Weight_cptr; value : Integer) is
    begin
        acc_this.Energy := value;
    end setEnergy;

    --Relation operations-----------------------------------------------------------

    function getpend_asc(this : Weight'class) return Pendulum_pkg.Pendulum_cptr is
    begin
        return this.pend_asc;

    end getpend_asc;
    procedure setpend_asc(acc_this : Weight_cptr; item : Pendulum_pkg.Pendulum_cptr) is
    begin
        acc_this.pend_asc := Item;

    end setpend_asc;



    --=============================================================================
    -- user extensible part
    --=============================================================================
    

    
    package body Weight_State_Machine is


        
        --State Machine Operations------------------------------------------------------

        --Activity Procedure------------------------------------------------------------
        procedure Perform_Activity (acc_This : Weight_cptr) is
            this : Weight'class renames acc_this.all;
            currentState : State_Type renames this.currentState;
        begin
            case currentState is
            when ST_hasEnergy =>
                --TRC Activity for state ST_hasEnergy
                -- Activity: 
                --#ACD# M(UDACT::UID_77C1C492-6EAE-11d4-8044-00104BF16798) Action Code
                null;   -- user defined code to be added here ...
                --#end ACD#
            when ST_hasNoEnergy =>
                --TRC Activity for state ST_hasNoEnergy
                -- Activity: 
                --#ACD# M(UDACT::UID_77C1C491-6EAE-11d4-8044-00104BF16798) Action Code
                null;   -- user defined code to be added here ...
                --#end ACD#

            end case;
        end Perform_Activity;

        procedure Take_raise_Event(acc_This : Weight_cptr; e : Integer := 1000) is
            this : Weight'class renames acc_this.all;
            use Weight_State_Machine;
            currentState : State_Type renames this.currentState;
        begin
            case currentState is
            when Weight_State_Machine.ST_hasNoEnergy =>
                if TRUE then
                    energize(this'access,e);

                    --TRC Entering state ST_hasEnergy
                        currentState := ST_hasEnergy;

                end if;
                Perform_Activity (acc_This);
            when others =>
                

                Text_IO.Put_Line ("Weight: illegal event raise_Event in state "
                    & Weight_State_Machine.State_Type'image (currentState));


            end case;
        end Take_raise_Event;

        procedure Take_sink_Event(acc_This : Weight_cptr; e : Integer := 10) is
            this : Weight'class renames acc_this.all;
            use Weight_State_Machine;
            currentState : State_Type renames this.currentState;
        begin
            case currentState is
            when Weight_State_Machine.ST_hasEnergy =>
                if hasEnergy(this'access,e) then
                    --TRC Leaving state ST_hasEnergy
                    decrease(this'access,e);

                        -- send move to pend
                        Pendulum_pkg.Pendulum_Signals.Send_move_Event (This.pend_asc.all'access);
                    --TRC Entering state ST_hasEnergy
                        currentState := ST_hasEnergy;

                end if;
                Perform_Activity (acc_This);
            when others =>
                

                Text_IO.Put_Line ("Weight: illegal event sink_Event in state "
                    & Weight_State_Machine.State_Type'image (currentState));


            end case;
        end Take_sink_Event;
end Weight_State_Machine;

    
    --Constructor Operations--------------------------------------------------------
    procedure InitializeSM (acc_this : Weight_cptr;Event : Weight_Event_Type) is
    begin
        case Event is

        when raise_Event =>
            Weight_State_Machine.Take_raise_Event(acc_This);
            return;
        when others =>
           raise constraint_error; -- error handling generated by genHandleStateError does not work
        end case;
    end InitializeSM;

    procedure Initialize (acc_this : Weight_cptr; Event : Weight_Event_Type) is
        --#ACD# M(UDID::UID_91DD92BE-6EAE-11d4-8044-00104BF16798) Initialization Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDIB::UID_91DD92BE-6EAE-11d4-8044-00104BF16798) Initialization Body
        Acc_This.Pend_Asc := Pendulum_Pkg.Pendulum_Cptr (Pendulum_Pkg.Create(Pendulum_Pkg.Move_Event));
        --#end ACD#
        InitializeSM(acc_This, raise_Event);
    end Initialize;

    function Create(Event : Weight_Event_Type) return Weight_ptr is
        acc_this : Weight_ptr;
        --#ACD# M(UDCD::UID_91DD92BE-6EAE-11d4-8044-00104BF16798) Create Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        acc_this := new Weight;
            Initialize (Weight_cptr (acc_this), raise_Event);
            --#ACD# M(UDCB::UID_91DD92BE-6EAE-11d4-8044-00104BF16798) Create Body
               -- user defined code to be added here ...
            --#end ACD#
        return acc_this;
    end Create;

    
    --Destructor Operations---------------------------------------------------------

    procedure Finalize (acc_this : in out Weight) is
    -- acc_this is NOT an access, but we keep the name for backward compatibility
        --#ACD# M(UDFID::UID_91DD92BE-6EAE-11d4-8044-00104BF16798) Finalize Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDFIB::UID_91DD92BE-6EAE-11d4-8044-00104BF16798) Finalize Body
        null;   -- user defined code to be added here ...
        --#end ACD#
    end Finalize;

    procedure Free (acc_this : in out Weight_cptr) is
        procedure Deallocate is new Unchecked_Deallocation (Weight'class, Weight_cptr);
        --#ACD# M(UDFRD::UID_91DD92BE-6EAE-11d4-8044-00104BF16798) Free Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDFRB::UID_91DD92BE-6EAE-11d4-8044-00104BF16798) Free Body
           -- user defined code to be added here ...
        --#end ACD#

        if acc_this = NULL then return; end if;
        Finalize (acc_this.all);
        Deallocate (acc_this);
    end Free;

    --Operations--------------------------------------------------------------------

    
    --------------------------------------------------------------------------------
    -- Operation :        decrease
    --
    -- Description :    
    --
    --------------------------------------------------------------------------------
    procedure decrease(acc_this : access Weight; e : IN Integer) is
    begin
         setEnergy(Weight_cptr(acc_this), getEnergy(acc_this.all)-e);
    end decrease;



    
    --------------------------------------------------------------------------------
    -- Operation :        energize
    --
    -- Description :    
    --
    --------------------------------------------------------------------------------
    procedure energize(acc_this : access Weight; e : IN Integer) is
    begin
         setEnergy(Weight_cptr(acc_this),e);
    end energize;



    
    --------------------------------------------------------------------------------
    -- Operation :        hasEnergy
    --
    -- Description :    
    --
    --------------------------------------------------------------------------------
    function hasEnergy(acc_this : access Weight; e : IN Integer) return Boolean is
    begin
        return getEnergy(acc_this.all)-e >= 0;
    end hasEnergy;




    --#ACD# M(UDBLD) package body last declarations
       -- user defined code to be added here ...
    --#end ACD#

--#ACD# M(UDBSP) package body statement part
begin
	null;   -- user defined code to be added here ...
--#end ACD#    
end Weight;
