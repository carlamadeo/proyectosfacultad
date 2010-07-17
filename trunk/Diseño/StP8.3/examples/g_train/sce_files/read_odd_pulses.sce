Diagram SceDiagram
{
    { Name "read_odd_pulses" }
    { Format "1.6" }
    { Scale 1.00 }
    { PanX -276 }
    { PanY -150 }
    { Alignment 0 }
    { GUID "UID_3C21FD65-931E-44ab-BB16-047CBB905D31" }
    { Summary
        { Nodes 3 }
        { Arcs 1 }
    }
}

Node 2
{
    { Type Anchor }
    { Loc 0 -113 }
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
    { Loc 0 87 }
    { Scale 409.00 128.00 }
    { Label
        { Text "read odd pulses" }
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
        { ID 2152 }
        { GUID "UID_2E735D29-E9BA-4bc0-BEB0-3922E6BBE8F5" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 2151 }
        { GUID "UID_E9E4B778-7DE4-43f0-860E-F03401A6F0D8" }
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
        { Loc -156 -112 }
        { Offset 0 0 }
    }
    { To
        { ID 3 }
        { Loc -156 60 }
        { Offset 0 0 }
    }
}

Cntx 6
{
    { Type OutData }
    { Loc 9 -14 }
    { Scale 128.00 128.00 }
    { Label
        { Text "time_1" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2712 }
        { GUID "UID_44F5BED0-D63F-4017-87EC-BF99EB381785" }
    }
}

Link 5
{
    { FirstArc 4 }
    { LastArc 4 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 2711 }
        { GUID "UID_F702F9A8-B410-47fd-B7FE-5D6F7AE04C95" }
    }
}
