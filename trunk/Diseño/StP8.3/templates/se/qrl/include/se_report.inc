#include "qrl/include/se_report_formats.inc"

// se_report.inc
//
//      Contains utilities for printing diagrams to fit nicely
//      on a page.
//


struct extremes
{
        int minx;
        int maxx;
        int miny;
        int maxy;
};


//
// oms_file_print()
//
// graphically print file depending on file type (diagram or table)
// with timestamp of report execution
//

int
oms_file_print(file one_file, string time)
{
    int         errors = 0;
    list        nr_by_xcoord = list_create("node_ref", 0);
    list        nr_by_ycoord = list_create("node_ref", 0);
    string      caption, query;
 
    caption = one_file.type + ": " + one_file.name + " (" + time + ")" ;
 
    // find all noderefs sorted by the xcoord
    query = "node_ref[file_id == ${one_file.id}] sort by xcoord";
    nr_by_xcoord = list_select(query);
 
    // find all noderefs sorted by the ycoord
    query = "node_ref[file_id == ${one_file.id}] sort by ycoord";
    nr_by_ycoord = list_select(query);

    if (!is_landscape(nr_by_xcoord, nr_by_ycoord))
    {
        diagram_orientation_scale_set(PortraitFit);
        diagram_print_area_set(nr_by_xcoord, nr_by_ycoord, PortraitFit);
    }
    else
    {
        diagram_orientation_scale_reset();
        diagram_frame_height_reset();
        diagram_frame_width_reset();
    }
 
    // print gsdfe or dydfe, depending upon setting
    if (one_file.type == "DfeDiagram")
    {
        if (output_symbols == "dydfe")
            file_print("DYPrint", one_file, caption);
	else
            file_print("GSPrint", one_file, caption);
    }
    else
	file_print(one_file, caption);
 
    return errors;
}




boolean
is_landscape(list xcoords, list ycoords)
{
    extremes xy;

    xy = find_extremes(xcoords, ycoords);

// This is basically a difference. However we add 50 to the Y coordinates
// to try and give the diagram some balance i.e. a diagram may be only
// fractionally wider than it is tall. We'll also try not to flip diagrams
// if they fit quite easily across the page. Hence, (xy.maxx - xy.minx) > 1100.

    if ((xy.maxx - xy.minx) > 1100 )
    {
        if ((xy.maxx - xy.minx) > ((xy.maxy - xy.miny)+50))
            return(True);
        else
            return(False);
    }

    return(False);

}

void
diagram_print_area_set(list xcoords, list ycoords, orientation_scale_enum orient)
{
    extremes xy;
    float x, y;
 
    xy = find_extremes(xcoords, ycoords);
 
    x = xy.maxx - xy.minx;
    y = xy.maxy - xy.miny;
 
    x = (x/80) - 5;
    y = ((y/80) - 1)/2;
 
    if ((x < 4) && (y < 2))
    {
        diagram_frame_height_set(3.5);
        diagram_frame_width_set(6.0);
    }
    else if ((x < 6) && (y < 4))
    {
        diagram_frame_height_set(3.5);
        diagram_frame_width_set(6.0);
    }
    else
    {
    if (orient == PortraitFit)
        {
            diagram_frame_height_set(8.0);
            diagram_frame_width_set(6.0);
        }
        else
        {
            diagram_frame_width_set(8.0);
            diagram_frame_height_set(6.0);
        }
    }
}

extremes
find_extremes(list xcoords, list ycoords)
{
    extremes	xy;
    node_ref	nr;
    int		count = list_count(xcoords) - 1;

    nr = list_get(xcoords, count);
    xy.maxx = nr.xcoord;

    nr = list_get(xcoords, 0);
    xy.minx = nr.xcoord;

    nr = list_get(ycoords, count);
    xy.maxy = nr.ycoord;

    nr = list_get(ycoords, 0);
    xy.miny = nr.ycoord;

    return xy;
}


