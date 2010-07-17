/***************************************************
 * ClockBird.java
 * --------------
 *
 * Author:  YOUR_NAME
 * Company: YOUR_COMPANY
 * Date:    04/08/02
 *
 * Copyright (c), 2001, YOUR_COMPANY
 **************************************************/    
package clock;

import display.Display;
//#ACD# M(UDIS::UID_378C31F2-544B-11d4-8778-00C04F8DEC45) ---- additional import statements
//user defined code to be added here ...

//#end ACD#

public class ClockBird {

  // ------------------------------------------------------------
  // instance attributes
  // ------------------------------------------------------------
  protected Door door;
  protected Display display;

  //#ACD# M(UDAT::UID_378C31F2-544B-11d4-8778-00C04F8DEC45) ---- additional attributes
  //user defined code to be added here ...

  //#end ACD#

  // ------------------------------------------------------------
  // constructors
  // ------------------------------------------------------------
  protected ClockBird() {
    //#ACD# M(UDCC::UID_2CE1BB6B-369A-11d6-AEA1-0080C88207C4) ---- constructor body
    //user defined code to be added here ...

    //#end ACD#
    door = Door.getInstance();
    display = new Display();
  }

  /**
   * This constructor initializes all instance attributes,
   * which are not derived.
   */
  public ClockBird(Door door, Display display) {
    //#ACD# M(UDCC::UID_378C31F2-544B-11d4-8778-00C04F8DEC45-full-ctor) ---- constructor body
    //user defined code to be added here ...

    //#end ACD#
    this.door = door;
    this.display = display;
  }

  //#ACD# M(UDCO::UID_378C31F2-544B-11d4-8778-00C04F8DEC45) ---- additional constructors
  //user defined code to be added here ...

  //#end ACD#

  // ------------------------------------------------------------
  // methods
  // ------------------------------------------------------------
  public void cry() {
    //#ACD# M(UDOP::UID_378C31FD-544B-11d4-8778-00C04F8DEC45) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
  }

  public boolean isDoorOpened() {
    //#ACD# M(UDOP::UID_066C8550-544E-11d4-8778-00C04F8DEC45) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    return door.getDoorStatus();
  }

  protected void finalize() {
    //#ACD# M(UDOP::UID_378C31F2-544B-11d4-8778-00C04F8DEC45-finalizer) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    try { super.finalize(); } 
    catch(Throwable t) { return; }
    finally { return; }
  }

  /**
   * Automatically generated <code>set</code>-method 
   * from attribute <code>door</code>.
   * 
   * @param door Sets this instance's 
   *    <code>door</code> to a new value.
   * @see #getDoor()
   */
  public void setDoor(Door door) {
    //#ACD# M(UDOP::UID_94B689F8-544C-11d4-8778-00C04F8DEC45-assoc-setter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    this.door = door;
  }

  /**
   * Automatically generated <code>get()</code>-method 
   * from attribute <code>door</code>.
   * 
   * @return This instance's <code>door</code>.
   * @see #setDoor(Door)
   */
  public Door getDoor() {
    //#ACD# M(UDOP::UID_94B689F8-544C-11d4-8778-00C04F8DEC45-assoc-getter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    return door;
  }

  /**
   * Automatically generated <code>set</code>-method 
   * from attribute <code>display</code>.
   * 
   * @param display Sets this instance's 
   *    <code>display</code> to a new value.
   * @see #getDisplay()
   */
  public void setDisplay(Display display) {
    //#ACD# M(UDOP::UID_04328C64-544E-11d4-8778-00C04F8DEC45-assoc-setter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    this.display = display;
  }

  /**
   * Automatically generated <code>get()</code>-method 
   * from attribute <code>display</code>.
   * 
   * @return This instance's <code>display</code>.
   * @see #setDisplay(Display)
   */
  public Display getDisplay() {
    //#ACD# M(UDOP::UID_04328C64-544E-11d4-8778-00C04F8DEC45-assoc-getter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    return display;
  }

  //#ACD# M(UDOP::UID_378C31F2-544B-11d4-8778-00C04F8DEC45) ---- additional operations
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
    strbuf.append("ClockBird:\n");
    if (door != null)
      strbuf.append("door: " + door.toString()).append("\n");
    else
      strbuf.append("door: null").append("\n"); 
    if (display != null)
      strbuf.append("display: " + display.toString()).append("\n");
    else
      strbuf.append("display: null").append("\n"); 
    //#ACD# M(UID_378C31F2-544B-11d4-8778-00C04F8DEC45-toString) ---- method body
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
    if (! (o instanceof ClockBird))
      return false;
    else {
      ClockBird c = (ClockBird) o;
      if (
          ((door == null && c.door == null) || (door != null && c.door != null && door.equals(c.door)))
          && ((display == null && c.display == null) || (display != null && c.display != null && display.equals(c.display)))
         )
        return true;
      else
        return false;
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
    ClockBird obj = new ClockBird();
    obj.door = door;
    obj.display = display;
    return obj;
  }

}
