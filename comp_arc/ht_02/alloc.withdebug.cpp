#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>


// #define MCB_FREE_STRICT_MODE

const unsigned char MCB_TYPE_ALLOCATED = 0;
const unsigned char MCB_TYPE_FREE = 1;
const unsigned char MCB_TYPE_COMPOSITE = 2;


const int FERROR_TOO_FEW_MEMORY = 1;
const int FERROR_COULD_NOT_ALLOCATE = 2;

struct mcb{
	unsigned char type;
	int freeSpace;
	int maxBlock;
	mcb* child_l;
	mcb* child_r;
	mcb* parent;
};



const int MCB_SIZE = sizeof(mcb);
mcb* MCB_NO_NODE = (mcb*)NULL;


void* memoryBegin;
int memorySize;

void* createMemory(int size);
void dropMemory(void* ptr);
void putMcb(void* ptr, int mcb_type, int mcb_freeSpace, int mcb_maxBlock, mcb* parent);
void* my_alloc(mcb* node, size_t size);
void* my_alloc(size_t size);
void* alloc_here(mcb* node, int size);
void my_delete(void *ptr);
void update(mcb* node);
void my_free(mcb* node);
bool my_free(int blockNumber);
mcb* merge(mcb* node_l, mcb* node_r, int height_l, int height_r);
// void memoryMap(mcb* node);
void memoryMap(mcb* node, int* count);
void memoryMap();
mcb* nearestRightNode(mcb* node);
mcb* nearestRightTerminatingNode(mcb* node, int* height_l, int* height_r);
mcb* nearestLeftTerminatingNode(mcb* node, int* height_l, int* height_r);
inline bool isRoot(mcb* node);
inline bool isFree(mcb* node);
inline mcb* rightNeighboor(mcb* node);
inline mcb* leftNeighboor(mcb* node);
int distFromBeg(void** node);
int distFromBeg(mcb* node);
mcb* nodeFromBlockNumber(int blockNumber);
int spaceToTheRight(mcb* node);
void info();
int usedBlocksCount(mcb* node);
int usedMemoryCount(mcb* node);
mcb* findNode(mcb* nodePtr);

// debug functions
void printTree(mcb* node);
void printTree();
std::string addressToString(mcb* node);


mcb* findNode(mcb* nodePtr){
	mcb* node = (mcb*)memoryBegin;
	std::cout << "findNode: node = " << distFromBeg(node) << std::endl;
	while(node->child_l != MCB_NO_NODE){

		if(node >= nodePtr) return MCB_NO_NODE; // this means we found node, but it has children => it's not allocatedspace => cannot free it

		if( (int)(node->child_r) > (int)nodePtr ){
			node = node->child_l;
		} else {
			node = node->child_r;
		}
		std::cout << "findNode: node = " << distFromBeg(node) << std::endl;
	}
	// here node->child_l == MCB_NO_NODE
	// so
	#ifdef MCB_FREE_STRICT_MODE
	if(node == nodePtr){
		return node;
	} else {
		return MCB_NO_NODE;
	}
	#else
	return node;
	#endif
}

int usedMemoryCount(mcb* node){

	if(node->type == MCB_TYPE_ALLOCATED) return spaceToTheRight(node);
	if(node->type == MCB_TYPE_FREE) return 0;

	return usedMemoryCount(node->child_l) + usedMemoryCount(node->child_r);

}
int usedBlocksCount(mcb* node){

	if(node->type == MCB_TYPE_ALLOCATED) return 1;
	if(node->type == MCB_TYPE_FREE) return 0;

	return usedBlocksCount(node->child_l) + usedBlocksCount(node->child_r);

}


void info(){
	std::cout << usedBlocksCount((mcb*)memoryBegin) << " " << usedMemoryCount((mcb*)memoryBegin) << " " << ((mcb*)memoryBegin)->freeSpace << " " << ((mcb*)memoryBegin)->maxBlock << std::endl;
}


std::string addressToString(mcb* node){
	// std::cout << "addressToString: distFromBeg(node)=" << distFromBeg(node) << std::endl;

	if(node != MCB_NO_NODE){
		char str[20];
		sprintf(str, "%d", distFromBeg(node));
		// std::cout << "addressToString: res=" << std::string(str) << std::endl;
		return std::string(str);
	} else {
		// std::cout << "addressToString: res=NULL" << std::endl;
		return std::string("NULL");
	}
}

