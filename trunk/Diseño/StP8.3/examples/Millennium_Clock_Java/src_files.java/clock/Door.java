/***************************************************
 * Door.java
 * ---------
 *
 * Author:  YOUR_NAME
 * Company: YOUR_COMPANY
 * Date:    04/08/02
 *
 * Copyright (c), 2001, YOUR_COMPANY
 **************************************************/    
package clock;

import display.Display;
//#ACD# M(UDIS::UID_378C31F6-544B-11d4-8778-00C04F8DEC45) ---- additional import statements
//user defined code to be added here ...

//#end ACD#

/**
 * The class Door is extended with an tagged value
 * "pattern=singleton".
 * The reason is that two other classes (Clockwork and Clockbird)
 * have associations to class Door.
 * Because the coocoo watch only has one door it must be ensured 
 * that the instances of Clockwork and Clockbird hold the same 
 * reference to the same (single) door.
 * If the codegenerator detects that tagged value it automatically
 * generates the appropriate class as singleton.
 * This has some impacts for the class (marked as singleton) as well
 * as classes that hold associations to the singleton class.
 * These impacts are:
 * - for the class Door:
 *   it will be generated a static instance variable of type Door,
 *   it will be generated a static instance access method that returns
 *   the content of the static instance variable.
 * - for classes having association to the class marked as singleton:
 *   to obtain a reference to the singleton class its static
 *   instance access method is invoked.
 */
public class Door {

  // ------------------------------------------------------------
  // static attributes
  // ------------------------------------------------------------
  /**
   * Single instance of class Door.
   */
  private static Door thisInstance = null;

  // ------------------------------------------------------------
  // instance attributes
  // ------------------------------------------------------------
  private boolean doorStatus;
  protected Display display;

  //#ACD# M(UDAT::UID_378C31F6-544B-11d4-8778-00C04F8DEC45) ---- additional attributes
  //user defined code to be added here ...

  //#end ACD#

  // ------------------------------------------------------------
  // constructors
  // ------------------------------------------------------------
  protected Door() {
    //#ACD# M(UDCC::UID_2CE1BDB2-369A-11d6-AEA1-0080C88207C4) ---- constructor body
    //user defined code to be added here ...

    //#end ACD#
    display = new Display();
  }

  /**
   * This constructor initializes all instance attributes,
   * which are not derived.
   */
  private Door(boolean doorStatus, Display display) {
    //#ACD# M(UDCC::UID_378C31F6-544B-11d4-8778-00C04F8DEC45-full-ctor) ---- constructor body
    //user defined code to be added here ...

    //#end ACD#
    this.doorStatus = doorStatus;
    this.display = display;
  }

  //#ACD# M(UDCO::UID_378C31F6-544B-11d4-8778-00C04F8DEC45) ---- additional constructors
  //user defined code to be added here ...

  //#end ACD#

  // ------------------------------------------------------------
  // methods
  // ------------------------------------------------------------
  public void close() {
    //#ACD# M(UDOP::UID_378C31F8-544B-11d4-8778-00C04F8DEC45) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    display.showDoorState("\nDoor closed\n\n");
    doorStatus = false;
  }

  public void open() {
    //#ACD# M(UDOP::UID_378C31F7-544B-11d4-8778-00C04F8DEC45) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    display.showDoorState("Door opened\n");
    doorStatus = true;
  }

  protected void finalize() {
    //#ACD# M(UDOP::UID_378C31F6-544B-11d4-8778-00C04F8DEC45-finalizer) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    try { super.finalize(); } 
    catch(Throwable t) { return; }
    finally { return; }
  }

  /**
   * Automatically generated <code>set</code>-method 
   * from attribute <code>doorStatus</code>.
   * 
   * @param doorStatus Sets this instance's 
   *    <code>doorStatus</code> to a new value.
   * @see #getDoorStatus()
   */
  public void setDoorStatus(boolean doorStatus) {
    //#ACD# M(UDOP::UID_378C3201-544B-11d4-8778-00C04F8DEC45-setter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    this.doorStatus = doorStatus;
  }

  /**
   * Automatically generated <code>get()</code>-method 
   * from attribute <code>doorStatus</code>.
   * 
   * @return This instance's <code>doorStatus</code>.
   * @see #setDoorStatus(boolean)
   */
  public boolean getDoorStatus() {
    //#ACD# M(UDOP::UID_378C3201-544B-11d4-8778-00C04F8DEC45-getter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    return doorStatus;
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
    //#ACD# M(UDOP::UID_04328C67-544E-11d4-8778-00C04F8DEC45-assoc-setter) ---- method body
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
    //#ACD# M(UDOP::UID_04328C67-544E-11d4-8778-00C04F8DEC45-assoc-getter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    return display;
  }

  /**
   * Returns the only instance of class Door.
   * Do not create a public or protected constructor for this
   * class, otherwise this method wouln't make much sense!
   * 
   * @return The single instance of this class.
   */
  public static Door getInstance() {
    //#ACD# M(UDOP::UID_378C31F6-544B-11d4-8778-00C04F8DEC45-inst-op) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    if (thisInstance == null)
      thisInstance = new Door();
    return thisInstance;
  }

  //#ACD# M(UDOP::UID_378C31F6-544B-11d4-8778-00C04F8DEC45) ---- additional operations
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
    strbuf.append("Door:\n");
    strbuf.append("doorStatus: " + doorStatus).append("\n");
    if (display != null)
      strbuf.append("display: " + display.toString()).append("\n");
    else
      strbuf.append("display: null").append("\n"); 
    //#ACD# M(UID_378C31F6-544B-11d4-8778-00C04F8DEC45-toString) ---- method body
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
    if (! (o instanceof Door))
      return false;
    else {
      Door c = (Door) o;
      if (
          this.doorStatus == c.doorStatus
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
    Door obj = new Door();
    obj.doorStatus = doorStatus;
    obj.display = display;
    return obj;
  }

}
