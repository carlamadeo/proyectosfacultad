Diagram SceDiagram
{
    { Name "process_weight" }
    { Format "1.6" }
    { Scale 0.81 }
    { PanX -348 }
    { PanY -144 }
    { Alignment 0 }
    { GUID "UID_36E23BB4-CF26-4e02-B246-1FA4AB64E282" }
    { Summary
        { Nodes 21 }
        { Arcs 7 }
    }
}

Node 4
{
    { Type Module }
    { Loc -40 -218 }
    { Scale 225.00 157.00 }
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

Node 7
{
    { Type LibraryModule }
    { Loc 241 90 }
    { Scale 156.00 112.00 }
    { Label
        { Text "count" }
    }
    { Object
        { PDMType Node }
        { MappingName LinkEnd }
        { ID 2627 }
        { GUID "UID_D9B5AFB2-2CB4-4800-99CE-485A9B155D34" }
    }
}

Node 8
{
    { Type Module }
    { Loc -33 249 }
    { Scale 183.00 120.00 }
    { Label
        { Text "accept" }
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

Node 9
{
    { Type Module }
    { Loc -168 70 }
    { Scale 198.00 120.00 }
    { Label
        { Text "reset" }
        { Text "scale" }
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
        { ID 1928 }
        { GUID "UID_91C14B80-EEAA-440a-82D0-8D931F4E8509" }
    }
    { Object
        { PDMType Node }
        { MappingName SEFileMapping }
        { ID 1927 }
        { GUID "UID_6549A923-FE3F-475f-A8C2-93DC8B0C579D" }
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
    { Type Module }
    { Loc -40 -90 }
    { Scale 198.00 120.00 }
    { Label
        { Text "compute" }
        { Text "wt_no" }
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

Node 18
{
    { Type LibraryModule }
    { Loc -290 357 }
    { Scale 156.00 112.00 }
    { Label
        { Text "change" }
        { Text "lights" }
    }
    { Object
        { PDMType Node }
        { MappingName LinkEnd }
        { ID 2632 }
        { GUID "UID_7E133D38-0851-415d-8B26-3038619633D7" }
    }
}

Node 71
{
    { Type Anchor }
    { Loc -40 -378 }
    { Scale 128.00 128.00 }
    { Object
        { PDMType Node }
        { MappingName LinkEnd }
        { ID 2522 }
        { GUID "UID_9B6F2467-4C1B-4512-BF8C-1535D2BB0E84" }
    }
}

Arc 21
{
    { Type ModuleCall }
    { From
        { ID 71 }
        { Loc -28 -340 }
        { Offset 0 0 }
    }
    { To
        { ID 4 }
        { Loc -28 -212 }
        { Offset 0 0 }
    }
}

Arc 24
{
    { Type ModuleCall }
    { From
        { ID 16 }
        { Loc -28 -30 }
        { Offset 0 0 }
    }
    { To
        { ID 7 }
        { Loc 253 105 }
        { Offset 0 0 }
    }
}

Arc 26
{
    { Type ModuleCall }
    { From
        { ID 16 }
        { Loc -28 -30 }
        { Offset 0 0 }
    }
    { To
        { ID 9 }
        { Loc -156 84 }
        { Offset 0 0 }
    }
}

Arc 27
{
    { Type ModuleCall }
    { From
        { ID 9 }
        { Loc -156 130 }
        { Offset 0 0 }
    }
    { To
        { ID 8 }
        { Loc -21 263 }
        { Offset 0 0 }
    }
}

Arc 35
{
    { Type ModuleCall }
    { From
        { ID 9 }
        { Loc -156 130 }
        { Offset 0 0 }
    }
    { To
        { ID 18 }
        { Loc -278 372 }
        { Offset 0 0 }
    }
}

Arc 36
{
    { Type ModuleCall }
    { From
        { ID 4 }
        { Loc -28 -150 }
        { Offset 0 0 }
    }
    { To
        { ID 16 }
        { Loc -28 -76 }
        { Offset 0 0 }
    }
}

Arc 38
{
    { Type ModuleCall }
    { From
        { ID 8 }
        { Loc -21 309 }
        { Offset 0 0 }
    }
    { To
        { ID 18 }
        { Loc -278 372 }
        { Offset 0 0 }
    }
}

Cntx 40
{
    { Type OutData }
    { Loc -54 -273 }
    { Scale 128.00 128.00 }
    { Label
        { Text "N_kgs" }
    }
    { Parent 21 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2611 }
        { GUID "UID_FE3C0D4E-4FF8-44fc-AA92-B0F48A25E0D2" }
    }
}

Cntx 46
{
    { Type OutFlag }
    { Loc 79 1 }
    { Scale 128.00 128.00 }
    { Label
        { Text "within" }
        { Text "limit" }
    }
    { Parent 24 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4888 }
        { GUID "UID_59D4583F-25AF-417d-B81F-7460D8D90BBF" }
    }
}

Cntx 47
{
    { Type OutFlag }
    { Loc -104 -58 }
    { Scale 128.00 128.00 }
    { Label
        { Text "done" }
    }
    { Parent 26 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4890 }
        { GUID "UID_78C24E45-C490-4ef9-9EEC-83C912E95D9A" }
    }
}

Cntx 48
{
    { Type InFlag }
    { Loc 79 -40 }
    { Scale 128.00 128.00 }
    { Label
        { Text "done" }
    }
    { Parent 24 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4891 }
        { GUID "UID_0834CD95-DAD4-4d97-897D-5DE998EF5957" }
    }
}

Cntx 49
{
    { Type OutData }
    { Loc 221 20 }
    { Scale 128.00 128.00 }
    { Label
        { Text "number" }
        { Text "of_passengers" }
    }
    { Parent 24 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4892 }
        { GUID "UID_1AE4BA61-568C-4599-B29A-FE54C8969DF4" }
    }
}

Cntx 50
{
    { Type OutData }
    { Loc -143 150 }
    { Scale 128.00 128.00 }
    { Label
        { Text "weight" }
    }
    { Parent 27 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4894 }
        { GUID "UID_D809817D-CE26-4cbe-8116-917AC9400220" }
    }
}