void printTree(mcb* node){
	std::cout << "[" << distFromBeg(node) << "]" << std::endl;
	std::cout << "type = ";
	if(node->type == MCB_TYPE_FREE){
		std::cout << "f";
	} else if(node->type == MCB_TYPE_ALLOCATED){
		std::cout << "a";
	} else {
		std::cout << "c";
	}
	std::cout << std::endl;
	std::cout << "freeSpace = " << node->freeSpace << std::endl;
	std::cout << "maxBlock = " << node->maxBlock << std::endl;
	std::cout << "child_l = " << addressToString(node->child_l) << std::endl;
	std::cout << "child_r = " << addressToString(node->child_r) << std::endl;
	std::cout << "parent = " << addressToString(node->parent) << std::endl;
	std::cout << std::endl;

	if(node->child_l != MCB_NO_NODE){
		printTree(node->child_l);	
	}
	if(node->child_l != node->child_r){
		printTree(node->child_r);
	}

}

void printTree(){
	printTree((mcb*)memoryBegin);
}

mcb* nodeFromBlockNumber(int blockNumber){
	return (mcb*)( (int)memoryBegin + blockNumber );
}

int distFromBeg(void* node){
	return ((int)node - (int)memoryBegin);
}
int distFromBeg(mcb* node){
	return ((int)node - (int)memoryBegin);
}

void memoryMap(){
	int count = 0;
	memoryMap((mcb*)memoryBegin, &count);
	// std::cout << std::endl;
	std::cout << " totally " << count << std::endl;
}

void memoryMap(mcb* node, int* count){
	if(node == MCB_NO_NODE) return;

	std::cout << "[";

	int thisMCBBlockSize = MCB_SIZE;
	if(node->type == MCB_TYPE_COMPOSITE && nearestRightNode(node) != MCB_NO_NODE){
		thisMCBBlockSize += spaceToTheRight(node);
	}

	// std::cout << "memoryMap (" << distFromBeg(node) << "), thisMCBBlockSize=" << thisMCBBlockSize << std::endl;
	
	for(int i = 0; i < thisMCBBlockSize; i++){
		++(*count);
		std::cout << "m";
	}

	if(node->type == MCB_TYPE_FREE){
		for(int i = 0; i < node->freeSpace; i++){
			++(*count);
			std::cout << "f";
		}
		// return;
	} else if(node->type == MCB_TYPE_ALLOCATED){
		int usedSpace = spaceToTheRight(node);
		// int usedSpace = (int)((void*)(rightNeighboor(node))) - (int)((void*)node) - MCB_SIZE;
		for(int i = 0; i < usedSpace; i++){
			++(*count);
			std::cout << "u";
		}
		// return;
	} else {
		memoryMap(node->child_l, count);
		memoryMap(node->child_r, count);
	}

	std::cout << "]";
}


void* createMemory(int size){
	return malloc(size);
}
void dropMemory(void* ptr){
	free(ptr);
}



int spaceToTheRight(mcb* node){
	mcb* node_r = nearestRightNode(node);
	if(node_r == MCB_NO_NODE){
		// if there is no node to the right => the space to the right is space to the end of memory
		return (int)memoryBegin + memorySize - (int)node - MCB_SIZE;
	} else {
		// if there is a node to the right => the space to the right is space between end of this node and node_r
		return (int)node_r - (int)node - MCB_SIZE;
	}
}

void putMcb(void* ptr, int mcb_type, int mcb_freeSpace, int mcb_maxBlock, mcb* parent){
	
	mcb newMcbBlock;
	
	newMcbBlock.type = mcb_type;
	newMcbBlock.freeSpace = mcb_freeSpace;
	newMcbBlock.maxBlock = mcb_maxBlock;
	newMcbBlock.child_l = MCB_NO_NODE;
	newMcbBlock.child_r = MCB_NO_NODE;
	newMcbBlock.parent = parent;
	
	memcpy(ptr, &newMcbBlock, MCB_SIZE);
}

