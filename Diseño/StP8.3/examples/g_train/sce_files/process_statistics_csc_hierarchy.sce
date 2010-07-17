Diagram SceDiagram
{
    { Name "process_statistics_csc_hierarchy" }
    { Format "1.6" }
    { Scale 0.89 }
    { PanX -402 }
    { PanY -213 }
    { Alignment 0 }
    { GUID "UID_DB56FD28-E04D-4329-935A-931D2DF2F675" }
    { Summary
        { Nodes 9 }
        { Arcs 3 }
    }
}

Node 5
{
    { Type Module }
    { Loc -130 -111 }
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

Node 9
{
    { Type Module }
    { Loc -308 104 }
    { Scale 409.00 102.00 }
    { Label
        { Text "process" }
        { Text "weight" }
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
        { ID 1718 }
        { GUID "UID_42BCAD54-FAFB-4d34-A9C1-3A2D906F273F" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 1717 }
        { GUID "UID_1EEA8DB2-5254-4960-8D94-5294AEA4F70B" }
    }
    { Object
        { PDMType Node }
        { MappingName SEDirectoryMapping }
        { ID 602 }
        { GUID "UID_BEA0A71E-2FA6-4494-9500-21DDFFBE18AD" }
    }
}

Node 10
{
    { Type Module }
    { Loc -4 62 }
    { Scale 409.00 102.00 }
    { Label
        { Text "process" }
        { Text "speed" }
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
        { ID 1792 }
        { GUID "UID_9E7DC827-1641-4220-9381-53097909DB04" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 1791 }
        { GUID "UID_200A5337-B992-4d9b-ADC1-DA62FCFC20BC" }
    }
    { Object
        { PDMType Node }
        { MappingName SEDirectoryMapping }
        { ID 602 }
        { GUID "UID_BEA0A71E-2FA6-4494-9500-21DDFFBE18AD" }
    }
}

Node 13
{
    { Type Anchor }
    { Loc -130 -207 }
    { Scale 128.00 128.00 }
    { Object
        { PDMType Node }
        { MappingName LinkEnd }
        { ID 2522 }
        { GUID "UID_9B6F2467-4C1B-4512-BF8C-1535D2BB0E84" }
    }
}

Node 15
{
    { Type Comment }
    { Loc -130 177 }
    { Scale 1100.00 163.00 }
    { Label
        { Text "CSCs decompose indefinitely until CSUs" }
        { Text "are encountered.  CSUs are defined in" }
        { Text "their own Constantine/Yourdon Structure" }
        { Text "Chart Diagrams." }
    }
}

Arc 11
{
    { Type ModuleCall }
    { From
        { ID 5 }
        { Loc -8 -91 }
        { Offset 0 0 }
    }
    { To
        { ID 9 }
        { Loc -186 84 }
        { Offset 0 0 }
    }
}

Arc 12
{
    { Type ModuleCall }
    { From
        { ID 5 }
        { Loc -8 -91 }
        { Offset 0 0 }
    }
    { To
        { ID 10 }
        { Loc 118 42 }
        { Offset 0 0 }
    }
}

Arc 14
{
    { Type ModuleCall }
    { From
        { ID 13 }
        { Loc -8 -206 }
        { Offset 0 0 }
    }
    { To
        { ID 5 }
        { Loc -8 -131 }
        { Offset 0 0 }
    }
}

Cntx 19
{
    { Type OutData }
    { Loc -210 -30 }
    { Scale 128.00 128.00 }
    { Label
        { Text "number of" }
        { Text "passengers" }
    }
    { Parent 11 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2508 }
        { GUID "UID_3ACA2D3E-ADB5-46f3-AC7E-19188166FE05" }
    }
}

Cntx 20
{
    { Type OutData }
    { Loc -278 34 }
    { Scale 128.00 128.00 }
    { Label
        { Text "N_Kgs" }
    }
    { Parent 11 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2515 }
        { GUID "UID_A1DD6E1D-18D9-4d4f-9E79-0DF75255D385" }
    }
}

Cntx 21
{
    { Type OutData }
    { Loc -55 -34 }
    { Scale 128.00 128.00 }
    { Label
        { Text "Ts" }
    }
    { Parent 12 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4866 }
        { GUID "UID_1A540F81-BE61-4f17-988A-34944FE4B2F1" }
    }
}

Cntx 22
{
    { Type OutData }
    { Loc -12 7 }
    { Scale 128.00 128.00 }
    { Label
        { Text "location" }
    }
    { Parent 12 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4867 }
        { GUID "UID_3F9F5073-6E9E-4784-99A9-7BF9E60180B4" }
    }
}

Link 19
{
    { FirstArc 11 }
    { LastArc 11 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 2507 }
        { GUID "UID_697D8371-5C25-4329-82B6-CA136274936D" }
    }
}

Link 20
{
    { FirstArc 12 }
    { LastArc 12 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 4865 }
        { GUID "UID_2FB9AFA1-6411-4413-B79B-616C6517EFCF" }
    }
}

Link 21
{
    { FirstArc 14 }
    { LastArc 14 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 4868 }
        { GUID "UID_5B8B4924-3073-4ac4-8294-0D8E3386762F" }
    }
}
