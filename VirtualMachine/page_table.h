#define _BITSET(x)	((1) << (x))
#define CR4_PAE_BIT	 _BITSET(5)



#define _PAGE_PRESENT   0x001
#define _PAGE_RW    0x002
#define _PAGE_USER  0x004
#define _PAGE_PWT   0x008
#define _PAGE_PCD   0x010
#define _PAGE_ACCESSED  0x020
#define _PAGE_DIRTY 0x040
#define _PAGE_PSE   0x080       /* 4 MB (or 2MB) page, Pentium+, if present.. */
#define _PAGE_GLOBAL    0x100   /* Global TLB entry PPro+ */
#define _PAGE_UNUSED1   0x200   /* available for programmer */
#define _PAGE_UNUSED2   0x400
#define _PAGE_UNUSED3   0x800

#define PTE_WRITE		(1UL<<51)		 /* same as DBM (51) */
#define PTE_DIRTY		(1UL<<55)
#define PTE_SPECIAL		(1UL<<56)
#define PTE_DEVMAP		(1UL<<57)
#define PTE_PROT_NONE	(1UL<<58)        /* only when !PTE_VALID */

#define CR0_PE          (1UL<<0) /* Protection Enable */
#define CR0_MP          (1UL<<1) /* Monitor Coprocessor */
#define CR0_EM          (1UL<<2) /* Emulation */
#define CR0_TS          (1UL<<3) /* Task Switched */
#define CR0_ET          (1UL<<4) /* Extension Type */
#define CR0_NE          (1UL<<5) /* Numeric Error */
#define CR0_WP          (1UL<<16) /* Write Protect */
#define CR0_AM          (1UL<<18) /* Alignment Mask */
#define CR0_NW          (1UL<<29) /* Not Write-through */
#define CR0_CD          (1UL<<30) /* Cache Disable */
#define CR0_PG          (1UL<<31) /* Paging */

#define EFER_SCE        _BITSET(0)  /* System Call Extensions */
#define EFER_LME        _BITSET(8)  /* Long Mode Enable */
#define EFER_LMA        _BITSET(10) /* Long Mode Active */

typedef unsigned long pgd_t;
typedef unsigned long pmd_t;
typedef unsigned long pte_t;

/*

like generic x86 arch:

    pgd = pml4t
    pud = pdpt
    pmd = pdt
    pte = pt

*/