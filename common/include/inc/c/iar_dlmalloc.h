/*
  IAR version of the header file for malloc-2.8.x, written by Doug Lea
  and released to the public domain, as explained at
  http://creativecommons.org/publicdomain/zero/1.0/ 
*/
/* Copyright 2014-2017 IAR Systems AB. */

#ifndef MALLOC_280_H
#define MALLOC_280_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ysizet.h>   /* for size_t */

#define MALLINFO_FIELD_TYPE size_t
#ifndef STRUCT_MALLINFO_DECLARED
  #define STRUCT_MALLINFO_DECLARED 1
  struct mallinfo {
    MALLINFO_FIELD_TYPE arena;    /* non-mmapped space allocated from system */
    MALLINFO_FIELD_TYPE ordblks;  /* number of free chunks */
    MALLINFO_FIELD_TYPE smblks;   /* always 0 */
    MALLINFO_FIELD_TYPE hblks;    /* always 0 */
    MALLINFO_FIELD_TYPE hblkhd;   /* space in mmapped regions */
    MALLINFO_FIELD_TYPE usmblks;  /* maximum total allocated space */
    MALLINFO_FIELD_TYPE fsmblks;  /* always 0 */
    MALLINFO_FIELD_TYPE uordblks; /* total allocated space */
    MALLINFO_FIELD_TYPE fordblks; /* total free space */
    MALLINFO_FIELD_TYPE keepcost; /* releasable (via malloc_trim) space */
  };
#endif /* STRUCT_MALLINFO_DECLARED */

/*
  realloc_in_place(void* p, size_t n)
  Resizes the space allocated for p to size n, only if this can be
  done without moving p (i.e., only if there is adjacent space
  available if n is greater than p's current allocated size, or n is
  less than or equal to p's size). This may be used instead of plain
  realloc if an alternative allocation strategy is needed upon failure
  to expand space; for example, reallocation of a buffer that must be
  memory-aligned or cleared. You can use realloc_in_place to trigger
  these alternatives only when needed.

  Returns p if successful; otherwise null.
*/
__ATTRIBUTES void * __iar_dlrealloc_in_place(void *, size_t);

/*
  memalign(size_t alignment, size_t n);
  Returns a pointer to a newly allocated chunk of n bytes, aligned
  in accord with the alignment argument.

  The alignment argument should be a power of two. If the argument is
  not a power of two, the nearest greater power is used.
  8-byte alignment is guaranteed by normal malloc calls, so don't
  bother calling memalign with an argument of 8 or less.

  Overreliance on memalign is a sure way to fragment space.
*/
__ATTRIBUTES void * __iar_dlmemalign(size_t, size_t);
__ATTRIBUTES void * __iar_dlaligned_alloc(size_t, size_t);

/*
  mallinfo()
  Returns (by copy) a struct containing various summary statistics:

  arena:     current total non-mmapped bytes allocated from system
  ordblks:   the number of free chunks
  smblks:    always zero.
  hblks:     current number of mmapped regions
  hblkhd:    total bytes held in mmapped regions
  usmblks:   the maximum total allocated space. This will be greater
                than current total if trimming has occurred.
  fsmblks:   always zero
  uordblks:  current total allocated space (normal or mmapped)
  fordblks:  total free space
  keepcost:  the maximum number of bytes that could ideally be released
               back to system via malloc_trim. ("ideally" means that
               it ignores page restrictions etc.)

  Because these fields are ints, but internal bookkeeping may
  be kept as longs, the reported values may wrap around zero and
  thus be inaccurate.
*/
__ATTRIBUTES struct mallinfo __iar_dlmallinfo(void);

/*
  malloc_stats();
  Prints on stderr the amount of space obtained from the system (both
  via sbrk and mmap), the maximum amount (which may be more than
  current if malloc_trim and/or munmap got called), and the current
  number of bytes allocated via malloc (or realloc, etc) but not yet
  freed. Note that this is the number of bytes allocated, not the
  number requested. It will be larger than the number requested
  because of alignment and bookkeeping overhead. Because it includes
  alignment wastage as being in use, this figure may be greater than
  zero even when no user-level chunks are allocated.

  The reported current and maximum system memory can be inaccurate if
  a program makes other calls to system memory allocation functions
  (normally sbrk) outside of malloc.

  malloc_stats prints only the most commonly interesting statistics.
  More information can be obtained by calling mallinfo.
*/
__ATTRIBUTES void  __iar_dlmalloc_stats(void);

