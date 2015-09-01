#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct page_tab {       //page table contains frame number and valid
    
        int frame_number;
        int valid;
    
} PT;
        
        
typedef struct physical_m {     //physical memory is represented with a space for 256 bytes
    
        char bytes[256];
    
} PM;

typedef struct TQueueElement { //this will be the data struct inside a tlb node
        
        int page;
        int frame;
        
}TQueueElement;

void TSOuraPlus_setValue (int framen, int pagen, TQueueElement * const source);

typedef struct TLB *TQueuePlus;

typedef struct node_type *pointer_type;

typedef struct node_type {   //a node will have a pointer to both previous and next nodes, and a struct for keeping its data
	
        TQueueElement data;
	pointer_type next;
	pointer_type prev;
        
} node_type;

typedef struct TLB {            //tlb will be implemented as a doubly linked list
    
        pointer_type Head,
                     Last,
                     Current;
        int ListSize;
        
}TLB;

void TLB_create (TQueuePlus * const queueP);

void TLB_destroy (TQueuePlus * const queueP);

int TLB_empty (const TQueuePlus queueP);

int TLB_size (const TQueuePlus queueP);

int TLB_addlast (int pagen, int framen, const TQueuePlus queueP);

int TLB_removefirst (const TQueuePlus queueP);

int TLB_movelast (const TQueuePlus queueP);

int TLB_next (const TQueuePlus queueP);

int TLB_first(const TQueuePlus queueP);