//
//      sccsid[] = %W% %Y% %D%
//      StP/Booch
//      StP/OMT
//      StP/OO
//	StP Core
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1995
//      All rights reserved
//

// Optional dependency variable declaration
// Dependent link objects of type OMTTransition
// Dependency name: GlobalEvent
boolean dep_OMTEventLink_OMTTransition_GlobalEvent = False;


// Optional dependency variable declaration
// Dependent link objects of type SimpleMessage
// Dependent link objects of type SynchronousMessage
// Dependent link objects of type BalkingMessage
// Dependent link objects of type TimeoutMessage
// Dependent link objects of type AsynchronousMessage
// Dependency name: Name
boolean dep_OMTEventLink_Message_Name = False;

// Optional dependency variable declaration
// Dependent node objects of type OMTEvent
// Dependency name: TransitionEvent
boolean dep_OMTTransition_OMTEvent_TransitionEvent = False;

// Optional dependency variable declaration
// Dependent link objects of type OMTTransition
// Dependency name: TransitionInSameCntx
boolean dep_OMTTransition_OMTTransition_TransitionInSameCntx = False;

// Optional dependency variable declaration
// Dependent node objects of type OMTOperation
// Dependency name: OpOverloaded
boolean dep_OMTOperation_OMTOperation_OpOverloaded = False;


// Optional dependency variable declaration
// Dependent link objects of type OMTTransition
// Dependent node objects of type OMTActivity
// Dependent node objects of type OMTAction
// Dependency name: OpToStateModel
boolean dep_OMTOperation_OpToStateModel = False;


// Optional dependency variable declaration
// Dependent link objects of type SimpleMessage
// Dependent link objects of type SynchronousMessage
// Dependent link objects of type BalkingMessage
// Dependent link objects of type TimeoutMessage
// Dependent link objects of type AsynchronousMessage
// Dependency name: MsgNameCntx
boolean dep_OMTOperation_MsgNameCntx = False;

// Optional dependency variable declaration
// Dependent link objects of type SimpleMessage
// Dependent link objects of type SynchronousMessage
// Dependent link objects of type BalkingMessage
// Dependent link objects of type TimeoutMessage
// Dependent link objects of type AsynchronousMessage
// Dependency name: MsgOperName
boolean dep_OMTOperation_MsgOperName = False;


// Optional dependency variable declaration
// Dependent node objects of type OMTOperation
// Dependency name: OpConstructor
boolean dep_OMTClass_OMTOperation_OpConstructor = False;

// Optional dependency variable declaration
// Dependent link objects of type SimpleMessage
// Dependency name: MessagesInSameCntx
boolean dep_SimpleMessage_SimpleMessage_MessagesInSameCntx = False;

