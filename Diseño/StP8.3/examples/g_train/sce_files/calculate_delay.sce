Diagram SceDiagram
{
    { Name "calculate_delay" }
    { Format "1.6" }
    { Scale 1.00 }
    { PanX -432 }
    { PanY -274 }
    { Alignment 0 }
    { GUID "UID_88296F68-AAB2-4d1d-8314-C954065C8AD3" }
    { Summary
        { Nodes 4 }
        { Arcs 1 }
    }
}

Node 2
{
    { Type Anchor }
    { Loc -156 -163 }
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
    { Loc -156 137 }
    { Scale 409.00 128.00 }
    { Label
        { Text "calculate delay" }
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
        { ID 2087 }
        { GUID "UID_F805EA5E-DFFE-424b-A7B6-63498961A9C0" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 2086 }
        { GUID "UID_4A21DBB6-0BA7-4829-BFF6-25B0EE7178D7" }
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
        { Loc 251 376 }
        { Offset 0 0 }
    }
}

Cntx 6
{
    { Type OutData }
    { Loc -147 36 }
    { Scale 128.00 128.00 }
    { Label
        { Text "delay" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2794 }
        { GUID "UID_21F22042-7F0B-4787-94E9-630AC00EF6AD" }
    }
}

Cntx 7
{
    { Type OutData }
    { Loc -147 -64 }
    { Scale 128.00 128.00 }
    { Label
        { Text "num" }
        { Text "pulses" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2786 }
        { GUID "UID_1C5B67A1-8422-432e-A127-4C2752506619" }
    }
}

Link 5
{
    { FirstArc 4 }
    { LastArc 4 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 2785 }
        { GUID "UID_1CF62505-77D0-461e-BCA7-477866D89998" }
    }
}
