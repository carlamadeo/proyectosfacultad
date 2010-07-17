

string 
get_validator_base_product()
{
    if (is_product_omt())
        return "OMT";
    //else if (is_product_omt())
    //    return "OMT";
    else
        return "UML";
}

string VALIDATOR_BASE_PRODUCT = get_validator_base_product();

