
//
//  Returns proper duplicate store display mark for the current scaling
//
string
DupStore()
{
    int level = gde_zoom_level();

    if (level == 0)
    	return "dupgstore64";
    if (level == 1)
    	return "dupgstore48";
    if (level == 2)
    	return "dupgstore32";
}

//
//  Returns proper duplicate external display mark for the current scaling
//
string
DupExternal()
{
    int level = gde_zoom_level();

    if (level == 0)
    	return "dupext64";
    if (level == 1)
    	return "dupext48";
    if (level == 2)
    	return "dupext32";
}


