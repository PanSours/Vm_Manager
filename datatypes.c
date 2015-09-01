#include"datatypes.h"

/*set given values to node data*/
void TSQueuePlus_setValue (int framen, int pagen, TQueueElement * const source) {
        
        source->page = pagen;
        source->frame = framen;
        
}

/*initializing a new list*/
void TLB_create (TQueuePlus * const queueP) {
    
        *queueP = malloc(sizeof(TLB));
        (*queueP)->Head=NULL;
        (*queueP)->Last=NULL;
        (*queueP)->Current=NULL;
	(*queueP)->ListSize=0;
        
}

/*frees the memory from list space*/
void TLB_destroy (TQueuePlus * const queueP) {
    
        pointer_type temporary;
        
        while((*queueP)->Head!=(*queueP)->Last) {
            
                temporary = (*queueP)->Head;
                (*queueP)->Current=(*queueP)->Head->next;
                (*queueP)->Current->prev=(*queueP)->Last;
                (*queueP)->Last->next=(*queueP)->Current;
            
                free(temporary);
                (*queueP)->ListSize--;
                (*queueP)->Head=(*queueP)->Current;
                
        }

        free((*queueP)->Last);
        free(*queueP);
        *queueP=NULL;
        
}

/*checks if list is empty, 0 for not empty,1 for empty*/
int TLB_empty (const TQueuePlus queueP) {
    
        if((queueP->Head)==NULL && (queueP->Last)==NULL && (queueP->Current)==NULL && (queueP->ListSize)==0)
		return 1;
	else
		return 0;
        
}

/*returns the size current size of the list*/
int TLB_size (const TQueuePlus queueP) {
    
        return queueP->ListSize;

}

/*adds a node to the end of the list*/
int TLB_addlast (int pagen, int framen, const TQueuePlus queueP) {
    
        pointer_type temporary;
        temporary =  malloc(sizeof(node_type));
        if(temporary == NULL){ return 0;}
        
        if (TLB_empty (queueP) == 0) {
            
                temporary->prev = queueP->Last;
                temporary->next = queueP->Head;
                (queueP->Head)->prev = temporary;
                (queueP->Last)->next = temporary;
                queueP->Current = temporary;
                
                TSQueuePlus_setValue (framen, pagen, &(queueP->Current)->data);
                queueP->Last = temporary;
                queueP->ListSize++;
                
        }
        else {
            
                queueP->Head = temporary;
                queueP->Last = temporary;
                queueP->Current = temporary;
                temporary->next = temporary;
                temporary->prev = temporary;
                TSQueuePlus_setValue (framen, pagen, &(queueP->Current)->data);
                queueP->ListSize++;
                
        }
        
        return 1;
        
}

/*removes the first node of the list*/
int TLB_removefirst (const TQueuePlus queueP) {
    
        pointer_type temporary;
        
        if (TLB_empty(queueP)== 0) {
            
                if((queueP->Head)->next!=NULL) {

                        temporary = queueP->Head;
                        queueP->Current = queueP->Head->next;
                        free(temporary);
                        
                        queueP->Head = queueP->Current;
                        (queueP->Last)->next = queueP->Current;
                        (queueP->Current)->prev = queueP->Last;
                        queueP->ListSize--;

            }
            else {
                
                        temporary = queueP->Head;
                        free(temporary);
                        queueP->Head = NULL;
                        queueP->Last = NULL;
                        queueP->Current = NULL;
                        queueP->ListSize--;
            
            }
                
            return 1;
            
        }
        else {
            
                return 0;
        
        }

}

/*this function moves a just used node at the end of the list, helping on lru logic*/
int TLB_movelast (const TQueuePlus queueP) {
    
        if (TLB_empty (queueP) == 0) {
            
                if (queueP->Current == queueP->Head) {
                        
                        queueP->Head = (queueP->Current)->next;
                        queueP->Last = queueP->Current;
            
                }
                else if ( queueP->Current != queueP->Last ) {
                
                        ((queueP->Current)->prev)->next = (queueP->Current)->next;
                        ((queueP->Current)->next)->prev = (queueP->Current)->prev;
                
                        (queueP->Current)->prev = queueP->Last;
                        (queueP->Current)->next = queueP->Head;
                
                        (queueP->Head)->prev = queueP->Current;
                        (queueP->Last)->next = queueP->Current;
                
                        queueP->Last = queueP->Current;
                
                }
                
                return 1;
                
        }
        else {
                
                return 0;
        
        }
      
}

/*moves current pointer to its next node*/
int TLB_next (const TQueuePlus queueP) {
    
        if(TLB_empty(queueP) == 0) {
            
                queueP->Current = (queueP->Current)->next;
                return 1;
        
        }
        else return 0;

}

/*moves current pointer at the start of the list*/
int TLB_first(const TQueuePlus queueP) {
        
        if (TLB_empty(queueP) == 0) {
            
                queueP->Current=queueP->Head;
                return 1;
        
        }
        else return 0;

}


