package ACD_Runtime.Synchronized_counter_pkg is
    
    -- These counters are essentially natural numbers that can be incremented and decremented
    -- under mutual exclusion.
    -- They have an initial value of ONE by default.
    -- This is because when you create them you have one reference, apparently;
    -- thus we need not explicitly increment them upon their first use.
    --
    -- They can be incremented and decremented under mutual exclusion.
    -- We do NOT implement the reference counter itself as a protected object but use one global PO to synchronize
    -- This is done because we want to avoid the overhead (in terms of memory and lock resources)
    -- and because the operations are extremely fast.
    
    type counter is limited private;

    type counter_access is access all counter;
    -- We must pass an access to make sure the assignment is made under mutual exclusion.
    
    procedure increment(item : counter_access);
    
    function decrement_returns_zero (item : counter_access) return boolean;
    
    -- pragma inline(increment, decrement_returns_zero);

private
    -- Using a record type we can apply an initial value.
    type counter is record
        i : natural := 1;
        end record;
end ACD_Runtime.Synchronized_counter_pkg;    
