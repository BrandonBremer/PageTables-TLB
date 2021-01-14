//Brandon Bremer beb8nj
//Discussed with William George Taliaferro wgt7xp
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <unistd.h>   
#include <string.h>
#include <stdlib.h>
#include "mlpt.h"
#include "config.h"
size_t ptbr=(size_t)NULL;
/**vpns helper method calculates the virtual page numbers of a given virtual adress, the adress is passed as a size_t arg, and places these into the virtual size_t array passed to the method as an argument.*/
void vpns(size_t *virtual, size_t adress)
{
    size_t v_size = (size_t)POBITS - 3;
	size_t shift = (size_t)pow((double)2,(double)(POBITS - 3)) - 1;
	for (int i = 0;i < LEVELS;i += 1){
		virtual[i] = (adress & (shift << (POBITS + v_size*(LEVELS - i - 1)))) >> (POBITS + v_size * (LEVELS - i - 1));
	}
}
/**Given a virtual adress va the translate returns the physical adress. If the given virtual adress does not have a physical adress then the adress returns all 1 bits*/
size_t translate(size_t va)
{	
	if (ptbr == (size_t)NULL)
		return -1;
	size_t *point = (size_t *)ptbr;
	size_t virtual[LEVELS];
	vpns(virtual,va);		
	for (int i = 0;i < LEVELS;i += 1){
		if ((point[virtual[i]]&1)){
			point = &point[virtual[i]];
		}
		else{
			return -1;
		}

	}
	return point[0]-1 | (va & ((1 << POBITS) - 1));//-1
	
}
/**page_allocate: On the first call allocates memory for a page table using the posix_memalign function. Creates a page table sufficient to map any given virtual adress, va, to a physical adress. If the pages required for the mapping exist then page_allocate does nothing.  */
void page_allocate(size_t va)
{
        if (ptbr == (size_t)NULL){
		void *ans;
	 	posix_memalign(&ans, 1 << POBITS, 1 << POBITS);
		memset(ans,0,1<<POBITS);
		ptbr = (size_t)ans;
	 }
	size_t *point = (size_t *)ptbr;
	size_t virtual[LEVELS];
        vpns(virtual,va);
	for (int i = 0; i < LEVELS;i += 1){
		if (!(point[virtual[i]]&1)){
			void *ans;
	 		posix_memalign(&ans, 1 << POBITS, 1 << POBITS);
			memset(ans,0,1 << POBITS);
			point[virtual[i]] = (size_t )(ans)+1;	
		}
		point = &point[virtual[i]];
	}	  
}



