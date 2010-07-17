Diagram SceDiagram
{
    { Name "artificial_csc" }
    { Format "1.6" }
    { Scale 1.00 }
    { PanX -432 }
    { PanY -274 }
    { Alignment 0 }
    { GUID "UID_864A64F5-D5FC-41b3-A0B0-672265916D9F" }
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
        { Text "artificial csc" }
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
        { ID 2383 }
        { GUID "UID_D6A6126E-5771-420f-88FB-75C97D1348D9" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 2382 }
        { GUID "UID_DE4F0512-53AD-4f3e-9623-5AF87251FB03" }
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
        { Loc 251 176 }
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
        { ID 4710 }
        { GUID "UID_9EFE2130-0E6C-4b7a-AF62-FEBD3DCD7369" }
    }
}
