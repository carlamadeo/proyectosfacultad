with ACD_Runtime.Active_Base_Class;
with ClockBird_pkg.Owl_pkg;
with ClockBird_pkg.Coocoo_pkg;
with Display_pkg;
with Door_pkg;
--#ACD# M(UDIF) includes
   -- user defined code to be added here ...
--#end ACD#
package Clockwork_pkg is

    --#ACD# M(UDUFPD) package first public declarations
       -- user defined code to be added here ...
    --#end ACD#

    
    type Clockwork is new ACD_Runtime.Active_Base_Class.ActiveInstance with private;
    type Clockwork_cptr is access all Clockwork'class;
    type Clockwork_ptr is access all Clockwork;


    --#ACD# M(UDUSPD) package second public declarations
       -- user defined code to be added here ...
    --#end ACD#
    

    --=============================================================================
    -- Class Clockwork
    --=============================================================================
    
    --event type--------------------------------------------------------------------
    type Clockwork_Event_Type is (
          setTime_Event
        , tick_Event
        );
    
    
    --Constructor Operations--------------------------------------------------------
    procedure Initialize (acc_this : Clockwork_cptr; Event : Clockwork_Event_Type);
        -- pragma inline(Initialize);

    function Create(Event : Clockwork_Event_Type) return Clockwork_ptr;
        -- pragma inline(Create);
    
    --Destructor Operations---------------------------------------------------------

    procedure Finalize (Acc_This : in out Clockwork);
        -- pragma inline(Finalize);

    procedure Free (acc_this : in out Clockwork_cptr);
        -- pragma inline(Free);

    --Operations--------------------------------------------------------------------
    procedure addMinute(acc_this : access Clockwork; m : IN Integer);
    procedure showTime(acc_this : access Clockwork);
    function solidHourAtDay(acc_this : access Clockwork) return Boolean;
    function solidHourAtNight(acc_this : access Clockwork) return Boolean;
    function notSolidHour(acc_this : access Clockwork) return Boolean;
    procedure start(acc_this : access Clockwork);
    --Relation operations-----------------------------------------------------------

    function getowl_asc(this : Clockwork'class) return ClockBird_pkg.Owl_pkg.Owl_cptr;
        -- pragma inline(getowl_asc);
    procedure setowl_asc(acc_this : Clockwork_cptr; item : ClockBird_pkg.Owl_pkg.Owl_cptr);
        -- pragma inline(setowl_asc);
    function Get_owl_asc(this : Clockwork'class) return ClockBird_pkg.Owl_pkg.Owl_cptr
        renames getowl_asc;
    procedure Set_owl_asc(acc_this : Clockwork_cptr; item : ClockBird_pkg.Owl_pkg.Owl_cptr)
        renames setowl_asc;

    function getcoocoo_asc(this : Clockwork'class) return ClockBird_pkg.Coocoo_pkg.Coocoo_cptr;
        -- pragma inline(getcoocoo_asc);
    procedure setcoocoo_asc(acc_this : Clockwork_cptr; item : ClockBird_pkg.Coocoo_pkg.Coocoo_cptr);
        -- pragma inline(setcoocoo_asc);
    function Get_coocoo_asc(this : Clockwork'class) return ClockBird_pkg.Coocoo_pkg.Coocoo_cptr
        renames getcoocoo_asc;
    procedure Set_coocoo_asc(acc_this : Clockwork_cptr; item : ClockBird_pkg.Coocoo_pkg.Coocoo_cptr)
        renames setcoocoo_asc;

    function getdisplay_asc(this : Clockwork'class) return Display_pkg.Display_cptr;
        -- pragma inline(getdisplay_asc);
    procedure setdisplay_asc(acc_this : Clockwork_cptr; item : Display_pkg.Display_cptr);
        -- pragma inline(setdisplay_asc);
    function Get_display_asc(this : Clockwork'class) return Display_pkg.Display_cptr
        renames getdisplay_asc;
    procedure Set_display_asc(acc_this : Clockwork_cptr; item : Display_pkg.Display_cptr)
        renames setdisplay_asc;

    function getdoor_asc(this : Clockwork'class) return Door_pkg.Door_cptr;
        -- pragma inline(getdoor_asc);
    procedure setdoor_asc(acc_this : Clockwork_cptr; item : Door_pkg.Door_cptr);
        -- pragma inline(setdoor_asc);
    function Get_door_asc(this : Clockwork'class) return Door_pkg.Door_cptr
        renames getdoor_asc;
    procedure Set_door_asc(acc_this : Clockwork_cptr; item : Door_pkg.Door_cptr)
        renames setdoor_asc;
    
    --State Machine Operations------------------------------------------------------
    procedure Take_Event(acc_This : Clockwork_cptr; Event : Clockwork_Event_Type);

    --#ACD# M(UDULPD) package last public declarations
       -- user defined code to be added here ...
    --#end ACD#
private

    --#ACD# M(UDPFPD) package first private declarations
       -- user defined code to be added here ...
    --#end ACD#
    

    --=============================================================================
    --Class Clockwork
    --=============================================================================
    

    package Clockwork_State_Machine is

        --State Type--------------------------------------------------------------------
        type State_Type is (
                ST_SolidHourAtNight
                , ST_NotSolidHour
                , ST_SolidHourAtDay
                , ST_NotMoving
            );

        -- Action procedures
            procedure ST_SolidHourAtNight_Action_tick_Event (acc_This : Clockwork_cptr);
                -- pragma inline(ST_SolidHourAtNight_Action_tick_Event);
            procedure ST_NotSolidHour_Action_tick_Event (acc_This : Clockwork_cptr);
                -- pragma inline(ST_NotSolidHour_Action_tick_Event);
            procedure ST_SolidHourAtDay_Action_tick_Event (acc_This : Clockwork_cptr);
                -- pragma inline(ST_SolidHourAtDay_Action_tick_Event);
            procedure ST_NotMoving_Action_setTime_Event (acc_This : Clockwork_cptr);
                -- pragma inline(ST_NotMoving_Action_setTime_Event);

        procedure Perform_Activity (acc_This : Clockwork_cptr);

        type Action_Procedure is access procedure (acc_This : Clockwork_cptr);
        StateEventTable : array (State_Type,Clockwork_Event_Type) of Action_Procedure;

    end Clockwork_State_Machine;  
    
    
    --Class Record------------------------------------------------------------------
    type Clockwork is new ACD_Runtime.Active_Base_Class.ActiveInstance with
        record
            --Attributes--------------------------------------------------------------------
            hour : Integer;
            minute : Integer;
            --Relations---------------------------------------------------------------------
            owl_asc : ClockBird_pkg.Owl_pkg.Owl_cptr;
            coocoo_asc : ClockBird_pkg.Coocoo_pkg.Coocoo_cptr;
            display_asc : Display_pkg.Display_cptr;
            door_asc : Door_pkg.Door_cptr;
            --State Machine-----------------------------------------------------------------
            currentState : Clockwork_State_Machine.State_Type := Clockwork_State_Machine.ST_NotMoving;
            --#ACD# M(UDCOMP::UID_91DD92B0-6EAE-11d4-8044-00104BF16798) User defined components
               -- user defined code to be added here ...
            --#end ACD#
        end record;


    --#ACD# M(UDPLPD) package last private declarations
       -- user defined code to be added here ...
    --#end ACD#

end Clockwork_pkg;
    