bool init(void* ptr, int size){
	// if not enough memory => can't start
	if(size < MCB_SIZE) return false;
	// otherwise - write the first mcb
	putMcb(ptr, MCB_TYPE_FREE, size - MCB_SIZE, size - MCB_SIZE, MCB_NO_NODE);
	return true;
}


inline bool isRoot(mcb* node){
	return (node->parent == MCB_NO_NODE);
}

inline bool isFree(mcb* node){
	if(node == MCB_NO_NODE) return false;
	return (node->type == MCB_TYPE_FREE);
}

inline mcb* rightNeighboor(mcb* node){
	// this function expects that it exists (i.e. it doesn't check if node->parent->child_r == node)
	return node->parent->child_r;
}

inline mcb* leftNeighboor(mcb* node){
	// this function expects that it exists (i.e. it doesn't check if node->parent->child_l == node)
	return node->parent->child_l;
}

mcb* nearestRightNode(mcb* node){
	// std::cout << "nearestRightNode(" << distFromBeg(node) << ") = ";
	if(node->child_l != MCB_NO_NODE){
		// std::cout << distFromBeg(node->child_l) << std::endl;
		return node->child_l;
	}

	while(1){
		if(node->parent == MCB_NO_NODE){
			// std::cout << distFromBeg(MCB_NO_NODE) << std::endl;
			return MCB_NO_NODE;
		}
		// std::cout << "node=" << distFromBeg(node) << std::endl;
		if(node == node->parent->child_r){
			node = node->parent; 
		} else {
			// std::cout << distFromBeg(node->parent->child_r) << std::endl;
			return node->parent->child_r;
		}
	}
}


mcb* nearestRightTerminatingNode(mcb* node, int* height_l, int* height_r){

	// height_l and height_r are values how far do we have to climb to reach LCA
	// of node and it's nearestRightTerminatingNode (from node - height_l, from nearestRightTerminatingNode - height_r)
	// if returning MCB_NO_NODE height_l and height_r are meaningless
	*height_l = 0;
	*height_r = 1;

	while(1){
		(*height_l)++;
		if(node->parent == MCB_NO_NODE){
			return MCB_NO_NODE;
		}
		if(node == node->parent->child_r){
			node = node->parent; 
		} else {
			node = node->parent->child_r;
			break;
		}
	}
	while(node->child_l != MCB_NO_NODE){
		(*height_r)++;
		node = node->child_l;
	}
	return node;
}

mcb* nearestLeftTerminatingNode(mcb* node, int* height_l, int* height_r){
	// height_l and height_r are values how far do we have to climb to reach LCA
	// of node and it's nearestLeftTerminatingNode (from nearestLeftTerminatingNode - height_l, from node - height_r)
	// if returning MCB_NO_NODE height_l and height_r are meaningless

	*height_l = 1;
	*height_r = 0;

	while(1){
		(*height_r)++;
		if(node->parent == MCB_NO_NODE){
			return MCB_NO_NODE;
		}
		if(node == node->parent->child_l){
			node = node->parent; 
		} else {
			node = node->parent->child_l;
			break;
		}
	}
	while(node->child_r != MCB_NO_NODE){
		(*height_l)++;
		node = node->child_r;
	}
	return node;
}

void my_free(mcb* node){
	
	// std::cout << "my_free " << distFromBeg(node) << std::endl;

	if(/*isRoot(node)*/0){
		// std::cout << "my_free : node is root" << std::endl;
	} else {

		// bool isLeftChild = (node->parent->child_l == node);

		int height_l = 0;
		int height_r = 0;

		// first simply erase this node
		node->type = MCB_TYPE_FREE;
		// in node->freeSpace put the space between this node and the next to the right
		node->freeSpace = spaceToTheRight(node);
		node->maxBlock = node->freeSpace;
		// std::cout << "my_free : node->freeSpace=" << node->freeSpace << std::endl;

		// memoryMap();

		// and then merge with nearest nodes if they are free too
		// merge here is the most complicated thing
		mcb* nearest_r_term = nearestRightTerminatingNode(node, &height_l, &height_r);
		if(isFree(nearest_r_term)){
			node = merge(node, nearest_r_term, height_l, height_r);
		}
		mcb* nearest_l_term = nearestLeftTerminatingNode(node, &height_l, &height_r);
		if(isFree(nearest_l_term)){
			node = merge(nearest_l_term, node, height_l, height_r);
		}
		update(node);
	}
}


