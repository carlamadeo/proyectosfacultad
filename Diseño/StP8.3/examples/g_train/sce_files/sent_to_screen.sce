Diagram SceDiagram
{
    { Name "sent_to_screen" }
    { Format "1.6" }
    { Scale 1.00 }
    { PanX -270 }
    { PanY -255 }
    { Alignment 0 }
    { GUID "UID_1571C22C-1F93-4ef9-AA26-3D3341932D4F" }
    { Summary
        { Nodes 9 }
        { Arcs 4 }
    }
}

Node 2
{
    { Type Module }
    { Loc -3 -26 }
    { Scale 225.00 157.00 }
    { Label
        { Text "send_to" }
        { Text "screen" }
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
        { ID 1571 }
        { GUID "UID_217373B0-2FB7-4f69-B376-465CBCBDAAEB" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 1570 }
        { GUID "UID_32B0DEB2-15A9-4f8b-917F-F2583B28DF08" }
    }
    { Object
        { PDMType Node }
        { MappingName SEDirectoryMapping }
        { ID 602 }
        { GUID "UID_BEA0A71E-2FA6-4494-9500-21DDFFBE18AD" }
    }
}

Node 17
{
    { Type Module }
    { Loc -3 195 }
    { Scale 190.00 140.00 }
    { Label
        { Text "update" }
        { Text "screen" }
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

Node 78
{
    { Type Anchor }
    { Loc -3 -224 }
    { Scale 128.00 128.00 }
    { Object
        { PDMType Node }
        { MappingName LinkEnd }
        { ID 2522 }
        { GUID "UID_9B6F2467-4C1B-4512-BF8C-1535D2BB0E84" }
    }
}

Node 85
{
    { Type LibraryModule }
    { Loc 133 96 }
    { Scale 409.00 102.00 }
    { Label
        { Text "artificial" }
        { Text "csc" }
    }
    { Object
        { PDMType Node }
        { MappingName LinkEnd }
        { ID 653 }
        { GUID "UID_59EEBC97-76A1-4d4e-BD17-C8A8B95BA3D7" }
    }
}

Node 93
{
    { Type GlobalData }
    { Loc -133 91 }
    { Scale 409.00 102.00 }
    { Label
        { Text "artificial" }
        { Text "data file" }
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
        { ID 2915 }
        { GUID "UID_A96CA1D8-2632-4af1-9965-CD9BAA5380F6" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 2914 }
        { GUID "UID_5365B6F1-D4FE-4859-B88A-4D368524955B" }
    }
    { Object
        { PDMType Node }
        { MappingName SEDirectoryMapping }
        { ID 2744 }
        { GUID "UID_EC87D812-675F-4d56-950D-3302FC4EC087" }
    }
}

Arc 23
{
    { Type ModuleCall }
    { From
        { ID 78 }
        { Loc -73 -222 }
        { Offset 0 0 }
    }
    { To
        { ID 2 }
        { Loc -73 -56 }
        { Offset 0 0 }
    }
}

Arc 37
{
    { Type ModuleCall }
    { From
        { ID 2 }
        { Loc -73 6 }
        { Offset 0 0 }
    }
    { To
        { ID 17 }
        { Loc -73 168 }
        { Offset 0 0 }
    }
}

Arc 86
{
    { Type ModuleCall }
    { From
        { ID 2 }
        { Loc -73 6 }
        { Offset 0 0 }
    }
    { To
        { ID 85 }
        { Loc 63 77 }
        { Offset 0 0 }
    }
}

Arc 94
{
    { Type ModuleCall }
    { From
        { ID 2 }
        { Loc -73 6 }
        { Offset 0 0 }
    }
    { To
        { ID 93 }
        { Loc -203 72 }
        { Offset 0 0 }
    }
}

Cntx 42
{
    { Type InData }
    { Loc 23 -155 }
    { Scale 128.00 128.00 }
    { Label
        { Text "number of" }
        { Text "passengers" }
    }
    { Parent 23 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2557 }
        { GUID "UID_9842B511-C710-48a7-87DD-64E6217FCCC6" }
    }
}

Cntx 43
{
    { Type InData }
    { Loc -20 -158 }
    { Scale 128.00 128.00 }
    { Label
        { Text "location" }
    }
    { Parent 23 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2549 }
        { GUID "UID_562D7CAF-2200-4a12-996E-CFDE5D979ADA" }
    }
}

Cntx 44
{
    { Type InData }
    { Loc -27 -118 }
    { Scale 128.00 128.00 }
    { Label
        { Text "Ts" }
    }
    { Parent 23 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2565 }
        { GUID "UID_83ED06BF-3444-4cfc-8F88-1062F69D3726" }
    }
}

Cntx 45
{
    { Type InData }
    { Loc 12 -105 }
    { Scale 128.00 128.00 }
    { Label
        { Text "N_Kgs" }
    }
    { Parent 23 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2573 }
        { GUID "UID_9009F09B-3F07-4c6c-80E5-DE5C5406826A" }
    }
}

Link 102
{
    { FirstArc 23 }
    { LastArc 23 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 2548 }
        { GUID "UID_4FA93701-DBFC-471d-AD44-1BBD76F1395F" }
    }
}

Link 103
{
    { FirstArc 37 }
    { LastArc 37 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 4986 }
        { GUID "UID_06EE9CFE-7278-41f8-9655-95FAFF26ED72" }
    }
}

Link 104
{
    { FirstArc 86 }
    { LastArc 86 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 4985 }
        { GUID "UID_85C00693-43C1-4205-8964-C005D9EB0D57" }
    }
}

Link 105
{
    { FirstArc 94 }
    { LastArc 94 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 2923 }
        { GUID "UID_9EAF446B-18D2-4ddd-88D4-1EF92B798DA6" }
    }
}
