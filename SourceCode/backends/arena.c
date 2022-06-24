#include <stdio.h>
#include <stdlib.h>

#include "arena.h"
#include "types.h"

#define BLOCK_SIZE 1024
#define INIT_POOL_SIZE 1024
#define align_to(p,a)  (char*) (((uint_32)(p)+((uint_32)(a)-1)) & ~((uint_32)(a)-1))
//对齐p指针到a字节
 
char **blocks;
size_t current_block;
size_t remain;
char *alloc_p=NULL;
void *AllocateBlock(size_t bytes);
void *AllocateBig(size_t bytes);
void *Allocate(size_t bytes);
void Free(void *ptr);
void Protect(void *ptr, size_t nbytes);

size_t MAX_BLOCKS=INIT_POOL_SIZE;
_GCControlBlock ArenaPool;

_GCControlBlock arenaInit()//初始化内存池 
{
	alloc_p = NULL;
	remain = 0;
	current_block = 0;
	MAX_BLOCKS=INIT_POOL_SIZE; 
	while (!(blocks=(char **)calloc(MAX_BLOCKS, sizeof(char *)))); 
	ArenaPool = (_GCControlBlock)malloc(sizeof(_GCControlBlock_self));
	ArenaPool->allocMethod = Allocate;
	ArenaPool->freeMethod = Free;
	ArenaPool->protectMethod = Protect;

	return ArenaPool;
}
void *AllocateBig(size_t bytes)//大块(过大或超过剩余空间)的处理 
{
	if (bytes>BLOCK_SIZE/4) {
		void *ret=AllocateBlock(bytes);
		return ret;
	}
	alloc_p = (char *)AllocateBlock(BLOCK_SIZE);
	blocks[current_block] = alloc_p;
	current_block++;
	remain = BLOCK_SIZE;
	
	if (current_block==MAX_BLOCKS) {//动态扩容 
		MAX_BLOCKS<<=1;
		blocks=(char **)realloc(blocks, MAX_BLOCKS*sizeof(char *));
	}
	
	char* ret = alloc_p;
	alloc_p += bytes;
	remain -= bytes;
	return (void*)ret;
}
void *AllocateBlock(size_t bytes)//分配新块 
{
	char *new_block_p;
	while(!(new_block_p = (char *)malloc(bytes)));
	return (void *)new_block_p;
}
void *Allocate(size_t bytes)//主要分配器 
{
	if (!(bytes>0)) return NULL;
	size_t step=align_to(alloc_p,8)-alloc_p+bytes;
//	size_t step=bytes;

	if (step<=remain) {
		char *ret = align_to(alloc_p,8);
//		char *ret = alloc_p;
		alloc_p+=step;
		remain-=step;
		return (void *)ret;
	}
	return (void *)AllocateBig(bytes);
}
// The "beautiful" implementation in strlib.c make this two funcitons extremely hard for coding
// A big "thank you" to you, libgraphics! 
void Free(void *ptr)
{
	void *_place_holder;
}
void Protect(void *ptr, size_t nbytes)
{
	void *_place_holder;
	
}

