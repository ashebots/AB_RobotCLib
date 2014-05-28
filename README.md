abLib-RobotC
============

Ashebots RobotC opensource software library, targeted at the NXT platform for use in the FIRST Tech Challenge (FTC)
(coming soon)

===
Standards
===

Local variables and functions which aren't exposed to other parts of the program follow the following naming conventions:

variablesLikeThis
FunctionsLikeThis
TypedefStructThingsLikeThis


Global variables and functions which ARE exposed to the rest of the program MUST be prefixed with "AB_", to avoid naming conflicts:

AB_variablesLikeThis
AB_FunctionsLikeThis
AB_TypedefStructThingsLikeThis
