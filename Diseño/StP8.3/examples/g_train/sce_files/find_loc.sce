Diagram SceDiagram
{
    { Name "find_loc" }
    { Format "1.6" }
    { Scale 1.00 }
    { PanX -432 }
    { PanY -274 }
    { Alignment 0 }
    { GUID "UID_9FBE3C77-E783-4cd5-8ECF-F00E647709C3" }
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
        { Text "find_loc" }
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
        { ID 1652 }
        { GUID "UID_6F0C66E9-BCE1-4037-92E3-007335019A77" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 1651 }
        { GUID "UID_BD30139B-878D-4438-8CD1-758022AF75CB" }
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
        { Text "location" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2722 }
        { GUID "UID_D597C402-6BE2-404d-9D7E-73E03450184C" }
    }
}

Cntx 7
{
    { Type InData }
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
        { ID 2714 }
        { GUID "UID_2665052B-91B5-48a2-8719-01C9C6F6D963" }
    }
}

Link 5
{
    { FirstArc 4 }
    { LastArc 4 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 2713 }
        { GUID "UID_7C681657-1E09-433e-A224-0985DF1A99D9" }
    }
}
