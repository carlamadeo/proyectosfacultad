/***************************************************
 * Pendulum.java
 * -------------
 *
 * Author:  YOUR_NAME
 * Company: YOUR_COMPANY
 * Date:    04/08/02
 *
 * Copyright (c), 2001, YOUR_COMPANY
 **************************************************/    
package clockwork;

//#ACD# M(UDIS::UID_378C31F1-544B-11d4-8778-00C04F8DEC45) ---- additional import statements
//user defined code to be added here ...

//#end ACD#

public class Pendulum {

  // ------------------------------------------------------------
  // instance attributes
  // ------------------------------------------------------------
  protected Clockwork clockwork;

  //#ACD# M(UDAT::UID_378C31F1-544B-11d4-8778-00C04F8DEC45) ---- additional attributes
  //user defined code to be added here ...

  //#end ACD#

  // ------------------------------------------------------------
  // constructors
  // ------------------------------------------------------------
  public Pendulum() {
    //#ACD# M(UDCC::UID_2CE1B817-369A-11d6-AEA1-0080C88207C4) ---- constructor body
    //user defined code to be added here ...

    //#end ACD#
    clockwork = new Clockwork();
    currentState = _NOTMOVING;
  }

  /**
   * This constructor initializes all instance attributes,
   * which are not derived.
   */
  public Pendulum(Clockwork clockwork) {
    //#ACD# M(UDCC::UID_378C31F1-544B-11d4-8778-00C04F8DEC45-full-ctor) ---- constructor body
    //user defined code to be added here ...

    //#end ACD#
    this.clockwork = clockwork;
  }

  //#ACD# M(UDCO::UID_378C31F1-544B-11d4-8778-00C04F8DEC45) ---- additional constructors
  //user defined code to be added here ...

  //#end ACD#

  // ------------------------------------------------------------
  // methods
  // ------------------------------------------------------------
  protected void finalize() {
    //#ACD# M(UDOP::UID_378C31F1-544B-11d4-8778-00C04F8DEC45-finalizer) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    try { super.finalize(); } 
    catch(Throwable t) { return; }
    finally { return; }
  }

  /**
   * Automatically generated <code>set</code>-method 
   * from attribute <code>clockwork</code>.
   * 
   * @param clockwork Sets this instance's 
   *    <code>clockwork</code> to a new value.
   * @see #getClockwork()
   */
  public void setClockwork(Clockwork clockwork) {
    //#ACD# M(UDOP::UID_94B689F5-544C-11d4-8778-00C04F8DEC45-assoc-setter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    this.clockwork = clockwork;
  }

  /**
   * Automatically generated <code>get()</code>-method 
   * from attribute <code>clockwork</code>.
   * 
   * @return This instance's <code>clockwork</code>.
   * @see #setClockwork(Clockwork)
   */
  public Clockwork getClockwork() {
    //#ACD# M(UDOP::UID_94B689F5-544C-11d4-8778-00C04F8DEC45-assoc-getter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    return clockwork;
  }

  //#ACD# M(UDOP::UID_378C31F1-544B-11d4-8778-00C04F8DEC45) ---- additional operations
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
    strbuf.append("Pendulum:\n");
    if (clockwork != null)
      strbuf.append("clockwork: " + clockwork.toString()).append("\n");
    else
      strbuf.append("clockwork: null").append("\n"); 
    //#ACD# M(UID_378C31F1-544B-11d4-8778-00C04F8DEC45-toString) ---- method body
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
    if (! (o instanceof Pendulum))
      return false;
    else {
      Pendulum c = (Pendulum) o;
      if (
          ((clockwork == null && c.clockwork == null) || (clockwork != null && c.clockwork != null && clockwork.equals(c.clockwork)))
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
    Pendulum obj = new Pendulum();
    obj.clockwork = clockwork;
    return obj;
  }


  // ------------------------------------------------------------
  // state machine (realized with nested switch statements)
  // ------------------------------------------------------------

  protected byte currentState;
  // events /////////////////////////////////////////////////////////////////
  public static final byte _MOVE = 0;


  // states /////////////////////////////////////////////////////////////////
  public static final byte _NOTMOVING = 0;
  public static final byte _ISLEFT = 1;
  public static final byte _ISRIGHT = 2;


  /// Event taker functions /////////////////////////////////////////////////
  public void takeEvent(byte ev) {
    switch(currentState) {
      case _NOTMOVING:
        switch(ev) {
                  
          case _MOVE:
            clockwork.takeEvent(clockwork._SETTIME);
            currentState = _ISLEFT;
            IsLeft_Action();
            break;
          default:
            System.out.println("Wrong event occurred "  + "Pendulum " + "NotMoving" + " " + ev);
            break;
        } // end event switch for state NotMoving
        break;
      case _ISLEFT:
        switch(ev) {
                  
          case _MOVE:
            clockwork.takeEvent(clockwork._TICK);
            currentState = _ISRIGHT;
            IsRight_Action();
            break;
          default:
            System.out.println("Wrong event occurred "  + "Pendulum " + "IsLeft" + " " + ev);
            break;
        } // end event switch for state IsLeft
        break;
      case _ISRIGHT:
        switch(ev) {
                  
          case _MOVE:
            clockwork.takeEvent(clockwork._TICK);
            currentState = _ISLEFT;
            IsLeft_Action();
            break;
          default:
            System.out.println("Wrong event occurred "  + "Pendulum " + "IsRight" + " " + ev);
            break;
        } // end event switch for state IsRight
        break;
    } // end state switch
  }

  /// State action functions ////////////////////////////////////////////////
  protected void IsLeft_Action() {
    //#ACD# M(UDAC::UID_98A70156-AC4F-11d4-BEDC-0060971B5EE0_Action) action code
    //user defined code to be added here ...

    //#end ACD#
  }

  protected void IsRight_Action() {
    //#ACD# M(UDAC::UID_98A70159-AC4F-11d4-BEDC-0060971B5EE0_Action) action code
    //user defined code to be added here ...

    //#end ACD#
  }

}
