/***************************************************
 * Weight.java
 * -----------
 *
 * Author:  YOUR_NAME
 * Company: YOUR_COMPANY
 * Date:    04/08/02
 *
 * Copyright (c), 2001, YOUR_COMPANY
 **************************************************/    
package clockwork;

//#ACD# M(UDIS::UID_378C31FE-544B-11d4-8778-00C04F8DEC45) ---- additional import statements
//user defined code to be added here ...

//#end ACD#

/**
 * Because Weight is the represantation of the physical weight
 * that delivers the energy for the clock to run, it should
 * contain the main method.
 * 
 * Inside the main method the class Weight will be instantiated
 * and as second step a run method will be invoked .
 * The behavioural aspect of Weight is modelled in a state diagram, so 
 * the label of the transition between the creation state and the first state
 * will although be automatically generated as part of the run method.
 */
public class Weight {

  // ------------------------------------------------------------
  // instance attributes
  // ------------------------------------------------------------
  private int energy;
  protected Pendulum pend;

  //#ACD# M(UDAT::UID_378C31FE-544B-11d4-8778-00C04F8DEC45) ---- additional attributes
  //user defined code to be added here ...

  //#end ACD#

  // ------------------------------------------------------------
  // constructors
  // ------------------------------------------------------------
  public Weight() {
    //#ACD# M(UDCC::UID_0228E2B3-3697-11d6-AEA1-0080C88207C4) ---- constructor body
    //user defined code to be added here ...

    //#end ACD#
    pend = new Pendulum();
    currentState = _HASNOENERGY;
  }

  /**
   * This constructor initializes all instance attributes,
   * which are not derived.
   */
  public Weight(int energy, Pendulum pend) {
    //#ACD# M(UDCC::UID_378C31FE-544B-11d4-8778-00C04F8DEC45-full-ctor) ---- constructor body
    //user defined code to be added here ...

    //#end ACD#
    this.energy = energy;
    this.pend = pend;
  }

  //#ACD# M(UDCO::UID_378C31FE-544B-11d4-8778-00C04F8DEC45) ---- additional constructors
  //user defined code to be added here ...

  //#end ACD#

  // ------------------------------------------------------------
  // methods
  // ------------------------------------------------------------
  public void energize(int e) {
    //#ACD# M(UDOP::UID_378C31FF-544B-11d4-8778-00C04F8DEC45) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    setEnergy(e);
  }

  public int decrease(int e) {
    //#ACD# M(UDOP::UID_378C3200-544B-11d4-8778-00C04F8DEC45) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    setEnergy(getEnergy()-e);
    return 0;
  }

  public static void main(String[] args) {
    //#ACD# M(UDOP::UID_6F3DBA97-060A-11d6-AE72-0080C88207C4) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    Weight weight = new Weight();
    weight.run();
  }

  public boolean hasEnergy(int e) {
    //#ACD# M(UDOP::UID_58BB63F5-36A9-11d6-AEA1-0080C88207C4) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    if (energy-e >= 0)
      return true;
    else
      return false;
  }

  protected void run() {
    //#ACD# M(UDOP::UID_6F3DBABC-060A-11d6-AE72-0080C88207C4) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    takeEvent(_RAISE);
    while(hasEnergy(10)) {
      takeEvent(_SINK);
    }
  }

  protected void finalize() {
    //#ACD# M(UDOP::UID_378C31FE-544B-11d4-8778-00C04F8DEC45-finalizer) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    try { super.finalize(); } 
    catch(Throwable t) { return; }
    finally { return; }
  }

  /**
   * Automatically generated <code>set</code>-method 
   * from attribute <code>energy</code>.
   * 
   * @param energy Sets this instance's 
   *    <code>energy</code> to a new value.
   * @see #getEnergy()
   */
  public void setEnergy(int energy) {
    //#ACD# M(UDOP::UID_C6C6DECF-36A8-11d6-AEA1-0080C88207C4-setter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    this.energy = energy;
  }

