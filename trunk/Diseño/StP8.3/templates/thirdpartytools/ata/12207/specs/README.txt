This directory defines the UseCaseModel "Derived Object" using Specifications.  

The starting Use Case Diagram of the UseCaseModel is specified on the command line of parse_fmt with the options oname and otype.    

Foreach Use Case Diagram in the tree
    A Section titled with the UCD's name and type is generated  
    The Object annotation is output
    The Use Case Diagram is output

    Foreach Use Case associated with the UCD
	A Section titled with the Use Case's name and type is generated
	The Object annotation is output

	A Section title is generated for the Pre-Conditions
	All Pre-Conditions are output

	A Section title is generated for the Post-Conditions
	All Post-Conditions are output

	A Section title is generated for the Exceptions
	All Exceptions are output

	Foreach Communicates Link associated with the Use Case
	    The Object annotation is extracted

	    Foreach Use Case Actor associated with the Communicates Link
		A Section titled with the Actor's name and type is generated
		The UseCaseActorDefinition annotation is output

	Foreach Sequence Diagram associated with the Use Case
	    A Section titled with the SD's name and type is generated
	    The Object annotation is output
	    The Sequence Diagram is output

	    Foreach Object associated with the SD
		A Section titled with the Object's name and type is generated
		The Object annotations are output

	Foreach Collaboration Diagram associated with the Use Case
	    A Section titled with the COD's name and type is generated
	    The Object annotation is output
	    The Collaboration Diagram is output

	    Foreach Object associated with the COD
		A Section titled with the Object's name and type is generated
		The Object annotations are output

	Foreach Activity Diagram associated with the Use Case
	    A Section titled with the AD's name and type is generated
	    The Object annotation is output
	    The Activity Diagram is output

	    Foreach Object associated with the AD
		A Section titled with the Object's name and type is generated
		The Object annotations are output
