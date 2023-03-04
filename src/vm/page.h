#ifndef VM_PAGE_H
#define VM_PAGE_H
#include <hash.h>
#include "filesys/off_t.h"
#include "threads/thread.h"

/* These are the different states/locations a file in our page table can be in */
#define IN_DISK 0
#define IN_SWAP 1
#define ALL_ZEROES 2
#define IN_MEM 3

/* This represents that the entry was not created by a call to mmap */
#define NO_MAPID -1

typedef int mapid_t;

/* This is our supplemental page struct, it keeps
track of all information needed to retrieve a page
of information in swap or file and write it to memory. */
struct page {
    /* In Memory */
    struct frame_entry *frame;
    void *physical_addr;

    /* In Disk */
    struct file *file;
    off_t file_ofs;
    size_t page_read_bytes;
    size_t page_zero_bytes;
    mapid_t mapid;

    /* In Swap*/
    int swap_slot;

    /* Everything */
    void *virtual_addr;
    struct hash_elem hash_elem;
    struct thread *process_reference;
    short memory_flag;    
    bool writable;
};

void init_supplemental_table(struct hash*);
unsigned page_hash(const struct hash_elem *p_, void *aux UNUSED);
bool page_less(const struct hash_elem *a_, const struct hash_elem *b_,
               void *aux UNUSED);

bool load_page(void *fault_addr);
void page_create_zero_entry(void *uaddr, struct frame_entry *frame, bool writable, bool loaded);
void page_create_file_entry(void *uaddr, struct frame_entry *frame, struct file *file, off_t file_ofs,
                             size_t read_bytes, size_t zero_bytes, 
                             bool writable, mapid_t mapid);
struct page *page_fetch(struct thread *t, void *uaddr);
void page_free(struct page *page_entry, bool delete_entry);

void free_thread_pages(void);

#endif  /* vm/page.h */