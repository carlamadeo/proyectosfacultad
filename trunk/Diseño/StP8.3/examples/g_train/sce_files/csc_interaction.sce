Diagram SceDiagram
{
    { Name "csc_interaction" }
    { Format "1.6" }
    { Scale 0.98 }
    { PanX -467 }
    { PanY -332 }
    { Alignment 0 }
    { GUID "UID_A5910063-5AE7-49c6-AFD3-75EF02D6315F" }
    { Summary
        { Nodes 8 }
        { Arcs 1 }
    }
}

Node 6
{
    { Type Module }
    { Loc -113 54 }
    { Scale 409.00 102.00 }
    { Label
        { Text "screen" }
        { Text "display" }
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
        { ID 604 }
        { GUID "UID_7F7413AB-59BE-485e-BB8E-43FB551F3B01" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 603 }
        { GUID "UID_7766590F-8612-4167-BCD4-E5AF4ED99F27" }
    }
    { Object
        { PDMType Node }
        { MappingName SEDirectoryMapping }
        { ID 602 }
        { GUID "UID_BEA0A71E-2FA6-4494-9500-21DDFFBE18AD" }
    }
}

Node 16
{
    { Type Comment }
    { Loc -17 182 }
    { Scale 2663.00 246.00 }
    { Label
        { Text "This interaction diagram shows the data passed between CSCs." }
        { Text "This information can be split over multiple diagrams.  Treat this" }
        { Text "diagram more like a network and less like a directed graph." }
        { Text "This diagram is specific to 2167A." }
    }
}

Node 20
{
    { Type Comment }
    { Loc 106 -78 }
    { Scale 1596.00 193.00 }
    { Label
        { Text "The direction of the data couples, and" }
        { Text "not the direction of the link, indicates" }
        { Text "the flow of information." }
    }
}

Node 28
{
    { Type Module }
    { Loc -113 -202 }
    { Scale 409.00 102.00 }
    { Label
        { Text "process" }
        { Text "statistics" }
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
        { ID 606 }
        { GUID "UID_E3D9AFEB-C6EC-4a53-A19A-B5DC053A0169" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 605 }
        { GUID "UID_909BB126-1DBF-4360-9E6A-9464DF2E3214" }
    }
    { Object
        { PDMType Node }
        { MappingName SEDirectoryMapping }
        { ID 602 }
        { GUID "UID_BEA0A71E-2FA6-4494-9500-21DDFFBE18AD" }
    }
}

Arc 11
{
    { Type ModuleCall }
    { From
        { ID 28 }
        { Loc -109 -176 }
        { Offset 0 0 }
    }
    { To
        { ID 6 }
        { Loc -109 40 }
        { Offset 0 0 }
    }
}

Cntx 15
{
    { Type InData }
    { Loc -145 -138 }
    { Scale 128.00 128.00 }
    { Label
        { Text "N_Kgs" }
    }
    { Parent 11 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 608 }
        { GUID "UID_1D57BF20-3034-405f-877B-4B815D3160EF" }
    }
}

Cntx 29
{
    { Type InData }
    { Loc -81 -138 }
    { Scale 128.00 128.00 }
    { Label
        { Text "location" }
    }
    { Parent 11 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2884 }
        { GUID "UID_EE0276C1-4910-433a-B07C-BF7CDFDD7076" }
    }
}

Cntx 31
{
    { Type InData }
    { Loc -81 -42 }
    { Scale 128.00 128.00 }
    { Label
        { Text "Ts" }
    }
    { Parent 11 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2891 }
        { GUID "UID_3B2AB416-8E44-42b3-A2CC-DAE9DBF8004A" }
    }
}

Cntx 33
{
    { Type InData }
    { Loc -140 -53 }
    { Scale 128.00 128.00 }
    { Label
        { Text "number of" }
        { Text "passengers" }
    }
    { Parent 11 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4776 }
        { GUID "UID_38AA6169-A32E-44e7-BCC2-EE7195286321" }
    }
}

Link 33
{
    { FirstArc 11 }
    { LastArc 11 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 607 }
        { GUID "UID_F171352B-9DEE-4f26-9094-A2853F563F2A" }
    }
}
