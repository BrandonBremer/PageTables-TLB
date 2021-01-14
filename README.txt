*To customize config.h set the values of POBITS and LEVELS, but do not edit PTBR. POBITS is the bit offset and LEVELS is how many levels there are to the page table 
implementation. LEVELS should be an integer value greater than 0. POBITS + LEVELS * (POBITS-3) must be less than or equal to 64. 

*Use Case: Our page table implementation could be utilized as a look up table with constant time look ups. It could accept any value which could be divided into levels,
for example ints could easily be mapped into the look up table but strings would have to be converted to their ascii values. 

*Bugs: Known bugs include that certain values of pobits levels and adressed cause Translate to pass an invalid adress. The number of pageTables allocated based on 
posix_memalign when tested in our code fits the required but fails in the automated tester. 

*Deallocate method for Page Table: In the man page of posix_memalign it states that you can free the void* that is set by posix_memalign, but there is no valid way 
which we could configure to access this pointer in a seperate deallocate method. The pointer accessed from a traversal of the page table in a way akin to translate
is invalid for this operation when we attempted it. 