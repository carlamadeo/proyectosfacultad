with ClockBird_pkg.Owl_pkg;
with ClockBird_pkg.Coocoo_pkg;
with Display_pkg;
with Door_pkg;
with Text_IO;
with Unchecked_Deallocation;
--#ACD# M(UDIF) includes
   -- user defined code to be added here ...
--#end ACD#

package body Clockwork_pkg is

    --#ACD# M(UDBFD) package body first declarations
       -- user defined code to be added here ...
    --#end ACD#

    --=============================================================================
    -- fixed part
    --=============================================================================

    
    --Attribute operations----------------------------------------------------------
    function gethour(this : Clockwork'class) return Integer is
    begin
        return this.hour;

    end gethour;

    procedure sethour(acc_this : Clockwork_cptr; value : Integer) is
    begin
        acc_this.hour := value;
    end sethour;

    function getminute(this : Clockwork'class) return Integer is
    begin
        return this.minute;

    end getminute;

    procedure setminute(acc_this : Clockwork_cptr; value : Integer) is
    begin
        acc_this.minute := value;
    end setminute;

    --Relation operations-----------------------------------------------------------

    function getowl_asc(this : Clockwork'class) return ClockBird_pkg.Owl_pkg.Owl_cptr is
    begin
        return this.owl_asc;

    end getowl_asc;
    procedure setowl_asc(acc_this : Clockwork_cptr; item : ClockBird_pkg.Owl_pkg.Owl_cptr) is
    begin
        acc_this.owl_asc := Item;

    end setowl_asc;

    function getcoocoo_asc(this : Clockwork'class) return ClockBird_pkg.Coocoo_pkg.Coocoo_cptr is
    begin
        return this.coocoo_asc;

    end getcoocoo_asc;
    procedure setcoocoo_asc(acc_this : Clockwork_cptr; item : ClockBird_pkg.Coocoo_pkg.Coocoo_cptr) is
    begin
        acc_this.coocoo_asc := Item;

    end setcoocoo_asc;

    function getdisplay_asc(this : Clockwork'class) return Display_pkg.Display_cptr is
    begin
        return this.display_asc;

    end getdisplay_asc;
    procedure setdisplay_asc(acc_this : Clockwork_cptr; item : Display_pkg.Display_cptr) is
    begin
        acc_this.display_asc := Item;

    end setdisplay_asc;

    function getdoor_asc(this : Clockwork'class) return Door_pkg.Door_cptr is
    begin
        return this.door_asc;

    end getdoor_asc;
    procedure setdoor_asc(acc_this : Clockwork_cptr; item : Door_pkg.Door_cptr) is
    begin
        acc_this.door_asc := Item;

    end setdoor_asc;
    
    --State Machine Operations------------------------------------------------------
    procedure Take_Event(acc_This : Clockwork_cptr; Event : Clockwork_Event_Type) is
        stateAction : Clockwork_State_Machine.Action_Procedure;
        use type Clockwork_State_Machine.Action_Procedure;
    begin
        stateAction := Clockwork_State_Machine.StateEventTable(acc_This.currentState,Event);
        if stateAction /= NULL then
            stateAction(acc_This);
            Clockwork_State_Machine.Perform_Activity (acc_This);
        else
            
            Text_IO.Put_Line ("Clockwork: illegal event "
                & Clockwork_Event_Type'image (Event) & " in state "
                & Clockwork_State_Machine.State_Type'image (acc_This.currentState));


        end if;
    end Take_Event;




    --=============================================================================
    -- user extensible part
    --=============================================================================
    

    
    package body Clockwork_State_Machine is


        --Activity Procedure------------------------------------------------------------
        procedure Perform_Activity (acc_This : Clockwork_cptr) is
            this : Clockwork'class renames acc_this.all;
            currentState : State_Type renames this.currentState;
        begin
            case currentState is
            when ST_SolidHourAtNight =>
                --TRC Activity for state ST_SolidHourAtNight
                -- Activity: 
                --#ACD# M(UDACT::UID_B3C9A144-6EAE-11d4-8044-00104BF16798) Action Code
                null;   -- user defined code to be added here ...
                --#end ACD#
            when ST_NotSolidHour =>
                --TRC Activity for state ST_NotSolidHour
                -- Activity: 
                --#ACD# M(UDACT::UID_B3C9A143-6EAE-11d4-8044-00104BF16798) Action Code
                null;   -- user defined code to be added here ...
                --#end ACD#
            when ST_SolidHourAtDay =>
                --TRC Activity for state ST_SolidHourAtDay
                -- Activity: 
                --#ACD# M(UDACT::UID_7A469DB1-6EAE-11d4-8044-00104BF16798) Action Code
                null;   -- user defined code to be added here ...
                --#end ACD#
            when ST_NotMoving =>
                --TRC Activity for state ST_NotMoving
                -- Activity: 
                --#ACD# M(UDACT::UID_B3C9A141-6EAE-11d4-8044-00104BF16798) Action Code
                null;   -- user defined code to be added here ...
                --#end ACD#

            end case;
        end Perform_Activity;

        -- Action procedure bodies

            procedure ST_SolidHourAtNight_Action_tick_Event (acc_This : Clockwork_cptr) is
                this : Clockwork'class renames acc_this.all;
                currentState : State_Type renames this.currentState; 
            begin
                if notSolidHour(this'access) then
                    --TRC Leaving state ST_SolidHourAtNight
                    showTime(this'access);addMinute(this'access,15);

                    --TRC Entering state ST_NotSolidHour
                        currentState := ST_NotSolidHour;

                end if;
            end ST_SolidHourAtNight_Action_tick_Event;
            procedure ST_NotSolidHour_Action_tick_Event (acc_This : Clockwork_cptr) is
                this : Clockwork'class renames acc_this.all;
                currentState : State_Type renames this.currentState; 
            begin
                if solidHourAtDay(this'access) then
                    --TRC Leaving state ST_NotSolidHour

                        showTime(this'access);
                        addMinute(this'access,15);
                        Door_pkg.open(this.door_asc);
                        ClockBird_pkg.Coocoo_pkg.cry(this.coocoo_asc,this.hour);
                        Door_pkg.close(this.door_asc);

                    --TRC Entering state ST_SolidHourAtDay
                        currentState := ST_SolidHourAtDay;

                elsif solidHourAtNight(this'access) then
                    --TRC Leaving state ST_NotSolidHour

                        showTime(this'access);
                        addMinute(this'access,15);
                        Door_pkg.open(this.door_asc);
                        ClockBird_pkg.Owl_pkg.cry(this.owl_asc,this.hour);
                        Door_pkg.close(this.door_asc);

                    --TRC Entering state ST_SolidHourAtNight
                        currentState := ST_SolidHourAtNight;

                elsif notSolidHour(this'access) then
                    --TRC Leaving state ST_NotSolidHour
                    showTime(this'access);addMinute(this'access,15);

                    --TRC Entering state ST_NotSolidHour
                        currentState := ST_NotSolidHour;

                end if;
            end ST_NotSolidHour_Action_tick_Event;
            procedure ST_SolidHourAtDay_Action_tick_Event (acc_This : Clockwork_cptr) is
                this : Clockwork'class renames acc_this.all;
                currentState : State_Type renames this.currentState; 
            begin
                if notSolidHour(this'access) then
                    --TRC Leaving state ST_SolidHourAtDay
                    showTime(this'access);addMinute(this'access,15);

                    --TRC Entering state ST_NotSolidHour
                        currentState := ST_NotSolidHour;

                end if;
            end ST_SolidHourAtDay_Action_tick_Event;
            procedure ST_NotMoving_Action_setTime_Event (acc_This : Clockwork_cptr) is
                this : Clockwork'class renames acc_this.all;
                currentState : State_Type renames this.currentState; 
            begin
                if TRUE then
                    sethour(this'access,13);setminute(this'access,15);

                    --TRC Entering state ST_NotSolidHour
                        currentState := ST_NotSolidHour;

                end if;
            end ST_NotMoving_Action_setTime_Event;

    begin
        StateEventTable(ST_SolidHourAtNight,tick_Event)
           := ST_SolidHourAtNight_Action_tick_Event'access;
        StateEventTable(ST_NotSolidHour,tick_Event)
           := ST_NotSolidHour_Action_tick_Event'access;
        StateEventTable(ST_SolidHourAtDay,tick_Event)
           := ST_SolidHourAtDay_Action_tick_Event'access;
        StateEventTable(ST_NotMoving,setTime_Event)
           := ST_NotMoving_Action_setTime_Event'access;
    end Clockwork_State_Machine;

    
    --Constructor Operations--------------------------------------------------------
    procedure InitializeSM (acc_this : Clockwork_cptr;Event : Clockwork_Event_Type) is
    begin
        case Event is

        when setTime_Event =>
            Take_Event(acc_This,setTime_Event);
            return;
        when others =>
           raise constraint_error; -- error handling generated by genHandleStateError does not work
        end case;
    end InitializeSM;

    procedure Initialize (acc_this : Clockwork_cptr; Event : Clockwork_Event_Type) is
        --#ACD# M(UDID::UID_91DD92B0-6EAE-11d4-8044-00104BF16798) Initialization Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDIB::UID_91DD92B0-6EAE-11d4-8044-00104BF16798) Initialization Body
        Acc_This.Owl_Asc := ClockBird_Pkg.Owl_Pkg.Owl_Cptr (ClockBird_Pkg.Owl_Pkg.Create );
    
        Acc_This.Coocoo_Asc := ClockBird_Pkg.Coocoo_Pkg.Coocoo_Cptr (ClockBird_Pkg.Coocoo_Pkg.Create );
    
        Acc_This.Display_Asc := Display_Pkg.Display_Cptr (Display_Pkg.Create );
    
        Acc_This.Door_Asc := Door_Pkg.Door_Cptr (Door_Pkg.Create );
        --#end ACD#
        InitializeSM(acc_This, setTime_Event);
    end Initialize;

    function Create(Event : Clockwork_Event_Type) return Clockwork_ptr is
        acc_this : Clockwork_ptr;
        --#ACD# M(UDCD::UID_91DD92B0-6EAE-11d4-8044-00104BF16798) Create Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        acc_this := new Clockwork;
            Initialize (Clockwork_cptr (acc_this), setTime_Event);
            --#ACD# M(UDCB::UID_91DD92B0-6EAE-11d4-8044-00104BF16798) Create Body
               -- user defined code to be added here ...
            --#end ACD#
        return acc_this;
    end Create;

    
    --Destructor Operations---------------------------------------------------------

    procedure Finalize (acc_this : in out Clockwork) is
    -- acc_this is NOT an access, but we keep the name for backward compatibility
        --#ACD# M(UDFID::UID_91DD92B0-6EAE-11d4-8044-00104BF16798) Finalize Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDFIB::UID_91DD92B0-6EAE-11d4-8044-00104BF16798) Finalize Body
        null;   -- user defined code to be added here ...
        --#end ACD#
    end Finalize;

    procedure Free (acc_this : in out Clockwork_cptr) is
        procedure Deallocate is new Unchecked_Deallocation (Clockwork'class, Clockwork_cptr);
        --#ACD# M(UDFRD::UID_91DD92B0-6EAE-11d4-8044-00104BF16798) Free Declarations
           -- user defined code to be added here ...
        --#end ACD#
    begin
        --#ACD# M(UDFRB::UID_91DD92B0-6EAE-11d4-8044-00104BF16798) Free Body
           -- user defined code to be added here ...
        --#end ACD#

        if acc_this = NULL then return; end if;
        Finalize (acc_this.all);
        Deallocate (acc_this);
    end Free;

    --Operations--------------------------------------------------------------------

    
    --------------------------------------------------------------------------------
    -- Operation :        addMinute
    --
    -- Description :    
    --
    --------------------------------------------------------------------------------
    procedure addMinute(acc_this : access Clockwork; m : IN Integer) is
    begin
       if acc_this.minute + m = 60 then
          acc_this.minute := 0;
          if acc_this.hour + 1 = 24 then
             acc_this.hour := 0;
          else
             acc_this.hour := acc_this.hour + 1;
          end if;
       else
          acc_this.minute := acc_this.minute + m;
       end if;
    end addMinute;



    
    --------------------------------------------------------------------------------
    -- Operation :        showTime
    --
    -- Description :    
    --
    --------------------------------------------------------------------------------
    procedure showTime(acc_this : access Clockwork) is
    begin
       display_pkg.showTime(acc_this.display_asc, acc_this.hour, acc_this.minute);
    end showTime;



    
    --------------------------------------------------------------------------------
    -- Operation :        solidHourAtDay
    --
    -- Description :    
    --
    --------------------------------------------------------------------------------
    function solidHourAtDay(acc_this : access Clockwork) return Boolean is
    begin
       return acc_this.hour in 6 .. 18 and then acc_this.minute = 0;
    end solidHourAtDay;



    
    --------------------------------------------------------------------------------
    -- Operation :        solidHourAtNight
    --
    -- Description :    
    --
    --------------------------------------------------------------------------------
    function solidHourAtNight(acc_this : access Clockwork) return Boolean is
    begin
       return acc_this.hour not in 6 .. 18 and then acc_this.minute = 0;
    end solidHourAtNight;



    
    --------------------------------------------------------------------------------
    -- Operation :        notSolidHour
    --
    -- Description :    
    --
    --------------------------------------------------------------------------------
    function notSolidHour(acc_this : access Clockwork) return Boolean is
    begin
       return acc_this.minute /= 0;
    end notSolidHour;



    
    --------------------------------------------------------------------------------
    -- Operation :        start
    --
    -- Description :    send  pendulum.go
    --
    --------------------------------------------------------------------------------
    procedure start(acc_this : access Clockwork) is
    --#ACD# M(UDOI::UID_91DD92B5-6EAE-11d4-8044-00104BF16798) procedure code
    begin
	null;   -- user defined code to be added here ...
    --#end ACD#
    end start;




    --#ACD# M(UDBLD) package body last declarations
       -- user defined code to be added here ...
    --#end ACD#

--#ACD# M(UDBSP) package body statement part
begin
	null;   -- user defined code to be added here ...
--#end ACD#    
end Clockwork_pkg;
