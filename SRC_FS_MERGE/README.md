Includes:
>C++


NOTE: Temporarily does not compile, my laptop had a hiccup and is doing something funky with my linker. I've just left a placeholder in the makefile, the source is clean and compilable but
the dependencies don't link.


A synchronous networked filesystem which slowly moves all changes to the connected machines as requested. Files not yet there will be accessed via network when not already synched.

Revisioning will be the primary aim, secondary which also will be the first draft of the code will simply allow for selective syncronization.

NOTE: Return types need to be set to auto in the future. C++14 must be setup for my other enviornment, GCC won't compile due to a dependency error. Types must be cleaned, prefer
string. Move improvements are to be made.

Work: Move semantics MUST be improved. Types must be unified to string, and error checking must be introduced by beta stage

__								IN_PROG		DRAFT		BETA		FINAL

_DEBUG					X					X						

CONCURRENCY			X						X	

ENCRYPTION				X				X					

FILESYSTEM				X					X			

HANDLER				X									

INTERNAL				X									

NETWORK				X					X						

REVISIONING			X									

SCHEDULER			X

FILE							X					X

CONNECTION						

TIME							X					X
	
