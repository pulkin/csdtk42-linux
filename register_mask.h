#ifndef REGISTER_MASK_H
#define REGISTER_MASK_H

// To avoid inclusion on  the asm files of GCC
#ifndef L_m16addsf3
#ifndef L_m16subsf3
#ifndef L_m16mulsf3
#ifndef L_m16divsf3
#ifndef L_m16eqsf2
#ifndef L_m16nesf2
#ifndef L_m16gtsf2
#ifndef L_m16gesf2
#ifndef L_m16lesf2
#ifndef L_m16ltsf2
#ifndef L_m16fltsisf
#ifndef L_m16fixsfsi
#ifndef L_m16adddf3
#ifndef L_m16subdf3
#ifndef L_m16muldf3
#ifndef L_m16divdf3
#ifndef L_m16extsfdf2
#ifndef L_m16trdfsf2
#ifndef L_m16eqdf2
#ifndef L_m16nedf2
#ifndef L_m16gtdf2
#ifndef L_m16gedf2
#ifndef L_m16ledf2
#ifndef L_m16ltdf2
#ifndef L_m16fltsidf
#ifndef L_m16fixdfsi
#ifndef L_m16retsf
#ifndef L_m16retdf
#ifndef L_m16stub1
#ifndef L_m16stub2
#ifndef L_m16stub5
#ifndef L_m16stub6
#ifndef L_m16stub9
#ifndef L_m16stub10
#ifndef L_m16stubsf0
#ifndef L_m16stubsf1
#ifndef L_m16stubsf2
#ifndef L_m16stubsf5
#ifndef L_m16stubsf6
#ifndef L_m16stubsf9
#ifndef L_m16stubsf10
#ifndef L_m16stubdf0
#ifndef L_m16stubdf1
#ifndef L_m16stubdf2
#ifndef L_m16stubdf5
#ifndef L_m16stubdf6
#ifndef L_m16stubdf9
#ifndef L_m16stubdf10





/*
 * These registers will not be used by GCC to 
 * output mips32 code, when WITH_REDUCED_REGS 
 * is set.
 * **** DO NOT CHANGE THIS WITHOUT MODIFYING
 * **** THE IRQ HANDLER ACCORDINGLY !
 */

//#ifdef __REDUCED_REGS__
/* 
* t-registers are also used as pseudo stack 
* for register save/restore in mips16
* 3 registers seem enough for most functions 
*/
   
/*
register unsigned long masked_t0 asm("$8");
register unsigned long masked_t1 asm("$9");
register unsigned long masked_t2 asm("$10");
*/

register unsigned long masked_t3 asm("$11");
register unsigned long masked_t4 asm("$12");
register unsigned long masked_t5 asm("$13");
register unsigned long masked_t6 asm("$14");
register unsigned long masked_t7 asm("$15");

/* t9 ($25) has a specific meaning from GCC's point-of-view
 * (PIC_FN_REGISTER, used in abi calls) and is used explicitly by
 * ct-gcc for mips16 compatible sibling calls in 32 bits,
 * therefore IT MUST NOT BE MASKED (if it is compilation will fail
 * anyway)
 */

register unsigned long masked_s2 asm("$18");
register unsigned long masked_s3 asm("$19");
register unsigned long masked_s4 asm("$20");
register unsigned long masked_s5 asm("$21");
register unsigned long masked_s6 asm("$22");
register unsigned long masked_s7 asm("$23");

register unsigned long masked_fp asm("$30");
//#endif





#endif // L_m16stubdf10
#endif // L_m16stubdf9
#endif // L_m16stubdf6
#endif // L_m16stubdf5 
#endif // L_m16stubdf2 
#endif // L_m16stubdf1 
#endif // L_m16stubdf0 
#endif // L_m16stubsf10
#endif // L_m16stubsf9
#endif // L_m16stubsf6
#endif // L_m16stubsf5
#endif // L_m16stubsf2
#endif // L_m16stubsf1
#endif // L_m16stubsf0
#endif // L_m16stub10
#endif // L_m16stub9
#endif // L_m16stub6
#endif // L_m16stub5
#endif // L_m16stub2
#endif // L_m16stub1
#endif // L_m16retdf
#endif // L_m16retsf
#endif // L_m16fixdfsi
#endif // L_m16fltsidf
#endif // L_m16ltdf2
#endif // L_m16ledf2
#endif // L_m16gedf2
#endif // L_m16gtdf2
#endif // L_m16nedf2
#endif // L_m16eqdf2
#endif // L_m16trdfsf2
#endif // L_m16extsfdf2
#endif // L_m16divdf3
#endif // L_m16muldf3
#endif // L_m16subdf3
#endif // L_m16adddf3
#endif // L_m16fixsfsi
#endif // L_m16fltsisf
#endif // L_m16ltsf2
#endif // L_m16lesf2
#endif // L_m16gesf2
#endif // L_m16gtsf2
#endif // L_m16nesf2
#endif // L_m16eqsf2
#endif // L_m16divsf3
#endif // L_m16mulsf3
#endif // L_m16addsf3
#endif // L_m16subsf3

#endif /* REGISTER_MASK_H */
