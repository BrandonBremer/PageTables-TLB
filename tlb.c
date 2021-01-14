/**Discussed with George Taliafero wgt7xp*/
#include "mlpt.h"
#include "config.h"
#include "tlb.h"
#include <assert.h>
struct cacheLine tlb[16][4];
/** invalidate all cache lines in the TLB */
void tlb_clear()
{
    for(int i = 0; i<16;i+=1)
        for(int j = 0; j<4; j+=1)
            tlb[i][j].v=0;
}
size_t getIndex(size_t va)
{
    return (va&15<<POBITS)>>POBITS;
}
size_t getTag(size_t va)
{
    return va&((-1)^((1<<POBITS+4)-1))>>(4);
}
/**
 * return 0 if this virtual address does not have a valid
 * mapping in the TLB. Otherwise, return its LRU status: 1
 * if it is the most-recently used, 2 if the next-to-most,
 * etc.
 */
int tlb_peek(size_t va)
{
    for(int i =0; i< 4; i+=1)
    {
       if( tlb[getIndex(va)][i].tag==getTag(va)&& tlb[getIndex(va)][i].v==1)
       {
           return tlb[getIndex(va)][i].LRU;
       }
    }
    return 0;
}

/**
 * If this virtual address is in the TLB, return its
 * corresponding physical address. If not, use
 * `translate(va)` to find that address, store the result
 * in the TLB, and return it. In either case, make its
 * cache line the least-recently used in its set.
 *
 * As an exception, if translate(va) returns -1, do not
 * update the TLB: just return -1.
 */
size_t tlb_translate(size_t va)
{
		int prev_lru;
         for(int i =0; i< 4; i+=1)
           {
			     if(tlb[getIndex(va)][i].tag==getTag(va)&&tlb[getIndex(va)][i].v==1)
                {
					if(tlb[getIndex(va)][i].LRU!=1)
					{	
						prev_lru=tlb[getIndex(va)][i].LRU;
						for(int j =0; j< 4; j++)
						{
							if(tlb[getIndex(va)][j].LRU<prev_lru)
								tlb[getIndex(va)][j].LRU+=1;
						}
						tlb[getIndex(va)][i].LRU=1;
					}
					return tlb[getIndex(va)][i].block|(va&(1<<POBITS)-1);
                }
			}
	int lru =0;
	int lru_index = 0;
	for(int i =0; i< 4; i++)
	{
		if(lru< tlb[getIndex(va)][i].LRU)
		{
			lru = tlb[getIndex(va)][i].LRU;
			lru_index = i;
		}
	}
    
        if(translate(va)!=-1)
        {
            tlb[getIndex(va)][lru_index].tag =getTag(va);
            tlb[getIndex(va)][lru_index].v=1;
            tlb[getIndex(va)][lru_index].block =translate(va&((-1)^((1<<POBITS)-1)));	
			tlb[getIndex(va)][0].LRU+=1;
			tlb[getIndex(va)][1].LRU+=1;
			tlb[getIndex(va)][2].LRU+=1;
			tlb[getIndex(va)][3].LRU+=1;
			tlb[getIndex(va)][lru_index].LRU=1;
			return tlb[getIndex(va)][lru_index].block|(va&(1<<POBITS)-1);
        }
    
	return -1;
    
}

