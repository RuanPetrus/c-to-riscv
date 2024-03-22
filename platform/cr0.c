void _enter(void)  __attribute__ ((naked, section(".text.metal.init.enter")));
void _enter(void)   {
    // Setup SP and GP
    // The locations are defined in the linker script
    __asm__ volatile  (
					   ".option push;"
					   ".option norelax;"
					   "la    gp, __global_pointer$;"
					   ".option pop;"
					   "la    sp, __stack_pointer$;"
					   "jal   zero, _start;"
					   : /* output: none %0 */
					   : /* input: none */
					   : /* clobbers: none */); 
    // This point will not be executed, _start() will be called with no return.
}

// At this point we have a stack and global pointer, but no access to global variables.

extern int main(void);
extern unsigned int __bss_start;
extern unsigned int __bss_end;

extern void _start(void) __attribute__ ((noreturn));
void _start(void) {

    // Init memory regions
    // Clear the .bss section (global variables with no initial values)
	unsigned char *bss = (unsigned char *) &__bss_start;
	unsigned char *bss_end = (unsigned char *) &__bss_end;
	while(bss != bss_end) *(bss++) = 0;
    int rc = main();
    _Exit(rc);
}

void _Exit(int exit_code) __attribute__ ((noreturn,noinline));
void _Exit(int exit_code) {
    (void)exit_code;
    // Halt
    while (1) {}
}
