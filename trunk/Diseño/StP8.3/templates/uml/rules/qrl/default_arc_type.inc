//ECR 6469
string
ShowDefaultArcTypeToolbar_label_func()
{
    string label;
    if (ui_is_toolbar_visible("DefaultArcType"))
        label = "Hide";
    else
        label = "Show";

    label += " DefaultArcType Toolbar";
    return label;
}
//ECR 6469