Cntx 51
{
    { Type OutData }
    { Loc -136 -26 }
    { Scale 128.00 128.00 }
    { Label
        { Text "weight" }
    }
    { Parent 26 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4895 }
        { GUID "UID_BD6D13C2-1849-4e90-AA7A-7F37A27060C2" }
    }
}

Cntx 52
{
    { Type OutData }
    { Loc -53 -341 }
    { Scale 128.00 128.00 }
    { Label
        { Text "number_of" }
        { Text "passengers" }
    }
    { Parent 21 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2603 }
        { GUID "UID_8C874F23-FD08-44ad-8D96-47E0F769929D" }
    }
}

Cntx 65
{
    { Type OutFlag }
    { Loc -91 203 }
    { Scale 128.00 128.00 }
    { Label
        { Text "done" }
    }
    { Parent 27 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4896 }
        { GUID "UID_59B22541-677E-42b0-BC11-F3382E396EB1" }
    }
}

Cntx 66
{
    { Type OutData }
    { Loc -62 -155 }
    { Scale 128.00 128.00 }
    { Label
        { Text "N_Kgs" }
    }
    { Parent 36 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 2620 }
        { GUID "UID_6298E42C-CCE1-44d8-8EEA-F0404C3F84B6" }
    }
}

Cntx 67
{
    { Type OutData }
    { Loc -20 -143 }
    { Scale 128.00 128.00 }
    { Label
        { Text "number" }
        { Text "of_passengers" }
    }
    { Parent 36 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4897 }
        { GUID "UID_393FBAAE-A08B-429f-8DD1-F6A093356F12" }
    }
}

Cntx 68
{
    { Type OutFlag }
    { Loc -168 6 }
    { Scale 128.00 128.00 }
    { Label
        { Text "within" }
        { Text "limit" }
    }
    { Parent 26 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4898 }
        { GUID "UID_65180EC5-F1BA-472a-A225-7033FE6FD531" }
    }
}

Cntx 69
{
    { Type InFlag }
    { Loc -241 190 }
    { Scale 128.00 128.00 }
    { Label
        { Text "green" }
        { Text "pulse" }
    }
    { Parent 35 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4900 }
        { GUID "UID_87DE5E3A-409D-4a55-A3C4-AE7DFBF54B69" }
    }
}

Cntx 70
{
    { Type InFlag }
    { Loc -142 283 }
    { Scale 128.00 128.00 }
    { Label
        { Text "red" }
        { Text "pulse" }
    }
    { Parent 38 }
    { Object
        { PDMType Cntx }
        { MappingName Default }
        { ID 4902 }
        { GUID "UID_F10D6A79-B8AF-4224-B166-8E55A0B0D0B9" }
    }
}

Link 93
{
    { FirstArc 21 }
    { LastArc 21 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 2602 }
        { GUID "UID_F85EEE63-2C63-4949-8B55-5D1F46DB8A6F" }
    }
}

Link 94
{
    { FirstArc 24 }
    { LastArc 24 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 4887 }
        { GUID "UID_384C3D7C-0622-44a9-8C5F-38D628787932" }
    }
}

Link 95
{
    { FirstArc 26 }
    { LastArc 26 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 4889 }
        { GUID "UID_F8F3532D-9B07-4b83-8F6A-A782F1F5C186" }
    }
}

Link 96
{
    { FirstArc 27 }
    { LastArc 27 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 4893 }
        { GUID "UID_A0F9028A-36EF-4a29-9873-D39D31EA1005" }
    }
}

Link 97
{
    { FirstArc 35 }
    { LastArc 35 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 4899 }
        { GUID "UID_CB0F5A43-3A27-4c67-95D8-A4FC3F88162C" }
    }
}

Link 98
{
    { FirstArc 36 }
    { LastArc 36 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 2619 }
        { GUID "UID_7C846F76-E038-4ff3-A64E-0F8B4CAABF18" }
    }
}

Link 99
{
    { FirstArc 38 }
    { LastArc 38 }
    { Object
        { PDMType Link }
        { MappingName Default }
        { ID 4901 }
        { GUID "UID_124AE05C-9D67-4d84-ACAB-CB8AD8E9DD73" }
    }
}
