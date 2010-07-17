/***************************************************
 * Clockwork.java
 * --------------
 *
 * Author:  YOUR_NAME
 * Company: YOUR_COMPANY
 * Date:    04/08/02
 *
 * Copyright (c), 2001, YOUR_COMPANY
 **************************************************/    
package clockwork;

import clock.Owl;
import display.Display;
import clock.Door;
import clock.Cuckoo;
//#ACD# M(UDIS::UID_378C31F0-544B-11d4-8778-00C04F8DEC45) ---- additional import statements
//user defined code to be added here ...

//#end ACD#

public class Clockwork {

  // ------------------------------------------------------------
  // instance attributes
  // ------------------------------------------------------------
  private int hour;
  private int minute;
  protected Owl owl;
  protected Display display;
  protected Door door;
  protected Cuckoo cuckoo;

  //#ACD# M(UDAT::UID_378C31F0-544B-11d4-8778-00C04F8DEC45) ---- additional attributes
  //user defined code to be added here ...

  //#end ACD#

  // ------------------------------------------------------------
  // constructors
  // ------------------------------------------------------------
  public Clockwork() {
    //#ACD# M(UDCC::UID_2CE1B953-369A-11d6-AEA1-0080C88207C4) ---- constructor body
    //user defined code to be added here ...

    //#end ACD#
    door = Door.getInstance();
    cuckoo = new Cuckoo();
    owl = new Owl();
    display = new Display();
    currentState = _NOTMOVING;
  }

  /**
   * This constructor initializes all instance attributes,
   * which are not derived.
   */
  public Clockwork(int hour, int minute, Owl owl, Display display, Door door, Cuckoo cuckoo) {
    //#ACD# M(UDCC::UID_378C31F0-544B-11d4-8778-00C04F8DEC45-full-ctor) ---- constructor body
    //user defined code to be added here ...

    //#end ACD#
    this.hour = hour;
    this.minute = minute;
    this.owl = owl;
    this.display = display;
    this.door = door;
    this.cuckoo = cuckoo;
  }

  //#ACD# M(UDCO::UID_378C31F0-544B-11d4-8778-00C04F8DEC45) ---- additional constructors
  //user defined code to be added here ...

  //#end ACD#

