with ACD_Runtime.Active_Base_Class;
with Pendulum_pkg;
with Pendulum_pkg.Pendulum_Signals;
--#ACD# M(UDIF) includes
   -- user defined code to be added here ...
--#end ACD#
package Weight is

    --#ACD# M(UDUFPD) package first public declarations
       -- user defined code to be added here ...
    --#end ACD#

    
    type Weight is new ACD_Runtime.Active_Base_Class.ActiveInstance with private;
    type Weight_cptr is access all Weight'class;
    type Weight_ptr is access all Weight;


    --#ACD# M(UDUSPD) package second public declarations
       -- user defined code to be added here ...
    --#end ACD#
    

    --=============================================================================
    -- Class Weight
    --=============================================================================
    
    --event type--------------------------------------------------------------------
    type Weight_Event_Type is (
          raise_Event
        , sink_Event
        );
    
    
    --Constructor Operations--------------------------------------------------------
    procedure Initialize (acc_this : Weight_cptr; Event : Weight_Event_Type);
        -- pragma inline(Initialize);

    function Create(Event : Weight_Event_Type) return Weight_ptr;
        -- pragma inline(Create);
    
    --Destructor Operations---------------------------------------------------------

    procedure Finalize (Acc_This : in out Weight);
        -- pragma inline(Finalize);

    procedure Free (acc_this : in out Weight_cptr);
        -- pragma inline(Free);

    --Operations--------------------------------------------------------------------
    procedure decrease(acc_this : access Weight; e : IN Integer);
    procedure energize(acc_this : access Weight; e : IN Integer);
    function hasEnergy(acc_this : access Weight; e : IN Integer) return Boolean;
    --Relation operations-----------------------------------------------------------

    function getpend_asc(this : Weight'class) return Pendulum_pkg.Pendulum_cptr;
        -- pragma inline(getpend_asc);
    procedure setpend_asc(acc_this : Weight_cptr; item : Pendulum_pkg.Pendulum_cptr);
        -- pragma inline(setpend_asc);
    function Get_pend_asc(this : Weight'class) return Pendulum_pkg.Pendulum_cptr
        renames getpend_asc;
    procedure Set_pend_asc(acc_this : Weight_cptr; item : Pendulum_pkg.Pendulum_cptr)
        renames setpend_asc;

    --#ACD# M(UDULPD) package last public declarations
       -- user defined code to be added here ...
    --#end ACD#
private

    --#ACD# M(UDPFPD) package first private declarations
       -- user defined code to be added here ...
    --#end ACD#
    

    --=============================================================================
    --Class Weight
    --=============================================================================
    

    package Weight_State_Machine is

        --State Type--------------------------------------------------------------------
        type State_Type is (
                ST_hasEnergy
                , ST_hasNoEnergy
            );

        
        --State Machine Operations------------------------------------------------------
        procedure Take_raise_Event(acc_This : Weight_cptr; e : Integer := 1000);
            -- pragma inline(Take_raise_Event);
        procedure Take_sink_Event(acc_This : Weight_cptr; e : Integer := 10);
            -- pragma inline(Take_sink_Event);
    end Weight_State_Machine;  
    
    
    --Class Record------------------------------------------------------------------
    type Weight is new ACD_Runtime.Active_Base_Class.ActiveInstance with
        record
            --Attributes--------------------------------------------------------------------
            Energy : Integer;
            --Relations---------------------------------------------------------------------
            pend_asc : Pendulum_pkg.Pendulum_cptr;
            --State Machine-----------------------------------------------------------------
            currentState : Weight_State_Machine.State_Type := Weight_State_Machine.ST_hasNoEnergy;
            --#ACD# M(UDCOMP::UID_91DD92BE-6EAE-11d4-8044-00104BF16798) User defined components
               -- user defined code to be added here ...
            --#end ACD#
        end record;


    --#ACD# M(UDPLPD) package last private declarations
       -- user defined code to be added here ...
    --#end ACD#

end Weight;
    
