import java.awt.*;
import java.awt.event.*;

/*
 * Note - Copyright 1997 John Jensen.  All Rights Reserved.  See license.txt
 */

// This extension of Dialog implements modal dialogs with a callback interface
// to work around the "modal dialog problem" on Windows.  The "NoteListener"
// interface is used to return an action code to the parent when "continue" is
// pressed.

class Note
extends Dialog
implements WindowListener, ActionListener

{
	int noteAction;
	NoteListener noteListener;
	Button accepted;
	Button whatever;

	// Constructor takes a string and an action.  Note that action == 0 implies
	// that there is only an "ok" button, while action != 0 implies a "Continue" and
	// a "Cancel" button.

	public Note(Frame parent, String s, int action)
	{
		super(parent, "Note", true);

		noteListener = (NoteListener)parent;
		noteAction = action;

		Panel center = new Panel();
		center.add(new Label(s));
		add("Center", center);

		Panel south = new Panel();

		if (action != 0)
		{
			accepted = new Button("Continue");
			accepted.addActionListener(this);
			south.add(accepted);
			whatever = new Button("Cancel");
			whatever.addActionListener(this);
			south.add(whatever);
		}
		else
		{
			whatever = new Button("Ok");
			whatever.addActionListener(this);
			south.add(whatever);
		}

		add("South", south);

		setSize(300, 100);

		addWindowListener(this);
	}

	// if we get a "Continue" button, call the parent's noteListener

    public void actionPerformed(ActionEvent event)
	{
		Object source;

		source = event.getSource();

        if (source == accepted)
			noteListener.noteListen(noteAction);

        if ((source == accepted) || (source == whatever))
			dispose();
	}

	// add the 1.1 WindowListener stuff

    public void windowDeiconified(WindowEvent event) {
    }

    public void windowIconified(WindowEvent event) {
    }

    public void windowActivated(WindowEvent event) {
    }

    public void windowDeactivated(WindowEvent event) {
    }

    public void windowOpened(WindowEvent event) {
    }

    public void windowClosing(WindowEvent event) {
			dispose();
    }

    public void windowClosed(WindowEvent event) {
    }
}