  // ------------------------------------------------------------
  // methods
  // ------------------------------------------------------------
  public void addMinute(int m) {
    //#ACD# M(UDOP::UID_FD39C160-544D-11d4-8778-00C04F8DEC45) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    if (minute+m == 60) {
    	minute = 0;
    	if (hour + 1 == 24)
    		hour = 0;
    	else
    		++hour;
    } else
    	minute += m;
  }

  public void showTime() {
    //#ACD# M(UDOP::UID_FD39C161-544D-11d4-8778-00C04F8DEC45) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    display.showTime(hour, minute);
  }

  public boolean solidHourAtDay() {
    //#ACD# M(UDOP::UID_FD39C162-544D-11d4-8778-00C04F8DEC45) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    if ((hour>6 && hour<=18) && minute==0)
    	return true;
    else
    	return false;
  }

  public boolean solidHourAtNight() {
    //#ACD# M(UDOP::UID_FD39C163-544D-11d4-8778-00C04F8DEC45) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    if ((hour>18 || hour<=6) && minute==0)
    	return true;
    else
    	return false;
  }

  public boolean notSolidHour() {
    //#ACD# M(UDOP::UID_FD39C164-544D-11d4-8778-00C04F8DEC45) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    if (minute != 0)
    	return true;
    else 
    	return false;
  }

  /**
   * send  pendulum.go
   */
  public void start() {
    //#ACD# M(UDOP::UID_378C31F5-544B-11d4-8778-00C04F8DEC45) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
  }

  protected void finalize() {
    //#ACD# M(UDOP::UID_378C31F0-544B-11d4-8778-00C04F8DEC45-finalizer) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    try { super.finalize(); } 
    catch(Throwable t) { return; }
    finally { return; }
  }

  /**
   * Automatically generated <code>set</code>-method 
   * from attribute <code>hour</code>.
   * 
   * @param hour Sets this instance's 
   *    <code>hour</code> to a new value.
   * @see #getHour()
   */
  public void setHour(int hour) {
    //#ACD# M(UDOP::UID_378C31F4-544B-11d4-8778-00C04F8DEC45-setter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    this.hour = hour;
  }

  /**
   * Automatically generated <code>get()</code>-method 
   * from attribute <code>hour</code>.
   * 
   * @return This instance's <code>hour</code>.
   * @see #setHour(int)
   */
  public int getHour() {
    //#ACD# M(UDOP::UID_378C31F4-544B-11d4-8778-00C04F8DEC45-getter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    return hour;
  }

  /**
   * Automatically generated <code>set</code>-method 
   * from attribute <code>minute</code>.
   * 
   * @param minute Sets this instance's 
   *    <code>minute</code> to a new value.
   * @see #getMinute()
   */
  public void setMinute(int minute) {
    //#ACD# M(UDOP::UID_378C31F3-544B-11d4-8778-00C04F8DEC45-setter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    this.minute = minute;
  }

  /**
   * Automatically generated <code>get()</code>-method 
   * from attribute <code>minute</code>.
   * 
   * @return This instance's <code>minute</code>.
   * @see #setMinute(int)
   */
  public int getMinute() {
    //#ACD# M(UDOP::UID_378C31F3-544B-11d4-8778-00C04F8DEC45-getter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    return minute;
  }

  /**
   * Automatically generated <code>set</code>-method 
   * from attribute <code>owl</code>.
   * 
   * @param owl Sets this instance's 
   *    <code>owl</code> to a new value.
   * @see #getOwl()
   */
  public void setOwl(Owl owl) {
    //#ACD# M(UDOP::UID_94B689F6-544C-11d4-8778-00C04F8DEC45-assoc-setter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    this.owl = owl;
  }

  /**
   * Automatically generated <code>get()</code>-method 
   * from attribute <code>owl</code>.
   * 
   * @return This instance's <code>owl</code>.
   * @see #setOwl(Owl)
   */
  public Owl getOwl() {
    //#ACD# M(UDOP::UID_94B689F6-544C-11d4-8778-00C04F8DEC45-assoc-getter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    return owl;
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
    //#ACD# M(UDOP::UID_04328C61-544E-11d4-8778-00C04F8DEC45-assoc-setter) ---- method body
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
    //#ACD# M(UDOP::UID_04328C61-544E-11d4-8778-00C04F8DEC45-assoc-getter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    return display;
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
    //#ACD# M(UDOP::UID_94B689F2-544C-11d4-8778-00C04F8DEC45-assoc-setter) ---- method body
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
    //#ACD# M(UDOP::UID_94B689F2-544C-11d4-8778-00C04F8DEC45-assoc-getter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    return door;
  }

  /**
   * Automatically generated <code>set</code>-method 
   * from attribute <code>cuckoo</code>.
   * 
   * @param cuckoo Sets this instance's 
   *    <code>cuckoo</code> to a new value.
   * @see #getCuckoo()
   */
  public void setCuckoo(Cuckoo cuckoo) {
    //#ACD# M(UDOP::UID_94B689F0-544C-11d4-8778-00C04F8DEC45-assoc-setter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    this.cuckoo = cuckoo;
  }

  /**
   * Automatically generated <code>get()</code>-method 
   * from attribute <code>cuckoo</code>.
   * 
   * @return This instance's <code>cuckoo</code>.
   * @see #setCuckoo(Cuckoo)
   */
  public Cuckoo getCuckoo() {
    //#ACD# M(UDOP::UID_94B689F0-544C-11d4-8778-00C04F8DEC45-assoc-getter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    return cuckoo;
  }

  //#ACD# M(UDOP::UID_378C31F0-544B-11d4-8778-00C04F8DEC45) ---- additional operations
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
    strbuf.append("Clockwork:\n");
    strbuf.append("hour: " + hour).append("\n");
    strbuf.append("minute: " + minute).append("\n");
    if (owl != null)
      strbuf.append("owl: " + owl.toString()).append("\n");
    else
      strbuf.append("owl: null").append("\n"); 
    if (display != null)
      strbuf.append("display: " + display.toString()).append("\n");
    else
      strbuf.append("display: null").append("\n"); 
    if (door != null)
      strbuf.append("door: " + door.toString()).append("\n");
    else
      strbuf.append("door: null").append("\n"); 
    if (cuckoo != null)
      strbuf.append("cuckoo: " + cuckoo.toString()).append("\n");
    else
      strbuf.append("cuckoo: null").append("\n"); 
    //#ACD# M(UID_378C31F0-544B-11d4-8778-00C04F8DEC45-toString) ---- method body
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
    if (! (o instanceof Clockwork))
      return false;
    else {
      Clockwork c = (Clockwork) o;
      if (
          this.hour == c.hour
          && this.minute == c.minute
          && ((owl == null && c.owl == null) || (owl != null && c.owl != null && owl.equals(c.owl)))
          && ((display == null && c.display == null) || (display != null && c.display != null && display.equals(c.display)))
          && ((door == null && c.door == null) || (door != null && c.door != null && door.equals(c.door)))
          && ((cuckoo == null && c.cuckoo == null) || (cuckoo != null && c.cuckoo != null && cuckoo.equals(c.cuckoo)))
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
    Clockwork obj = new Clockwork();
    obj.hour = hour;
    obj.minute = minute;
    obj.owl = owl;
    obj.display = display;
    obj.door = door;
    obj.cuckoo = cuckoo;
    return obj;
  }


  // ------------------------------------------------------------
  // state machine (realized with nested switch statements)
  // ------------------------------------------------------------

  protected byte currentState;
  // events /////////////////////////////////////////////////////////////////
  public static final byte _SETTIME = 0;
  public static final byte _TICK = 1;


  // states /////////////////////////////////////////////////////////////////
  public static final byte _SOLIDHOURATNIGHT = 0;
  public static final byte _NOTSOLIDHOUR = 1;
  public static final byte _SOLIDHOURATDAY = 2;
  public static final byte _NOTMOVING = 3;


  /// Event taker functions /////////////////////////////////////////////////
  public void takeEvent(byte ev) {
    switch(currentState) {
      case _SOLIDHOURATNIGHT:
        switch(ev) {
                  
          case _TICK:
            if (notSolidHour()) {
              showTime();
              addMinute(15);
              currentState = _NOTSOLIDHOUR;
              NotSolidHour_Action();
              break;
            }
            break;
          default:
            System.out.println("Wrong event occurred "  + "Clockwork " + "SolidHourAtNight" + " " + ev);
            break;
        } // end event switch for state SolidHourAtNight
        break;
      case _NOTSOLIDHOUR:
        switch(ev) {
                  
          case _TICK:
            if (solidHourAtDay()) {
              showTime();
              addMinute(15);
              door.open();
              cuckoo.cry(hour);
              door.close();
              currentState = _SOLIDHOURATDAY;
              SolidHourAtDay_Action();
              break;
            }
            if (solidHourAtNight()) {
              showTime();
              addMinute(15);
              owl.cry(hour);
              door.close();
              currentState = _SOLIDHOURATNIGHT;
              SolidHourAtNight_Action();
              break;
            }
            if (notSolidHour()) {
              showTime();
              addMinute(15);
              currentState = _NOTSOLIDHOUR;
              NotSolidHour_Action();
              break;
            }
            break;
          default:
            System.out.println("Wrong event occurred "  + "Clockwork " + "NotSolidHour" + " " + ev);
            break;
        } // end event switch for state NotSolidHour
        break;
      case _SOLIDHOURATDAY:
        switch(ev) {
                  
          case _TICK:
            if (notSolidHour()) {
              showTime();
              addMinute(15);
              currentState = _NOTSOLIDHOUR;
              NotSolidHour_Action();
              break;
            }
            break;
          default:
            System.out.println("Wrong event occurred "  + "Clockwork " + "SolidHourAtDay" + " " + ev);
            break;
        } // end event switch for state SolidHourAtDay
        break;
      case _NOTMOVING:
        switch(ev) {
                  
          case _SETTIME:
            setHour(13);
            setMinute(15);
            currentState = _NOTSOLIDHOUR;
            NotSolidHour_Action();
            break;
          default:
            System.out.println("Wrong event occurred "  + "Clockwork " + "NotMoving" + " " + ev);
            break;
        } // end event switch for state NotMoving
        break;
    } // end state switch
  }

  /// State action functions ////////////////////////////////////////////////
  protected void SolidHourAtNight_Action() {
    //#ACD# M(UDAC::UID_98A70130-AC4F-11d4-BEDC-0060971B5EE0_Action) action code
    //user defined code to be added here ...

    //#end ACD#
  }

  protected void NotSolidHour_Action() {
    //#ACD# M(UDAC::UID_98A7012D-AC4F-11d4-BEDC-0060971B5EE0_Action) action code
    //user defined code to be added here ...

    //#end ACD#
  }

  protected void SolidHourAtDay_Action() {
    //#ACD# M(UDAC::UID_98A7012A-AC4F-11d4-BEDC-0060971B5EE0_Action) action code
    //user defined code to be added here ...

    //#end ACD#
  }

}
