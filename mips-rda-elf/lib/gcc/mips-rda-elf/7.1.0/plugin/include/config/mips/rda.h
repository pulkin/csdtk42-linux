/* Definitions of MIPS sub target machine for GNU compiler.
   RDA XCPU/XCPU2.  You should include mips.h after this.

   Copyright (C) 2016-2016 Free Software Foundation, Inc.
   Contributed by Klaus Pedersen (klauskpedersen@rdamicro.com).

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#undef MIPS_CPU_STRING_DEFAULT
#define MIPS_CPU_STRING_DEFAULT "xcpu"
#define MIPS_ISA_DEFAULT 1

#define TARGET_OS_CPP_BUILTINS()			\
  do							\
    {							\
      builtin_assert ("system=sde");			\
      builtin_assert ("system=posix");			\
      builtin_define ("__SDE_MIPS__");			\
							\
      if (TARGET_NO_FLOAT) 				\
	builtin_define ("__NO_FLOAT");			\
      else if (TARGET_SOFT_FLOAT_ABI)			\
	builtin_define ("__SOFT_FLOAT");		\
							\
      builtin_assert ("endian=little");			\
      builtin_assert ("cpu=mipsel");			\
    }							\
  while (0)

#undef TARGET_ENDIAN_DEFAULT
#define TARGET_ENDIAN_DEFAULT    0 // MASK_LITTLE_ENDIAN

#undef MULTILIB_DEFAULTS
#define MULTILIB_DEFAULTS { MULTILIB_ENDIAN_DEFAULT, "msoft-float" }

/* We use the MIPS EABI by default.  */
#undef MIPS_ABI_DEFAULT
#define MIPS_ABI_DEFAULT ABI_32

#undef SUBTARGET_CC1_SPEC
#define SUBTARGET_CC1_SPEC "\
	%{mhard-float:%e-mhard-float not supported}	\
	%{msingle-float:%e-msingle-float not supported}	\
	%{ffixed-t3:-ffixed-t4 -ffixed-t5 -ffixed-t6 -ffixed-t7 -ffixed-s2   \
	 -ffixed-s3 -ffixed-s4 -ffixed-s5 -ffixed-s6 -ffixed-s7 -ffixed-fp } \
	%{meb:%ebig endian not supported}		\
	%{march=xcpu:-mel -msoft-float -fshort-wchar }	\
	%{march=xcpu2: \
	 %{O2:-mbranch-cost=0x305 -fipa-pta}  \
	 -mel -mdivide-traps -mno-check-zero-division      \
	 -msoft-float -fshort-wchar }"

#undef SUBTARGET_ASM_SPEC
#define SUBTARGET_ASM_SPEC "\
	%{march=xcpu:--break -msoft-float}	\
	%{march=xcpu2:--trap -msoft-float}"


#undef DEFAULT_SIGNED_CHAR
#define DEFAULT_SIGNED_CHAR 0

/* Using long will always be right for size_t and ptrdiff_t, since
   sizeof(long) must equal sizeof(void *), following from the setting
   of the -mlong64 option.  */
#undef SIZE_TYPE
#define SIZE_TYPE "long unsigned int"
#undef PTRDIFF_TYPE
#define PTRDIFF_TYPE "long int"

/* Force all .init and .fini entries to be 32-bit, not mips16, so that
   in a mixed environment they are all the same mode. The crti.asm and
   crtn.asm files will also be compiled as 32-bit due to the
   -no-mips16 flag in SUBTARGET_ASM_SPEC above. */
#undef CRT_CALL_STATIC_FUNCTION
#define CRT_CALL_STATIC_FUNCTION(SECTION_OP, FUNC) \
   asm (SECTION_OP "\n\
	.set push\n\
	.set nomips16\n\
	jal " USER_LABEL_PREFIX #FUNC "\n\
	.set pop\n\
	" TEXT_SECTION_ASM_OP);
