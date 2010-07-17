Diagram SceDiagram
{
    { Name "Make_Receipt" }
    { Format "1.6" }
    { Scale 0.71 }
    { PanX -363 }
    { PanY -288 }
    { Alignment 32 }
    { GUID "UID_D6223692-7C7C-4511-87CB-B36595262DEC" }
    { Summary
        { Nodes 6 }
        { Arcs 1 }
    }
}

Node 2
{
    { Type Anchor }
    { Loc -51 -263 }
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
    { Loc -51 237 }
    { Scale 409.00 128.00 }
    { Label
        { Text "Make Receipt" }
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
        { ID 272 }
        { GUID "UID_7C04C186-AC56-11d4-BEDC-0060971B5EE0" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 271 }
        { GUID "UID_7C04C183-AC56-11d4-BEDC-0060971B5EE0" }
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
        { Loc -105 -262 }
        { Offset 0 0 }
    }
    { To
        { ID 3 }
        { Loc -105 212 }
        { Offset 0 0 }
    }
}

Cntx 6
{
    { Type InData }
    { Loc -41 137 }
    { Scale 128.00 128.00 }
    { Label
        { Text "Account" }
        { Text "Information" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 1078 }
        { GUID "UID_102C2EF3-B5C2-11d4-879B-00C04F8DEC45" }
    }
}

Cntx 7
{
    { Type InFlag }
    { Loc -41 37 }
    { Scale 128.00 128.00 }
    { Label
        { Text "Request" }
        { Text "Receipt" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 1079 }
        { GUID "UID_102C2EF6-B5C2-11d4-879B-00C04F8DEC45" }
    }
}

Cntx 8
{
    { Type InData }
    { Loc -41 -63 }
    { Scale 128.00 128.00 }
    { Label
        { Text "Transaction" }
        { Text "Data" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 1080 }
        { GUID "UID_102C2EF9-B5C2-11d4-879B-00C04F8DEC45" }
    }
}

Cntx 9
{
    { Type OutData }
    { Loc -41 -163 }
    { Scale 128.00 128.00 }
    { Label
        { Text "Receipt" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 275 }
        { GUID "UID_7C04C18F-AC56-11d4-BEDC-0060971B5EE0" }
    }
}

Link 5
{
    { FirstArc 4 }
    { LastArc 4 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 274 }
        { GUID "UID_7C04C18C-AC56-11d4-BEDC-0060971B5EE0" }
    }
}
