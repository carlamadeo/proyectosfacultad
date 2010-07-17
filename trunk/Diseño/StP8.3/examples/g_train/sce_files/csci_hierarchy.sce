Diagram SceDiagram
{
    { Name "csci_hierarchy" }
    { Format "1.6" }
    { Scale 0.82 }
    { PanX -492 }
    { PanY -330 }
    { Alignment 0 }
    { GUID "UID_BC40EB63-343A-447e-AAEC-984F9942FC5A" }
    { Summary
        { Nodes 15 }
        { Arcs 9 }
    }
}

Node 2
{
    { Type Module }
    { Loc -134 -179 }
    { Scale 409.00 102.00 }
    { Label
        { Text "control" }
        { Text "g_train" }
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
        { ID 1500 }
        { GUID "UID_FA6E6C2E-5240-4d91-B16F-2D3686CA257F" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 1499 }
        { GUID "UID_3B71EE7C-98E5-423f-9AEC-8D2700D509FD" }
    }
    { Object
        { PDMType Node }
        { MappingName SEDirectoryMapping }
        { ID 602 }
        { GUID "UID_BEA0A71E-2FA6-4494-9500-21DDFFBE18AD" }
    }
}

Node 3
{
    { Type Anchor }
    { Loc -134 -275 }
    { Scale 128.00 128.00 }
    { Object
        { PDMType Node }
        { MappingName LinkEnd }
        { ID 2522 }
        { GUID "UID_9B6F2467-4C1B-4512-BF8C-1535D2BB0E84" }
    }
}

Node 5
{
    { Type Module }
    { Loc -332 156 }
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

Node 6
{
    { Type Module }
    { Loc 97 -45 }
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
    { Loc -390 -243 }
    { Scale 832.00 163.00 }
    { Label
        { Text "This module represents the" }
        { Text "sole CSCI. This designation" }
        { Text "results from the decomposition" }
        { Text "level annotation" }
    }
}

Node 18
{
    { Type Comment }
    { Loc -294 237 }
    { Scale 1493.00 204.00 }
    { Label
        { Text "This diagram shows the csci hierarchy from" }
        { Text "CSCI to CSCs and CSUs. This can be split" }
        { Text "over multiple diagrams if needed. The top" }
        { Text "module is always a module with a module" }
        { Text "call and the subordinates are always included modules." }
    }
}

Node 19
{
    { Type Comment }
    { Loc 74 -243 }
    { Scale 836.00 122.00 }
    { Label
        { Text "No annotation is needed to" }
        { Text "designate this included module" }
        { Text "as a CSC or SubCSC" }
    }
}

Node 21
{
    { Type Comment }
    { Loc 26 237 }
    { Scale 857.00 204.00 }
    { Label
        { Text "CSUs are defined in typical" }
        { Text "Constantine/Yourdon diagrams." }
        { Text "No annotations are needed" }
        { Text "here to designate these as" }
        { Text "CSUs." }
    }
}

Node 31
{
    { Type Module }
    { Loc 72 103 }
    { Scale 409.00 102.00 }
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

Node 42
{
    { Type Comment }
    { Loc -243 41 }
    { Scale 742.00 81.00 }
    { Label
        { Text "The structure of this CSC is" }
        { Text "shown in it's own diagram." }
    }
}

Node 51
{
    { Type Module }
    { Loc -370 -50 }
    { Scale 409.00 102.00 }
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
        { ID 3 }
        { Loc 22 -274 }
        { Offset 0 0 }
    }
    { To
        { ID 2 }
        { Loc 22 -199 }
        { Offset 0 0 }
    }
}

Arc 7
{
    { Type ModuleCall }
    { From
        { ID 51 }
        { Loc -214 -30 }
        { Offset 0 0 }
    }
    { To
        { ID 5 }
        { Loc -176 136 }
        { Offset 0 0 }
    }
}

Arc 8
{
    { Type ModuleCall }
    { From
        { ID 2 }
        { Loc 22 -159 }
        { Offset 0 0 }
    }
    { To
        { ID 6 }
        { Loc 253 -65 }
        { Offset 0 0 }
    }
}

Arc 17
{
    { Type CommentLink }
    { From
        { ID 16 }
        { Loc -136 -241 }
        { Offset 49 -24 }
    }
    { To
        { ID 2 }
        { Loc -60 -198 }
        { Offset -43 2 }
    }
}

Arc 20
{
    { Type CommentLink }
    { From
        { ID 19 }
        { Loc 291 -220 }
        { Offset 61 15 }
    }
    { To
        { ID 6 }
        { Loc 283 -67 }
        { Offset 30 6 }
    }
}

Arc 22
{
    { Type CommentLink }
    { From
        { ID 21 }
        { Loc 257 197 }
        { Offset 77 -28 }
    }
    { To
        { ID 31 }
        { Loc 233 121 }
        { Offset 4 10 }
    }
}

Arc 32
{
    { Type ModuleCall }
    { From
        { ID 6 }
        { Loc 253 -25 }
        { Offset 0 0 }
    }
    { To
        { ID 31 }
        { Loc 228 83 }
        { Offset 0 0 }
    }
}

Arc 43
{
    { Type CommentLink }
    { From
        { ID 42 }
        { Loc -49 56 }
        { Offset 51 7 }
    }
    { To
        { ID 5 }
        { Loc -170 134 }
        { Offset -41 8 }
    }
}

Arc 52
{
    { Type ModuleCall }
    { From
        { ID 2 }
        { Loc 22 -159 }
        { Offset 0 0 }
    }
    { To
        { ID 51 }
        { Loc -214 -70 }
        { Offset 0 0 }
    }
}

Cntx 61
{
    { Type InData }
    { Loc -97 -129 }
    { Scale 128.00 128.00 }
    { Label
        { Text "location" }
    }
    { Parent 8 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4742 }
        { GUID "UID_F6FB674A-B9BF-4e84-9610-5428E66340CC" }
    }
}

