/* simple algorithm to calculate the square root of a number.
 * returning the real square root or 1 above it is OK for the user of this
 * function
 */
int 
int_sqrt (int num)
{
    int loop;
    for( loop = 1; loop < 100; loop++ ) {
		if ( loop * loop >= num )
			break;
	}
	return loop;
}

struct node_layout_info {
	gde_node sym;
	int x;
	int y;
	int w;
	int h;
} ;


const int LAYOUT_VERT_START_OFFSET = 15;
const int LAYOUT_HORIZ_START_OFFSET = 20;
const int LAYOUT_HORIZ_END_OFFSET = 35;
const int LAYOUT_HORIZ_SPACING = 20;
const int LAYOUT_VERT_SPACING = 20;
const int LAYOUT_VERT_END_OFFSET = 35;
// Input:
//	contents - a list of gde_nodes
void
uml_node_layout(gde_node container, list contents)
{
	int i;
	int avgWidth, avgHeight;
	int maxWidth, maxHeight;
	node_layout_info inf;
    int lblHeight;
	list all_inf = list_create("node_layout_info",0);

// first, find out how many things we have and their average size
	int nItems = list_count(contents);

	if (0 == nItems) {
		return; // nothing to do
	}

    lblHeight = gde_node_height(container) -
        gde_node_container_height(container);

	avgWidth = 0;
	avgHeight = 0;
	maxWidth = 0;
	maxHeight = 0;
	for(i=0;i<list_count(contents); i++) {
		inf.sym = list_get(contents,i);
		inf.w = gde_node_width(inf.sym);
		inf.h = gde_node_height(inf.sym);
		if (inf.w > maxWidth) {
			maxWidth = inf.w;
		}
		if (inf.h > maxHeight) {
			maxHeight = inf.h;
		}
		avgWidth += inf.w;
		avgHeight += inf.h;
		list_append(all_inf,inf);
	}
	avgWidth /= nItems;
	avgHeight /= nItems;


// then, find out what the container width and height should be by the
// following formula:
	int cols = int_sqrt(nItems);
	int rows = cols;

// if the calculated width or height of the container is not large enough to
// hold its largest item, then make it at least that big
	int shapeWidth = (cols * avgWidth) + ((cols-1) * LAYOUT_HORIZ_SPACING);

	if (maxWidth > shapeWidth) {
		shapeWidth = maxWidth;
	}
	shapeWidth += LAYOUT_HORIZ_START_OFFSET + LAYOUT_HORIZ_END_OFFSET;

	int shapeHeight = (rows * avgHeight) + ((rows-1) * LAYOUT_VERT_SPACING);

	if (maxHeight > shapeHeight) {
		shapeHeight = maxHeight;
	}
	shapeHeight += LAYOUT_VERT_START_OFFSET + LAYOUT_VERT_END_OFFSET +
        lblHeight;

	// JED
	// print_message("nItems: " + nItems);
	// print_message("rows: " + rows);
	// print_message("cols: " + cols);
	// print_message("shapeWidth: " + shapeWidth);
	// print_message("shapeHeight: " + shapeHeight);


// resize the container to accomodate the layout in a rows x cols grid
	gde_select_node(container);
	gde_node_scale(container,0,shapeWidth, shapeHeight);


// loop through all the contents and set their desired positions
// NOTE: Calculate in upper-left coordinates, store center coordinates
	int contX = gde_node_container_x(container)  -
        gde_node_container_width(container)/2;
	int contY = gde_node_container_y(container)  -
        gde_node_container_height(container)/2;
	int curRow, curCol;
	int curXPos;
	int curYPos = contY + LAYOUT_VERT_START_OFFSET;
	// reuse these to track the largest thing in a row
	maxWidth = 0;
	maxHeight = 0;
	i = 0;

    // JED - to store the difference between the real X and the one reported
    // by gde_node_x
    int xOffset;
	// print_message("Cur y (start) " + curYPos);
	for (curRow = 0; curRow < rows && i<list_count(all_inf); curRow++) {
		curXPos = contX + LAYOUT_HORIZ_START_OFFSET;
		// update the curYPos with the height of the largest thing in the
		// previous row
		curYPos += maxHeight + LAYOUT_VERT_SPACING;
		maxHeight = 0;
		for (curCol = 0; curCol < cols && i<list_count(all_inf); curCol++) {
			inf = list_get(all_inf,i);
            // JED - Calculate the difference for this node
            xOffset = gde_node_x(inf.sym) - gde_node_container_x(inf.sym);
			// the desired position is curXPos, curYPos
			inf.x = curXPos + inf.w/2 - xOffset;
			inf.y = curYPos + inf.h/2;
			// print_message("JED: x,y,w,h: (" + inf.x + ", " + inf.y + ", " + inf.w + ", " + inf.h + ")");
			curXPos += inf.w + LAYOUT_HORIZ_SPACING;
			list_set(all_inf,i,inf);
			i++;
			if (inf.h > maxHeight) {
				maxHeight = inf.h;
			}
		}
	}
	// print_message("Cur x (end) " + curXPos);
	// print_message("Cur y (end) " + curYPos);


// move them into place
	int maxX = -65535;
	int maxY = -65535;
	boolean doResize = False;
	for(i=0;i<list_count(all_inf);i++) {
		inf = list_get(all_inf,i);
		gde_select_node(inf.sym);
		gde_builtin("SymbolMove " + inf.x + " " + inf.y);
		if (inf.x + inf.w/2 > maxX) {
			maxX = inf.x + inf.w/2;
		}
		if (inf.y + inf.h/2 > maxY) {
			maxY = inf.y + inf.h/2;
		}
	}
	maxX += LAYOUT_HORIZ_END_OFFSET;
	maxY += LAYOUT_VERT_END_OFFSET;
// JED: If out fiddling above DIDN'T give us a large enough
// container, resize it as necessary. So I'm not a GUI programmer......
	if (maxX - contX > shapeWidth) {
		shapeWidth = maxX - contX;
		doResize = True;
	}
	if (maxY - contY > shapeHeight) {
		shapeHeight = maxY - contY;
		doResize = True;
	}
	if (doResize) {
		// print_message("Resizing (bummer)");
		// print_message("shapeWidth: " + shapeWidth);
		// print_message("shapeHeight: " + shapeHeight);
		gde_select_node(container);
		gde_node_scale(container,0,shapeWidth, shapeHeight);
	}


}
