import sys

from sniffaccess import IAC
from sniffaccess import Sniffaccess

def BrowseClass(name, port):	
	sa = Sniffaccess.Sniffaccess(port)
	sa.SA_BringIntoForeground(1)
	snurls = sa.SA_GetClassFromName(name)
	if len(snurls) > 0:
		sa.SA_ShowInClassBrowser(snurls[0])
		
def EditSymbol(name, port):	
	sa = Sniffaccess.Sniffaccess(port)
	sa.SA_BringIntoForeground(1)
	snurls = sa.SA_GetSymbolFromName(name)
	if len(snurls) > 0:
		sa.SA_NavigateToSymbol(snurls[0])

def BrowseClassInEntireHierarchy(name, port):	
	sa = Sniffaccess.Sniffaccess(port)
	sa.SA_BringIntoForeground(1)
	snurls = sa.SA_GetClassFromName(name)
	if len(snurls) > 0:
		sa.SA_ShowInHierarchyBrowser(snurls[0])

def BrowseClassInRelativeHierarchy(name, port):	
	sa = Sniffaccess.Sniffaccess(port)
	sa.SA_BringIntoForeground(1)
	snurls = sa.SA_GetClassFromName(name)
	if len(snurls) > 0:
		sa.SA_ShowClassRelatives(snurls[0])
		
if __name__ == '__main__':
	try:
		if sys.argv[2] == "browse_class":
			className = sys.argv[3]
			port = int(sys.argv[1])
			BrowseClass(className, port)
		elif sys.argv[2] == "edit_symbol":
			symbolName = sys.argv[3]
			port = int(sys.argv[1])
			EditSymbol(symbolName, port)
		elif sys.argv[2] == "hierarchy":
			className = sys.argv[3]
			port = int(sys.argv[1])
			BrowseClassInEntireHierarchy(className, port)
		elif sys.argv[2] == "relative_hierarchy":
			className = sys.argv[3]
			port = int(sys.argv[1])
			BrowseClassInRelativeHierarchy(className, port)
	except IndexError:
		print "*** Error: parameter missing!"
		sys.exit()