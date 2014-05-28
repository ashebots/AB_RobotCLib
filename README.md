# AB_RobotCLib

Ashebots RobotC opensource software library, targeted at the NXT platform for use in the FIRST Tech Challenge (FTC)


#### WARNING
- We're still early in development
- Things may change or break without warning
- Features may be broken or incomplete


## Standards


### Naming Convention

Local variables and functions which aren't exposed to other parts of the program follow the following naming conventions:
````c
variablesLikeThis
FunctionsLikeThis
TypedefStructThingsLikeThis
````

Global variables and functions which ARE exposed to the rest of the program MUST be prefixed with "AB_", to avoid naming conflicts:
````c
AB_variablesLikeThis
AB_FunctionsLikeThis
AB_TypedefStructThingsLikeThis
````
