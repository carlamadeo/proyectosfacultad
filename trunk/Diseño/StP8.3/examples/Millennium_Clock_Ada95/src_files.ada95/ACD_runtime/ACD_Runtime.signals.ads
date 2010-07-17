package ACD_Runtime.Signals is
    
    type Priority_Type is (Normal, High, Sdbg);
    type Signal is abstract tagged private;
    
    procedure Send (S : Signal'class; Priority : Priority_Type := Normal) is abstract;
    
    function Image (S : Signal) return String is abstract;

    
private
    
    type Signal is abstract tagged null record;
   
end ACD_Runtime.Signals;
