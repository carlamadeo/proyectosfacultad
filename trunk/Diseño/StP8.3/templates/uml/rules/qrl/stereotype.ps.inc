//  ECR 3162 begin
struct stereot_def {
	string name;
	string stp_symbol;
	string uml_element;
	string page;
	string constraint;
	string desc;
};
list builtin_stereotypes = list_create("stereot_def",0);
void
init_stereot_def()
{
	stereot_def dummy;
if(list_count(builtin_stereotypes) > 0) return;
dummy.name="access";
dummy.stp_symbol="Dependency";
dummy.uml_element="Permission";
dummy.page="Page: 2-42";
dummy.constraint="";
dummy.desc="Permission Access is a stereotyped permission dependency between two namespaces denoting that the public contents of the target namespace are accessible to the namespace of the source package.";
list_append( builtin_stereotypes, dummy );

dummy.name="association";
dummy.stp_symbol="Role";
dummy.uml_element="AssociationEnd";
dummy.page="Page: 2-23";
dummy.constraint="";
dummy.desc="Specifies a real association (default and redundant, but may be included for emphasis";
list_append( builtin_stereotypes, dummy );

dummy.name="become";
dummy.stp_symbol="Dependency";
dummy.uml_element="Flow";
dummy.page="Page: 2-34";
dummy.constraint="";
dummy.desc="Specifies a Flow relationship, source and target of which represent the same instance at different points in time, but each with potentially different values, state instance, and roles. A Become Dependency from A to B means that instance A becomes B with possibly new values, state instance, and roles at a different moment in time/space.";
list_append( builtin_stereotypes, dummy );

dummy.name="call";
dummy.stp_symbol="Dependency";
dummy.uml_element="Usage";
dummy.page="Page: 2-43";
dummy.constraint="";
dummy.desc="Call is a stereotyped usage dependency whose source is an operation and whose target is an operation. The relationship may also be subsumed to the class containing an operation, with the meaning that there exists an operation in the class to which the dependency applies. A call dependency specifies that the source operation or an operation in the source class invokes the target operation or an operation in the target class. A call dependency may connect a source operation to any target operation that is within scope including, but not limited to, operations of the enclosing classifier and operations of other visible classifiers.";
list_append( builtin_stereotypes, dummy );

dummy.name="copy";
dummy.stp_symbol="Dependency";
dummy.uml_element="Flow";
dummy.page="Page: 2-34";
dummy.constraint="";
dummy.desc="Flow Specifies a Flow relationship, the source and target of which are different instances, but each with the same values, state instance, and roles (but a distinct identity). A Copy Dependency from A to B means that B is an exact copy of A. Future changes in A are not necessarily reflected in B.";
list_append( builtin_stereotypes, dummy );

dummy.name="create";
dummy.stp_symbol="Operation";
dummy.uml_element="BehavioralFeature";
dummy.page="Page: 2-26";
dummy.constraint="";
dummy.desc="Specifies that the designated feature creates an instance of the classifier to which the feature is attached. May be promoted to the Classifier containing the feature.";
list_append( builtin_stereotypes, dummy );

dummy.name="create";
dummy.stp_symbol="Message";
dummy.uml_element="CallEvent";
dummy.page="Page: 2-132";
dummy.constraint="";
dummy.desc="Create is a stereotyped call event denoting that the instance receiving that event has just been created. For state machines, it triggers the initial transition at the topmost level of the state machine (and is the only kind of trigger that may be applied to an initial transition).";
list_append( builtin_stereotypes, dummy );

dummy.name="create";
dummy.stp_symbol="Dependency";
dummy.uml_element="Usage";
dummy.page="Page: 2-44";
dummy.constraint="";
dummy.desc="Create is a stereotyped usage dependency denoting that the client classifier creates instances of the supplier classifier.";
list_append( builtin_stereotypes, dummy );

dummy.name="derive";
dummy.stp_symbol="Dependency";
dummy.uml_element="Abstraction";
dummy.page="Page: 2-19";
dummy.constraint="";
dummy.desc="(Name for the stereotyped class is Derivation.) Specifies a derivation relationship among model elements that are usually, but not necessarily, of the same type. A derived dependency specifies that the client may be computed from the supplier. The mapping specifies the computation. The client may be implemented for design reasons, such as efficiency, even though it is logically redundant.";
list_append( builtin_stereotypes, dummy );

dummy.name="destroy";
dummy.stp_symbol="Operation";
dummy.uml_element="BehavioralFeature";
dummy.page="Page: 2-26";
dummy.constraint="";
dummy.desc="Specifies that the designated feature destroys an instance of the classifier to which the feature is attached. May be promoted to the classifier containing the feature.";
list_append( builtin_stereotypes, dummy );

dummy.name="destroy";
dummy.stp_symbol="Message";
dummy.uml_element="CallEvent";
dummy.page="Page: 2-132";
dummy.constraint="";
dummy.desc="Destroy is a stereotyped call event denoting that the instance receiving the event is being destroyed.";
list_append( builtin_stereotypes, dummy );

dummy.name="document";
dummy.stp_symbol="sourceComponent#executableComponent#binaryComponent#interfaceComponent#objectComponent";
dummy.uml_element="Component";
dummy.page="Page: 2-29";
dummy.constraint="";
dummy.desc="Denotes a document.";
list_append( builtin_stereotypes, dummy );

dummy.name="executable";
dummy.stp_symbol="sourceComponent#executableComponent#binaryComponent#interfaceComponent#objectComponent";
dummy.uml_element="Component";
dummy.page="Page: 2-29";
dummy.constraint="";
dummy.desc="Denotes a program that may be run on a node.";
list_append( builtin_stereotypes, dummy );

dummy.name="facade";
dummy.stp_symbol="Package";
dummy.uml_element="Package";
dummy.page="Page: 2-174";
dummy.constraint="";
dummy.desc="A facade is a stereotyped package containing only references to model elements owned by another package. It is used to provide a ‘public view’ of some of the contents of a package. A facade does not contain any model elements of its own.";
list_append( builtin_stereotypes, dummy );

dummy.name="file";
dummy.stp_symbol="Component";
dummy.uml_element="sourceComponent#executableComponent#binaryComponent#interfaceComponent#objectComponent";
dummy.page="Page: 2-29";
dummy.constraint="";
dummy.desc="Denotes a document containing source code or data.";
list_append( builtin_stereotypes, dummy );

dummy.name="framework";
dummy.stp_symbol="Package";
dummy.uml_element="Package";
dummy.page="Page: 2-174";
dummy.constraint="";
dummy.desc="A framework is a stereotyped package consisting mainly of patterns, where patterns are defined as template collaborations.";
list_append( builtin_stereotypes, dummy );

dummy.name="friend";
dummy.stp_symbol="Dependency";
dummy.uml_element="Permission";
dummy.page="Page: 2-42";
dummy.constraint="";
dummy.desc="Friend is a stereotyped permission dependency whose source is a model element, such as an operation, class, or package, and whose target is a model element in a different package, such as an operation, class or package. A friend relationship grants the source access to the target regardless of the declared visibility. It extends the visibility of the supplier so that the client can see into the supplier.";
list_append( builtin_stereotypes, dummy );

dummy.name="global";
dummy.stp_symbol="Role";
dummy.uml_element="AssociationEnd";
dummy.page="Page: 2-23";
dummy.constraint="";
dummy.desc="Specifies that the target is a global value that is known to all elements rather than an actual association.";
list_append( builtin_stereotypes, dummy );

dummy.name="implementation";
dummy.stp_symbol="Generalization";
dummy.uml_element="Generalization";
dummy.page="Page: 2-36";
dummy.constraint="";
dummy.desc="Specifies that the child inherits the implementation of the parent (its attributes, operations and methods) but does not make public the supplier’s interfaces nor guarantee to support them, thereby violating substitutability. This is private inheritance and is usually used only for programming implementation purposes.";
list_append( builtin_stereotypes, dummy );

dummy.name="implementationClass";
dummy.stp_symbol="Class";
dummy.uml_element="Class";
dummy.page="Page: 2-27";
dummy.constraint="";
dummy.desc="Specifies the implementation of a class in some programming language in which an instance may not have more than one class. This is in contrast to a general UML Class, for which an instance may have multiple classes at one time and may gain or lose classes over time, and an object (a child of instance) may dynamically have multiple classes.";
list_append( builtin_stereotypes, dummy );

// ECR 6197 - added page and description elements
// ECR 5196 begin add
dummy.name="interface";
dummy.stp_symbol="Class";
dummy.uml_element="Class";
dummy.page="Page: 2-36";
dummy.constraint="";
dummy.desc="Contains a set of Operations that together define a service offered by a Classifier realizing the Interface. A Classifier may offer several services, which means that it may realize several Interfaces, and several Classifiers may realize the same Interface.";
list_append( builtin_stereotypes, dummy );
// ECR 5196 end add

dummy.name="implicit";
dummy.stp_symbol="Association";
dummy.uml_element="Association";
dummy.page="Page: 2-20";
dummy.constraint="";
dummy.desc="The «implicit» stereotype is applied to an association, specifying that the association is not manifest, but rather is only conceptual.";
list_append( builtin_stereotypes, dummy );

dummy.name="import";
dummy.stp_symbol="Dependency";
dummy.uml_element="Permission";
dummy.page="Page: 2-42";
dummy.constraint="";
dummy.desc="Import is a stereotyped permission dependency between two namespaces, denoting that the public contents of the target package are added to the namespace of the source package.";
list_append( builtin_stereotypes, dummy );

dummy.name="instantiate";
dummy.stp_symbol="Dependency";
dummy.uml_element="Usage";
dummy.page="Page: 2-44";
dummy.constraint="";
dummy.desc="A stereotyped usage dependency among classifiers indicating that operations on the client create instances of the supplier.";
list_append( builtin_stereotypes, dummy );

dummy.name="invariant";
dummy.stp_symbol="Constraint";
dummy.uml_element="Constraint";
dummy.page="Page: 2-30";
dummy.constraint="";
dummy.desc="Specifies a constraint that must be attached to a set of classifiers or relationships. It indicates that the conditions of the constraint must hold over time (for the time period of concern in the particular containing element) for the classifiers or relationships and their instances.";
list_append( builtin_stereotypes, dummy );

dummy.name="library";
dummy.stp_symbol="sourceComponent#executableComponent#binaryComponent#interfaceComponent#objectComponent";
dummy.uml_element="Component";
dummy.page="Page: 2-29";
dummy.constraint="";
dummy.desc="Denotes a static or dynamic library.";
list_append( builtin_stereotypes, dummy );

dummy.name="local";
dummy.stp_symbol="Role";
dummy.uml_element="AssociationEnd";
dummy.page="Page: 2-23";
dummy.constraint="";
dummy.desc="Specifies that the relationship represents a local variable within a procedure rather than an actual association.";
list_append( builtin_stereotypes, dummy );

dummy.name="metaclass";
dummy.stp_symbol="Class";
dummy.uml_element="Classifier";
dummy.page="Page: 2-28";
dummy.constraint="";
dummy.desc="Specifies that the instances of the classifier are classes.";
list_append( builtin_stereotypes, dummy );

dummy.name="metamodel";
dummy.stp_symbol="Package";
dummy.uml_element="Package";
dummy.page="Page: 2-173";
dummy.constraint="";
dummy.desc="A metamodel is a stereotyped model denoting that the model is an abstraction of another model, i.e., it is a model of a model. Hence, if M2 is a model of the model M1, then M2 is a metamodel of M1. It follows then that classes in M1 are instances of metaclasses in M2. The stereotype can be recursively applied, as in the case of a 4-layer metamodel architecture.";
list_append( builtin_stereotypes, dummy );

dummy.name="parameter";
dummy.stp_symbol="Role";
dummy.uml_element="AssociationEnd";
dummy.page="Page: 2-23";
dummy.constraint="";
dummy.desc="Specifies that the relationship represents a procedure parameter rather than an actual association.";
list_append( builtin_stereotypes, dummy );

dummy.name="postcondition";
dummy.stp_symbol="Constraint";
dummy.uml_element="Constraint";
dummy.page="Page: 2-30";
dummy.constraint="";
dummy.desc="Specifies a constraint that must be attached to an operation, and denotes that the conditions of the constraint must hold after the invocation of the operation.";
list_append( builtin_stereotypes, dummy );

dummy.name="powertype";
dummy.stp_symbol="Class";
dummy.uml_element="Classifier";
dummy.page="Page: 2-28";
dummy.constraint="";
dummy.desc="Specifies that the classifier is a metatype, the instances of which are children marked by the same discriminator.";
list_append( builtin_stereotypes, dummy );

dummy.name="precondition";
dummy.stp_symbol="Constraint";
dummy.uml_element="Constraint";
dummy.page="Page: 2-30";
dummy.constraint="";
dummy.desc="Specifies a constraint that must be attached to an operation, and denotes that the conditions of the constraint must hold for the invocation of the operation.";
list_append( builtin_stereotypes, dummy );

dummy.name="process";
dummy.stp_symbol="Class";
dummy.uml_element="Classifier";
dummy.page="Page: 2-28";
dummy.constraint="";
dummy.desc="Specifies a classifier that represents a heavy-weight flow of control.";
list_append( builtin_stereotypes, dummy );

dummy.name="realize";
dummy.stp_symbol="Dependency";
dummy.uml_element="Abstraction";
dummy.page="Page: 2-19";
dummy.constraint="";
dummy.desc="(Name for the stereotyped class is Realization.) Specifies a realization relationship between a specification model element or elements (the supplier) and a model element or elements that implement it (the client). The implementation model element is required to support all of the operations or received signals that the specification model element declares. The implementation model element must make or inherit its own declarations of the operations and signal receptions. The mapping specifies the relationship between the two. The mapping may or may not be computable. Realization can be used to model stepwise refinement, optimizations, transformations, templates, model synthesis, framework composition, etc.";
list_append( builtin_stereotypes, dummy );

dummy.name="refine";
dummy.stp_symbol="Dependency";
dummy.uml_element="Abstraction";
dummy.page="Page: 2-19";
dummy.constraint="";
dummy.desc="(Name for the stereotyped class is Refinement.) Specifies refinement relationship between model elements at different semantic levels, such as analysis and design. The mapping specifies the relationship between the two elements or sets of elements. The mapping may or may not be computable, and it may be unidirectional or bidirectional. Refinement can be used to model transformations from analysis to design and other such changes.";
list_append( builtin_stereotypes, dummy );

dummy.name="requirement";
dummy.stp_symbol="Comment";
dummy.uml_element="Comment";
dummy.page="Page: 2-29";
dummy.constraint="";
dummy.desc="Specifies a desired feature, property, or behavior of an element as part of a system.";
list_append( builtin_stereotypes, dummy );

dummy.name="responsibility";
dummy.stp_symbol="Comment";
dummy.uml_element="Comment";
dummy.page="Page: 2-29";
dummy.constraint="";
dummy.desc="Specifies a contract or an obligation of an element in its relationship to other elements.";
list_append( builtin_stereotypes, dummy );

dummy.name="self";
dummy.stp_symbol="Role";
dummy.uml_element="AssociationEnd";
dummy.page="Page: 2-23";
dummy.constraint="";
dummy.desc="Specifies that the relationship represents a reference to the object that owns an operation or action rather than an actual association.";
list_append( builtin_stereotypes, dummy );

dummy.name="send";
dummy.stp_symbol="Dependency";
dummy.uml_element="Usage";
dummy.page="Page: 2-44";
dummy.constraint="";
dummy.desc="Send is a stereotyped usage dependency whose source is an operation and whose target is a signal, specifying that the source sends the target signal.";
list_append( builtin_stereotypes, dummy );

dummy.name="signalflow";
dummy.stp_symbol="ObjectDependencyState";
dummy.uml_element="ObjectFlowState";
dummy.page="Page: 2-163";
dummy.constraint="";
dummy.desc="Signalflow is a stereotype of ObjectFlowState with a Signal as its type.";
list_append( builtin_stereotypes, dummy );

dummy.name="stub";
dummy.stp_symbol="Package";
dummy.uml_element="Package";
dummy.page="Page: 2-174";
dummy.constraint="";
dummy.desc="A stub is a stereotyped package representing a package that is incompletely transferred; specifically, a stub provides the public parts of the package, but nothing more.";
list_append( builtin_stereotypes, dummy );

// Added for ModelManagement, ECR 4052
dummy.name="subsystem";
dummy.stp_symbol="Package";
dummy.uml_element="Package";
dummy.page="";
dummy.constraint="";
dummy.desc="";
list_append( builtin_stereotypes, dummy );
// End ECR 4052

dummy.name="systemModel";
dummy.stp_symbol="Package";
dummy.uml_element="Package";
dummy.page="Page: 2-173";
dummy.constraint="";
dummy.desc="A systemModel is a stereotyped model that contains a collection of models of the same physical system. A systemModel also contains all relationships and constraints between model elements contained in different models.";
list_append( builtin_stereotypes, dummy );

dummy.name="table";
dummy.stp_symbol="sourceComponent#executableComponent#binaryComponent#interfaceComponent#objectComponent";
dummy.uml_element="Component";
dummy.page="Page: 2-29";
dummy.constraint="";
dummy.desc="Denotes a data base table.";
list_append( builtin_stereotypes, dummy );

dummy.name="thread";
dummy.stp_symbol="Class";
dummy.uml_element="Classifier";
dummy.page="Page: 2-28";
dummy.constraint="";
dummy.desc="Specifies a classifier that represents a flow of control.";
list_append( builtin_stereotypes, dummy );

dummy.name="topLevel";
dummy.stp_symbol="Package";
dummy.uml_element="Package";
dummy.page="Page: 2-174";
dummy.constraint="";
dummy.desc="TopLevel is a stereotype of package denoting the top-most package in a containment hierarchy. The topLevel stereotype defines the outer limit for looking up names, as namespaces “see” outwards. A topLevel subsystem represents the top of the subsystem containment hierarchy, i.e., it is the model element that represents the boundary of the entire physical system being modeled.";
list_append( builtin_stereotypes, dummy );

dummy.name="trace";
dummy.stp_symbol="Dependency";
dummy.uml_element="Abstraction";
dummy.page="Page: 2-19";
dummy.constraint="";
dummy.desc="(Name for the stereotyped class is Trace.) Specifies a trace relationship between model elements or sets of model elements that represent the same concept in different models. Traces are mainly used for tracking requirements and changes across models. Since model changes can occur in both directions, the directionality of the dependency can often be ignored. The mapping specifies the relationship between the two, but it is rarely computable and is usually informal.";
list_append( builtin_stereotypes, dummy );

dummy.name="type";
dummy.stp_symbol="Class";
dummy.uml_element="Class";
dummy.page="Page: 2-27";
dummy.constraint="";
dummy.desc="Specifies a domain of instances (objects) together with the operations applicable to the objects. A type may not contain any methods, but it may have attributes and associations.";
list_append( builtin_stereotypes, dummy );

dummy.name="utility";
dummy.stp_symbol="Class";
dummy.uml_element="Classifier";
dummy.page="Page: 2-28";
dummy.constraint="";
dummy.desc="Specifies a classifier that has no instances, but rather denotes a named collection of non-member attributes and operations, all of which are class-scoped.";
list_append( builtin_stereotypes, dummy );

// ECR 6913 begin
dummy.name="bind";
dummy.stp_symbol="Refines";
dummy.uml_element="Binding";
dummy.page="Page: 2-26";
dummy.constraint="";
dummy.desc="A binding of template parameters to actual values to create a nonparameterized element.";
list_append( builtin_stereotypes, dummy );
// ECR 6913 end
}
//  ECR 3162 end
