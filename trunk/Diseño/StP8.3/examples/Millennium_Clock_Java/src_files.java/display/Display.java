/***************************************************
 * Display.java
 * ------------
 *
 * Author:  YOUR_NAME
 * Company: YOUR_COMPANY
 * Date:    04/08/02
 *
 * Copyright (c), 2001, YOUR_COMPANY
 **************************************************/    
package display;

//#ACD# M(UDIS::UID_04328C60-544E-11d4-8778-00C04F8DEC45) ---- additional import statements
//user defined code to be added here ...

//#end ACD#

public class Display {

  //#ACD# M(UDAT::UID_04328C60-544E-11d4-8778-00C04F8DEC45) ---- additional attributes
  //user defined code to be added here ...

  //#end ACD#

  // ------------------------------------------------------------
  // constructors
  // ------------------------------------------------------------
  /**
   * Default constructor
   */
  public Display() {
    //#ACD# M(UDCC::UID_04328C60-544E-11d4-8778-00C04F8DEC45-def-ctor) ---- constructor body
    //user defined code to be added here ...

    //#end ACD#
  }

  //#ACD# M(UDCO::UID_04328C60-544E-11d4-8778-00C04F8DEC45) ---- additional constructors
  //user defined code to be added here ...

  //#end ACD#

  // ------------------------------------------------------------
  // methods
  // ------------------------------------------------------------
  public void showTime(int hour, int minute) {
    //#ACD# M(UDOP::UID_066C8551-544E-11d4-8778-00C04F8DEC45) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    System.out.print(hour + "." + minute + " ");
    if (minute == 0)
    	System.out.print("\n");
  }

  public void showBirdCry(String s) {
    //#ACD# M(UDOP::UID_066C8552-544E-11d4-8778-00C04F8DEC45) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    System.out.print(s);
  }

  public void showDoorState(String s) {
    //#ACD# M(UDOP::UID_066C8553-544E-11d4-8778-00C04F8DEC45) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    System.out.print(s);
  }

  protected void finalize() {
    //#ACD# M(UDOP::UID_04328C60-544E-11d4-8778-00C04F8DEC45-finalizer) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    try { super.finalize(); } 
    catch(Throwable t) { return; }
    finally { return; }
  }

  //#ACD# M(UDOP::UID_04328C60-544E-11d4-8778-00C04F8DEC45) ---- additional operations
  //user defined code to be added here ...

  //#end ACD#
  // ------------------------------------------------------------
  // standard methods
  // ------------------------------------------------------------

  /**
   * Generates a String representation of this object.
   * Loops over all non-static fields of this object.
   *
   * Users are encouraged to accustom this function to
   * their specific needs!
   *
   * @return The content (state) of this object as string.
   */
  public String toString() {
    StringBuffer strbuf = new StringBuffer();
    strbuf.append("Display:\n");
    //#ACD# M(UID_04328C60-544E-11d4-8778-00C04F8DEC45-toString) ---- method body
    //user defined code to be added here ...
    
    //#end ACD#
    return strbuf.toString();
  }

  /**
   * Tests this instance against another object for equality.
   * Rather to test for simple object reference equality as in
   * {@link java.lang.Object#equals(Object) Object.equals(Object o)} 
   * this function tests against <i>semantic</i> equivalence of the 
   * content of the two objects.<br><br>
   *
   * Note that instances of classes without instance attributes return
   * always <code>true</code>, since they <b>must</b> have the same
   * state.<br><br>
   *
   * Users are encouraged to customize this function to
   * their specific needs!
   *
   * @param o The object this instance is compared to.
   * @return <code>true</code>, if the two objects are equivalent semantically.
   */
  public boolean equals(Object o) {
    if (! (o instanceof Display))
      return false;
    else {
      return true;
    }
  }

  /**
   * Provides a (shallow) copy of this instance and returns it. 
   * It achieves that by looping over all non-static fields of 
   * this instance and assigning their values to the corresponding 
   * fields of the new object. <br><br>
   * Note, that there <b>must</b> be an accessible no-arg default 
   * constructor for this function to work, either explicitly in 
   * the class or by <b>not</b> defining any other constructors!
   *
   * @return The newly created object.
   */
  public Object clone() {
    Display obj = new Display();
    return obj;
  }

}
