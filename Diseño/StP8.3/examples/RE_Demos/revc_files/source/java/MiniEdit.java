import java.awt.*;
import java.io.*;
import java.util.*;
import java.awt.event.*;

/*
 * MiniEdit - Copyright 1997 John Jensen.  All Rights Reserved.  See license.txt
 *
 * Version: 3/9/97 (Modified for JDK 1.1)
 */

// ----------
// Notes:
//
// MiniEdit is an application.  You have to run it from a command line by typing
// "java MiniEdit" or "jview MiniEdit" or by assoicating the MiniEdit.class icon
// to the java interpreter by a method appropriate to your operating system.
// 
// This application uses some basic Java classes (TextArea, FileDialog,
// FileInputStream, FileOutputStream, etc.) to implement a simple NotePad
// or TeachText style text editor.
// 
// I didn't put in a limit on file size, but things definitely start to lag when
// editing files greater than 100k or so.
// 
// ----------

public class MiniEdit
extends Frame
implements NoteListener, WindowListener, ActionListener, KeyListener
{
	// The NoteListen member fuction of NoteListener is overridded to support
	// the following actions.

	final private int NOACTION	= 0;
	final private int NEWFILE	= 1;
	final private int OPENFILE	= 2;
	final private int QUITAPP	= 3;

	// size of internal input and output buffer

	final private int BUFSIZE	= 5000;

	// allocate standard components of MiniEdit as part of initialization:

	private TextArea textArea	= new TextArea();

	private String newString	= new String("New");
	private String openString	= new String("Open");
	private String saveString	= new String("Save");
	private String saveAsString = new String("Save As");
	private String quitString	= new String("Quit");

	private File file			= new File("Untitled");

	private String openPrompt	= new String("Open text file");
	private String closePrompt	= new String("Save file as");

	// initialize some state flags

	private boolean neverNamed	= true;
	private boolean dirty		= false;
	private boolean menuBusy	= false;

	// constructor puts together UI

	public MiniEdit()
	{
		this.setTitle();

		MenuBar mbar = new MenuBar();
		Menu m = new Menu("File");
		m.add(new MenuItem(newString));
		m.add(new MenuItem(openString));
		m.add(new MenuItem(saveString));
		m.add(new MenuItem(saveAsString));
		m.addSeparator();
		m.add(new MenuItem(quitString));    
		m.addActionListener(this);        
		mbar.add(m);
		setMenuBar(mbar);

		textArea.addKeyListener(this);
		add("Center", textArea);

		addWindowListener(this);
	}

	// add current filename to window title

	private void setTitle()
	{
		super.setTitle("MiniEdit - " + file.getName());
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

    public void windowClosed(WindowEvent event) {
    }

    public void windowClosing(WindowEvent event) {
		if (isItSafe(QUITAPP))
			System.exit(0);
    }

	// action handler, part of std java event handling

    public void actionPerformed(ActionEvent evt)
	{
		String cmd = evt.getActionCommand();

			if (cmd.equals(newString))
			{
				if (isItSafe(NEWFILE))
					newFile();
			}
			else
			if (cmd.equals(openString))
			{
				if (isItSafe(OPENFILE))
					openFile();
			}
			else
			if (cmd.equals(saveString))
				saveFile(false);
			else
			if (cmd.equals(saveAsString))
				saveFile(true);
			else
			if (cmd.equals(quitString))
			{
				if (isItSafe(QUITAPP))
					System.exit(0);
			}
	}

	// if the file has been modified (dirty), fire off a notify dialog
	// and pass it the "action" you want returned to noteListen.  This
	// dataflow beats the "modal dialog problem" on Windows

	// I believe isItSafe is what Laurence Olivier said repeatedly to
	// Dustan Hoffman in Marathon Man.

	private boolean isItSafe(int action)
	{
		if (dirty)
		{
			notify( "The current file has not been saved. ", action );
			return false;
		}
		else
			return true;
	}

	// implementation of the NoteListener interface - it does whatever
	// action the user agreed to by pressing "Continue" on a modal dialog

	public void noteListen(int action)
	{
		switch (action) 
		{
		case NOACTION:
			break;
		case NEWFILE:
			newFile();
			break;
		case OPENFILE:
			openFile();
			break;
		case QUITAPP:
			System.exit(0);
			break;
		}
	}

	// pick up keytyped events soley to mark the document as dirty.

	public void keyTyped(KeyEvent e)
	{
		dirty = true;
	}

	public void keyPressed(KeyEvent e)
	{
	}

	public void keyReleased(KeyEvent e)
	{
	}

	// clear out the document

	private void newFile()
	{
		neverNamed = true;
		dirty = false;
		file = new File("Untitled");
		this.setTitle();
		textArea.setText("");
		repaint();
	}

	// call getFile to get a file path, and if you get one open it

	private void openFile()
	{
		String part;
		StringBuffer all = new StringBuffer("");

		if (getFile(FileDialog.LOAD))
		{
			try
			{
				FileReader fr = new FileReader(file);
				BufferedReader br=new BufferedReader(fr);

				while ( (part=br.readLine()) != null)
				{
					all.append(part);
					all.append('\n');
				}

				br.close();
				fr.close();
				neverNamed = false;
				dirty = false;
				this.setTitle();
				textArea.setText(all.toString());
			}

			catch (IOException e)
			{
				notify("Error - could not read file", NOACTION);
			}
		}

		repaint();
	}

	// call getFile to get a file path, and if you get one save to it

	private void saveFile(boolean As)
	{
		boolean ok;

		if (neverNamed || As)
		{
			ok = getFile(FileDialog.SAVE);
			neverNamed = false;
		}
		else
			ok = true;

		if (ok)
		{
			String all = textArea.getText();
			try
			{
				FileWriter fw = new FileWriter(file);
				BufferedWriter bw=new BufferedWriter(fw);
				bw.write(all,0,all.length());
				bw.close();
				fw.close();
				neverNamed = false;
				dirty = false;
				this.setTitle();
			}

			catch (IOException e)
			{
				notify("Error - could not write file", NOACTION);
			}
		}
		repaint();
	}

	// use FileDialog to get a filepath

	private boolean getFile(int mode)
	{
		String prompt;
		String filename;
		String pathname;

		if (mode == FileDialog.LOAD)
			prompt = openPrompt;
		else
			prompt = closePrompt;

		FileDialog d = new FileDialog(this, prompt, mode);

		if (mode == FileDialog.LOAD)
			d.setFile("*");
		else
			d.setFile(file.getName());

		d.setDirectory(".");
		d.setVisible(true);

		filename = d.getFile(); 

		pathname = d.getDirectory() + filename; 

		if (filename != null)
		{
			file = new File(pathname);
			return(true);
		}
		else
			return(false);
	}

	// launch a note

	private void notify(String s, int action)
	{
		Note note = new Note(this,s,action);
		note.setVisible(true);
	}

	// main declares the MiniEdit frame which does everything

	public static void main(String[] args)
	{
		Frame f = new MiniEdit();
		f.setSize(500, 400);
		f.setVisible(true);  
	}
}
