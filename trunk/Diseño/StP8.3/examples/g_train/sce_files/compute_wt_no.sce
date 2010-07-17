Diagram SceDiagram
{
    { Name "compute_wt_no" }
    { Format "1.6" }
    { Scale 1.00 }
    { PanX -432 }
    { PanY -274 }
    { Alignment 0 }
    { GUID "UID_17778DE5-462B-4ab9-8D51-1F388B8588EC" }
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
        { Text "compute wt_no" }
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
        { ID 2289 }
        { GUID "UID_ED1733B9-7C32-4e4a-B6D8-D66181661FBC" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 2288 }
        { GUID "UID_409F9E7B-2420-4a6f-B01A-C67E108A8184" }
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
        { Text "N_Kgs" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2638 }
        { GUID "UID_179B27C0-EB36-40ef-BFE2-16745029E7AF" }
    }
}

Cntx 7
{
    { Type OutData }
    { Loc -147 -64 }
    { Scale 128.00 128.00 }
    { Label
        { Text "number" }
        { Text "of_passengers" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2646 }
        { GUID "UID_1F2FA229-232E-49ca-BE82-7EB851C5C74B" }
    }
}

Link 5
{
    { FirstArc 4 }
    { LastArc 4 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 2637 }
        { GUID "UID_0CE56187-470F-4cc9-87FA-89E9E9F9D343" }
    }
}
