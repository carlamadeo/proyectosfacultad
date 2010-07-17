with ClockBird_pkg;
--#ACD# M(UDIF) includes
   -- user defined code to be added here ...
--#end ACD#
package ClockBird_pkg.Coocoo_pkg is

    --#ACD# M(UDUFPD) package first public declarations
       -- user defined code to be added here ...
    --#end ACD#

    
    type Coocoo is new ClockBird_pkg.ClockBird with private;
    type Coocoo_cptr is access all Coocoo'class;
    type Coocoo_ptr is access all Coocoo;


    --#ACD# M(UDUSPD) package second public declarations
       -- user defined code to be added here ...
    --#end ACD#
    

    --=============================================================================
    -- Class Coocoo
    --=============================================================================
    
    
    --Constructor Operations--------------------------------------------------------
    procedure Initialize (acc_this : Coocoo_cptr);
        -- pragma inline(Initialize);

    function Create return Coocoo_ptr;
        -- pragma inline(Create);
    
    --Destructor Operations---------------------------------------------------------

    procedure Finalize (Acc_This : in out Coocoo);
        -- pragma inline(Finalize);

    procedure Free (acc_this : in out Coocoo_cptr);
        -- pragma inline(Free);

    --Operations--------------------------------------------------------------------
    procedure cry(acc_this : access Coocoo; hour : IN Integer);

    --#ACD# M(UDULPD) package last public declarations
       -- user defined code to be added here ...
    --#end ACD#
private

    --#ACD# M(UDPFPD) package first private declarations
       -- user defined code to be added here ...
    --#end ACD#
    

    --=============================================================================
    --Class Coocoo
    --=============================================================================
    
    
    --Class Record------------------------------------------------------------------
    type Coocoo is new ClockBird_pkg.ClockBird with
        record
            --#ACD# M(UDCOMP::UID_91DD92BA-6EAE-11d4-8044-00104BF16798) User defined components
            null; -- Delete this line if you add your own components   -- user defined code to be added here ...
            --#end ACD#
        end record;


    --#ACD# M(UDPLPD) package last private declarations
       -- user defined code to be added here ...
    --#end ACD#

end ClockBird_pkg.Coocoo_pkg;
    
