@echo off

rem This batch file creates a table "customer" in WebLogic's Cloudscape database 
rem (needed to deploy the com package in the ejb_model example system into WLS)
rem The provided DDL script customer.sql only provides for a 1:1 direct mapping
rem for CustomerBean which should be a very basic example only.

rem Change the paths in <> below according to your installation

rem Set this to the WebLogic installation directory
set WLS_HOME=<weblogic-path>

rem Set this to the JDK installation directory
set JAVA_HOME=<jdk-path>

rem Set this to the location of your SQL/DDL file
set DDL_FILE=<path>\customer.sql

rem Set this to the connection pool defined in weblogic.properties
rem (weblogic.jdbc.connectionPool etc.)
set WLS_POOL=WLS_POOL

rem Set this to the cloudscape data storage directory
rem as found in weblogic.properties
rem (value of java.system.property.cloudscape.system.home)
set CSC_DATA=%WLS_HOME%/eval/cloudscape/data

set WLS_CP=%WLS_HOME%/classes;%WLS_HOME%/lib/weblogicaux.jar;%WLS_HOME%/eval/cloudscape/lib/cloudscape.jar
%JAVA_HOME%\bin\java -classpath %WLS_CP% -Dcloudscape.system.home=%CSC_DATA% utils.Schema jdbc:cloudscape:%WLS_POOL%;create=true COM.cloudscape.core.JDBCDriver -u null -p null -verbose %DDL_FILE%

