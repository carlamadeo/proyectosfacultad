Diagram SceDiagram
{
    { Name "calculate_speed" }
    { Format "1.6" }
    { Scale 1.00 }
    { PanX -432 }
    { PanY -274 }
    { Alignment 0 }
    { GUID "UID_AB6CE2E8-90C0-4a9a-8CCD-6690FD6257DC" }
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
        { Text "calculate speed" }
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
        { ID 2022 }
        { GUID "UID_B860C5A9-2744-4cf5-9896-A17899C193D0" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 2021 }
        { GUID "UID_E67259D1-6E39-4244-BEBD-841C20CD3139" }
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
        { Text "num" }
        { Text "pulses" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2731 }
        { GUID "UID_5C27899D-E882-4e1c-A742-0685F8F6AD66" }
    }
}

Cntx 7
{
    { Type OutData }
    { Loc -147 -64 }
    { Scale 128.00 128.00 }
    { Label
        { Text "Ts" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2777 }
        { GUID "UID_156CDC7D-BBCD-4d55-AD35-A8BF698BADD1" }
    }
}

Link 5
{
    { FirstArc 4 }
    { LastArc 4 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 2730 }
        { GUID "UID_1B07350B-0251-4683-8CFC-0739414D5206" }
    }
}