mcb* merge(mcb* node_l, mcb* node_r, int height_l, int height_r){
	

	// this function merges node_l and node_r. It expects that theese 2 nodes are two nearest terminating nodes (leaves)
	// and that both of them are free

	// std::cout << "merge (" << distFromBeg(node_l) << "," << distFromBeg(node_r) << ", height_l=" <<  height_l << ", height_r=" << height_r << ")" << std::endl;

	mcb* thingToReturn;

	if(height_r == 1){
		if(height_l == 1){

			// both heights are == 1. This means we can fully eliminate this two nodes and
			// give all the space to their parent
			thingToReturn = node_l->parent;

			int fullSpace = spaceToTheRight(node_l->parent) + spaceToTheRight(node_l) + spaceToTheRight(node_r) + 2 * MCB_SIZE;

			node_l->parent->type = MCB_TYPE_FREE;
			node_l->parent->freeSpace = fullSpace;
			node_l->parent->maxBlock = fullSpace;
			node_l->parent->child_l = MCB_NO_NODE;
			node_l->parent->child_r = MCB_NO_NODE;

			// std::cout << "fullSpace=" << fullSpace << std::endl;

			update(node_l->parent);

		} else {

			// here parent of node_r takes all the node_l, so
			// node_l's parent after this has only one child(it's child_l and child_r fields are equal)
			thingToReturn = node_l;

			mcb* node_l_left_neighboor = node_l->parent->child_l;

			int fullSpace = spaceToTheRight(node_l) + spaceToTheRight(node_r) + MCB_SIZE;
			
			node_l->parent->child_r = node_l->parent->child_l;
			node_r->parent->child_r = node_l;
			node_l->freeSpace = fullSpace;
			node_l->maxBlock = fullSpace;

			// std::cout << "fullSpace=" << fullSpace << std::endl;

			// but update can fix this
			update(node_l);
			update(node_l_left_neighboor);
		}
		
	} else if(height_r == 2){

		thingToReturn = node_l;

		mcb* node_r_right_neighboor = rightNeighboor(node_r);

		int fullSpace = spaceToTheRight(node_l) + spaceToTheRight(node_r->parent) + spaceToTheRight(node_r) + 2 * MCB_SIZE; // this is too slow, but

		node_r_right_neighboor->parent = node_r->parent->parent;
		node_r->parent->parent->child_r = node_r_right_neighboor;
		
		// std::cout << "spaceToTheRight(" << distFromBeg(node_l) << ")=" << spaceToTheRight(node_l) << std::endl;
		// std::cout << "spaceToTheRight(" << distFromBeg(node_r->parent) << ")=" << spaceToTheRight(node_r->parent) << std::endl;
		// std::cout << "spaceToTheRight(" << distFromBeg(node_r) << ")=" << spaceToTheRight(node_r) << std::endl;

		// int addedSpace = spaceToTheRight(node_l) + spaceToTheRight(node_r->parent) + spaceToTheRight(node_r) + 2 * MCB_SIZE; // this is too slow, but
		// int addedSpace = (int)node_r_right_neighboor - (int)(node_r->parent); // this and previous are equivalent

		node_l->freeSpace = fullSpace;
		node_l->maxBlock = fullSpace;

		update(node_l);
		update(node_r_right_neighboor);

	} else {

		thingToReturn = node_l;

		mcb* node_r_right_neighboor = rightNeighboor(node_r);
		// std::cout << "node_r_right_neighboor=" << distFromBeg(node_r_right_neighboor) << std::endl;


		// here all space of node_r comes to node_l, so node_r vanishes, we drop the parent of the node_r
		// and the grendparent of the node_r becomes the parent of right neighboor of node_r
		node_r_right_neighboor->parent = node_r->parent->parent;
		node_r->parent->parent->child_l = node_r_right_neighboor;

		// then we move to the right all the mcb-s from the grandparent of node_r to the LCA on node_l and node_r,
		// so that they are now close to each other to the left of right neighboor of node_r.
		mcb* node_src = node_r->parent->parent;
		mcb* node_dst = (mcb*)( (int)rightNeighboor(node_r) - MCB_SIZE);

		for(int i = 2; i < height_r; i++){
			// IMPORTANT - redirect links first and copy memory after
			

			// copy the node
			// std::cout << "memcpy(" << distFromBeg(node_src) << ", " << distFromBeg(node_dst) << ")" << std::endl;
			memcpy(  (void*)node_dst, (void*)node_src, MCB_SIZE);
			// std::cout << "memcpy() success" << std::endl;

			

			// redirect it's children to the right place
			node_src->child_l->parent = node_dst;
			node_src->child_r->parent = node_dst;
			// redirect it's parent to the right place
			node_src->parent->child_r = node_dst;
			// note that till we are in this loop we are always the right child of our parent


			// and move up the tree
			// std::cout << "node_src = node_src->parent;" << std::endl;
			node_src = node_src->parent;
			// std::cout << "node_src = node_src->parent;" << std::endl;
			node_dst = (mcb*)((int)node_dst - MCB_SIZE);
		}
		// now node_dst points to place where we didn't write (it has been shifted two lines ago)
		// we could shift it (+MCB_SIZE) but we don't correct this cause we need to calculate
		// fullSpace

		// so what space do we have?
		// all the space from node_l to node_dst

		int fullSpace = (int)node_dst - (int)node_l;

		node_l->freeSpace = fullSpace;
		node_l->maxBlock = fullSpace;

		// std::cout << "ready to update" << std::endl;
		update(node_l);
		update(node_r_right_neighboor);
	}

	// std::cout << "return " << distFromBeg(thingToReturn) << std::endl;
	return thingToReturn;
}

