Diagram SceDiagram
{
    { Name "reset_scale" }
    { Format "1.6" }
    { Scale 1.00 }
    { PanX -432 }
    { PanY -275 }
    { Alignment 0 }
    { GUID "UID_2742993D-06FE-4f22-8CA6-D87B52F248F0" }
    { Summary
        { Nodes 5 }
        { Arcs 1 }
    }
}

Node 2
{
    { Type Anchor }
    { Loc -156 -213 }
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
    { Loc -156 187 }
    { Scale 409.00 128.00 }
    { Label
        { Text "reset scale" }
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
        { ID 1928 }
        { GUID "UID_91C14B80-EEAA-440a-82D0-8D931F4E8509" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 1927 }
        { GUID "UID_6549A923-FE3F-475f-A8C2-93DC8B0C579D" }
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
        { Loc 251 476 }
        { Offset 0 0 }
    }
}

Cntx 6
{
    { Type OutFlag }
    { Loc -147 86 }
    { Scale 128.00 128.00 }
    { Label
        { Text "done" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2655 }
        { GUID "UID_6DD35488-3AC8-4d45-802B-B14A03DDD705" }
    }
}

Cntx 7
{
    { Type OutData }
    { Loc -147 -14 }
    { Scale 128.00 128.00 }
    { Label
        { Text "weight" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2663 }
        { GUID "UID_26672576-6F53-410d-B369-41F064A2105D" }
    }
}

Cntx 8
{
    { Type OutFlag }
    { Loc -147 -114 }
    { Scale 128.00 128.00 }
    { Label
        { Text "within" }
        { Text "limit" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2671 }
        { GUID "UID_BA908703-D9D4-4865-A9C5-646429E869A3" }
    }
}

Link 5
{
    { FirstArc 4 }
    { LastArc 4 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 2654 }
        { GUID "UID_47197B8D-0EAF-4254-AB99-D8ABCE0282DF" }
    }
}
