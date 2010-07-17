Diagram SceDiagram
{
    { Name "read_even_pulses" }
    { Format "1.6" }
    { Scale 1.00 }
    { PanX -432 }
    { PanY -274 }
    { Alignment 0 }
    { GUID "UID_C28D6178-29BC-4cef-A3CA-96BC651609D0" }
    { Summary
        { Nodes 3 }
        { Arcs 1 }
    }
}

Node 2
{
    { Type Anchor }
    { Loc -156 -113 }
    { Scale 128.00 128.00 }
    { Object
        { PDMType Node }
        { MappingName LinkEnd }
        { ID 2522 }
        { GUID "UID_9B6F2467-4C1B-4512-BF8C-1535D2BB0E84" }
    }
}

Node 3
{
    { Type Module }
    { Loc -156 87 }
    { Scale 409.00 128.00 }
    { Label
        { Text "read even pulses" }
    }
    { PropertyList
        { Property
            { Name "SEDirectory" }
            { Value "/demo/stp/core1.3.1/Examples/mig5train/src_files" }
        }
    }
    { Object
        { PDMType Node }
        { MappingName LinkEnd }
        { ID 2223 }
        { GUID "UID_9C1088B5-66A8-4963-B07B-441A8B1C86DA" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 2222 }
        { GUID "UID_8E75250C-7C96-4044-A674-AE0405F6D352" }
    }
    { Object
        { PDMType Node }
        { MappingName SEDirectoryMapping }
        { ID 602 }
        { GUID "UID_BEA0A71E-2FA6-4494-9500-21DDFFBE18AD" }
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
        { Loc 251 276 }
        { Offset 0 0 }
    }
}

Cntx 6
{
    { Type OutData }
    { Loc -147 -14 }
    { Scale 128.00 128.00 }
    { Label
        { Text "time_2" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2703 }
        { GUID "UID_7E7447C5-4172-431a-AFC3-ABA5AAF474A4" }
    }
}

Link 5
{
    { FirstArc 4 }
    { LastArc 4 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 2702 }
        { GUID "UID_429A71AB-4366-424e-ABC1-D0CB578245FD" }
    }
}
