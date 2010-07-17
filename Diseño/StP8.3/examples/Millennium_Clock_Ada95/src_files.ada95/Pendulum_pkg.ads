with ACD_Runtime.Active_Base_Class;
with Clockwork_pkg;
with Clockwork_pkg.Clockwork_Signals;
--#ACD# M(UDIF) includes
   -- user defined code to be added here ...
--#end ACD#
package Pendulum_pkg is

    --#ACD# M(UDUFPD) package first public declarations
       -- user defined code to be added here ...
    --#end ACD#

    
    type Pendulum is new ACD_Runtime.Active_Base_Class.ActiveInstance with private;
    type Pendulum_cptr is access all Pendulum'class;
    type Pendulum_ptr is access all Pendulum;


    --#ACD# M(UDUSPD) package second public declarations
       -- user defined code to be added here ...
    --#end ACD#
    

    --=============================================================================
    -- Class Pendulum
    --=============================================================================
    
    --event type--------------------------------------------------------------------
    type Pendulum_Event_Type is (
          move_Event
        );
    
    
    --Constructor Operations--------------------------------------------------------
    procedure Initialize (acc_this : Pendulum_cptr; Event : Pendulum_Event_Type);
        -- pragma inline(Initialize);

    function Create(Event : Pendulum_Event_Type) return Pendulum_ptr;
        -- pragma inline(Create);
    
    --Destructor Operations---------------------------------------------------------

    procedure Finalize (Acc_This : in out Pendulum);
        -- pragma inline(Finalize);

    procedure Free (acc_this : in out Pendulum_cptr);
        -- pragma inline(Free);

    --Relation operations-----------------------------------------------------------

    function getclockwork_asc(this : Pendulum'class) return Clockwork_pkg.Clockwork_cptr;
        -- pragma inline(getclockwork_asc);
    procedure setclockwork_asc(acc_this : Pendulum_cptr; item : Clockwork_pkg.Clockwork_cptr);
        -- pragma inline(setclockwork_asc);
    function Get_clockwork_asc(this : Pendulum'class) return Clockwork_pkg.Clockwork_cptr
        renames getclockwork_asc;
    procedure Set_clockwork_asc(acc_this : Pendulum_cptr; item : Clockwork_pkg.Clockwork_cptr)
        renames setclockwork_asc;

    
    --State Machine Operations------------------------------------------------------
    procedure Take_Event(acc_This : Pendulum_cptr; Event : Pendulum_Event_Type);

    --#ACD# M(UDULPD) package last public declarations
       -- user defined code to be added here ...
    --#end ACD#
private

    --#ACD# M(UDPFPD) package first private declarations
       -- user defined code to be added here ...
    --#end ACD#
    

    --=============================================================================
    --Class Pendulum
    --=============================================================================
    

    package Pendulum_State_Machine is

        --State Type--------------------------------------------------------------------
        type State_Type is (
                ST_NotMoving
                , ST_IsLeft
                , ST_IsRight
            );

        -- Action procedures
            procedure ST_NotMoving_Action_move_Event (acc_This : Pendulum_cptr);
                -- pragma inline(ST_NotMoving_Action_move_Event);
            procedure ST_IsLeft_Action_move_Event (acc_This : Pendulum_cptr);
                -- pragma inline(ST_IsLeft_Action_move_Event);
            procedure ST_IsRight_Action_move_Event (acc_This : Pendulum_cptr);
                -- pragma inline(ST_IsRight_Action_move_Event);

        procedure Perform_Activity (acc_This : Pendulum_cptr);

        type Action_Procedure is access procedure (acc_This : Pendulum_cptr);
        StateEventTable : array (State_Type,Pendulum_Event_Type) of Action_Procedure;

    end Pendulum_State_Machine;  
    
    
    --Class Record------------------------------------------------------------------
    type Pendulum is new ACD_Runtime.Active_Base_Class.ActiveInstance with
        record
            --Relations---------------------------------------------------------------------
            clockwork_asc : Clockwork_pkg.Clockwork_cptr;
            --State Machine-----------------------------------------------------------------
            currentState : Pendulum_State_Machine.State_Type := Pendulum_State_Machine.ST_NotMoving;
            --#ACD# M(UDCOMP::UID_91DD92B1-6EAE-11d4-8044-00104BF16798) User defined components
               -- user defined code to be added here ...
            --#end ACD#
        end record;


    --#ACD# M(UDPLPD) package last private declarations
       -- user defined code to be added here ...
    --#end ACD#

end Pendulum_pkg;
    
