with Clockwork_pkg;
with Text_IO;
with Unchecked_Deallocation;
--#ACD# M(UDIF) includes
   -- user defined code to be added here ...
--#end ACD#

package body Pendulum_pkg is

    --#ACD# M(UDBFD) package body first declarations
       -- user defined code to be added here ...
    --#end ACD#

    --=============================================================================
    -- fixed part
    --=============================================================================

    
    --Relation operations-----------------------------------------------------------

    function getclockwork_asc(this : Pendulum'class) return Clockwork_pkg.Clockwork_cptr is
    begin
        return this.clockwork_asc;

    end getclockwork_asc;
    procedure setclockwork_asc(acc_this : Pendulum_cptr; item : Clockwork_pkg.Clockwork_cptr) is
    begin
        acc_this.clockwork_asc := Item;

    end setclockwork_asc;
    
    --State Machine Operations------------------------------------------------------
    procedure Take_Event(acc_This : Pendulum_cptr; Event : Pendulum_Event_Type) is
        stateAction : Pendulum_State_Machine.Action_Procedure;
        use type Pendulum_State_Machine.Action_Procedure;
    begin
        stateAction := Pendulum_State_Machine.StateEventTable(acc_This.currentState,Event);
        if stateAction /= NULL then
            stateAction(acc_This);
            Pendulum_State_Machine.Perform_Activity (acc_This);
        else
            
            Text_IO.Put_Line ("Pendulum: illegal event "
                & Pendulum_Event_Type'image (Event) & " in state "
                & Pendulum_State_Machine.State_Type'image (acc_This.currentState));


        end if;
    end Take_Event;




    --=============================================================================
    -- user extensible part
    --=============================================================================
    

    
    package body Pendulum_State_Machine is


        --Activity Procedure------------------------------------------------------------
        procedure Perform_Activity (acc_This : Pendulum_cptr) is
            this : Pendulum'class renames acc_this.all;
            currentState : State_Type renames this.currentState;
        begin
            case currentState is
            when ST_NotMoving =>
                --TRC Activity for state ST_NotMoving
                -- Activity: 
                --#ACD# M(UDACT::UID_B3C9A157-6EAE-11d4-8044-00104BF16798) Action Code
                null;   -- user defined code to be added here ...
                --#end ACD#
            when ST_IsLeft =>
                --TRC Activity for state ST_IsLeft
                -- Activity: 
                --#ACD# M(UDACT::UID_B3C9A158-6EAE-11d4-8044-00104BF16798) Action Code
                null;   -- user defined code to be added here ...
                --#end ACD#
            when ST_IsRight =>
                --TRC Activity for state ST_IsRight
                -- Activity: 
                --#ACD# M(UDACT::UID_B3C9A159-6EAE-11d4-8044-00104BF16798) Action Code
                null;   -- user defined code to be added here ...
                --#end ACD#

            end case;
        end Perform_Activity;

        -- Action procedure bodies

            procedure ST_NotMoving_Action_move_Event (acc_This : Pendulum_cptr) is
                this : Pendulum'class renames acc_this.all;
                currentState : State_Type renames this.currentState; 
            begin
                if TRUE then
                    
                        -- send tick to clockwork
                        Clockwork_pkg.Clockwork_Signals.Send_tick_Event (This.clockwork_asc.all'access);
                    --TRC Entering state ST_IsLeft
                        currentState := ST_IsLeft;

                end if;
            end ST_NotMoving_Action_move_Event;
            procedure ST_IsLeft_Action_move_Event (acc_This : Pendulum_cptr) is
                this : Pendulum'class renames acc_this.all;
                currentState : State_Type renames this.currentState; 
            begin
                if TRUE then
                    --TRC Leaving state ST_IsLeft

                        -- send tick to clockwork
                        Clockwork_pkg.Clockwork_Signals.Send_tick_Event (This.clockwork_asc.all'access);
                    --TRC Entering state ST_IsRight
                        currentState := ST_IsRight;

                end if;
            end ST_IsLeft_Action_move_Event;
            procedure ST_IsRight_Action_move_Event (acc_This : Pendulum_cptr) is
                this : Pendulum'class renames acc_this.all;
                currentState : State_Type renames this.currentState; 
            begin
                if TRUE then
                    --TRC Leaving state ST_IsRight

                        -- send tick to clockwork
                        Clockwork_pkg.Clockwork_Signals.Send_tick_Event (This.clockwork_asc.all'access);
                    --TRC Entering state ST_IsLeft
                        currentState := ST_IsLeft;

                end if;
            end ST_IsRight_Action_move_Event;

    begin
        StateEventTable(ST_NotMoving,move_Event)
           := ST_NotMoving_Action_move_Event'access;
        StateEventTable(ST_IsLeft,move_Event)
           := ST_IsLeft_Action_move_Event'access;
        StateEventTable(ST_IsRight,move_Event)
           := ST_IsRight_Action_move_Event'access;
    end Pendulum_State_Machine;

    
    --Constructor Operations--------------------------------------------------------
    procedure InitializeSM (acc_this : Pendulum_cptr;Event : Pendulum_Event_Type) is
    begin
        case Event is

        when move_Event =>
            Take_Event(acc_This,move_Event);
            return;
        when others =>
           raise constraint_error; -- error handling generated by genHandleStateError does not work
        end case;
    end InitializeSM;

    procedure Initialize (acc_this : Pendulum_cptr; Event : Pendulum_Event_Type) is
        --#ACD# M(UDID::UID_91DD92B1-6EAE-11d4-8044-00104BF16798) Initialization Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDIB::UID_91DD92B1-6EAE-11d4-8044-00104BF16798) Initialization Body
        Acc_This.Clockwork_Asc := Clockwork_Pkg.Clockwork_Cptr (Clockwork_Pkg.Create (Clockwork_Pkg.SetTime_Event));
        --#end ACD#
        InitializeSM(acc_This, move_Event);
    end Initialize;

    function Create(Event : Pendulum_Event_Type) return Pendulum_ptr is
        acc_this : Pendulum_ptr;
        --#ACD# M(UDCD::UID_91DD92B1-6EAE-11d4-8044-00104BF16798) Create Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        acc_this := new Pendulum;
            Initialize (Pendulum_cptr (acc_this), move_Event);
            --#ACD# M(UDCB::UID_91DD92B1-6EAE-11d4-8044-00104BF16798) Create Body
               -- user defined code to be added here ...
            --#end ACD#
        return acc_this;
    end Create;

    
    --Destructor Operations---------------------------------------------------------

    procedure Finalize (acc_this : in out Pendulum) is
    -- acc_this is NOT an access, but we keep the name for backward compatibility
        --#ACD# M(UDFID::UID_91DD92B1-6EAE-11d4-8044-00104BF16798) Finalize Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDFIB::UID_91DD92B1-6EAE-11d4-8044-00104BF16798) Finalize Body
        null;   -- user defined code to be added here ...
        --#end ACD#
    end Finalize;

    procedure Free (acc_this : in out Pendulum_cptr) is
        procedure Deallocate is new Unchecked_Deallocation (Pendulum'class, Pendulum_cptr);
        --#ACD# M(UDFRD::UID_91DD92B1-6EAE-11d4-8044-00104BF16798) Free Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDFRB::UID_91DD92B1-6EAE-11d4-8044-00104BF16798) Free Body
           -- user defined code to be added here ...
        --#end ACD#

        if acc_this = NULL then return; end if;
        Finalize (acc_this.all);
        Deallocate (acc_this);
    end Free;



    --#ACD# M(UDBLD) package body last declarations
       -- user defined code to be added here ...
    --#end ACD#

--#ACD# M(UDBSP) package body statement part
begin
	null;   -- user defined code to be added here ...
--#end ACD#    
end Pendulum_pkg;
