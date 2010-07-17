Diagram SceDiagram
{
    { Name "update_screen" }
    { Format "1.6" }
    { Scale 1.00 }
    { PanX -432 }
    { PanY -274 }
    { Alignment 0 }
    { GUID "UID_2088DCBB-5A7A-4350-A875-50EFDD98DEE1" }
    { Summary
        { Nodes 2 }
        { Arcs 1 }
    }
}

Node 2
{
    { Type Anchor }
    { Loc -156 -63 }
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
    { Loc -156 37 }
    { Scale 409.00 128.00 }
    { Label
        { Text "update screen" }
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
        { ID 2354 }
        { GUID "UID_EF7D97AC-E68E-4e9a-907C-8A7EABFD67D0" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 2353 }
        { GUID "UID_5FB94611-159D-43a7-8223-F77FE6E60E67" }
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
        { Loc -156 -62 }
        { Offset 0 0 }
    }
    { To
        { ID 3 }
        { Loc -156 12 }
        { Offset 0 0 }
    }
}

Link 5
{
    { FirstArc 4 }
    { LastArc 4 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 4002 }
        { GUID "UID_3173FCBD-056E-40d0-89B3-B2C9C43F8E5D" }
    }
}