void* alloc_here(mcb* node, int size){
	// std::cout << "alloc_here " << distFromBeg(node) << std::endl;
	if(node->freeSpace < size) return NULL; // I don't know how can we get here, but anyway - if there's not enough space => do nothing

	if(node->freeSpace >= size + 2 * MCB_SIZE){
		
		// i.e we can allocate what we need and create node to the right

		void* startOfAllocatedSpace = (void*)((int)node + 2 * MCB_SIZE);
		void* placeForChild_l = (void*)((int)node + MCB_SIZE);
		void* placeForChild_r = (void*)((int)startOfAllocatedSpace + size);

		int spaceLeft = node->freeSpace - 2 * MCB_SIZE - size;

		node->child_l = (mcb*)( placeForChild_l );
		node->child_r = (mcb*)( placeForChild_r );
		node->type = MCB_TYPE_COMPOSITE;

		// std::cout << "putMcb( placeForChild_l, MCB_TYPE_ALLOCATED, 0, 0, node ); placeForChild_l=" << distFromBeg(placeForChild_l) << std::endl;
		putMcb( placeForChild_l, MCB_TYPE_ALLOCATED, 0, 0, node );
		// std::cout << "putMcb( placeForChild_r, MCB_TYPE_FREE, spaceLeft, spaceLeft, node ); placeForChild_r=" << distFromBeg(placeForChild_r) << std::endl;
		putMcb( placeForChild_r, MCB_TYPE_FREE, spaceLeft, spaceLeft, node );

		update(node);

		// std::cout << "alloc_here: readyToReturn" << std::endl;
		return startOfAllocatedSpace;

	} else {
		
		// we have enough space but we can't allocate full construction inside

		void* startOfAllocatedSpace = (void*)((int)node + MCB_SIZE);
		node->type = MCB_TYPE_ALLOCATED;
		node->freeSpace = 0;
		node->maxBlock = 0;

		update(node);
		return startOfAllocatedSpace;

	}
}

