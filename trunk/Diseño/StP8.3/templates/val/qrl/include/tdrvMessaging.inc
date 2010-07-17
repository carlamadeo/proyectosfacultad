void initValidatorOperationMessage(string sOperationName, string sUsecases)
{
  message("____AONIX_StP/Validator________________________________________________________");
  message(" "); 
  message("Executed operation: "+ sOperationName); 
  message("for usecases: " + UseCases); 
  message("system: " + current_system());
  message("project:" + current_projdir());
  message("_______________________________________________________________________________");
  message(" ");

}

void finalizeValidatorOperationMessage(string sOperationName)
{
    
  message("_______________________________________________________________________________");
  message(" ");
  message( sOperationName + " complete.");

}
void initValidatorMessaging(string sReportName)
{

  message("____AONIX_StP/Validator________________________________________________________");
  message(" ");
  message("Generating "+ sReportName);
  message("Author:   " + e_DOCAUTHOR);
  message("Company:  " + e_COMPANY);
  message("Division: " + e_DIVISION);
  message(" ");
  message("Report type: " + to_upper(e_REPORT_TYPE));
  message("Format file: " + getFormatFileName());
  message(" ");
  message("_______________________________________________________________________________");
  message(" ");

}

void finalizeValidatorMessaging(string sMessage)
{

  message("_______________________________________________________________________________");
  message(" ");
  if (sMessage !=NULL && sMessage != "")
      message(sMessage);
  message("Reports generation complete.");

}


void validatorUserMessage(string sMessage)
{
    // message(" ");
    message(sMessage);
}


void validatorWarningMessage(string sMessage)
{
    if(sMessage != NULL)
        message("WARNING: " + sMessage);
}

void validatorErrorMessage(string sMessage)
{
    if(sMessage != NULL)
        message("@" + sMessage);
}