/*
  malloc_usable_size(void* p);

  Returns the number of bytes you can actually use in
  an allocated chunk, which may be more than you requested (although
  often not) due to alignment and minimum size constraints.
  You can use this many bytes without worrying about
  overwriting other allocated objects. This is not a particularly great
  programming practice. malloc_usable_size can be more useful in
  debugging and assertions, for example:

  p = malloc(n);
  assert(malloc_usable_size(p) >= 256);
*/
__ATTRIBUTES size_t __iar_dlmalloc_usable_size(const void*);


/* **********************************************************************
 
   Debug fuctions

   ********************************************************************** */

/* **********************************************************************

   __iar_is_heap_block(void * ptr, __iar_heap_info * info);

   Checks if ptr is a heap block.
   ptr must be the pointer returned by malloc.

   If ptr is a heap block alocated by the debug heap
      1 is returned
      if info is not NULL 
         heap block info will be written to info
   otherwise
       0 is returned
       info is not used

   Note: This is not 100% reliable. It is possible that a non heap
   block will be classified as a heap block and that a heap block that
   has suffered from corruption won't be classified correctly  
   ********************************************************************** */

/* for extracting information about heap blocks */
typedef struct 
{
  size_t allocator_address;
  size_t block_id;
} __iar_heap_info;

__ATTRIBUTES size_t 
__iar_is_heap_block(void const * ptr, __iar_heap_info * info);

/* **********************************************************************

  __iar_get_heap_counter(void);

  

  ********************************************************************** */

/* **********************************************************************

   Debug heap interface

   ********************************************************************** */


/* verify the integrity of the heap, returns number of problems found */
__ATTRIBUTES size_t __iar_check_heap_integrity(void);

/* set the frequency (in heap operations) of heap integrity checks
   returns the old frequency */
__ATTRIBUTES size_t __iar_set_heap_check_frequency(size_t freq);

/* set the size of the delayed free list, returns the old size */
__ATTRIBUTES size_t __iar_set_delayed_free_size(size_t size);

/* shrink the delay list to count elements, returns the number of 
   freed elements */
__ATTRIBUTES size_t __iar_free_delayed_free_list(size_t count);

/* scan the heap for lost blocks. */
__ATTRIBUTES void __iar_check_leaks(void);

/* mark all currently allocated blocks as ignored by leak checker. */
__ATTRIBUTES void __iar_leaks_ignore_all(void);

/* mark a specific block as ignored by the leak checker. */
__ATTRIBUTES void __iar_leaks_ignore_block(void *block);

/* set the new request limit value, returns the old limit */
__ATTRIBUTES size_t __iar_set_request_limit(size_t limit);

/* set the new integrity report limit, returns the old value */
__ATTRIBUTES size_t __iar_set_integrity_report_limit(size_t value);

/* the maximum number of integrity violoation that are reported */
extern size_t __iar_debug_maximum_number_of_violations;

/* ****************************************************************

   All functions below, __iar_set_*_value, sets values used
   internally by the debug heap.  They must be called before any heap
   block is allocated. If the internal values are changed after a heap
   block has been allocated the integrity checks will report the block
   as being corrupted.

   In C it is sufficient to call the functions at the start of main.
   In C++ the constructors for static objects can use dynamic memory,
   there is currently no way to handle that. 

   **************************************************************** */

/* set the new guard value, returns the old guard */
__ATTRIBUTES size_t __iar_set_guard_value(size_t guard);

/* set the new unallocated fill value, returns the old value */
__ATTRIBUTES size_t __iar_set_unallocated_fill_value(size_t value);

/* set the new allocated fill value, returns the old value */
__ATTRIBUTES size_t __iar_set_allocated_fill_value(size_t value);

/* set the new freed fill value, returns the old value */
__ATTRIBUTES size_t __iar_set_freed_fill_value(size_t value);

/* set the new initial fill value, returns the old value */
__ATTRIBUTES size_t __iar_set_initial_fill_value(size_t value);


/* ********************************************************************** */
/*              End of __iar_debug_set_*_value functions                  */
/* ********************************************************************** */


/* returns the current heap block counter */
__ATTRIBUTES size_t __iar_get_heap_counter(void);

#ifdef __cplusplus
}  /* end of extern "C" */
#endif

#endif /* MALLOC_280_H */
