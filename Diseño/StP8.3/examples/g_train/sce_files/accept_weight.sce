Diagram SceDiagram
{
    { Name "accept_weight" }
    { Format "1.6" }
    { Scale 1.00 }
    { PanX -432 }
    { PanY -274 }
    { Alignment 0 }
    { GUID "UID_8D0DAE2B-98F5-4cb7-AA30-59E62DDE6C4C" }
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
        { Text "accept weight" }
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
        { ID 1862 }
        { GUID "UID_1DE1A147-1B29-41c9-961F-313FE15E8056" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 1861 }
        { GUID "UID_8AD8D2DA-1BA3-4185-BBAF-24017FB6B373" }
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
        { Text "weight" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2680 }
        { GUID "UID_DAA3DFC6-7FAC-4580-A36F-9A27D7878F1B" }
    }
}

Cntx 7
{
    { Type OutFlag }
    { Loc -147 -64 }
    { Scale 128.00 128.00 }
    { Label
        { Text "done" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2688 }
        { GUID "UID_E59630C8-FD20-428f-AFED-0C822311418B" }
    }
}

Link 5
{
    { FirstArc 4 }
    { LastArc 4 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 2679 }
        { GUID "UID_4709012F-DC20-4888-941F-2B11329A471B" }
    }
}
