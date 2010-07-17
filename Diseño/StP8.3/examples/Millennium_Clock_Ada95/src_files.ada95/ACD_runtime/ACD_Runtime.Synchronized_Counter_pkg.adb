package body ACD_Runtime.Synchronized_counter_pkg is
    
    protected mutex is
        procedure increment(item : in out counter);
        
        procedure decrement_returns_zero (item : in out counter; result : out boolean);
    end mutex;
        

    protected body mutex is
        
        procedure increment(item : in out counter) is
        begin
            item.i := item.i + 1;
        end increment;

        procedure decrement_returns_zero (item : in out counter; result : out boolean) is
        begin
            item.i := item.i - 1;
            result := item.i = 0;
        end decrement_returns_zero;

    end mutex;
    

    procedure increment(item : counter_access) is
    begin
        mutex.increment (item.all);
    end increment;
    
    function decrement_returns_zero (item : counter_access) return boolean is
        result : boolean;
    begin
        mutex.decrement_returns_zero (item.all, result);
        return result;
    end decrement_returns_zero;
    
end ACD_Runtime.Synchronized_counter_pkg;
