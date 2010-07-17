# sccsid = "%W% %Y% %D%; Copyright (C) IDE 1984-1988"
SHELL=/bin/sh
BINDIR=../../../bin/sun4
LIBDIR=../../../lib/sun4
MSGFILE=$(LIBDIR)/msg_file

MSGTOF=$(BINDIR)/msg_to_file
TEXTFILES= *.txt

all:	$(MSGFILE)

$(MSGFILE):	$(TEXTFILES)
	$(MSGTOF) -v -u -o $(MSGFILE) -i $?

COMLIB = -lpixrect $(LIBDIR)/libtools.a
SUNLIB = -lsuntool -lsunwindow
CFLAGS= -g -DSUN_REL_2=1 -DSUN_REL_3=1  -g -DBSD -DBSD42 -DWSIZE4   -DASY_CONF -DNOIDETOOL