Cntx 62
{
    { Type InData }
    { Loc -18 -95 }
    { Scale 128.00 128.00 }
    { Label
        { Text "Ts" }
    }
    { Parent 8 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4743 }
        { GUID "UID_4DA38E5B-5BE4-4ef8-8FE7-77AE3F06718C" }
    }
}

Cntx 63
{
    { Type InData }
    { Loc -11 -121 }
    { Scale 128.00 128.00 }
    { Label
        { Text "number of" }
        { Text "passengers" }
    }
    { Parent 8 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4744 }
        { GUID "UID_29EA524C-423C-40b5-9053-B5F538EF54CC" }
    }
}

Cntx 64
{
    { Type InData }
    { Loc 67 -86 }
    { Scale 128.00 128.00 }
    { Label
        { Text "N_Kgs" }
    }
    { Parent 8 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4745 }
        { GUID "UID_D7A01178-AC96-4ec2-ACAD-F61187C27204" }
    }
}

Link 61
{
    { FirstArc 4 }
    { LastArc 4 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 4747 }
        { GUID "UID_7210BF00-9B69-4361-99DA-22D9F3274A3B" }
    }
}

Link 62
{
    { FirstArc 7 }
    { LastArc 7 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 4749 }
        { GUID "UID_57E7A31A-C1A5-4007-876A-4E66769ED282" }
    }
}

Link 63
{
    { FirstArc 8 }
    { LastArc 8 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 4741 }
        { GUID "UID_69B74787-FD89-4a0a-9283-A60AED098C02" }
    }
}

Link 64
{
    { FirstArc 17 }
    { LastArc 17 }
}

Link 65
{
    { FirstArc 20 }
    { LastArc 20 }
}

Link 66
{
    { FirstArc 22 }
    { LastArc 22 }
}

Link 67
{
    { FirstArc 32 }
    { LastArc 32 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 4748 }
        { GUID "UID_EFB4835F-EF7B-4748-A7E9-D4016DCDE1DD" }
    }
}

Link 68
{
    { FirstArc 43 }
    { LastArc 43 }
}

Link 69
{
    { FirstArc 52 }
    { LastArc 52 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 4746 }
        { GUID "UID_E842C7B9-6EFF-4499-8202-64DEA8FE1B9A" }
    }
}
