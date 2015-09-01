#include"datatypes.h"

int main(int argc, char *argv[]) {

        FILE *fp;
        FILE *fp2;
        
        char line[128];
        int int_line;
        
        int page_number;
        int frame;
        int offset;
        int listsize;
        
        char val[256];
        
        int i,j;
        int tlbfound;
        int pagetfound;
        int counter = 0;
        
        int tlbhits = 0;
        int pagefault;
        int addresses = 0;
        
        if(argc!=2) {
            
                printf("Check Arguments!\n");
                return (EXIT_FAILURE);
                
        }
        
        /*check if the file containing addresses is empty*/
        if ((fp = fopen(argv[1], "r")) == NULL) {
                
                perror("File Is Empty");
                return (EXIT_FAILURE);
        }
        
        /*open BACKING_STORE for byte read*/
        fp2 = fopen("BACKING_STORE.bin","rb");
        if (!fp2) {

                printf("Unable To Open File!");
                return (EXIT_FAILURE);

        }
        
        /*define a page table and initialize it*/
        PT page_table[256];
        for (i=0;i<256;i++) {
            
                page_table[i].frame_number = -999; //a value to make sure pt has no entry
                page_table[i].valid = 0;
        
        }
        
        /*define a physical memory an initialize it*/
        PM physical_memory[256];
        
        for (i=0;i<256;i++) {
            
                memset (physical_memory[i].bytes,0,256);
           
        }
        
        /*create the TLB*/
        TQueuePlus tlb = NULL;
        TLB_create (&tlb);
                
        /*the program will run as long as we read an address from input file*/
        while(fgets(line,sizeof(line),fp) != NULL) {
                
                addresses++;    //counting how many addresses we read
                printf("----------------------------------------------------------\n");
                int_line = atoi(line);  //convert the address from char* to int to use it 
                printf("Current Address From File Is:%d, ",int_line);
                
                page_number = int_line/256;     //find the page number
                offset = int_line%256;          //find the offset
                printf("Address Translates To Page:%d With Offset:%d\n",page_number,offset);
                
                tlbfound = 0;   //initialize the flag variables in each cycle
                pagetfound = 0;
                
                if (TLB_empty (tlb) == 1) { //tlb is empty, it means page table and physical mem are also empty
                        
                        /*there is no address so we have to go to BACKINGSTORE.bin and grab the current page's 256 bytes*/
                        fseek(fp2,256*page_number,SEEK_SET);
                        fread(val,sizeof(char),256,fp2);
                        
                        memcpy(physical_memory[counter].bytes,val,256); //place the 256 bytes in a slot in phys.memory
                        page_table[page_number].valid = 1;              //update the page table with valid bit
                        page_table[page_number].frame_number = counter; //and with the frame that contains the 256 bytes to refer to
                        TLB_addlast (page_number,counter,tlb);          //add the bytes and frame number also in the tlb
                        printf ("Value Of Byte In Physical Memory:%d\n",(int) physical_memory[counter].bytes[offset]);
                        pagefault++;
                        counter++;
                    
                }
                else { //tlb is not empty

                        listsize = TLB_size (tlb);      //keep the size of the tlb list
                        TLB_first(tlb);                 //move the pointer into the first node of the list

                        for(i=0;i<listsize;i++) {       //search every node in the list to see if current page exists in tlb

                                if (tlb->Current->data.page == page_number) {   //if it does

                                        frame = tlb->Current->data.frame;       //keep the frame that page table slot refers to
                                        TLB_movelast (tlb);                     //enabling lru algorithm for the node
                                        printf ("Value Of Byte In Physical Memory:%d\n",(int) physical_memory[frame].bytes[offset]);
                                        tlbfound = 1;
                                        tlbhits++;
                                        break;

                                }

                                TLB_next (tlb); //move the pointer to next node

                        }
                        
                        if (tlbfound == 0) {    //means that current page number does not exist in tlb


                                if(page_table[page_number].valid == 1) { //if the slot of the current page number has the address

                                        frame = page_table[page_number].frame_number; //keep the frame number

                                        listsize = TLB_size (tlb); //and update the tlb checking with lru logic
                                        if (listsize == 16) {

                                                TLB_removefirst (tlb);

                                        }
                                        TLB_addlast (page_number,frame,tlb);

                                        printf ("Value Of Byte In Physical Memory:%d\n",(int) physical_memory[frame].bytes[offset]);
                                        pagetfound = 1;

                                }

                        }
                        
                        if (pagetfound == 0 && tlbfound == 0) { //if the address does not exit neither in tlb nor page table

                                fseek(fp2,256*page_number,SEEK_SET); //then get the address from .bin and update the tlb,page table,and physical memory
                                fread(val,sizeof(char),256,fp2);
                
                                memcpy(physical_memory[counter].bytes,val,256);
                                page_table[page_number].valid = 1;
                                page_table[page_number].frame_number = counter;
                                
                                listsize = TLB_size (tlb);
                                if (listsize == 16) {

                                        TLB_removefirst (tlb);

                                }
                                TLB_addlast (page_number,frame,tlb);
                                
                                printf ("Value Of Byte In Physical Memory:%d\n",(int) physical_memory[counter].bytes[offset]);
                                pagefault++;
                                counter++;

                        }

                }
        
        }
        
        float perc1,perc2,perc3;
        int temp = addresses-tlbhits;

        perc1 = (float)pagefault/addresses*100;
	perc2 = (float)temp/addresses*100;
	perc3 = (float)tlbhits/addresses*100;
	        
        printf("\n");
        printf("------------------------------STATISTICS------------------------------------\n");
        printf("Page Faults in Physical Memory:%d\n",pagefault);
        printf("Page Fault in Physical Memory Ratio:%.2f%%\n",perc1);
        printf("TLB Misses:%d\n",addresses-tlbhits);
        printf("TLB Miss Ratio:%.2f%%\n",perc2);
        printf("TLB Hits:%d\n",tlbhits);
        printf("TLB Hit Ratio:%.2f%%\n",perc3);
        
        fclose(fp);
        fclose(fp2);
        TLB_destroy (&tlb);
    
        return (EXIT_SUCCESS);

}