void* my_alloc(mcb* node, size_t size){
	// std::cout << "my_alloc(" << distFromBeg(node) << ", " << size << ")" << std::endl;
	if(node->child_l == MCB_NO_NODE){
		// no left child => no children at all => allocate here
		return alloc_here(node, size);
	} else {
		// both children exist. Allocate where is less maxBlock parameter
		
		if( (node->child_l->maxBlock) < (node->child_r->maxBlock) ){
			// if left is less
			if( (node->child_l->maxBlock) > size){
				// if it's still enough
				// allocate there
				return my_alloc(node->child_l, size);
			} else {
				// else allocate in the larger one
				return my_alloc(node->child_r, size);
			}
		} else {
			// the samememoryBegin
			if( (node->child_r->maxBlock) > size){
				return my_alloc(node->child_r, size);
			} else {
				return my_alloc(node->child_l, size);
			}
		}
	
	}
}

void* my_alloc(size_t size){
	if( ((mcb*)memoryBegin)->maxBlock < size) return NULL;

	return my_alloc((mcb*)memoryBegin, size);
	
}

void my_delete(void *ptr){
	mcb* mcbToBeKilled = (mcb*)( (int)ptr - MCB_SIZE );
	my_free(mcbToBeKilled);
}

bool my_free(int blockNumber){
	mcb* nodePtr = nodeFromBlockNumber(blockNumber - MCB_SIZE);
	mcb* node = findNode(nodePtr);
	std::cout << "findNode(" << distFromBeg(nodePtr) << ") = " << distFromBeg(node) << std::endl;
	if(node != MCB_NO_NODE){
		my_free(node);
		return true;
	} else {
		return false;
	}
}

void update(mcb* node){

	// std::cout << "update(" << distFromBeg(node) << ")" << std::endl;

	if(node->child_l != MCB_NO_NODE){
		
		
		if(node->child_l != node->child_r){

			// std::cout << "update: child_l != child_r" << std::endl;
			// if this node is OK (child_l != child_r) => racalculate it's parameters
			node->maxBlock = std::max( node->child_l->maxBlock, node->child_r->maxBlock );
			node->freeSpace = node->child_l->freeSpace + node->child_r->freeSpace;

		} else {

			// std::cout << "update: child_l == child_r" << std::endl;
		
			// if not OK => jusk drop this node. Unfortunately we can't reuse this space.
			if(node->parent->child_l == node){
				node->parent->child_l = node->child_l;
			}
			if(node->parent->child_r == node){
				node->parent->child_r = node->child_l;
			}
			node->child_l->parent = node->parent;
		}
	}

	if( node->parent != MCB_NO_NODE ){
		update(node->parent);
	}
	// std::cout << "update ret" << std::endl;
}





int main(){
	
	// freopen("alloc.in", "rt", stdin);

	memorySize = 0;
	memoryBegin = NULL;
	const char POSSIBLE_COMMANDS[] = "ALLOC, FREE, INFO, MAP, EXIT";

	std::cin >> memorySize;
	
	memoryBegin = malloc(memorySize);

	if(memoryBegin != NULL){

		bool initOk = init(memoryBegin, memorySize);

		if(!initOk){
			std::cout << "fatal error: programm could not init, too few memory, exiting (need at least " << MCB_SIZE << " bytes" << std::endl;
			return FERROR_TOO_FEW_MEMORY;
		}

		


		
		while(1){

			std::string str;
			std::cin >> str;

			if(str == "ALLOC"){

				int newBlockSize = 0;
				std::cin >> newBlockSize;

				void* res = my_alloc(newBlockSize);

				if(res == NULL){
					std::cout << "-" << std::endl;
				} else {
					std::cout << "+" << distFromBeg(res) << std::endl;
				}
				
			} else if(str == "FREE"){

				int block = 0;
				std::cin >> block;

				if(my_free(block)){
					std::cout << "+" << std::endl;
				} else {
					std::cout << "-" << std::endl;
				}
	
			} else if(str == "INFO"){
				
				info();

			} else if(str == "MAP"){
				
				memoryMap();

			} else if(str == "EXIT"){

				break;

			} else {

				std::cout << "error: programm could not recognise command: \"" << str << "\", avaliable commads are: " << POSSIBLE_COMMANDS << std::endl;

			}

		}


		free(memoryBegin);

	} else {

		std::cout << "fatal error: programm could not allocate " << memorySize << " bytes, exiting." << std::endl;
		return FERROR_COULD_NOT_ALLOCATE;

	}

	return 0;
}