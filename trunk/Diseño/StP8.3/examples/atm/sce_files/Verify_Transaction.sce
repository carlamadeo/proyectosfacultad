Diagram SceDiagram
{
    { Name "Verify_Transaction" }
    { Format "1.6" }
    { Scale 1.00 }
    { PanX -297 }
    { PanY -69 }
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
    { Loc -65 -262 }
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
    { Loc -65 238 }
    { Scale 409.00 128.00 }
    { Label
        { Text "Verify Transaction" }
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
        { ID 293 }
        { GUID "UID_7C04C056-AC56-11d4-BEDC-0060971B5EE0" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 292 }
        { GUID "UID_7C04C053-AC56-11d4-BEDC-0060971B5EE0" }
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
        { Loc 251 576 }
        { Offset 0 0 }
    }
}

Cntx 6
{
    { Type OutFlag }
    { Loc -56 137 }
    { Scale 128.00 128.00 }
    { Label
        { Text "Transaction" }
        { Text "Verification" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 1092 }
        { GUID "UID_102C2F0F-B5C2-11d4-879B-00C04F8DEC45" }
    }
}

Cntx 7
{
    { Type InData }
    { Loc -56 37 }
    { Scale 128.00 128.00 }
    { Label
        { Text "Account" }
        { Text "Information" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 1093 }
        { GUID "UID_102C2F12-B5C2-11d4-879B-00C04F8DEC45" }
    }
}

Cntx 8
{
    { Type InData }
    { Loc -56 -63 }
    { Scale 128.00 128.00 }
    { Label
        { Text "Transaction" }
        { Text "Data" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 1094 }
        { GUID "UID_102C2F15-B5C2-11d4-879B-00C04F8DEC45" }
    }
}

Cntx 9
{
    { Type InData }
    { Loc -56 -163 }
    { Scale 128.00 128.00 }
    { Label
        { Text "PIN" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 1095 }
        { GUID "UID_102C2F18-B5C2-11d4-879B-00C04F8DEC45" }
    }
}

Link 5
{
    { FirstArc 4 }
    { LastArc 4 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 1091 }
        { GUID "UID_102C2F0C-B5C2-11d4-879B-00C04F8DEC45" }
    }
}
