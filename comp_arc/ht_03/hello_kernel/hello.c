/* hello.c */
#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)
{
    u32 cr0, cr2, cr3;
#ifdef __x86_64__
    __asm__ __volatile__ (
        "mov %%cr0, %%rax\n\t"
        "mov %%eax, %0\n\t"
        "mov %%cr2, %%rax\n\t"
        "mov %%eax, %1\n\t"
        "mov %%cr3, %%rax\n\t"
        "mov %%eax, %2\n\t"
    : "=m" (cr0), "=m" (cr2), "=m" (cr3)
    : /* no input */
    : "%rax"
    );
#elif defined(__i386__)
    __asm__ __volatile__ (
        "mov %%cr0, %%eax\n\t"
        "mov %%eax, %0\n\t"
        "mov %%cr2, %%eax\n\t"
        "mov %%eax, %1\n\t"
        "mov %%cr3, %%eax\n\t"
        "mov %%eax, %2\n\t"
    : "=m" (cr0), "=m" (cr2), "=m" (cr3)
    : /* no input */
    : "%eax"
    );
#endif
    printk(KERN_INFO "cr0 = 0x%8.8X\n", cr0);
    printk(KERN_INFO "cr2 = 0x%8.8X\n", cr2);
    printk(KERN_INFO "cr3 = 0x%8.8X\n", cr3);
    return 0;
}

void cleanup_module(void)
{
}