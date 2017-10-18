/*
 * This part of code is from https://gist.github.com/rehrumesh/5fa2bf97e508888f0401
 * Thanks for Rumesh Eranga <rehrumesh>
 */
#include <stdio.h>
#include <mm.h>

/**
 * stucture to hold the details about the spaces
*/
typedef struct node{
    int size;			//	Original requested size
    char isFree;		// 	1 if free or 0 if in use
    struct node *next;	//	pointer to to next memory chunk
} node;

#define MEMORY_POOL_SIZE					1024
static char memory[MEMORY_POOL_SIZE];		//	Memory, used to supply memory chunks
static int initialized = 0;	//	a tag to mark the 1st use of myMalloc

static node *head=(node*)memory;	// Header node to hold the starting point

/**
 * initialize the header on the 1st call
*/
static void heap_fixme(void) {
    //	checks for the 1st run
    if(!initialized) {
        head->size = MEMORY_POOL_SIZE - sizeof(node);	// Total Available Space
        head->isFree =1 ;
        head->next = NULL;
        initialized = 1;	//	set the value to 1, so this code won't run again
    }
}

/**
 * mm_malloc, which we can used instead of inbuilt "malloc"
*/
char* mm_malloc(int size) {
    int isFit=0;		//	represents the found chunk is fitting or not.
    int isFound=0;		//	represents if found a eligible chunk
    node *temp = head;	//	temp to slide throgh the list

	if(size <= 0) return NULL;	//	returns null if requested size is negative or zero

	heap_fixme();	//	If the first run of malloc

    while(temp->next != NULL){
		temp = temp->next;
		/*
		 * if the current chunk is free && its size is bigger than requested.
		 * so the remaining chunk after allocation should atleast fit the node size.
		 * so checking for "sizeof(node)" additional space.
		*/
		if(temp->isFree){
			if(temp->size > size + 2*sizeof(node)){
				break;
				isFound=1;

			}
			if(temp->size >= size + sizeof(node)){
				isFit=1;
				isFound=1;
				break;
			}
		}
	}

	if(isFound || temp==head){
		//	is there is a nearly fitting chunk, just allocate it.
		if(isFit){
			temp->size=size;
			temp->isFree=0;

		}else{
			//	new node to hold the remaining chunk
			node *new=(node*) ((char*) temp + size + sizeof(node));

			int prevSize=temp->size;
			temp->next = new;
			temp->isFree = 0;
			temp->size = size;

			new->isFree = 1;
			new->size = prevSize - sizeof(node)-size;	//	set the size of remaining chunk
			//	return the pointer points to allocated space

		}
		return (char*)temp + sizeof(node);
	}else{
		return NULL;
	}
}

/**
 * mm_free, which we can used instead of inbuilt "free"
 */
void mm_free(int* target){
	//	get the pointer to set free
	node* temp=(node*) ((char*)target-sizeof(node));
	if(temp!=NULL){
		temp->isFree=1;	//	set free

		//	if the next chunk is also free, combine it with the current chunk.
		if(temp->next!=NULL && temp->next->isFree==1){
			temp->size+=sizeof(node)+temp->next->size;	//	combined size
			temp->next = temp->next->next;	//	join
		}
	}
}
