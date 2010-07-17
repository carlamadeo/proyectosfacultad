with Unchecked_Deallocation;
--T with Ada.Tags;
--T with Text_IO;
package body ACD_Runtime.Signal_Queue is

    Max_Pending_Normal : constant := 10;
    Max_Pending_High : constant := 5;
    Max_Pending_Sdbg : constant := 1;
    
    type Count_Type is range 0 .. Max_Pending_Normal + Max_Pending_High + Max_Pending_Sdbg;
    subtype Index_Type is Count_Type range 1 .. Count_Type'last;
    type Signal_Ptr is access all Signals.Signal'class;
    type Signal_Array is array (Index_Type range <>) of Signal_Ptr; 
    subtype queue_image_string is string (1 .. 10_000);
    
    To_Be_Freed : Signal_Ptr;

    protected Queue is
        entry Enqueue_Normal (S : Signal_Ptr);
        entry Enqueue_High (S : Signal_Ptr);
        entry Enqueue_Sdbg (S : Signal_Ptr);
        entry Dequeue (S : out Signal_Ptr);
        procedure Image (str : out queue_image_string; Last_Char : out positive);
    private
        Normal_Signals : Signal_Array (1 .. Max_Pending_Normal);  
        Normal_Read_Pos, 
        Normal_Write_Pos : Index_Type := Index_Type'first;
        Normal_Count : Count_Type := 0;
        
        High_Signals : Signal_Array (1 .. Max_Pending_High);  
        High_Read_Pos, 
        High_Write_Pos : Index_Type := Index_Type'first;
        High_Count : Count_Type := 0;
        
        Sdbg_Signals : Signal_Array (1 .. Max_Pending_Sdbg);  
        Sdbg_Read_Pos, 
        Sdbg_Write_Pos : Index_Type := Index_Type'first;
        Sdbg_Count : Count_Type := 0;
        
        Total_Count : Count_Type := 0;
    end Queue;
    
    procedure Enqueue (S : Signals.Signal'class; Priority : Signals.Priority_Type) is
        P : Signal_Ptr := new Signals.Signal'class'(S);
    begin
        case Priority is
        when Signals.Normal =>
           Queue.Enqueue_Normal (P);
        when Signals.High =>
           Queue.Enqueue_High (P);
        when Signals.Sdbg =>
           Queue.Enqueue_Sdbg (P);
        end case;
    end Enqueue;
    
    function Dequeue return Signals.Signal'class is
    procedure Free is new Unchecked_Deallocation (Signals.Signal'class, Signal_Ptr);
        Result_Ptr : Signal_Ptr;
    begin
        if To_Be_Freed /= null then
            Free (To_Be_Freed);
        end if;
        Queue.Dequeue (Result_Ptr);
        To_Be_Freed := Result_Ptr;
        return Result_Ptr.all;
    end Dequeue;
    
    function Image return String is
        str : queue_image_string;
        lc : positive;
    begin
        Queue.Image(str, LC);
        return Str(1 .. LC);
    end Image;
    
    procedure Inc (POS : in out Index_Type; Last : Index_Type) is
    begin
        if POS = Last then
           POS := Index_Type'first;
        else
           POS := POS + 1;
        end if;
    end Inc;             
    
    protected body Queue is

        entry Enqueue_Normal (S : Signal_Ptr) when Normal_Count < Max_Pending_Normal is
        begin
            --T text_io.put_line ("Enqueue_Normal " 
            --T       & ada.tags.external_tag (s.all'tag));
            Normal_Signals (Normal_Write_POS) := S;
            Inc (Normal_Write_POS, Max_Pending_Normal);
            Normal_Count := Normal_Count + 1;
            Total_Count := Total_Count + 1;
        end Enqueue_Normal;
        
        entry Enqueue_High (S : Signal_Ptr) when High_Count < Max_Pending_High is
        begin
            --T text_io.put_line ("Enqueue_High " 
            --T       & ada.tags.external_tag (s.all'tag));
            High_Signals (High_Write_POS) := S;
            Inc (High_Write_POS, Max_Pending_High);
            High_Count := High_Count + 1;
            Total_Count := Total_Count + 1;
        end Enqueue_High;
        
        entry Enqueue_Sdbg (S : Signal_Ptr) when Sdbg_Count < Max_Pending_Sdbg is
        begin
            --T text_io.put_line ("Enqueue_Sdbg " 
            --T       & ada.tags.external_tag (s.all'tag));
            Sdbg_Signals (Sdbg_Write_POS) := S;
            Inc (Sdbg_Write_POS, Max_Pending_Sdbg);
            Sdbg_Count := Sdbg_Count + 1;
            Total_Count := Total_Count + 1;
        end Enqueue_Sdbg;

        entry Dequeue (S : out Signal_Ptr) when Total_Count > 0 is
        begin
            if Sdbg_Count > 0 then
               S := Sdbg_Signals (Sdbg_Read_POS);
               --T text_io.put_line ("Dequeue_Sdbg " 
               --T       & ada.tags.external_tag (s.all'tag));
               Inc (Sdbg_Read_POS, Max_Pending_Sdbg);
               Sdbg_Count := Sdbg_Count - 1;
               Total_Count := Total_Count - 1;
               return;
            end if;   
            if High_Count > 0 then
               S := High_Signals (High_Read_POS);
               --T text_io.put_line ("Dequeue_High " 
               --T       & ada.tags.external_tag (s.all'tag));
               Inc (High_Read_POS, Max_Pending_High);
               High_Count := High_Count - 1;
               Total_Count := Total_Count - 1;
               return;
            end if;   
            if Normal_Count > 0 then
               S := Normal_Signals (Normal_Read_POS);
               --T text_io.put_line ("Dequeue_Normal " 
               --T       & ada.tags.external_tag (s.all'tag));
               Inc (Normal_Read_POS, Max_Pending_Normal);
               Normal_Count := Normal_Count - 1;
               Total_Count := Total_Count - 1;
               return;
            end if;   
            raise Program_Error; 
        end Dequeue;
        
        procedure Image (str : out queue_image_string; Last_char : out positive) is
            Result : queue_image_string; POS : Positive := 1;
            
            procedure Add (Read_Pos : Index_Type; Count : Count_Type;
                           The_Signals : in out Signal_Array) is
               RPos : Index_Type := Read_POS;
            
               procedure Add (Item : String) is
               begin
                  Result (POS .. POS + Item'length-1) := Item;
                  POS := POS + Item'length;
               end Add;
                    
            begin
               for I in 1 .. Count loop
                  Add (Signals.Image (The_Signals (RPos).all));
                  Result (POS) := ASCII.lf; POS := POS + 1;
                  Inc (RPos, The_Signals'last);
               end loop;   
            end Add;
            
        begin
            Add (Sdbg_Read_Pos, Sdbg_Count, Sdbg_Signals);
            Add (High_Read_Pos, High_Count, High_Signals);
            Add (Normal_Read_Pos, Normal_Count, Normal_Signals);
            str := Result;
            Last_char := Pos - 1;
        end Image;
    
    end Queue;

end ACD_Runtime.Signal_Queue;
