with Ada.Text_Io;
package body ACD_Runtime is
    procedure Report_Exception (Context, Excp_Name, Excp_Info : string) is
    begin
        Ada.Text_Io.Put_Line (Context & " got " & Excp_Name);
        Ada.Text_Io.Put_Line ("Info => " & Excp_Info);
    end Report_Exception;
end ACD_Runtime;