  /**
   * Automatically generated <code>get()</code>-method 
   * from attribute <code>energy</code>.
   * 
   * @return This instance's <code>energy</code>.
   * @see #setEnergy(int)
   */
  public int getEnergy() {
    //#ACD# M(UDOP::UID_C6C6DECF-36A8-11d6-AEA1-0080C88207C4-getter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    return energy;
  }

  /**
   * Automatically generated <code>set</code>-method 
   * from attribute <code>pend</code>.
   * 
   * @param pend Sets this instance's 
   *    <code>pend</code> to a new value.
   * @see #getPend()
   */
  public void setPend(Pendulum pend) {
    //#ACD# M(UDOP::UID_94B689FA-544C-11d4-8778-00C04F8DEC45-assoc-setter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    this.pend = pend;
  }

  /**
   * Automatically generated <code>get()</code>-method 
   * from attribute <code>pend</code>.
   * 
   * @return This instance's <code>pend</code>.
   * @see #setPend(Pendulum)
   */
  public Pendulum getPend() {
    //#ACD# M(UDOP::UID_94B689FA-544C-11d4-8778-00C04F8DEC45-assoc-getter) ---- method body
    //user defined code to be added here ...
      
    //#end ACD#
    return pend;
  }

  //#ACD# M(UDOP::UID_378C31FE-544B-11d4-8778-00C04F8DEC45) ---- additional operations
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
    strbuf.append("Weight:\n");
    strbuf.append("energy: " + energy).append("\n");
    if (pend != null)
      strbuf.append("pend: " + pend.toString()).append("\n");
    else
      strbuf.append("pend: null").append("\n"); 
    //#ACD# M(UID_378C31FE-544B-11d4-8778-00C04F8DEC45-toString) ---- method body
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
    if (! (o instanceof Weight))
      return false;
    else {
      Weight c = (Weight) o;
      if (
          this.energy == c.energy
          && ((pend == null && c.pend == null) || (pend != null && c.pend != null && pend.equals(c.pend)))
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
    Weight obj = new Weight();
    obj.energy = energy;
    obj.pend = pend;
    return obj;
  }


  // ------------------------------------------------------------
  // state machine (realized with nested switch statements)
  // ------------------------------------------------------------

  protected byte currentState;
  // events /////////////////////////////////////////////////////////////////
  public static final byte _RAISE = 0;
  public static final byte _SINK = 1;


  // states /////////////////////////////////////////////////////////////////
  public static final byte _HASENERGY = 0;
  public static final byte _HASNOENERGY = 1;


  /// Event taker functions /////////////////////////////////////////////////
  public void takeEvent(byte ev) {
    switch(currentState) {
      case _HASENERGY:
        switch(ev) {
                  
          case _SINK:
            int e = 10;
            if (hasEnergy(e)) {
              decrease(e);
              pend.takeEvent(pend._MOVE);
              currentState = _HASENERGY;
              hasEnergy_Action();
              break;
            }
            break;
          default:
            System.out.println("Wrong event occurred "  + "Weight " + "hasEnergy" + " " + ev);
            break;
        } // end event switch for state hasEnergy
        break;
      case _HASNOENERGY:
        switch(ev) {
                  
          case _RAISE:
            int e = 1000;
            energize(e);
            currentState = _HASENERGY;
            hasEnergy_Action();
            break;
          default:
            System.out.println("Wrong event occurred "  + "Weight " + "hasNoEnergy" + " " + ev);
            break;
        } // end event switch for state hasNoEnergy
        break;
    } // end state switch
  }

  /// State action functions ////////////////////////////////////////////////
  protected void hasEnergy_Action() {
    //#ACD# M(UDAC::UID_98A70172-AC4F-11d4-BEDC-0060971B5EE0_Action) action code
    //user defined code to be added here ...

    //#end ACD#
  }

}
