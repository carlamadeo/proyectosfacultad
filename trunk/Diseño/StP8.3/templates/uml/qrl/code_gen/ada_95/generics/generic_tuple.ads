generic
 
    type type1 is private;
    type type2 is private;
 
package generic_tuple is
 
    type tuple is record
        comp1: type1;
        comp2: type2;
    end record;
 
end generic_tuple;

