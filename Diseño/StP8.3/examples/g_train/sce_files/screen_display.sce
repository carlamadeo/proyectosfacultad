Diagram SceDiagram
{
    { Name "screen_display" }
    { Format "1.6" }
    { Scale 1.00 }
    { PanX -403 }
    { PanY 168 }
    { Alignment 0 }
    { GUID "UID_D3714F32-149E-467d-B605-F7B92111D7AB" }
    { Summary
        { Nodes 11 }
        { Arcs 2 }
    }
}

Node 2
{
    { Type Anchor }
    { Loc -159 -288 }
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
    { Loc -156 112 }
    { Scale 409.00 128.00 }
    { Label
        { Text "screen display" }
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

Node 9
{
    { Type Module }
    { Loc -156 262 }
    { Scale 409.00 128.00 }
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

Arc 4
{
    { Type ModuleCall }
    { From
        { ID 2 }
        { Loc -159 -287 }
        { Offset 0 0 }
    }
    { To
        { ID 3 }
        { Loc -156 87 }
        { Offset 0 0 }
    }
}

Arc 13
{
    { Type ModuleCall }
    { From
        { ID 3 }
        { Loc -156 137 }
        { Offset 0 0 }
    }
    { To
        { ID 9 }
        { Loc -156 237 }
        { Offset 0 0 }
    }
}

Cntx 6
{
    { Type InData }
    { Loc -144 -55 }
    { Scale 128.00 128.00 }
    { Label
        { Text "N_Kgs" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2524 }
        { GUID "UID_D97E4B12-7454-4b38-9AA2-E2BB82E7F73C" }
    }
}

Cntx 7
{
    { Type InData }
    { Loc -149 -115 }
    { Scale 128.00 128.00 }
    { Label
        { Text "location" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2532 }
        { GUID "UID_AFC387D6-3F7D-4c96-8E81-CBE934A3343A" }
    }
}

Cntx 8
{
    { Type InData }
    { Loc -149 -189 }
    { Scale 128.00 128.00 }
    { Label
        { Text "Ts" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2540 }
        { GUID "UID_805FB861-4F01-405d-A341-F51F4DD84C07" }
    }
}

Cntx 21
{
    { Type InData }
    { Loc -166 158 }
    { Scale 128.00 128.00 }
    { Label
        { Text "location" }
    }
    { Parent 13 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4963 }
        { GUID "UID_73E7A728-35E7-4209-ADF1-9E523F03B6E7" }
    }
}

Cntx 22
{
    { Type InData }
    { Loc -166 206 }
    { Scale 128.00 128.00 }
    { Label
        { Text "Ts" }
    }
    { Parent 13 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4964 }
        { GUID "UID_45A27325-6C5E-4446-96AB-6CDE1CAB0239" }
    }
}

Cntx 23
{
    { Type InData }
    { Loc -140 160 }
    { Scale 128.00 128.00 }
    { Label
        { Text "number of" }
        { Text "passengers" }
    }
    { Parent 13 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4965 }
        { GUID "UID_B1517DD6-1E04-4a92-8FDB-4B7BAC975D04" }
    }
}

Cntx 24
{
    { Type InData }
    { Loc -143 206 }
    { Scale 128.00 128.00 }
    { Label
        { Text "N_Kgs" }
    }
    { Parent 13 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4966 }
        { GUID "UID_BF47978D-65A8-4f3b-8CBC-54B9FD997E3A" }
    }
}

Cntx 25
{
    { Type InData }
    { Loc -143 10 }
    { Scale 128.00 128.00 }
    { Label
        { Text "number of passengers" }
    }
    { Parent 4 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2720 }
        { GUID "UID_70823388-518D-4f64-9D3D-D2C55FA2D3F8" }
    }
}

Link 5
{
    { FirstArc 4 }
    { LastArc 4 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 2523 }
        { GUID "UID_594031CF-E10B-4915-B17C-30E263773905" }
    }
}

Link 14
{
    { FirstArc 13 }
    { LastArc 13 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 4748 }
        { GUID "UID_EFB4835F-EF7B-4748-A7E9-D4016DCDE1DD" }
    }
}
