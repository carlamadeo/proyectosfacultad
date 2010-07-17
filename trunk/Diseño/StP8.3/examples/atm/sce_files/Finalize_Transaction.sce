Diagram SceDiagram
{
    { Name "Finalize_Transaction" }
    { Format "1.6" }
    { Scale 1.00 }
    { PanX -472 }
    { PanY -317 }
    { Alignment 32 }
    { GUID "UID_3A70216C-733B-4640-AEA3-5CB03CF435FD" }
    { Summary
        { Nodes 4 }
        { Arcs 1 }
    }
}

Node 2
{
    { Type Anchor }
    { Loc -8 -162 }
    { Scale 128.00 128.00 }
    { Object
        { PDMType Node }
        { MappingName LinkEnd }
        { ID 273 }
        { GUID "UID_7C04C189-AC56-11d4-BEDC-0060971B5EE0" }
    }
}

Node 3
{
    { Type Module }
    { Loc -8 138 }
    { Scale 409.00 128.00 }
    { Label
        { Text "Finalize Transaction" }
    }
    { PropertyList
        { Property
            { Name "SEDirectory" }
            { Value "." }
        }
    }
    { Object
        { PDMType Node }
        { MappingName LinkEnd }
        { ID 218 }
        { GUID "UID_C091AD4F-8F65-45a3-A477-2336419BB9B5" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 203 }
        { GUID "UID_7C04BDFF-AC56-11d4-BEDC-0060971B5EE0" }
    }
    { Object
        { PDMType Node }
        { MappingName SEDirectoryMapping }
        { ID 202 }
        { GUID "UID_7C04BDFC-AC56-11d4-BEDC-0060971B5EE0" }
    }
}

Arc 4
{
    { Type ModuleCall }
    { From
        { ID 2 }
        { Loc 251 102 }
        { Offset 0 0 }
    }
    { To
        { ID 3 }
        { Loc 251 376 }
        { Offset 0 0 }
    }
}

Cntx 6
{
    { Type InData }
    { Loc 1 37 }
    { Scale 128.00 128.00 }
    { Label
        { Text "Transaction" }
        { Text "Data" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 1003 }
        { GUID "UID_57E7E10B-E72E-4c4a-9678-A89672F46061" }
    }
}

Cntx 7
{
    { Type InData }
    { Loc 1 -63 }
    { Scale 128.00 128.00 }
    { Label
        { Text "Account" }
        { Text "Information" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 1004 }
        { GUID "UID_BF988A9F-8ADB-4cc8-A9DF-AE4ED49FB6E7" }
    }
}

Link 5
{
    { FirstArc 4 }
    { LastArc 4 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 1002 }
        { GUID "UID_2E24C8A5-D5C5-4f4d-8EA0-6766F3F121BE" }
    }
}
