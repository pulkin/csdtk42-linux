/* This is the header file of the C interface of the Parma Polyhedra Library.
   Copyright (C) 2001-2009 Roberto Bagnara <bagnara@cs.unipr.it>

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The PPL is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://www.cs.unipr.it/ppl/ . */

#ifndef PPL_ppl_c_h
#define PPL_ppl_c_h 1

/*!
  \defgroup PPL_C_interface C Language Interface

  The Parma Polyhedra Library comes equipped with an interface
  for the C language.
*/

/*! \mainpage

All the declarations needed for using the PPL's C interface
(preprocessor symbols, data types, variables and
functions) are collected in the header file <CODE>ppl_c.h</CODE>.
This file, which is designed to work with pre-ANSI and ANSI C compilers
as well as C99 and C++ compilers, should be included, either directly
or via some other header file, with the directive
\code
#include <ppl_c.h>
\endcode
If this directive does not work, then your compiler is unable to find
the file <CODE>ppl_c.h</CODE>. So check that the library is installed
(if it is not installed, you may want to <CODE>make install</CODE>,
perhaps with root privileges) in the right place
(if not you may want to reconfigure the library using the appropriate
pathname for the <CODE>--prefix</CODE> option); and that your compiler
knows where it is installed (if not you should add the path to the
directory where <CODE>ppl_c.h</CODE> is located to the compiler's
include file search path; this is usually done with the
<CODE>-I</CODE> option).

The name space of the PPL's C interface is <CODE>PPL_*</CODE> for
preprocessor symbols, enumeration values and variables; and
<CODE>ppl_*</CODE> for data types and function names. The interface
systematically uses <EM>opaque data types</EM> (generic pointers that
completely hide the internal representations from the client code) and
provides all required access functions. By using just the interface,
the client code can exploit all the functionalities of the library yet
avoid directly manipulating the library's data structures.  The advantages
are that (1) applications do not depend on the internals of the library
(these may change from release to release), and (2) the interface
invariants can be thoroughly checked (by the access functions).

The PPL's C interface is initialized by means of the
<CODE>ppl_initialize</CODE> function.  This function must
be called <EM>before using any other interface of the library</EM>.
The application can release the resources allocated by the library by
calling the <CODE>ppl_finalize</CODE> function. After this function
is called <EM>no other interface of the library may be used</EM>
until the interface is re-initialized using <CODE>ppl_initialize</CODE>.

Any application using the PPL should make sure that only the
intended version(s) of the library are ever used.  The version used can be
checked at compile-time thanks to the macros PPL_VERSION_MAJOR,
PPL_VERSION_MINOR, PPL_VERSION_REVISION and PPL_VERSION_BETA, which
give, respectively major, minor, revision and beta numbers of the PPL
version.  This is an example of their use:
\code
#if PPL_VERSION_MAJOR == 0 && PPL_VERSION_MINOR < 6
# error "PPL version 0.6 or following is required"
#endif
\endcode
Compile-time checking, however, is not normally enough, particularly in
an environment where there is dynamic linking.  Run-time checking can
be performed by means of the functions <CODE>ppl_version_major</CODE>,
<CODE>ppl_version_minor</CODE>, <CODE>ppl_version_revision</CODE>, and
<CODE>ppl_version_beta</CODE>. The PPL's C interface also provides
functions <CODE>ppl_version</CODE>, returning character string
containing the full version number, and <CODE>ppl_banner</CODE>,
returning a string that, in addition, provides (pointers to) other
useful information for the library user.

All programs using the PPL's C interface must link with the
following libraries: <CODE>libppl_c</CODE> (PPL's C interface),
<CODE>libppl</CODE> (PPL's core), <CODE>libgmpxx</CODE> (GMP's C++
interface), and <CODE>libgmp</CODE> (GMP's library core).  On most
Unix-like systems, this is done by adding <CODE>-lppl_c</CODE>,
<CODE>-lppl</CODE>, <CODE>-lgmpxx</CODE>, and <CODE>-lgmp</CODE> to
the compiler's or linker's command line.  For example:
\verbatim
gcc myprogram.o -lppl_c -lppl -lgmpxx -lgmp
\endverbatim
If this does not work, it means that your compiler/linker is not
finding the libraries where it expects.  Again, this could be because you
forgot to install the library or you installed it in a non-standard
location.  In the latter case you will need to use the appropriate
options (usually <CODE>-L</CODE>) and, if you use shared libraries,
some sort of run-time path selection mechanisms.  Consult your
compiler's documentation for details.  Notice that the PPL is built
using <A HREF="http://www.gnu.org/software/libtool/">Libtool</A> and
an application can exploit this fact to significantly simplify the
linking phase.  See Libtool's documentation for details.  Those
working under Linux can find a lot of useful information on how to use
program libraries (including static, shared, and dynamically loaded
libraries) in the
<A HREF="http://www.dwheeler.com/program-library/">Program Library
HOWTO</A>.

For examples on how to use the functions provided by the C interface,
you are referred to the directory <CODE>demos/ppl_lpsol/</CODE> in
the source distribution.  It contains a <EM>Mixed Integer (Linear)
Programming</EM> solver written in C.  In order to use this solver
you will need to install
<A HREF="http://www.gnu.org/software/glpk/">GLPK</A> (the GNU Linear
Programming Kit): this is used to read linear programs in MPS format.
*/ /* \mainpage */

/*
  For some reason, GMP up to and including version 4.1.3 requires
  <stdio.h> to be included before <gmp.h>.
*/

#include <stdio.h>
#include <gmp.h>
#include <stddef.h>

/*
  PPL_PROTO is a macro used to wrap function prototypes, so that
  compilers that don't understand ANSI C prototypes still work, and
  ANSI C compilers can issue warnings about type mismatches.
*/
#if defined(__STDC__)                              \
  || defined(__cplusplus)                          \
  || defined (_AIX)                                \
  || (defined (__mips) && defined (_SYSTYPE_SVR4)) \
  || defined(_WIN32)
# define PPL_PROTO(protos) protos
#else
# define PPL_PROTO(protos) ()
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*! \defgroup Init Library Initialization and Finalization
  Functions for initialization/finalization of the library,
  as well as setting/resetting of floating-point rounding mode.
*/
/*@{*/

/*! \brief
  Initializes the Parma Polyhedra Library.
  This function must be called before any other function.

  \return
  <CODE>PPL_ERROR_INVALID_ARGUMENT</CODE> if the library
  was already initialized.
*/
int
ppl_initialize PPL_PROTO((void));

/*! \brief
  Finalizes the Parma Polyhedra Library.
  This function must be called after any other function.

  \return
  <CODE>PPL_ERROR_INVALID_ARGUMENT</CODE> if the library
  was already finalized.
*/
int
ppl_finalize PPL_PROTO((void));

/*! \brief
  Sets the FPU rounding mode so that the PPL abstractions based on
  floating point numbers work correctly.

  This is performed automatically at initialization-time.  Calling
  this function is needed only if restore_pre_PPL_rounding() has been
  previously called.
*/
int
ppl_set_rounding_for_PPL PPL_PROTO((void));

/*! \brief
  Sets the FPU rounding mode as it was before initialization of the PPL.

  After calling this function it is absolutely necessary to call
  set_rounding_for_PPL() before using any PPL abstractions based on
  floating point numbers.
  This is performed automatically at finalization-time.
*/
int
ppl_restore_pre_PPL_rounding PPL_PROTO((void));

/*@}*/ /* Init */

/*! \defgroup Version Version Checking
  Symbolic constants and functions related to library version checking.
*/
/*@{*/

/* Automatically generated from PPL source file ../../interfaces/C/ppl_c_version.h line 1. */
/* Header file providing version information for the C interface.  -*- C -*-
*/

/*! \brief
  A string containing the PPL version.

  \ingroup Version
  Let <CODE>M</CODE> and <CODE>m</CODE> denote the numbers associated
  to PPL_VERSION_MAJOR and PPL_VERSION_MINOR, respectively.  The
  format of PPL_VERSION is <CODE>M "." m</CODE> if both
  PPL_VERSION_REVISION (<CODE>r</CODE>) and PPL_VERSION_BETA
  (<CODE>b</CODE>)are zero, <CODE>M "." m "pre" b</CODE> if
  PPL_VERSION_REVISION is zero and PPL_VERSION_BETA is not zero,
  <CODE>M "." m "." r</CODE> if PPL_VERSION_REVISION is not zero and
  PPL_VERSION_BETA is zero, <CODE>M "." m "." r "pre" b</CODE> if
  neither PPL_VERSION_REVISION nor PPL_VERSION_BETA are zero.
*/
#define PPL_VERSION "0.10.2"

/*! \brief
  The major number of the PPL version.
  \ingroup Version
*/
#define PPL_VERSION_MAJOR 0

/*! \brief
  The minor number of the PPL version.
  \ingroup Version
*/
#define PPL_VERSION_MINOR 10

/*! \brief
  The revision number of the PPL version.
  \ingroup Version
*/
#define PPL_VERSION_REVISION 2

/*! \brief
  The beta number of the PPL version.  This is zero for official
  releases and nonzero for development snapshots.
  \ingroup Version
*/
#define PPL_VERSION_BETA 0

/*! \brief
  Returns the major number of the PPL version.
*/
int
ppl_version_major PPL_PROTO((void));

/*! \brief
  Returns the minor number of the PPL version.
*/
int
ppl_version_minor PPL_PROTO((void));

/*! \brief
  Returns the revision number of the PPL version.
*/
int
ppl_version_revision PPL_PROTO((void));

/*! \brief
  Returns the beta number of the PPL version.
*/
int
ppl_version_beta PPL_PROTO((void));

/*! \brief
  Writes to \c *p a pointer to a character string containing the
  PPL version.
*/
int
ppl_version PPL_PROTO((const char** p));

/*! \brief
  Writes to \c *p a pointer to a character string containing the PPL banner.

  The banner provides information about the PPL version, the licensing,
  the lack of any warranty whatsoever, the C++ compiler used to build
  the library, where to report bugs and where to look for further
  information.
*/
int
ppl_banner PPL_PROTO((const char** p));

/*@}*/ /* Version Checking */

/*! \defgroup Error Error Handling
  Symbolic constants and functions related to error reporting/handling.
*/
/*@{*/

/*! \brief
  Defines the error codes that any function may return.
*/
enum ppl_enum_error_code {
  /*! \hideinitializer
    The virtual memory available to the process has been exhausted. */
  PPL_ERROR_OUT_OF_MEMORY = -2,
  /*! \hideinitializer
    A function has been invoked with an invalid argument. */
  PPL_ERROR_INVALID_ARGUMENT = -3,
  /*! \hideinitializer
    A function has been invoked outside its domain of definition. */
  PPL_ERROR_DOMAIN_ERROR = -4,
  /*! \hideinitializer
    The construction of an object that would exceed its maximum
    permitted size was attempted. */
  PPL_ERROR_LENGTH_ERROR = -5,
  /*! \hideinitializer
    An arithmetic overflow occurred and the computation was consequently
    interrupted.  This can <EM>only</EM> happen in library's incarnations
    using bounded integers as coefficients. */
  PPL_ARITHMETIC_OVERFLOW = -6,
  /*! \hideinitializer
    An error occurred during a C input/output operation.  A more
    precise indication of what went wrong is available via
    <CODE>errno</CODE>. */
  PPL_STDIO_ERROR = -7,
  /*! \hideinitializer
    An internal error that was diagnosed by the PPL itself.
    This indicates a bug in the PPL. */
  PPL_ERROR_INTERNAL_ERROR = -8,
  /*! \hideinitializer
    A standard exception has been raised by the C++ run-time environment.
    This indicates a bug in the PPL. */
  PPL_ERROR_UNKNOWN_STANDARD_EXCEPTION = -9,
  /*! \hideinitializer
    A totally unknown, totally unexpected error happened.
    This indicates a bug in the PPL. */
  PPL_ERROR_UNEXPECTED_ERROR = -10
};

/*! \brief
  Installs the user-defined error handler pointed at by \p h.

  The error handler takes an error code and a textual description that
  gives further information about the actual error.  The C string
  containing the textual description is read-only and its existence is
  not guaranteed after the handler has returned.
*/
int
ppl_set_error_handler PPL_PROTO((void (*h)(enum ppl_enum_error_code code,
					   const char* description)));

/*@}*/ /* Error */

/*! \defgroup Datatypes Library Datatypes
  \brief
  Typedefs for the library datatypes and related symbolic constants.

  The datatypes provided by the library should be manipulated
  by means of the corresponding opaque pointer types and
  the functions working on them.

  \note
  To simplify the detection of common programming mistakes,
  we provide both pointer-to-const and pointer-to-nonconst
  opaque pointers, with implicit conversions mapping each
  pointer-to-nonconst to the corresponding pointer-to-const when needed.
  The user of the C interface is therefore recommended to adopt
  the pointer-to-const type whenever read-only access is meant.
*/
/*@{*/

/*! \brief
  An unsigned integral type for representing space dimensions.
*/
typedef size_t ppl_dimension_type;

/*! \brief
  Writes to \p m the maximum space dimension this library can handle.
*/
int
ppl_max_space_dimension PPL_PROTO((ppl_dimension_type* m));

/*! \brief
  Writes to \p m a value that does not designate a valid dimension.
*/
int
ppl_not_a_dimension PPL_PROTO((ppl_dimension_type* m));

/*! \brief
  Pretty-prints \p var to <CODE>stdout</CODE>.
*/
int
ppl_io_print_variable PPL_PROTO((ppl_dimension_type var));

/*! \brief
  Pretty-prints \p var to the given output \p stream.
*/
int
ppl_io_fprint_variable PPL_PROTO((FILE* stream, ppl_dimension_type var));

/*! \brief
  Pretty-prints \p var to a malloc-allocated string, a pointer to which
  is returned via \p strp.
*/
int
ppl_io_asprint_variable PPL_PROTO((char** strp, ppl_dimension_type var));

/*! \brief
  The type of output functions used for printing variables.

  An output function for variables must write a textual representation
  for \p var to a character buffer, null-terminate it, and return a
  pointer to the beginning of the buffer.  In case the operation fails,
  0 should be returned and perhaps <CODE>errno</CODE> should be set
  in a meaningful way.  The library does nothing with the buffer, besides
  printing its contents.
*/
typedef const char*
ppl_io_variable_output_function_type(ppl_dimension_type var);

/*! \brief
  Sets the output function to be used for printing variables to \p p.
*/
int
ppl_io_set_variable_output_function(ppl_io_variable_output_function_type* p);

/*! \brief
  Writes a pointer to the current variable output function to \p pp.
*/
int
ppl_io_get_variable_output_function(ppl_io_variable_output_function_type** pp);

/*! \brief Utility function for the wrapping of lines of text.

  \param src
  The source string holding the text to wrap.

  \param indent_depth
  The indentation depth.

  \param preferred_first_line_length
  The preferred length for the first line of text.

  \param preferred_line_length
  The preferred length for all the lines but the first one.

  \return
  The wrapped string in a malloc-allocated buffer.
*/
char*
ppl_io_wrap_string(const char* src,
		   unsigned indent_depth,
		   unsigned preferred_first_line_length,
		   unsigned preferred_line_length);

/*@}*/ /* Datatypes */

#undef PPL_TYPE_DECLARATION

#define PPL_TYPE_DECLARATION(Type)                                    \
/*! \brief Opaque pointer \ingroup Datatypes */                       \
typedef struct ppl_##Type##_tag* ppl_##Type##_t;                      \
/*! \brief Opaque pointer to const object \ingroup Datatypes */       \
typedef struct ppl_##Type##_tag const* ppl_const_##Type##_t;

/*! \interface ppl_Coefficient_tag
  \brief
  Types and functions for coefficients.

  The types and functions for coefficients provide an interface towards
  \extref{Parma_Polyhedra_Library::Coefficient, Coefficient}.
  Depending on configuration, the PPL coefficients may be implemented
  by the unbounded precision integers provided by GMP (default),
  or by bounded precision integers (with checks for overflows).
*/
PPL_TYPE_DECLARATION(Coefficient)

/*! \interface ppl_Linear_Expression_tag
  \brief
  Types and functions for linear expressions.

  The types and functions for linear expression provide an interface towards
  \extref{Parma_Polyhedra_Library::Linear_Expression, Linear_Expression}.
*/
PPL_TYPE_DECLARATION(Linear_Expression)

/*! \interface ppl_Constraint_tag
  \brief
  Types and functions for constraints.

  The types and functions for constraints provide an interface towards
  \extref{Parma_Polyhedra_Library::Constraint, Constraint}.
*/
PPL_TYPE_DECLARATION(Constraint)

/*! \interface ppl_Constraint_System_tag
  \brief
  Types and functions for constraint systems.

  The types and functions for constraint systems provide an interface
  towards
  \extref{Parma_Polyhedra_Library::Constraint_System, Constraint_System}.
*/
PPL_TYPE_DECLARATION(Constraint_System)

/*! \interface ppl_Constraint_System_const_iterator_tag
  \brief
  Types and functions for iterating on constraint systems.

  The types and functions for constraint systems iterators provide
  read-only access to the elements of a constraint system by interfacing
  \extref{Parma_Polyhedra_Library::Constraint_System::const_iterator,
  Constraint_System::const_iterator}.
*/
PPL_TYPE_DECLARATION(Constraint_System_const_iterator)

/*! \interface ppl_Generator_tag
  \brief
  Types and functions for generators.

  The types and functions for generators provide an interface
  towards \extref{Parma_Polyhedra_Library::Generator, Generator}.
*/
PPL_TYPE_DECLARATION(Generator)

/*! \interface ppl_Generator_System_tag
  \brief
  Types and functions for generator systems.

  The types and functions for generator systems provide an interface
  towards
  \extref{Parma_Polyhedra_Library::Generator_System, Generator_System}.
*/
PPL_TYPE_DECLARATION(Generator_System)

/*! \interface ppl_Generator_System_const_iterator_tag
  \brief
  Types and functions for iterating on generator systems.

  The types and functions for generator systems iterators provide
  read-only access to the elements of a generator system by interfacing
  \extref{Parma_Polyhedra_Library::Generator_System::const_iterator,
  Generator_System::const_iterator}.
*/
PPL_TYPE_DECLARATION(Generator_System_const_iterator)

/*! \interface ppl_Congruence_tag
  \brief
  Types and functions for congruences.

  The types and functions for congruences provide an interface
  towards \extref{Parma_Polyhedra_Library::Congruence, Congruence}.
*/
PPL_TYPE_DECLARATION(Congruence)

/*! \interface ppl_Congruence_System_tag
  \brief
  Types and functions for congruence systems.

  The types and functions for congruence systems provide an interface
  towards
  \extref{Parma_Polyhedra_Library::Congruence_System, Congruence_System}.
*/
PPL_TYPE_DECLARATION(Congruence_System)

/*! \interface ppl_Congruence_System_const_iterator_tag
  \brief
  Types and functions for iterating on congruence systems.

  The types and functions for congruence systems iterators provide
  read-only access to the elements of a congruence system by interfacing
  \extref{Parma_Polyhedra_Library::Congruence_System::const_iterator,
  Congruence_System::const_iterator}.
*/
PPL_TYPE_DECLARATION(Congruence_System_const_iterator)

/*! \interface ppl_Grid_Generator_tag
  \brief
  Types and functions for grid generators.

  The types and functions for grid generators provide an interface
  towards \extref{Parma_Polyhedra_Library::Grid_Generator, Grid_Generator}.
*/
PPL_TYPE_DECLARATION(Grid_Generator)

/*! \interface ppl_Grid_Generator_System_tag
  \brief
  Types and functions for grid generator systems.

  The types and functions for grid generator systems provide an interface
  towards
  \extref{Parma_Polyhedra_Library::Grid_Generator_System,
  Grid_Generator_System}.
*/
PPL_TYPE_DECLARATION(Grid_Generator_System)

/*! \interface ppl_Grid_Generator_System_const_iterator_tag
  \brief
  Types and functions for iterating on grid generator systems.

  The types and functions for grid generator systems iterators provide
  read-only access to the elements of a grid generator system by interfacing
  \extref{Parma_Polyhedra_Library::Grid_Generator_System::const_iterator,
  Grid_Generator_System::const_iterator}.
*/
PPL_TYPE_DECLARATION(Grid_Generator_System_const_iterator)

/*! \interface ppl_MIP_Problem_tag
  \brief
  Types and functions for MIP problems.

  The types and functions for MIP problems provide an interface
  towards \extref{Parma_Polyhedra_Library::MIP_Problem, MIP_Problem}.
*/
PPL_TYPE_DECLARATION(MIP_Problem)


#undef PPL_DECLARE_PRINT_FUNCTIONS
#undef PPL_DECLARE_ASCII_DUMP_LOAD_FUNCTIONS
#undef PPL_DECLARE_IO_FUNCTIONS
#undef PPL_DECLARE_AND_DOCUMENT_PRINT_FUNCTIONS
#undef PPL_DECLARE_AND_DOCUMENT_ASCII_DUMP_LOAD_FUNCTIONS
#undef PPL_DECLARE_AND_DOCUMENT_IO_FUNCTIONS

#define PPL_DECLARE_PRINT_FUNCTIONS(Type)                               \
/*! \relates ppl_##Type##_tag */                                        \
int                                                                     \
ppl_io_print_##Type PPL_PROTO((ppl_const_##Type##_t x));                \
/*! \relates ppl_##Type##_tag */                                        \
int                                                                     \
ppl_io_fprint_##Type PPL_PROTO((FILE* stream, ppl_const_##Type##_t x)); \
/*! \relates ppl_##Type##_tag */                                        \
int                                                                     \
ppl_io_asprint_##Type PPL_PROTO((char** strp, ppl_const_##Type##_t x));

#define PPL_DECLARE_ASCII_DUMP_LOAD_FUNCTIONS(Type) \
/*! \relates ppl_##Type##_tag */                    \
int                                                 \
ppl_##Type##_ascii_dump                             \
PPL_PROTO((ppl_const_##Type##_t x, FILE* stream));  \
/*! \relates ppl_##Type##_tag */                    \
int                                                 \
ppl_##Type##_ascii_load                             \
PPL_PROTO((ppl_##Type##_t x, FILE* stream));

#define PPL_DECLARE_IO_FUNCTIONS(Type)      \
PPL_DECLARE_PRINT_FUNCTIONS(Type)           \
PPL_DECLARE_ASCII_DUMP_LOAD_FUNCTIONS(Type)

#define PPL_DECLARE_AND_DOCUMENT_PRINT_FUNCTIONS(Type)                  \
/*! \relates ppl_##Type##_tag \brief Prints \p x to \c stdout. */       \
int                                                                     \
ppl_io_print_##Type PPL_PROTO((ppl_const_##Type##_t x));                \
/*! \relates ppl_##Type##_tag \brief Prints \p x to the given output \p stream. */ \
int                                                                     \
ppl_io_fprint_##Type PPL_PROTO((FILE* stream, ppl_const_##Type##_t x)); \
/*! \relates ppl_##Type##_tag \brief Prints \p x to a malloc-allocated string, a pointer to which is returned via \p strp. */ \
int                                                                     \
ppl_io_asprint_##Type PPL_PROTO((char** strp, ppl_const_##Type##_t x));


#define PPL_DECLARE_AND_DOCUMENT_ASCII_DUMP_LOAD_FUNCTIONS(Type) \
/*! \relates ppl_##Type##_tag \brief Dumps an ascii representation of \p x on \p stream. */ \
int                                                              \
ppl_##Type##_ascii_dump                                          \
PPL_PROTO((ppl_const_##Type##_t x, FILE* stream));               \
/*! \relates ppl_##Type##_tag \brief Loads an ascii representation of \p x from \p stream. */ \
int                                                              \
ppl_##Type##_ascii_load                                          \
PPL_PROTO((ppl_##Type##_t x, FILE* stream));

#define PPL_DECLARE_AND_DOCUMENT_IO_FUNCTIONS(Type)      \
/*! \brief \name Input/Output Functions */               \
/*@{*/                                                   \
PPL_DECLARE_AND_DOCUMENT_PRINT_FUNCTIONS(Type)           \
PPL_DECLARE_AND_DOCUMENT_ASCII_DUMP_LOAD_FUNCTIONS(Type) \
/*@}*/ /* Input/Output Functions */


/*! \brief \name Constructors, Assignment and Destructor */
/*@{*/

/*! \relates ppl_Coefficient_tag \brief
  Creates a new coefficient with value 0 and writes a handle for the
  newly created coefficient at address \p pc.
*/
int
ppl_new_Coefficient PPL_PROTO((ppl_Coefficient_t* pc));

/*! \relates ppl_Coefficient_tag \brief
  Creates a new coefficient with the value given by the GMP integer
  \p z and writes a handle for the newly created coefficient
  at address \p pc.
*/
int
ppl_new_Coefficient_from_mpz_t PPL_PROTO((ppl_Coefficient_t* pc, mpz_t z));

/*! \relates ppl_Coefficient_tag \brief
  Builds a coefficient that is a copy of \p c; writes a handle
  for the newly created coefficient at address \p pc.
*/
int
ppl_new_Coefficient_from_Coefficient PPL_PROTO((ppl_Coefficient_t* pc,
						ppl_const_Coefficient_t c));

/*! \relates ppl_Coefficient_tag \brief
  Assign to \p dst the value given by the GMP integer \p z.
*/
int
ppl_assign_Coefficient_from_mpz_t PPL_PROTO((ppl_Coefficient_t dst, mpz_t z));

/*! \relates ppl_Coefficient_tag \brief
  Assigns a copy of the coefficient \p src to \p dst.
*/
int
ppl_assign_Coefficient_from_Coefficient
PPL_PROTO((ppl_Coefficient_t dst, ppl_const_Coefficient_t src));

/*! \relates ppl_Coefficient_tag \brief
  Invalidates the handle \p c: this makes sure the corresponding
  resources will eventually be released.
*/
int
ppl_delete_Coefficient PPL_PROTO((ppl_const_Coefficient_t c));

/*@}*/ /* Constructors, Assignment and Destructor */

/*! \brief \name Read-Only Accessor Functions */
/*@{*/

/*! \relates ppl_Coefficient_tag \brief
  Sets the value of the GMP integer \p z to the value of \p c.
*/
int
ppl_Coefficient_to_mpz_t PPL_PROTO((ppl_const_Coefficient_t c, mpz_t z));

/*! \relates ppl_Coefficient_tag \brief
  Returns a positive integer if \p c is well formed, i.e., if it
  satisfies all its implementation invariants; returns 0 and perhaps
  makes some noise if \p c is broken.  Useful for debugging purposes.
*/
int
ppl_Coefficient_OK PPL_PROTO((ppl_const_Coefficient_t c));

/*! \relates ppl_Coefficient_tag \brief
  Returns a positive integer if coefficients are bounded; returns 0
  otherwise.
*/
int
ppl_Coefficient_is_bounded PPL_PROTO((void));

/*! \relates ppl_Coefficient_tag \brief
  Returns a positive integer if coefficients are bounded, in which case
  \p min is set to their minimum value; returns 0 otherwise.
*/
int
ppl_Coefficient_min PPL_PROTO((mpz_t min));

/*! \relates ppl_Coefficient_tag \brief
  Returns a positive integer if coefficients are bounded, in which case
  \p max is set to their maximum value; returns 0 otherwise.
*/
int
ppl_Coefficient_max PPL_PROTO((mpz_t max));

/*@}*/ /* Read-Only Accessor Functions */

/* No ascii dump for Coefficient */
/*! \brief \name I/O Functions */
/*@{*/
PPL_DECLARE_AND_DOCUMENT_PRINT_FUNCTIONS(Coefficient)
/*@}*/ /* I/O Functions */


/*! \brief \name Constructors, Assignment and Destructor */
/*@{*/

/*! \relates ppl_Linear_Expression_tag \brief
  Creates a new linear expression corresponding to the constant 0 in a
  zero-dimensional space; writes a handle for the new linear
  expression at address \p ple.
*/
int
ppl_new_Linear_Expression PPL_PROTO((ppl_Linear_Expression_t* ple));

/*! \relates ppl_Linear_Expression_tag \brief
  Creates a new linear expression corresponding to the constant 0 in a
  <TT>d</TT>-dimensional space; writes a handle for the new linear
  expression at address \p ple.
*/
int
ppl_new_Linear_Expression_with_dimension
PPL_PROTO((ppl_Linear_Expression_t* ple, ppl_dimension_type d));

/*! \relates ppl_Linear_Expression_tag \brief
  Builds a linear expression that is a copy of \p le; writes a handle
  for the newly created linear expression at address \p ple.
*/
int
ppl_new_Linear_Expression_from_Linear_Expression
PPL_PROTO((ppl_Linear_Expression_t* ple, ppl_const_Linear_Expression_t le));

/*! \relates ppl_Linear_Expression_tag \brief
  Builds a linear expression corresponding to constraint \p c;
  writes a handle for the newly created linear expression at address \p ple.
*/
int
ppl_new_Linear_Expression_from_Constraint
PPL_PROTO((ppl_Linear_Expression_t* ple, ppl_const_Constraint_t c));

/*! \relates ppl_Linear_Expression_tag \brief
  Builds a linear expression corresponding to generator \p g;
  writes a handle for the newly created linear expression at address \p ple.
*/
int
ppl_new_Linear_Expression_from_Generator
PPL_PROTO((ppl_Linear_Expression_t* ple, ppl_const_Generator_t g));

/*! \relates ppl_Linear_Expression_tag \brief
  Builds a linear expression corresponding to congruence \p c;
  writes a handle for the newly created linear expression at address \p ple.
*/
int
ppl_new_Linear_Expression_from_Congruence
PPL_PROTO((ppl_Linear_Expression_t* ple, ppl_const_Congruence_t c));

/*! \relates ppl_Linear_Expression_tag \brief
  Builds a linear expression corresponding to grid generator \p g;
  writes a handle for the newly created linear expression at address \p ple.
*/
int
ppl_new_Linear_Expression_from_Grid_Generator
PPL_PROTO((ppl_Linear_Expression_t* ple, ppl_const_Grid_Generator_t g));

/*! \relates ppl_Linear_Expression_tag \brief
  Assigns a copy of the linear expression \p src to \p dst.
*/
int
ppl_assign_Linear_Expression_from_Linear_Expression
PPL_PROTO((ppl_Linear_Expression_t dst, ppl_const_Linear_Expression_t src));

/*! \relates ppl_Linear_Expression_tag \brief
  Invalidates the handle \p le: this makes sure the corresponding
  resources will eventually be released.
*/
int
ppl_delete_Linear_Expression PPL_PROTO((ppl_const_Linear_Expression_t le));

/*@}*/ /* Constructors, Assignment and Destructor */

/*! \brief \name Functions that Do Not Modify the Linear Expression */
/*@{*/

/*! \relates ppl_Linear_Expression_tag \brief
  Writes to \p m the space dimension of \p le.
*/
int
ppl_Linear_Expression_space_dimension
PPL_PROTO((ppl_const_Linear_Expression_t le, ppl_dimension_type* m));

/*! \relates ppl_Linear_Expression_tag \brief
  Copies into \p n the coefficient of variable \p var in
  the linear expression \p le.
*/
int
ppl_Linear_Expression_coefficient PPL_PROTO((ppl_const_Linear_Expression_t le,
					     ppl_dimension_type var,
					     ppl_Coefficient_t n));

/*! \relates ppl_Linear_Expression_tag \brief
  Copies into \p n the inhomogeneous term of linear expression \p le.
*/
int
ppl_Linear_Expression_inhomogeneous_term
PPL_PROTO((ppl_const_Linear_Expression_t le, ppl_Coefficient_t n));

/*! \relates ppl_Linear_Expression_tag \brief
  Returns a positive integer if \p le is well formed, i.e., if it
  satisfies all its implementation invariants; returns 0 and perhaps
  makes some noise if \p le is broken.  Useful for debugging purposes.
*/
int
ppl_Linear_Expression_OK PPL_PROTO((ppl_const_Linear_Expression_t le));

/*@}*/ /* Functions that Do Not Modify the Linear Expression */

/*! \brief \name Functions that May Modify the Linear Expression */
/*@{*/

/*! \relates ppl_Linear_Expression_tag \brief
  Adds \p n to the coefficient of variable \p var in the linear
  expression \p le.  The space dimension is set to be the maximum
  between \p var + 1 and the old space dimension.
*/
int
ppl_Linear_Expression_add_to_coefficient
PPL_PROTO((ppl_Linear_Expression_t le,
	   ppl_dimension_type var,
	   ppl_const_Coefficient_t n));

/*! \relates ppl_Linear_Expression_tag \brief
  Adds \p n to the inhomogeneous term of the linear expression \p le.
*/
int
ppl_Linear_Expression_add_to_inhomogeneous
PPL_PROTO((ppl_Linear_Expression_t le, ppl_const_Coefficient_t n));

/*! \relates ppl_Linear_Expression_tag \brief
  Adds the linear expression \p src to \p dst.
*/
int
ppl_add_Linear_Expression_to_Linear_Expression
PPL_PROTO((ppl_Linear_Expression_t dst, ppl_const_Linear_Expression_t src));

/*! \relates ppl_Linear_Expression_tag \brief
  Subtracts the linear expression \p src from \p dst.
*/
int
ppl_subtract_Linear_Expression_from_Linear_Expression
PPL_PROTO((ppl_Linear_Expression_t dst, ppl_const_Linear_Expression_t src));

/*! \relates ppl_Linear_Expression_tag \brief
  Multiply the linear expression \p dst by \p n.
*/
int
ppl_multiply_Linear_Expression_by_Coefficient
PPL_PROTO((ppl_Linear_Expression_t le, ppl_const_Coefficient_t n));

/*@}*/ /* Functions that May Modify the Linear Expression */

PPL_DECLARE_AND_DOCUMENT_IO_FUNCTIONS(Linear_Expression)

/*! \brief \ingroup Datatypes
  Describes the relations represented by a constraint.
*/
enum ppl_enum_Constraint_Type {
  /*! The constraint is of the form \f$e < 0\f$. */
  PPL_CONSTRAINT_TYPE_LESS_THAN,
  /*! The constraint is of the form \f$e \leq 0\f$. */
  PPL_CONSTRAINT_TYPE_LESS_OR_EQUAL,
  /*! The constraint is of the form \f$e = 0\f$. */
  PPL_CONSTRAINT_TYPE_EQUAL,
  /*! The constraint is of the form \f$e \geq 0\f$. */
  PPL_CONSTRAINT_TYPE_GREATER_OR_EQUAL,
  /*! The constraint is of the form \f$e > 0\f$. */
  PPL_CONSTRAINT_TYPE_GREATER_THAN
};

/*! \brief \name Constructors, Assignment and Destructor */
/*@{*/

/*! \relates ppl_Constraint_tag \brief
  Creates the new constraint `\p le \p rel 0' and writes a handle for
  it at address \p pc.  The space dimension of the new constraint is
  equal to the space dimension of \p le.
*/
int
ppl_new_Constraint PPL_PROTO((ppl_Constraint_t* pc,
			      ppl_const_Linear_Expression_t le,
			      enum ppl_enum_Constraint_Type rel));

/*! \relates ppl_Constraint_tag \brief
  Creates the unsatisfiable (zero-dimension space) constraint \f$0 = 1\f$
  and writes a handle for it at address \p pc.
*/
int
ppl_new_Constraint_zero_dim_false PPL_PROTO((ppl_Constraint_t* pc));

/*! \relates ppl_Constraint_tag \brief
  Creates the true (zero-dimension space) constraint \f$0 \leq 1\f$,
  also known as <EM>positivity constraint</EM>.
  A handle for the newly created constraint is written at address \p pc.
*/
int
ppl_new_Constraint_zero_dim_positivity PPL_PROTO((ppl_Constraint_t* pc));

/*! \relates ppl_Constraint_tag \brief
  Builds a constraint that is a copy of \p c; writes a handle
  for the newly created constraint at address \p pc.
*/
int
ppl_new_Constraint_from_Constraint PPL_PROTO((ppl_Constraint_t* pc,
					      ppl_const_Constraint_t c));

/*! \relates ppl_Constraint_tag \brief
  Assigns a copy of the constraint \p src to \p dst.
*/
int
ppl_assign_Constraint_from_Constraint PPL_PROTO((ppl_Constraint_t dst,
						 ppl_const_Constraint_t src));

/*! \relates ppl_Constraint_tag \brief
  Invalidates the handle \p c: this makes sure the corresponding
  resources will eventually be released.
*/
int
ppl_delete_Constraint PPL_PROTO((ppl_const_Constraint_t c));

/*@}*/ /* Constructors, Assignment and Destructor */

/*! \brief \name Functions that Do Not Modify the Constraint */
/*@{*/

/*! \relates ppl_Constraint_tag \brief
  Writes to \p m the space dimension of \p c.
*/
int
ppl_Constraint_space_dimension PPL_PROTO((ppl_const_Constraint_t c,
					  ppl_dimension_type* m));

/*! \relates ppl_Constraint_tag \brief
  Returns the type of constraint \p c.
*/
int
ppl_Constraint_type PPL_PROTO((ppl_const_Constraint_t c));

/*! \relates ppl_Constraint_tag \brief
  Copies into \p n the coefficient of variable \p var in
  constraint \p c.
*/
int
ppl_Constraint_coefficient PPL_PROTO((ppl_const_Constraint_t c,
				      ppl_dimension_type var,
				      ppl_Coefficient_t n));

/*! \relates ppl_Constraint_tag \brief
  Copies into \p n the inhomogeneous term of constraint \p c.
*/
int
ppl_Constraint_inhomogeneous_term PPL_PROTO((ppl_const_Constraint_t c,
					     ppl_Coefficient_t n));

/*! \relates ppl_Constraint_tag \brief
  Returns a positive integer if \p c is well formed, i.e., if it
  satisfies all its implementation invariants; returns 0 and perhaps
  makes some noise if \p c is broken.  Useful for debugging purposes.
*/
int
ppl_Constraint_OK PPL_PROTO((ppl_const_Constraint_t c));

/*@}*/ /* Functions that Do Not Modify the Constraint */

PPL_DECLARE_AND_DOCUMENT_IO_FUNCTIONS(Constraint)


/*! \brief \name Constructors, Assignment and Destructor */
/*@{*/

/*! \relates ppl_Constraint_System_tag \brief
  Builds an empty system of constraints and writes a handle to it at
  address \p pcs.
*/
int
ppl_new_Constraint_System PPL_PROTO((ppl_Constraint_System_t* pcs));

/*! \relates ppl_Constraint_System_tag \brief
  Builds a zero-dimensional, unsatisfiable constraint system and
  writes a handle to it at address \p pcs.
*/
int
ppl_new_Constraint_System_zero_dim_empty
PPL_PROTO((ppl_Constraint_System_t* pcs));

/*! \relates ppl_Constraint_System_tag \brief
  Builds the singleton constraint system containing only a copy of
  constraint \p c; writes a handle for the newly created system at
  address \p pcs.
*/
int
ppl_new_Constraint_System_from_Constraint
PPL_PROTO((ppl_Constraint_System_t* pcs, ppl_const_Constraint_t c));

/*! \relates ppl_Constraint_System_tag \brief
  Builds a constraint system that is a copy of \p cs; writes a handle
  for the newly created system at address \p pcs.
*/
int
ppl_new_Constraint_System_from_Constraint_System
PPL_PROTO((ppl_Constraint_System_t* pcs, ppl_const_Constraint_System_t cs));

/*! \relates ppl_Constraint_System_tag \brief
  Assigns a copy of the constraint system \p src to \p dst.
*/
int
ppl_assign_Constraint_System_from_Constraint_System
PPL_PROTO((ppl_Constraint_System_t dst, ppl_const_Constraint_System_t src));

/*! \relates ppl_Constraint_System_tag \brief
  Invalidates the handle \p cs: this makes sure the corresponding
  resources will eventually be released.
*/
int
ppl_delete_Constraint_System PPL_PROTO((ppl_const_Constraint_System_t cs));

/*@}*/ /* Constructors, Assignment and Destructor */

/*! \brief \name Functions that Do Not Modify the Constraint System */
/*@{*/

/*! \relates ppl_Constraint_System_tag \brief
  Writes to \p m the dimension of the vector space enclosing \p cs.
*/
int
ppl_Constraint_System_space_dimension
PPL_PROTO((ppl_const_Constraint_System_t cs, ppl_dimension_type* m));

/*! \relates ppl_Constraint_System_tag \brief
  Returns a positive integer if \p cs contains no (non-trivial) constraint;
  returns 0 otherwise.
*/
int
ppl_Constraint_System_empty
PPL_PROTO((ppl_const_Constraint_System_t cs));

/*! \relates ppl_Constraint_System_tag \brief
  Returns a positive integer if \p cs contains any (non-trivial) strict
  inequality; returns 0 otherwise.
*/
int
ppl_Constraint_System_has_strict_inequalities
PPL_PROTO((ppl_const_Constraint_System_t cs));

/*! \relates ppl_Constraint_System_tag \brief
  Assigns to \p cit a const iterator "pointing" to the beginning of
  the constraint system \p cs.
*/
int
ppl_Constraint_System_begin
PPL_PROTO((ppl_const_Constraint_System_t cs,
	   ppl_Constraint_System_const_iterator_t cit));

/*! \relates ppl_Constraint_System_tag \brief
  Assigns to \p cit a const iterator "pointing" past the end of the
  constraint system \p cs.
*/
int
ppl_Constraint_System_end
PPL_PROTO((ppl_const_Constraint_System_t cs,
	   ppl_Constraint_System_const_iterator_t cit));

/*! \relates ppl_Constraint_System_tag \brief
  Returns a positive integer if \p cs is well formed, i.e., if it
  satisfies all its implementation invariants; returns 0 and perhaps
  makes some noise if \p cs is broken.  Useful for debugging purposes.
*/
int
ppl_Constraint_System_OK PPL_PROTO((ppl_const_Constraint_System_t cs));

/*@}*/ /* Functions that Do Not Modify the Constraint System */

/*! \brief \name Functions that May Modify the Constraint System */
/*@{*/

/*! \relates ppl_Constraint_System_tag \brief
  Removes all the constraints from the constraint system \p cs
  and sets its space dimension to 0.
*/
int
ppl_Constraint_System_clear PPL_PROTO((ppl_Constraint_System_t cs));

/*! \relates ppl_Constraint_System_tag \brief
  Inserts a copy of the constraint \p c into \p cs; the space
  dimension is increased, if necessary.
*/
int
ppl_Constraint_System_insert_Constraint PPL_PROTO((ppl_Constraint_System_t cs,
						   ppl_const_Constraint_t c));

/*@}*/ /* Functions that May Modify the Constraint System */

PPL_DECLARE_AND_DOCUMENT_IO_FUNCTIONS(Constraint_System)


/*! \brief \name Constructors, Assignment and Destructor */
/*@{*/

/*! \relates ppl_Constraint_System_const_iterator_tag \brief
  Builds a new `const iterator' and writes a handle to it at address
  \p pcit.
*/
int
ppl_new_Constraint_System_const_iterator
PPL_PROTO((ppl_Constraint_System_const_iterator_t* pcit));

/*! \relates ppl_Constraint_System_const_iterator_tag \brief
  Builds a const iterator that is a copy of \p cit; writes an
  handle for the newly created const iterator at address \p pcit.
*/
int
ppl_new_Constraint_System_const_iterator_from_Constraint_System_const_iterator
PPL_PROTO((ppl_Constraint_System_const_iterator_t* pcit,
	   ppl_const_Constraint_System_const_iterator_t cit));

/*! \relates ppl_Constraint_System_const_iterator_tag \brief
  Assigns a copy of the const iterator \p src to \p dst.
*/
int
ppl_assign_Constraint_System_const_iterator_from_Constraint_System_const_iterator
PPL_PROTO((ppl_Constraint_System_const_iterator_t dst,
	   ppl_const_Constraint_System_const_iterator_t src));

/*! \relates ppl_Constraint_System_const_iterator_tag \brief
  Invalidates the handle \p cit: this makes sure the corresponding
  resources will eventually be released.
*/
int
ppl_delete_Constraint_System_const_iterator
PPL_PROTO((ppl_const_Constraint_System_const_iterator_t cit));

/*@}*/ /* Constructors, Assignment and Destructor */

/*! \brief \name Dereferencing, Incrementing and Equality Testing */
/*@{*/

/*! \relates ppl_Constraint_System_const_iterator_tag \brief
  Dereference \p cit writing a const handle to the resulting
  constraint at address \p pc.
*/
int
ppl_Constraint_System_const_iterator_dereference
PPL_PROTO((ppl_const_Constraint_System_const_iterator_t cit,
	   ppl_const_Constraint_t* pc));

/*! \relates ppl_Constraint_System_const_iterator_tag \brief
  Increment \p cit so that it "points" to the next constraint.
*/
int
ppl_Constraint_System_const_iterator_increment
PPL_PROTO((ppl_Constraint_System_const_iterator_t cit));

/*! \relates ppl_Constraint_System_const_iterator_tag \brief
  Returns a positive integer if the iterators corresponding to \p x and
  \p y are equal; returns 0 if they are different.
*/
int
ppl_Constraint_System_const_iterator_equal_test
PPL_PROTO((ppl_const_Constraint_System_const_iterator_t x,
	   ppl_const_Constraint_System_const_iterator_t y));

/*@}*/ /* Dereferencing, Incrementing and Equality Testing */


/*! \brief \ingroup Datatypes
  Describes the different kinds of generators.
*/
enum ppl_enum_Generator_Type {
  /*! The generator is a line. */
  PPL_GENERATOR_TYPE_LINE,
  /*! The generator is a ray. */
  PPL_GENERATOR_TYPE_RAY,
  /*! The generator is a point. */
  PPL_GENERATOR_TYPE_POINT,
  /*! The generator is a closure point. */
  PPL_GENERATOR_TYPE_CLOSURE_POINT
};

/*! \brief \name Constructors, Assignment and Destructor */
/*@{*/

/*! \relates ppl_Generator_tag \brief
  Creates a new generator of direction \p le and type \p t.  If the
  generator to be created is a point or a closure point, the divisor
  \p d is applied to \p le.  For other types of generators \p d is
  simply disregarded.  A handle for the new generator is written at
  address \p pg.  The space dimension of the new generator is equal to
  the space dimension of \p le.
*/
int
ppl_new_Generator PPL_PROTO((ppl_Generator_t* pg,
			     ppl_const_Linear_Expression_t le,
			     enum ppl_enum_Generator_Type t,
			     ppl_const_Coefficient_t d));

/*! \relates ppl_Generator_tag \brief
  Creates the point that is the origin of the zero-dimensional space
  \f$\Rset^0\f$.  Writes a handle for the new generator at address
  \p pg.
*/
int
ppl_new_Generator_zero_dim_point PPL_PROTO((ppl_Generator_t* pg));

/*! \relates ppl_Generator_tag \brief
  Creates, as a closure point, the point that is the origin of the
  zero-dimensional space \f$\Rset^0\f$.  Writes a handle for the new
  generator at address \p pg.
*/
int
ppl_new_Generator_zero_dim_closure_point PPL_PROTO((ppl_Generator_t* pg));

/*! \relates ppl_Generator_tag \brief
  Builds a generator that is a copy of \p g; writes a handle
  for the newly created generator at address \p pg.
*/
int
ppl_new_Generator_from_Generator PPL_PROTO((ppl_Generator_t* pg,
					    ppl_const_Generator_t g));

/*! \relates ppl_Generator_tag \brief
  Assigns a copy of the generator \p src to \p dst.
*/
int
ppl_assign_Generator_from_Generator PPL_PROTO((ppl_Generator_t dst,
					       ppl_const_Generator_t src));

/*! \relates ppl_Generator_tag \brief
  Invalidates the handle \p g: this makes sure the corresponding
  resources will eventually be released.
*/
int
ppl_delete_Generator PPL_PROTO((ppl_const_Generator_t g));

/*@}*/ /* Constructors, Assignment and Destructor */

/*! \brief \name Functions that Do Not Modify the Generator */
/*@{*/

/*! \relates ppl_Generator_tag \brief
  Writes to \p m the space dimension of \p g.
*/
int
ppl_Generator_space_dimension PPL_PROTO((ppl_const_Generator_t g,
					 ppl_dimension_type* m));

/*! \relates ppl_Generator_tag \brief
  Returns the type of generator \p g.
*/
int
ppl_Generator_type PPL_PROTO((ppl_const_Generator_t g));

/*! \relates ppl_Generator_tag \brief
  Copies into \p n the coefficient of variable \p var in generator \p g.
*/
int
ppl_Generator_coefficient PPL_PROTO((ppl_const_Generator_t g,
				     ppl_dimension_type var,
				     ppl_Coefficient_t n));

/*! \relates ppl_Generator_tag \brief
  If \p g is a point or a closure point assigns its divisor to \p n.
*/
int
ppl_Generator_divisor PPL_PROTO((ppl_const_Generator_t g,
				 ppl_Coefficient_t n));

/*! \relates ppl_Generator_tag \brief
  Returns a positive integer if \p g is well formed, i.e., if it
  satisfies all its implementation invariants; returns 0 and perhaps
  makes some noise if \p g is broken.  Useful for debugging purposes.
*/
int
ppl_Generator_OK PPL_PROTO((ppl_const_Generator_t g));

/*@}*/ /* Functions that Do Not Modify the Generator */

PPL_DECLARE_AND_DOCUMENT_IO_FUNCTIONS(Generator)


/*! \brief \name Constructors, Assignment and Destructor */
/*@{*/

/*! \relates ppl_Generator_System_tag \brief
  Builds an empty system of generators and writes a handle to it at
  address \p pgs.
*/
int
ppl_new_Generator_System PPL_PROTO((ppl_Generator_System_t* pgs));

/*
  Creates the universe zero-dimensional system of generators (i.e.,
  containing the origin only).  Writes a handle to the new system at
  address \p pgs.
*/
int
ppl_new_Generator_System_zero_dim_univ
PPL_PROTO((ppl_Generator_System_t* pgs));

/*! \relates ppl_Generator_System_tag \brief
  Builds the singleton generator system containing only a copy of
  generator \p g; writes a handle for the newly created system at
  address \p pgs.
*/
int
ppl_new_Generator_System_from_Generator PPL_PROTO((ppl_Generator_System_t* pgs,
						   ppl_const_Generator_t g));

/*! \relates ppl_Generator_System_tag \brief
  Builds a generator system that is a copy of \p gs; writes a handle
  for the newly created system at address \p pgs.
*/
int
ppl_new_Generator_System_from_Generator_System
PPL_PROTO((ppl_Generator_System_t* pgs, ppl_const_Generator_System_t gs));

/*! \relates ppl_Generator_System_tag \brief
  Assigns a copy of the generator system \p src to \p dst.
*/
int
ppl_assign_Generator_System_from_Generator_System
PPL_PROTO((ppl_Generator_System_t dst, ppl_const_Generator_System_t src));

/*! \relates ppl_Generator_System_tag \brief
  Invalidates the handle \p gs: this makes sure the corresponding
  resources will eventually be released.
*/
int
ppl_delete_Generator_System PPL_PROTO((ppl_const_Generator_System_t gs));

/*@}*/ /* Constructors, Assignment and Destructor */

/*! \brief \name Functions that Do Not Modify the Generator System */
/*@{*/

/*! \relates ppl_Generator_System_tag \brief
  Writes to \p m the dimension of the vector space enclosing \p gs.
*/
int
ppl_Generator_System_space_dimension
PPL_PROTO((ppl_const_Generator_System_t gs, ppl_dimension_type* m));

/*! \relates ppl_Generator_System_tag \brief
  Returns a positive integer if \p gs contains no generators;
  returns 0 otherwise.
*/
int
ppl_Generator_System_empty
PPL_PROTO((ppl_const_Generator_System_t gs));

/*! \relates ppl_Generator_System_tag \brief
  Assigns to \p git a const iterator "pointing" to the beginning of
  the generator system \p gs.
*/
int
ppl_Generator_System_begin
PPL_PROTO((ppl_const_Generator_System_t gs,
	   ppl_Generator_System_const_iterator_t git));

/*! \relates ppl_Generator_System_tag \brief
  Assigns to \p git a const iterator "pointing" past the end of the
  generator system \p gs.
*/
int
ppl_Generator_System_end
PPL_PROTO((ppl_const_Generator_System_t gs,
	   ppl_Generator_System_const_iterator_t git));

/*! \relates ppl_Generator_System_tag \brief
  Returns a positive integer if \p gs is well formed, i.e., if it
  satisfies all its implementation invariants; returns 0 and perhaps
  makes some noise if \p gs is broken.  Useful for debugging purposes.
*/
int
ppl_Generator_System_OK PPL_PROTO((ppl_const_Generator_System_t gs));

/*@}*/ /* Functions that Do Not Modify the Generator System */

/*! \brief \name Functions that May Modify the Generator System */
/*@{*/

/*! \relates ppl_Generator_System_tag \brief
  Removes all the generators from the generator system \p gs
  and sets its space dimension to 0.
*/
int
ppl_Generator_System_clear PPL_PROTO((ppl_Generator_System_t gs));

/*! \relates ppl_Generator_System_tag \brief
  Inserts a copy of the generator \p g into \p gs; the space
  dimension is increased, if necessary.
*/
int
ppl_Generator_System_insert_Generator PPL_PROTO((ppl_Generator_System_t gs,
						 ppl_const_Generator_t g));

/*@}*/ /* Functions that May Modify the Generator System */

PPL_DECLARE_AND_DOCUMENT_IO_FUNCTIONS(Generator_System)


/*! \brief \name Constructors, Assignment and Destructor */
/*@{*/

/*! \relates ppl_Generator_System_const_iterator_tag \brief
  Builds a new `const iterator' and writes a handle to it at address
  \p pgit.
*/
int
ppl_new_Generator_System_const_iterator
PPL_PROTO((ppl_Generator_System_const_iterator_t* pgit));

/*! \relates ppl_Generator_System_const_iterator_tag \brief
  Builds a const iterator that is a copy of \p git; writes an
  handle for the newly created const iterator at address \p pgit.
*/
int
ppl_new_Generator_System_const_iterator_from_Generator_System_const_iterator
PPL_PROTO((ppl_Generator_System_const_iterator_t* pgit,
	   ppl_const_Generator_System_const_iterator_t git));

/*! \relates ppl_Generator_System_const_iterator_tag \brief
  Assigns a copy of the const iterator \p src to \p dst.
*/
int
ppl_assign_Generator_System_const_iterator_from_Generator_System_const_iterator
PPL_PROTO((ppl_Generator_System_const_iterator_t dst,
	   ppl_const_Generator_System_const_iterator_t src));

/*! \relates ppl_Generator_System_const_iterator_tag \brief
  Invalidates the handle \p git: this makes sure the corresponding
  resources will eventually be released.
*/
int
ppl_delete_Generator_System_const_iterator
PPL_PROTO((ppl_const_Generator_System_const_iterator_t git));

/*@}*/ /* Constructors, Assignment and Destructor */

/*! \brief \name Dereferencing, Incrementing and Equality Testing */
/*@{*/

/*! \relates ppl_Generator_System_const_iterator_tag \brief
  Dereference \p git writing a const handle to the resulting
  generator at address \p pg.
*/
int
ppl_Generator_System_const_iterator_dereference
PPL_PROTO((ppl_const_Generator_System_const_iterator_t git,
	   ppl_const_Generator_t* pg));

/*! \relates ppl_Generator_System_const_iterator_tag \brief
  Increment \p git so that it "points" to the next generator.
*/
int
ppl_Generator_System_const_iterator_increment
PPL_PROTO((ppl_Generator_System_const_iterator_t git));

/*! \relates ppl_Generator_System_const_iterator_tag \brief
  Returns a positive integer if the iterators corresponding to \p x and
  \p y are equal; returns 0 if they are different.
*/
int
ppl_Generator_System_const_iterator_equal_test
PPL_PROTO((ppl_const_Generator_System_const_iterator_t x,
	   ppl_const_Generator_System_const_iterator_t y));

/*@}*/ /* Dereferencing, Incrementing and Equality Testing */


/*! \brief \name Constructors, Assignment and Destructor */
/*@{*/

/*! \relates ppl_Congruence_tag \brief
  Creates the new congruence \f$le = 0 \pmod{m}\f$ and writes a handle for
  it at address \p pc.  The space dimension of the new congruence is
  equal to the space dimension of \p le.
*/
int
ppl_new_Congruence PPL_PROTO((ppl_Congruence_t* pc,
			      ppl_const_Linear_Expression_t le,
			      ppl_const_Coefficient_t m));

/*! \relates ppl_Congruence_tag \brief
  Creates the unsatisfiable (zero-dimension space) congruence
  \f$0 = 1 \pmod{0}\f$ and writes a handle for it at address \p pc.
*/
int
ppl_new_Congruence_zero_dim_false PPL_PROTO((ppl_Congruence_t* pc));

/*! \relates ppl_Congruence_tag \brief
  Creates the true (zero-dimension space) congruence \f$0 = 1 \pmod{1}\f$,
  also known as <EM>integrality congruence</EM>.
  A handle for the newly created congruence is written at address \p pc.
*/
int
ppl_new_Congruence_zero_dim_integrality PPL_PROTO((ppl_Congruence_t* pc));

/*! \relates ppl_Congruence_tag \brief
  Builds a congruence that is a copy of \p c; writes a handle
  for the newly created congruence at address \p pc.
*/
int
ppl_new_Congruence_from_Congruence PPL_PROTO((ppl_Congruence_t* pc,
					      ppl_const_Congruence_t c));

/*! \relates ppl_Congruence_tag \brief
  Assigns a copy of the congruence \p src to \p dst.
*/
int
ppl_assign_Congruence_from_Congruence PPL_PROTO((ppl_Congruence_t dst,
						 ppl_const_Congruence_t src));

/*! \relates ppl_Congruence_tag \brief
  Invalidates the handle \p c: this makes sure the corresponding
  resources will eventually be released.
*/
int
ppl_delete_Congruence PPL_PROTO((ppl_const_Congruence_t c));

/*@}*/ /* Constructors, Assignment and Destructor */

/*! \brief \name Functions that Do Not Modify the Congruence */
/*@{*/

/*! \relates ppl_Congruence_tag \brief
  Writes to \p m the space dimension of \p c.
*/
int
ppl_Congruence_space_dimension PPL_PROTO((ppl_const_Congruence_t c,
					  ppl_dimension_type* m));

/*! \relates ppl_Congruence_tag \brief
  Copies into \p n the coefficient of variable \p var in
  congruence \p c.
*/
int
ppl_Congruence_coefficient PPL_PROTO((ppl_const_Congruence_t c,
				      ppl_dimension_type var,
				      ppl_Coefficient_t n));

/*! \relates ppl_Congruence_tag \brief
  Copies into \p n the inhomogeneous term of congruence \p c.
*/
int
ppl_Congruence_inhomogeneous_term PPL_PROTO((ppl_const_Congruence_t c,
					     ppl_Coefficient_t n));

/*! \relates ppl_Congruence_tag \brief
  Copies into \p m the modulus of congruence \p c.
*/
int
ppl_Congruence_modulus PPL_PROTO((ppl_const_Congruence_t c,
				  ppl_Coefficient_t m));

/*! \relates ppl_Congruence_tag \brief
  Returns a positive integer if \p c is well formed, i.e., if it
  satisfies all its implementation invariants; returns 0 and perhaps
  makes some noise if \p c is broken.  Useful for debugging purposes.
*/
int
ppl_Congruence_OK PPL_PROTO((ppl_const_Congruence_t c));

/*@}*/ /* Functions that Do Not Modify the Congruence */

PPL_DECLARE_AND_DOCUMENT_IO_FUNCTIONS(Congruence)


/*! \brief \name Constructors, Assignment and Destructor */
/*@{*/

/*! \relates ppl_Congruence_System_tag \brief
  Builds an empty system of congruences and writes a handle to it at
  address \p pcs.
*/
int
ppl_new_Congruence_System PPL_PROTO((ppl_Congruence_System_t* pcs));

/*! \relates ppl_Congruence_System_tag \brief
  Builds a zero-dimensional, unsatisfiable congruence system and
  writes a handle to it at address \p pcs.
*/
int
ppl_new_Congruence_System_zero_dim_empty
PPL_PROTO((ppl_Congruence_System_t* pcs));

/*! \relates ppl_Congruence_System_tag \brief
  Builds the singleton congruence system containing only a copy of
  congruence \p c; writes a handle for the newly created system at
  address \p pcs.
*/
int
ppl_new_Congruence_System_from_Congruence
PPL_PROTO((ppl_Congruence_System_t* pcs, ppl_const_Congruence_t c));

/*! \relates ppl_Congruence_System_tag \brief
  Builds a congruence system that is a copy of \p cs; writes a handle
  for the newly created system at address \p pcs.
*/
int
ppl_new_Congruence_System_from_Congruence_System
PPL_PROTO((ppl_Congruence_System_t* pcs, ppl_const_Congruence_System_t cs));

/*! \relates ppl_Congruence_System_tag \brief
  Assigns a copy of the congruence system \p src to \p dst.
*/
int
ppl_assign_Congruence_System_from_Congruence_System
PPL_PROTO((ppl_Congruence_System_t dst, ppl_const_Congruence_System_t src));

/*! \relates ppl_Congruence_System_tag \brief
  Invalidates the handle \p cs: this makes sure the corresponding
  resources will eventually be released.
*/
int
ppl_delete_Congruence_System PPL_PROTO((ppl_const_Congruence_System_t cs));

/*@}*/ /* Constructors, Assignment and Destructor */

/*! \brief \name Functions that Do Not Modify the Congruence System */
/*@{*/

/*! \relates ppl_Congruence_System_tag \brief
  Writes to \p m the dimension of the vector space enclosing \p cs.
*/
int
ppl_Congruence_System_space_dimension
PPL_PROTO((ppl_const_Congruence_System_t cs, ppl_dimension_type* m));

/*! \relates ppl_Congruence_System_tag \brief
  Returns a positive integer if \p cs contains no (non-trivial) congruence;
  returns 0 otherwise.
*/
int
ppl_Congruence_System_empty
PPL_PROTO((ppl_const_Congruence_System_t cs));

/*! \relates ppl_Congruence_System_tag \brief
  Assigns to \p cit a const iterator "pointing" to the beginning of
  the congruence system \p cs.
*/
int
ppl_Congruence_System_begin
PPL_PROTO((ppl_const_Congruence_System_t cs,
	   ppl_Congruence_System_const_iterator_t cit));

/*! \relates ppl_Congruence_System_tag \brief
  Assigns to \p cit a const iterator "pointing" past the end of the
  congruence system \p cs.
*/
int
ppl_Congruence_System_end
PPL_PROTO((ppl_const_Congruence_System_t cs,
	   ppl_Congruence_System_const_iterator_t cit));

/*! \relates ppl_Congruence_System_tag \brief
  Returns a positive integer if \p cs is well formed, i.e., if it
  satisfies all its implementation invariants; returns 0 and perhaps
  makes some noise if \p cs is broken.  Useful for debugging purposes.
*/
int
ppl_Congruence_System_OK PPL_PROTO((ppl_const_Congruence_System_t cs));

/*@}*/ /* Functions that Do Not Modify the Congruence System */

/*! \brief \name Functions that May Modify the Congruence System */
/*@{*/

/*! \relates ppl_Congruence_System_tag \brief
  Removes all the congruences from the congruence system \p cs
  and sets its space dimension to 0.
*/
int
ppl_Congruence_System_clear PPL_PROTO((ppl_Congruence_System_t cs));

/*! \relates ppl_Congruence_System_tag \brief
  Inserts a copy of the congruence \p c into \p cs; the space
  dimension is increased, if necessary.
*/
int
ppl_Congruence_System_insert_Congruence PPL_PROTO((ppl_Congruence_System_t cs,
						   ppl_const_Congruence_t c));

/*@}*/ /* Functions that May Modify the Congruence System */

PPL_DECLARE_AND_DOCUMENT_IO_FUNCTIONS(Congruence_System)


/*! \brief \name Constructors, Assignment and Destructor */
/*@{*/

/*! \relates ppl_Congruence_System_const_iterator_tag \brief
  Builds a new `const iterator' and writes a handle to it at address
  \p pcit.
*/
int
ppl_new_Congruence_System_const_iterator
PPL_PROTO((ppl_Congruence_System_const_iterator_t* pcit));

/*! \relates ppl_Congruence_System_const_iterator_tag \brief
  Builds a const iterator that is a copy of \p cit; writes an
  handle for the newly created const iterator at address \p pcit.
*/
int
ppl_new_Congruence_System_const_iterator_from_Congruence_System_const_iterator
PPL_PROTO((ppl_Congruence_System_const_iterator_t* pcit,
	   ppl_const_Congruence_System_const_iterator_t cit));

/*! \relates ppl_Congruence_System_const_iterator_tag \brief
  Assigns a copy of the const iterator \p src to \p dst.
*/
int
ppl_assign_Congruence_System_const_iterator_from_Congruence_System_const_iterator
PPL_PROTO((ppl_Congruence_System_const_iterator_t dst,
	   ppl_const_Congruence_System_const_iterator_t src));

/*! \relates ppl_Congruence_System_const_iterator_tag \brief
  Invalidates the handle \p cit: this makes sure the corresponding
  resources will eventually be released.
*/
int
ppl_delete_Congruence_System_const_iterator
PPL_PROTO((ppl_const_Congruence_System_const_iterator_t cit));

/*@}*/ /* Constructors, Assignment and Destructor */

/*! \brief \name Dereferencing, Incrementing and Equality Testing */
/*@{*/

/*! \relates ppl_Congruence_System_const_iterator_tag \brief
  Dereference \p cit writing a const handle to the resulting
  congruence at address \p pc.
*/
int
ppl_Congruence_System_const_iterator_dereference
PPL_PROTO((ppl_const_Congruence_System_const_iterator_t cit,
	   ppl_const_Congruence_t* pc));

/*! \relates ppl_Congruence_System_const_iterator_tag \brief
  Increment \p cit so that it "points" to the next congruence.
*/
int
ppl_Congruence_System_const_iterator_increment
PPL_PROTO((ppl_Congruence_System_const_iterator_t cit));

/*! \relates ppl_Congruence_System_const_iterator_tag \brief
  Returns a positive integer if the iterators corresponding to \p x and
  \p y are equal; returns 0 if they are different.
*/
int
ppl_Congruence_System_const_iterator_equal_test
PPL_PROTO((ppl_const_Congruence_System_const_iterator_t x,
	   ppl_const_Congruence_System_const_iterator_t y));

/*@}*/ /* Dereferencing, Incrementing and Equality Testing */


/*! \brief \ingroup Datatypes
  Describes the different kinds of grid generators.
*/
enum ppl_enum_Grid_Generator_Type {
  /*! The grid generator is a line. */
  PPL_GRID_GENERATOR_TYPE_LINE,
  /*! The grid generator is a parameter. */
  PPL_GRID_GENERATOR_TYPE_PARAMETER,
  /*! The grid generator is a point. */
  PPL_GRID_GENERATOR_TYPE_POINT
};

/*! \brief \name Constructors, Assignment and Destructor */
/*@{*/

/*! \relates ppl_Grid_Generator_tag \brief
  Creates a new grid generator of direction \p le and type \p t.  If the
  grid generator to be created is a point or a parameter, the divisor
  \p d is applied to \p le.  If it is a line, \p d is simply disregarded.
  A handle for the new grid generator is written at address \p pg.
  The space dimension of the new grid generator is equal to the space
  dimension of \p le.
*/
int
ppl_new_Grid_Generator PPL_PROTO((ppl_Grid_Generator_t* pg,
				  ppl_const_Linear_Expression_t le,
				  enum ppl_enum_Grid_Generator_Type t,
				  ppl_const_Coefficient_t d));

/*! \relates ppl_Grid_Generator_tag \brief
  Creates the point that is the origin of the zero-dimensional space
  \f$\Rset^0\f$.  Writes a handle for the new grid generator at address
  \p pg.
*/
int
ppl_new_Grid_Generator_zero_dim_point PPL_PROTO((ppl_Grid_Generator_t* pg));

/*! \relates ppl_Grid_Generator_tag \brief
  Builds a grid generator that is a copy of \p g; writes a handle
  for the newly created grid generator at address \p pg.
*/
int
ppl_new_Grid_Generator_from_Grid_Generator
PPL_PROTO((ppl_Grid_Generator_t* pg, ppl_const_Grid_Generator_t g));

/*! \relates ppl_Grid_Generator_tag \brief
  Assigns a copy of the grid generator \p src to \p dst.
*/
int
ppl_assign_Grid_Generator_from_Grid_Generator
PPL_PROTO((ppl_Grid_Generator_t dst,
	   ppl_const_Grid_Generator_t src));

/*! \relates ppl_Grid_Generator_tag \brief
  Invalidates the handle \p g: this makes sure the corresponding
  resources will eventually be released.
*/
int
ppl_delete_Grid_Generator PPL_PROTO((ppl_const_Grid_Generator_t g));

/*@}*/ /* Constructors, Assignment and Destructor */

/*! \brief \name Functions that Do Not Modify the Grid Generator */
/*@{*/

/*! \relates ppl_Grid_Generator_tag \brief
  Writes to \p m the space dimension of \p g.
*/
int
ppl_Grid_Generator_space_dimension PPL_PROTO((ppl_const_Grid_Generator_t g,
					      ppl_dimension_type* m));

/*! \relates ppl_Grid_Generator_tag \brief
  Returns the type of grid generator \p g.
*/
int
ppl_Grid_Generator_type PPL_PROTO((ppl_const_Grid_Generator_t g));

/*! \relates ppl_Grid_Generator_tag \brief
  Copies into \p n the coefficient of variable \p var in
  grid generator \p g.
*/
int
ppl_Grid_Generator_coefficient PPL_PROTO((ppl_const_Grid_Generator_t g,
					  ppl_dimension_type var,
					  ppl_Coefficient_t n));

/*! \relates ppl_Grid_Generator_tag \brief
  If \p g is a point or a parameter assigns its divisor to \p n.
*/
int
ppl_Grid_Generator_divisor PPL_PROTO((ppl_const_Grid_Generator_t g,
				      ppl_Coefficient_t n));

/*! \relates ppl_Grid_Generator_tag \brief
  Returns a positive integer if \p g is well formed, i.e., if it
  satisfies all its implementation invariants; returns 0 and perhaps
  makes some noise if \p g is broken.  Useful for debugging purposes.
*/
int
ppl_Grid_Generator_OK PPL_PROTO((ppl_const_Grid_Generator_t g));

/*@}*/ /* Functions that Do Not Modify the Generator */

PPL_DECLARE_AND_DOCUMENT_IO_FUNCTIONS(Grid_Generator)


/*! \brief \name Constructors, Assignment and Destructor */
/*@{*/

/*! \relates ppl_Grid_Generator_System_tag \brief
  Builds an empty system of grid generators and writes a handle to it at
  address \p pgs.
*/
int
ppl_new_Grid_Generator_System PPL_PROTO((ppl_Grid_Generator_System_t* pgs));

/*
  Creates the universe zero-dimensional system of grid generators (i.e.,
  containing the origin only).  Writes a handle to the new system at
  address \p pgs.
*/
int
ppl_new_Grid_Generator_System_zero_dim_univ
PPL_PROTO((ppl_Grid_Generator_System_t* pgs));

/*! \relates ppl_Grid_Generator_System_tag \brief
  Builds the singleton grid generator system containing only a copy of
  generator \p g; writes a handle for the newly created system at
  address \p pgs.
*/
int
ppl_new_Grid_Generator_System_from_Grid_Generator
PPL_PROTO((ppl_Grid_Generator_System_t* pgs,
	   ppl_const_Grid_Generator_t g));

/*! \relates ppl_Grid_Generator_System_tag \brief
  Builds a grid generator system that is a copy of \p gs; writes a handle
  for the newly created system at address \p pgs.
*/
int
ppl_new_Grid_Generator_System_from_Grid_Generator_System
PPL_PROTO((ppl_Grid_Generator_System_t* pgs,
	   ppl_const_Grid_Generator_System_t gs));

/*! \relates ppl_Grid_Generator_System_tag \brief
  Assigns a copy of the grid generator system \p src to \p dst.
*/
int
ppl_assign_Grid_Generator_System_from_Grid_Generator_System
PPL_PROTO((ppl_Grid_Generator_System_t dst,
	   ppl_const_Grid_Generator_System_t src));

/*! \relates ppl_Grid_Generator_System_tag \brief
  Invalidates the handle \p gs: this makes sure the corresponding
  resources will eventually be released.
*/
int
ppl_delete_Grid_Generator_System
PPL_PROTO((ppl_const_Grid_Generator_System_t gs));

/*@}*/ /* Constructors, Assignment and Destructor */

/*! \brief \name Functions that Do Not Modify the Grid Generator System */
/*@{*/

/*! \relates ppl_Grid_Generator_System_tag \brief
  Writes to \p m the dimension of the vector space enclosing \p gs.
*/
int
ppl_Grid_Generator_System_space_dimension
PPL_PROTO((ppl_const_Grid_Generator_System_t gs, ppl_dimension_type* m));

/*! \relates ppl_Grid_Generator_System_tag \brief
  Returns a positive integer if \p gs contains no generator;
  returns 0 otherwise.
*/
int
ppl_Grid_Generator_System_empty
PPL_PROTO((ppl_const_Grid_Generator_System_t gs));

/*! \relates ppl_Grid_Generator_System_tag \brief
  Assigns to \p git a const iterator "pointing" to the beginning of
  the grid generator system \p gs.
*/
int
ppl_Grid_Generator_System_begin
PPL_PROTO((ppl_const_Grid_Generator_System_t gs,
	   ppl_Grid_Generator_System_const_iterator_t git));

/*! \relates ppl_Grid_Generator_System_tag \brief
  Assigns to \p git a const iterator "pointing" past the end of the
  grid generator system \p gs.
*/
int
ppl_Grid_Generator_System_end
PPL_PROTO((ppl_const_Grid_Generator_System_t gs,
	   ppl_Grid_Generator_System_const_iterator_t git));

/*! \relates ppl_Grid_Generator_System_tag \brief
  Returns a positive integer if \p gs is well formed, i.e., if it
  satisfies all its implementation invariants; returns 0 and perhaps
  makes some noise if \p gs is broken.  Useful for debugging purposes.
*/
int
ppl_Grid_Generator_System_OK PPL_PROTO((ppl_const_Grid_Generator_System_t gs));

/*@}*/ /* Functions that Do Not Modify the Grid Generator System */

/*! \brief \name Functions that May Modify the Grid Generator System */
/*@{*/

/*! \relates ppl_Grid_Generator_System_tag \brief
  Removes all the generators from the grid generator system \p gs
  and sets its space dimension to 0.
*/
int
ppl_Grid_Generator_System_clear PPL_PROTO((ppl_Grid_Generator_System_t gs));

/*! \relates ppl_Grid_Generator_System_tag \brief
  Inserts a copy of the grid generator \p g into \p gs; the space
  dimension is increased, if necessary.
*/
int
ppl_Grid_Generator_System_insert_Grid_Generator
PPL_PROTO((ppl_Grid_Generator_System_t gs,
	   ppl_const_Grid_Generator_t g));

/*@}*/ /* Functions that May Modify the Grid Generator System */

PPL_DECLARE_AND_DOCUMENT_IO_FUNCTIONS(Grid_Generator_System)


/*! \brief \name Constructors, Assignment and Destructor */
/*@{*/

/*! \relates ppl_Grid_Generator_System_const_iterator_tag \brief
  Builds a new `const iterator' and writes a handle to it at address
  \p pgit.
*/
int
ppl_new_Grid_Generator_System_const_iterator
PPL_PROTO((ppl_Grid_Generator_System_const_iterator_t* pgit));

/*! \relates ppl_Grid_Generator_System_const_iterator_tag \brief
  Builds a const iterator that is a copy of \p git; writes an
  handle for the newly created const iterator at address \p pgit.
*/
int
ppl_new_Grid_Generator_System_const_iterator_from_Grid_Generator_System_const_iterator
PPL_PROTO((ppl_Grid_Generator_System_const_iterator_t* pgit,
	   ppl_const_Grid_Generator_System_const_iterator_t git));

/*! \relates ppl_Grid_Generator_System_const_iterator_tag \brief
  Assigns a copy of the const iterator \p src to \p dst.
*/
int
ppl_assign_Grid_Generator_System_const_iterator_from_Grid_Generator_System_const_iterator
PPL_PROTO((ppl_Grid_Generator_System_const_iterator_t dst,
	   ppl_const_Grid_Generator_System_const_iterator_t src));

/*! \relates ppl_Grid_Generator_System_const_iterator_tag \brief
  Invalidates the handle \p git: this makes sure the corresponding
  resources will eventually be released.
*/
int
ppl_delete_Grid_Generator_System_const_iterator
PPL_PROTO((ppl_const_Grid_Generator_System_const_iterator_t git));

/*@}*/ /* Constructors, Assignment and Destructor */

/*! \brief \name Dereferencing, Incrementing and Equality Testing */
/*@{*/

/*! \relates ppl_Grid_Generator_System_const_iterator_tag \brief
  Dereference \p git writing a const handle to the resulting
  grid generator at address \p pg.
*/
int
ppl_Grid_Generator_System_const_iterator_dereference
PPL_PROTO((ppl_const_Grid_Generator_System_const_iterator_t git,
	   ppl_const_Grid_Generator_t* pg));

/*! \relates ppl_Grid_Generator_System_const_iterator_tag \brief
  Increment \p git so that it "points" to the next grid generator.
*/
int
ppl_Grid_Generator_System_const_iterator_increment
PPL_PROTO((ppl_Grid_Generator_System_const_iterator_t git));

/*! \relates ppl_Grid_Generator_System_const_iterator_tag \brief
  Returns a positive integer if the iterators corresponding to \p x and
  \p y are equal; returns 0 if they are different.
*/
int
ppl_Grid_Generator_System_const_iterator_equal_test
PPL_PROTO((ppl_const_Grid_Generator_System_const_iterator_t x,
	   ppl_const_Grid_Generator_System_const_iterator_t y));

/*@}*/ /* Dereferencing, Incrementing and Equality Testing */


/*! \brief \ingroup Datatypes
  Code of the worst-case polynomial complexity class.
*/
extern unsigned int PPL_COMPLEXITY_CLASS_POLYNOMIAL;

/*! \brief \ingroup Datatypes
  Code of the worst-case exponential but typically polynomial
  complexity class.
*/
extern unsigned int PPL_COMPLEXITY_CLASS_SIMPLEX;

/*! \brief \ingroup Datatypes
  Code of the universal complexity class.
*/
extern unsigned int PPL_COMPLEXITY_CLASS_ANY;

/*! \brief \ingroup Datatypes
  Individual bit saying that the polyhedron and the set of points
  satisfying the constraint are disjoint.
*/
extern unsigned int PPL_POLY_CON_RELATION_IS_DISJOINT;

/*! \brief \ingroup Datatypes
  Individual bit saying that the polyhedron intersects the set of
  points satisfying the constraint, but it is not included in it.
*/
extern unsigned int PPL_POLY_CON_RELATION_STRICTLY_INTERSECTS;

/*! \brief \ingroup Datatypes
  Individual bit saying that the polyhedron is included in the set of
  points satisfying the constraint.
*/
extern unsigned int PPL_POLY_CON_RELATION_IS_INCLUDED;

/*! \brief \ingroup Datatypes
  Individual bit saying that the polyhedron is included in the set of
  points saturating the constraint.
*/
extern unsigned int PPL_POLY_CON_RELATION_SATURATES;

/*! \brief \ingroup Datatypes
  Individual bit saying that adding the generator would not change the
  polyhedron.
*/
extern unsigned int PPL_POLY_GEN_RELATION_SUBSUMES;


/*! \brief \name Symbolic Constants */
/*@{*/

/*! \relates ppl_MIP_Problem_tag \brief
  Code of the "maximization" optimization mode.
*/
extern int PPL_OPTIMIZATION_MODE_MAXIMIZATION;

/*! \relates ppl_MIP_Problem_tag \brief
  Code of the "minimization" optimization mode.
*/
extern int PPL_OPTIMIZATION_MODE_MINIMIZATION;

/*! \relates ppl_MIP_Problem_tag \brief
  Code of the "unfeasible MIP problem" status.
*/
extern int PPL_MIP_PROBLEM_STATUS_UNFEASIBLE;

/*! \relates ppl_MIP_Problem_tag \brief
  Code of the "unbounded MIP problem" status.
*/
extern int PPL_MIP_PROBLEM_STATUS_UNBOUNDED;

/*! \relates ppl_MIP_Problem_tag \brief
  Code of the "optimized MIP problem" status.
*/
extern int PPL_MIP_PROBLEM_STATUS_OPTIMIZED;

/*! \relates ppl_MIP_Problem_tag \brief
  Code for the MIP problem's "pricing" control parameter name.
*/
extern int PPL_MIP_PROBLEM_CONTROL_PARAMETER_NAME_PRICING;

/*! \relates ppl_MIP_Problem_tag \brief
  Code of MIP problem's "textbook" pricing method.
*/
extern int PPL_MIP_PROBLEM_CONTROL_PARAMETER_PRICING_TEXTBOOK;

/*! \relates ppl_MIP_Problem_tag \brief
  Code of MIP problem's "exact steepest-edge" pricing method.
*/
extern int PPL_MIP_PROBLEM_CONTROL_PARAMETER_PRICING_STEEPEST_EDGE_EXACT;

/*! \relates ppl_MIP_Problem_tag \brief
  Code of MIP problem's "float steepest-edge" pricing method.
*/
extern int PPL_MIP_PROBLEM_CONTROL_PARAMETER_PRICING_STEEPEST_EDGE_FLOAT;

/*@}*/ /* Symbolic Constants */

/*! \brief \name Constructors, Assignment and Destructor */
/*@{*/

/*! \relates ppl_MIP_Problem_tag \brief
  Builds a trivial MIP problem of dimension \p d and writes an
  handle to it at address \p pmip.
*/
int
ppl_new_MIP_Problem_from_space_dimension PPL_PROTO((ppl_MIP_Problem_t* pmip,
						    ppl_dimension_type d));

/*! \relates ppl_MIP_Problem_tag \brief
  Builds an MIP problem of space dimension \p d having feasible region \p cs,
  objective function \p le and optimization mode \p m; writes a handle to
  it at address \p pmip.
*/
int
ppl_new_MIP_Problem PPL_PROTO((ppl_MIP_Problem_t* pmip,
			       ppl_dimension_type d,
			       ppl_const_Constraint_System_t cs,
			       ppl_const_Linear_Expression_t le,
			       int m));

/*! \relates ppl_MIP_Problem_tag \brief
  Builds an MIP problem that is a copy of \p mip; writes a handle
  for the newly created system at address \p pmip.
*/
int
ppl_new_MIP_Problem_from_MIP_Problem
PPL_PROTO((ppl_MIP_Problem_t* pmip, ppl_const_MIP_Problem_t mip));

/*! \relates ppl_MIP_Problem_tag \brief
  Assigns a copy of the MIP problem \p src to \p dst.
*/
int
ppl_assign_MIP_Problem_from_MIP_Problem
PPL_PROTO((ppl_MIP_Problem_t dst, ppl_const_MIP_Problem_t src));

/*! \relates ppl_MIP_Problem_tag \brief
  Invalidates the handle \p mip: this makes sure the corresponding
  resources will eventually be released.
*/
int
ppl_delete_MIP_Problem PPL_PROTO((ppl_const_MIP_Problem_t mip));

/*@}*/ /* Constructors, Assignment and Destructor for MIP_Problem */

/*! \brief \name Functions that Do Not Modify the MIP_Problem */
/*@{*/

/*! \relates ppl_MIP_Problem_tag \brief
  Writes to \p m the dimension of the vector space enclosing \p mip.
*/
int
ppl_MIP_Problem_space_dimension
PPL_PROTO((ppl_const_MIP_Problem_t mip, ppl_dimension_type* m));

/*! \relates ppl_MIP_Problem_tag \brief
  Writes to \p m the number of integer space dimensions of \p mip.
*/
int
ppl_MIP_Problem_number_of_integer_space_dimensions
PPL_PROTO((ppl_const_MIP_Problem_t mip, ppl_dimension_type* m));

/*! \relates ppl_MIP_Problem_tag \brief
  Writes in the first positions of the array \p ds all the integer space
  dimensions of problem \p mip. If the array is not big enough to hold
  all of the integer space dimensions, the behavior is undefined.
*/
int
ppl_MIP_Problem_integer_space_dimensions
PPL_PROTO((ppl_const_MIP_Problem_t mip, ppl_dimension_type ds[]));

/*! \relates ppl_MIP_Problem_tag \brief
  Writes to \p m the number of constraints defining
  the feasible region of \p mip.
*/
int
ppl_MIP_Problem_number_of_constraints PPL_PROTO((ppl_const_MIP_Problem_t mip,
						 ppl_dimension_type* m));

/*! \relates ppl_MIP_Problem_tag \brief
  Writes at address \p pc a const handle to the \p i-th constraint
  defining the feasible region of the MIP problem \p mip
*/
int
ppl_MIP_Problem_constraint_at_index PPL_PROTO((ppl_const_MIP_Problem_t mip,
					       ppl_dimension_type i,
					       ppl_const_Constraint_t* pc));

/*! \relates ppl_MIP_Problem_tag \brief
  Writes a const handle to the linear expression defining the
  objective function of the MIP problem \p mip at address \p ple.
*/
int
ppl_MIP_Problem_objective_function
PPL_PROTO((ppl_const_MIP_Problem_t mip, ppl_const_Linear_Expression_t* ple));

/*! \relates ppl_MIP_Problem_tag \brief
  Returns the optimization mode of the MIP problem \p mip.
*/
int
ppl_MIP_Problem_optimization_mode PPL_PROTO((ppl_const_MIP_Problem_t mip));

/*! \relates ppl_MIP_Problem_tag \brief
  Returns a positive integer if \p mip is well formed, i.e., if it
  satisfies all its implementation invariants; returns 0 and perhaps
  makes some noise if \p mip is broken.  Useful for debugging purposes.
*/
int
ppl_MIP_Problem_OK PPL_PROTO((ppl_const_MIP_Problem_t mip));

/*@}*/ /* Functions that Do Not Modify the MIP_Problem */

/*! \brief \name Functions that May Modify the MIP_Problem */
/*@{*/

/*! \relates ppl_MIP_Problem_tag \brief
  Resets the MIP problem to be a trivial problem of space dimension 0.
*/
int
ppl_MIP_Problem_clear PPL_PROTO((ppl_MIP_Problem_t mip));

/*! \relates ppl_MIP_Problem_tag \brief
  Adds \p d new dimensions to the space enclosing the MIP problem \p mip
  and to \p mip itself.
*/
int
ppl_MIP_Problem_add_space_dimensions_and_embed
PPL_PROTO((ppl_MIP_Problem_t mip, ppl_dimension_type d));

/*! \relates ppl_MIP_Problem_tag \brief
  Sets the space dimensions that are specified in first \p n positions
  of the array \p ds to be integer dimensions of problem \p mip.
  The presence of duplicates in \p ds is a waste but an innocuous one.
*/
int
ppl_MIP_Problem_add_to_integer_space_dimensions
PPL_PROTO((ppl_MIP_Problem_t mip, ppl_dimension_type ds[], size_t n));

/*! \relates ppl_MIP_Problem_tag \brief
  Modifies the feasible region of the MIP problem \p mip by adding a copy
  of the constraint \p c.
*/
int
ppl_MIP_Problem_add_constraint PPL_PROTO((ppl_MIP_Problem_t mip,
					  ppl_const_Constraint_t c));

/*! \relates ppl_MIP_Problem_tag \brief
  Modifies the feasible region of the MIP problem \p mip by adding a copy
  of the constraints in \p cs.
*/
int
ppl_MIP_Problem_add_constraints PPL_PROTO((ppl_MIP_Problem_t mip,
					   ppl_const_Constraint_System_t cs));

/*! \relates ppl_MIP_Problem_tag \brief
  Sets the objective function of the MIP problem \p mip to a copy of \p le.
*/
int
ppl_MIP_Problem_set_objective_function
PPL_PROTO((ppl_MIP_Problem_t mip, ppl_const_Linear_Expression_t le));

/*! \relates ppl_MIP_Problem_tag \brief
  Sets the optimization mode of the MIP problem \p mip to \p mode.
*/
int
ppl_MIP_Problem_set_optimization_mode PPL_PROTO((ppl_MIP_Problem_t mip,
						 int mode));

/*@}*/ /* Functions that May Modify the MIP_Problem */

/*! \brief \name Computing the Solution of the MIP_Problem */
/*@{*/

/*! \relates ppl_MIP_Problem_tag \brief
  Returns a positive integer if \p mip is satisfiable; returns 0 otherwise.
*/
int
ppl_MIP_Problem_is_satisfiable PPL_PROTO((ppl_const_MIP_Problem_t mip));

/*! \relates ppl_MIP_Problem_tag \brief
  Solves the MIP problem \p mip, returning an exit status.

  \return
  <CODE>PPL_MIP_PROBLEM_STATUS_UNFEASIBLE</CODE> if the MIP problem
  is not satisfiable;
  <CODE>PPL_MIP_PROBLEM_STATUS_UNBOUNDED</CODE> if the MIP problem
  is satisfiable but there is no finite bound to the value of
  the objective function;
  <CODE>PPL_MIP_PROBLEM_STATUS_OPTIMIZED</CODE> if the MIP problem
  admits an optimal solution.
*/
int
ppl_MIP_Problem_solve PPL_PROTO((ppl_const_MIP_Problem_t mip));

/*! \relates ppl_MIP_Problem_tag \brief
  Evaluates the objective function of \p mip on point \p g.

  \param mip
  The MIP problem defining the objective function;

  \param g
  The generator on which the objective function will be evaluated;

  \param num
  Will be assigned the numerator of the objective function value;

  \param den
  Will be assigned the denominator of the objective function value;
*/
int
ppl_MIP_Problem_evaluate_objective_function
PPL_PROTO((ppl_const_MIP_Problem_t mip, ppl_const_Generator_t g,
	   ppl_Coefficient_t num, ppl_Coefficient_t den));

/*! \relates ppl_MIP_Problem_tag \brief
  Writes a const handle to a feasible point for the MIP problem \p mip
  at address \p pg.
*/
int
ppl_MIP_Problem_feasible_point PPL_PROTO((ppl_const_MIP_Problem_t mip,
					  ppl_const_Generator_t* pg));

/*! \relates ppl_MIP_Problem_tag \brief
  Writes a const handle to an optimizing point for the MIP problem \p mip
  at address \p pg.
*/
int
ppl_MIP_Problem_optimizing_point PPL_PROTO((ppl_const_MIP_Problem_t mip,
					    ppl_const_Generator_t* pg));

/*! \relates ppl_MIP_Problem_tag \brief
  Returns the optimal value for \p mip.

  \param mip
  The MIP problem;

  \param num
  Will be assigned the numerator of the optimal value;

  \param den
  Will be assigned the denominator of the optimal value.
*/
int
ppl_MIP_Problem_optimal_value
PPL_PROTO((ppl_const_MIP_Problem_t mip,
	   ppl_Coefficient_t num, ppl_Coefficient_t den));

/*@}*/ /* Computing the Solution of the MIP_Problem */

/*! \brief \name Querying/Setting Control Parameters */
/*@{*/

/*! \relates ppl_MIP_Problem_tag \brief
  Returns the value of control parameter \p name in problem \p mip.
*/
int
ppl_MIP_Problem_get_control_parameter
PPL_PROTO((ppl_const_MIP_Problem_t mip, int name));

/*! \relates ppl_MIP_Problem_tag \brief
  Sets control parameter \p value in problem \p mip.
*/
int
ppl_MIP_Problem_set_control_parameter
PPL_PROTO((ppl_MIP_Problem_t mip, int value));

/*@}*/ /* Querying/Setting Control Parameters */

PPL_DECLARE_AND_DOCUMENT_IO_FUNCTIONS(MIP_Problem)

/* Automatically generated from PPL source file ../../interfaces/C/ppl_c_domains.h line 1. */
/*C interface code: header file.
*/

PPL_TYPE_DECLARATION(Polyhedron)
PPL_TYPE_DECLARATION(Grid)
PPL_TYPE_DECLARATION(Rational_Box)
PPL_TYPE_DECLARATION(BD_Shape_mpz_class)
PPL_TYPE_DECLARATION(BD_Shape_mpq_class)
PPL_TYPE_DECLARATION(Octagonal_Shape_mpz_class)
PPL_TYPE_DECLARATION(Octagonal_Shape_mpq_class)
PPL_TYPE_DECLARATION(Constraints_Product_C_Polyhedron_Grid)
PPL_TYPE_DECLARATION(Pointset_Powerset_C_Polyhedron)
PPL_TYPE_DECLARATION(Pointset_Powerset_NNC_Polyhedron)
PPL_TYPE_DECLARATION(Double_Box)
PPL_TYPE_DECLARATION(BD_Shape_double)
PPL_TYPE_DECLARATION(Octagonal_Shape_double)

/*! \interface ppl_Polyhedron_tag
  \brief Types and functions for ppl_Polyhedron_tag
*/

/*! \relates ppl_Polyhedron_tag */
int
ppl_delete_Polyhedron
PPL_PROTO((ppl_const_Polyhedron_t ph));




/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_space_dimension
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_dimension_type d, int empty));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_space_dimension
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_dimension_type d, int empty));





/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_C_Polyhedron
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_C_Polyhedron
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_NNC_Polyhedron
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_NNC_Polyhedron
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_Grid
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Grid_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_Grid
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Grid_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_Rational_Box
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Rational_Box_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_Rational_Box
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Rational_Box_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_BD_Shape_mpz_class
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_BD_Shape_mpz_class_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_BD_Shape_mpz_class
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_BD_Shape_mpz_class_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_BD_Shape_mpq_class
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_BD_Shape_mpq_class_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_BD_Shape_mpq_class
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_BD_Shape_mpq_class_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_Double_Box
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Double_Box_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_Double_Box
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Double_Box_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_BD_Shape_double
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_BD_Shape_double_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_BD_Shape_double
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_BD_Shape_double_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_Octagonal_Shape_double
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Octagonal_Shape_double_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_Octagonal_Shape_double
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Octagonal_Shape_double_t ph));






/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_C_Polyhedron_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_C_Polyhedron_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_NNC_Polyhedron_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_NNC_Polyhedron_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_Grid_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Grid_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_Grid_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Grid_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_Rational_Box_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Rational_Box_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_Rational_Box_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Rational_Box_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_BD_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_BD_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_BD_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_BD_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_BD_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_BD_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_BD_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_BD_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_Octagonal_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_Octagonal_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_Octagonal_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_Octagonal_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_Double_Box_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Double_Box_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_Double_Box_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Double_Box_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_BD_Shape_double_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_BD_Shape_double_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_BD_Shape_double_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_BD_Shape_double_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_Octagonal_Shape_double_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Octagonal_Shape_double_t ph, int complexity));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_Octagonal_Shape_double_with_complexity
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Octagonal_Shape_double_t ph, int complexity));






/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_Constraint_System
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Constraint_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_Constraint_System
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Constraint_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_Congruence_System
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Congruence_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_Congruence_System
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Congruence_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_from_Generator_System
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Generator_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_from_Generator_System
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_const_Generator_System_t cs));








/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_space_dimension
PPL_PROTO((ppl_const_Polyhedron_t ph, ppl_dimension_type* m));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_affine_dimension
PPL_PROTO((ppl_const_Polyhedron_t ph, ppl_dimension_type* m));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_relation_with_Constraint
PPL_PROTO((ppl_const_Polyhedron_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_relation_with_Generator
PPL_PROTO((ppl_const_Polyhedron_t ph,
           ppl_const_Generator_t c));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_relation_with_Congruence
PPL_PROTO((ppl_const_Polyhedron_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_get_constraints
PPL_PROTO((ppl_const_Polyhedron_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_get_congruences
PPL_PROTO((ppl_const_Polyhedron_t ph,
           ppl_const_Congruence_System_t* pcs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_get_generators
PPL_PROTO((ppl_const_Polyhedron_t ph,
           ppl_const_Generator_System_t* pcs));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_get_minimized_constraints
PPL_PROTO((ppl_const_Polyhedron_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_get_minimized_congruences
PPL_PROTO((ppl_const_Polyhedron_t ph,
           ppl_const_Congruence_System_t* pcs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_get_minimized_generators
PPL_PROTO((ppl_const_Polyhedron_t ph,
           ppl_const_Generator_System_t* pcs));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_is_empty
PPL_PROTO((ppl_const_Polyhedron_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_is_universe
PPL_PROTO((ppl_const_Polyhedron_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_is_bounded
PPL_PROTO((ppl_const_Polyhedron_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_contains_integer_point
PPL_PROTO((ppl_const_Polyhedron_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_is_topologically_closed
PPL_PROTO((ppl_const_Polyhedron_t ph));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_is_discrete
PPL_PROTO((ppl_const_Polyhedron_t ph));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_topological_closure_assign
PPL_PROTO((ppl_Polyhedron_t ph));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_bounds_from_above
PPL_PROTO((ppl_const_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_bounds_from_below
PPL_PROTO((ppl_const_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_maximize
PPL_PROTO((ppl_const_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_minimize
PPL_PROTO((ppl_const_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_maximize_with_point
PPL_PROTO((ppl_const_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_minimize_with_point
PPL_PROTO((ppl_const_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_contains_Polyhedron
PPL_PROTO((ppl_const_Polyhedron_t x,
           ppl_const_Polyhedron_t y));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_strictly_contains_Polyhedron
PPL_PROTO((ppl_const_Polyhedron_t x,
           ppl_const_Polyhedron_t y));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_is_disjoint_from_Polyhedron
PPL_PROTO((ppl_const_Polyhedron_t x,
           ppl_const_Polyhedron_t y));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_equals_Polyhedron
PPL_PROTO((ppl_const_Polyhedron_t x,
           ppl_const_Polyhedron_t y));




/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_OK
PPL_PROTO((ppl_const_Polyhedron_t ph));




/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_add_constraint
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_add_congruence
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Congruence_t c));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_add_generator
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Generator_t c));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_add_constraints
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_add_congruences
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Congruence_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_add_generators
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Generator_System_t cs));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_refine_with_constraint
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_refine_with_congruence
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_refine_with_constraints
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_refine_with_congruences
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_intersection_assign
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_upper_bound_assign
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_difference_assign
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_concatenate_assign
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_time_elapse_assign
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_poly_hull_assign
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_poly_difference_assign
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_upper_bound_assign_if_exact
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_poly_hull_assign_if_exact
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_simplify_using_context_assign
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y));




/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_constrains
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_unconstrain_space_dimension
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_unconstrain_space_dimensions
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_affine_image
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_affine_preimage
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_bounded_affine_image
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_bounded_affine_preimage
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_generalized_affine_image
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_generalized_affine_preimage
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_generalized_affine_image_lhs_rhs
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_generalized_affine_preimage_lhs_rhs
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_add_space_dimensions_and_embed
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_dimension_type d));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_add_space_dimensions_and_project
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_dimension_type d));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_remove_space_dimensions
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_remove_higher_space_dimensions
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_dimension_type d));




/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_expand_space_dimension
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_dimension_type d,
           ppl_dimension_type m));




/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_fold_space_dimensions
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_dimension_type ds[],
           size_t n,
           ppl_dimension_type d));




/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_map_space_dimensions
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_dimension_type maps[],
           size_t n));






/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_external_memory_in_bytes
PPL_PROTO((ppl_const_Polyhedron_t ps,
           size_t* sz));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_total_memory_in_bytes
PPL_PROTO((ppl_const_Polyhedron_t ps,
           size_t* sz));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_BHRZ03_widening_assign_with_tokens
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y,
           unsigned* tp));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_H79_widening_assign_with_tokens
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y,
           unsigned* tp));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_BHRZ03_widening_assign
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_H79_widening_assign
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_widening_assign_with_tokens
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y,
           unsigned* tp));




/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_widening_assign
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y));




/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_limited_BHRZ03_extrapolation_assign_with_tokens
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign_with_tokens
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_limited_H79_extrapolation_assign_with_tokens
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_bounded_H79_extrapolation_assign_with_tokens
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));







/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_limited_BHRZ03_extrapolation_assign
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_limited_H79_extrapolation_assign
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_bounded_H79_extrapolation_assign
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y,
           ppl_const_Constraint_System_t cs));







/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_linear_partition
PPL_PROTO((ppl_const_Polyhedron_t x,
           ppl_const_Polyhedron_t y,
           ppl_Polyhedron_t* p_inters,
           ppl_Pointset_Powerset_NNC_Polyhedron_t* p_rest));





/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_recycle_Constraint_System
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_Constraint_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_recycle_Constraint_System
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_Constraint_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_recycle_Congruence_System
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_Congruence_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_recycle_Congruence_System
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_Congruence_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_C_Polyhedron_recycle_Generator_System
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_Generator_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_new_NNC_Polyhedron_recycle_Generator_System
PPL_PROTO((ppl_Polyhedron_t* pph, ppl_Generator_System_t cs));






/*! \relates ppl_Polyhedron_tag */
int
ppl_assign_C_Polyhedron_from_C_Polyhedron
PPL_PROTO((ppl_Polyhedron_t dst, ppl_const_Polyhedron_t src));

/*! \relates ppl_Polyhedron_tag */
int
ppl_assign_NNC_Polyhedron_from_NNC_Polyhedron
PPL_PROTO((ppl_Polyhedron_t dst, ppl_const_Polyhedron_t src));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_add_recycled_constraints
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_Constraint_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_add_recycled_congruences
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_Congruence_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_add_recycled_generators
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_Generator_System_t cs));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_add_constraint_and_minimize
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_add_congruence_and_minimize
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Congruence_t c));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_add_generator_and_minimize
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Generator_t c));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_add_constraints_and_minimize
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_add_congruences_and_minimize
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Congruence_System_t cs));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_add_generators_and_minimize
PPL_PROTO((ppl_Polyhedron_t ph,
           ppl_const_Generator_System_t cs));





/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_intersection_assign_and_minimize
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y));

/*! \relates ppl_Polyhedron_tag */
int
ppl_Polyhedron_poly_hull_assign_and_minimize
PPL_PROTO((ppl_Polyhedron_t x,
           ppl_const_Polyhedron_t y));






PPL_DECLARE_IO_FUNCTIONS(Polyhedron)

/* End of Functions Related to ppl_Polyhedron_tag */

/*! \interface ppl_Grid_tag
  \brief Types and functions for ppl_Grid_tag
*/

/*! \relates ppl_Grid_tag */
int
ppl_delete_Grid
PPL_PROTO((ppl_const_Grid_t ph));




/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_space_dimension
PPL_PROTO((ppl_Grid_t* pph, ppl_dimension_type d, int empty));





/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_C_Polyhedron
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_NNC_Polyhedron
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_Grid
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Grid_t ph));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_Rational_Box
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Rational_Box_t ph));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_BD_Shape_mpz_class
PPL_PROTO((ppl_Grid_t* pph, ppl_const_BD_Shape_mpz_class_t ph));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_BD_Shape_mpq_class
PPL_PROTO((ppl_Grid_t* pph, ppl_const_BD_Shape_mpq_class_t ph));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_Double_Box
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Double_Box_t ph));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_BD_Shape_double
PPL_PROTO((ppl_Grid_t* pph, ppl_const_BD_Shape_double_t ph));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_Octagonal_Shape_double
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Octagonal_Shape_double_t ph));






/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_C_Polyhedron_with_complexity
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_NNC_Polyhedron_with_complexity
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_Grid_with_complexity
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Grid_t ph, int complexity));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_Rational_Box_with_complexity
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Rational_Box_t ph, int complexity));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_BD_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Grid_t* pph, ppl_const_BD_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_BD_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Grid_t* pph, ppl_const_BD_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_Octagonal_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_Octagonal_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_Double_Box_with_complexity
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Double_Box_t ph, int complexity));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_BD_Shape_double_with_complexity
PPL_PROTO((ppl_Grid_t* pph, ppl_const_BD_Shape_double_t ph, int complexity));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_Octagonal_Shape_double_with_complexity
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Octagonal_Shape_double_t ph, int complexity));






/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_Constraint_System
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Constraint_System_t cs));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_Congruence_System
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Congruence_System_t cs));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_from_Grid_Generator_System
PPL_PROTO((ppl_Grid_t* pph, ppl_const_Grid_Generator_System_t cs));








/*! \relates ppl_Grid_tag */
int
ppl_Grid_space_dimension
PPL_PROTO((ppl_const_Grid_t ph, ppl_dimension_type* m));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_affine_dimension
PPL_PROTO((ppl_const_Grid_t ph, ppl_dimension_type* m));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_relation_with_Constraint
PPL_PROTO((ppl_const_Grid_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_relation_with_Generator
PPL_PROTO((ppl_const_Grid_t ph,
           ppl_const_Generator_t c));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_relation_with_Congruence
PPL_PROTO((ppl_const_Grid_t ph,
           ppl_const_Congruence_t c));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_relation_with_Grid_Generator
PPL_PROTO((ppl_const_Grid_t ph,
           ppl_const_Grid_Generator_t c));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_get_constraints
PPL_PROTO((ppl_const_Grid_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_get_congruences
PPL_PROTO((ppl_const_Grid_t ph,
           ppl_const_Congruence_System_t* pcs));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_get_grid_generators
PPL_PROTO((ppl_const_Grid_t ph,
           ppl_const_Grid_Generator_System_t* pcs));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_get_minimized_constraints
PPL_PROTO((ppl_const_Grid_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_get_minimized_congruences
PPL_PROTO((ppl_const_Grid_t ph,
           ppl_const_Congruence_System_t* pcs));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_get_minimized_grid_generators
PPL_PROTO((ppl_const_Grid_t ph,
           ppl_const_Grid_Generator_System_t* pcs));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_is_empty
PPL_PROTO((ppl_const_Grid_t ph));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_is_universe
PPL_PROTO((ppl_const_Grid_t ph));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_is_bounded
PPL_PROTO((ppl_const_Grid_t ph));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_contains_integer_point
PPL_PROTO((ppl_const_Grid_t ph));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_is_topologically_closed
PPL_PROTO((ppl_const_Grid_t ph));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_is_discrete
PPL_PROTO((ppl_const_Grid_t ph));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_topological_closure_assign
PPL_PROTO((ppl_Grid_t ph));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_bounds_from_above
PPL_PROTO((ppl_const_Grid_t ph,
           ppl_const_Linear_Expression_t le));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_bounds_from_below
PPL_PROTO((ppl_const_Grid_t ph,
           ppl_const_Linear_Expression_t le));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_maximize
PPL_PROTO((ppl_const_Grid_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_minimize
PPL_PROTO((ppl_const_Grid_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_maximize_with_point
PPL_PROTO((ppl_const_Grid_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_minimize_with_point
PPL_PROTO((ppl_const_Grid_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_contains_Grid
PPL_PROTO((ppl_const_Grid_t x,
           ppl_const_Grid_t y));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_strictly_contains_Grid
PPL_PROTO((ppl_const_Grid_t x,
           ppl_const_Grid_t y));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_is_disjoint_from_Grid
PPL_PROTO((ppl_const_Grid_t x,
           ppl_const_Grid_t y));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_equals_Grid
PPL_PROTO((ppl_const_Grid_t x,
           ppl_const_Grid_t y));




/*! \relates ppl_Grid_tag */
int
ppl_Grid_OK
PPL_PROTO((ppl_const_Grid_t ph));




/*! \relates ppl_Grid_tag */
int
ppl_Grid_add_constraint
PPL_PROTO((ppl_Grid_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_add_congruence
PPL_PROTO((ppl_Grid_t ph,
           ppl_const_Congruence_t c));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_add_grid_generator
PPL_PROTO((ppl_Grid_t ph,
           ppl_const_Grid_Generator_t c));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_add_constraints
PPL_PROTO((ppl_Grid_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_add_congruences
PPL_PROTO((ppl_Grid_t ph,
           ppl_const_Congruence_System_t cs));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_add_grid_generators
PPL_PROTO((ppl_Grid_t ph,
           ppl_const_Grid_Generator_System_t cs));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_refine_with_constraint
PPL_PROTO((ppl_Grid_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_refine_with_congruence
PPL_PROTO((ppl_Grid_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_refine_with_constraints
PPL_PROTO((ppl_Grid_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_refine_with_congruences
PPL_PROTO((ppl_Grid_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_intersection_assign
PPL_PROTO((ppl_Grid_t x,
           ppl_const_Grid_t y));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_upper_bound_assign
PPL_PROTO((ppl_Grid_t x,
           ppl_const_Grid_t y));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_difference_assign
PPL_PROTO((ppl_Grid_t x,
           ppl_const_Grid_t y));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_concatenate_assign
PPL_PROTO((ppl_Grid_t x,
           ppl_const_Grid_t y));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_time_elapse_assign
PPL_PROTO((ppl_Grid_t x,
           ppl_const_Grid_t y));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_upper_bound_assign_if_exact
PPL_PROTO((ppl_Grid_t x,
           ppl_const_Grid_t y));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_simplify_using_context_assign
PPL_PROTO((ppl_Grid_t x,
           ppl_const_Grid_t y));




/*! \relates ppl_Grid_tag */
int
ppl_Grid_constrains
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Grid_tag */
int
ppl_Grid_unconstrain_space_dimension
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Grid_tag */
int
ppl_Grid_unconstrain_space_dimensions
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Grid_tag */
int
ppl_Grid_affine_image
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_affine_preimage
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_bounded_affine_image
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_bounded_affine_preimage
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_generalized_affine_image
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_generalized_affine_preimage
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_generalized_affine_image_lhs_rhs
PPL_PROTO((ppl_Grid_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_generalized_affine_preimage_lhs_rhs
PPL_PROTO((ppl_Grid_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_generalized_affine_image_with_congruence
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d,
           ppl_const_Coefficient_t m));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_generalized_affine_preimage_with_congruence
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d,
           ppl_const_Coefficient_t m));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_generalized_affine_image_lhs_rhs_with_congruence
PPL_PROTO((ppl_Grid_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs,
           ppl_const_Coefficient_t m));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_generalized_affine_preimage_lhs_rhs_with_congruence
PPL_PROTO((ppl_Grid_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs,
           ppl_const_Coefficient_t m));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_add_space_dimensions_and_embed
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type d));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_add_space_dimensions_and_project
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type d));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_remove_space_dimensions
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Grid_tag */
int
ppl_Grid_remove_higher_space_dimensions
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type d));




/*! \relates ppl_Grid_tag */
int
ppl_Grid_expand_space_dimension
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type d,
           ppl_dimension_type m));




/*! \relates ppl_Grid_tag */
int
ppl_Grid_fold_space_dimensions
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type ds[],
           size_t n,
           ppl_dimension_type d));




/*! \relates ppl_Grid_tag */
int
ppl_Grid_map_space_dimensions
PPL_PROTO((ppl_Grid_t ph,
           ppl_dimension_type maps[],
           size_t n));






/*! \relates ppl_Grid_tag */
int
ppl_Grid_external_memory_in_bytes
PPL_PROTO((ppl_const_Grid_t ps,
           size_t* sz));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_total_memory_in_bytes
PPL_PROTO((ppl_const_Grid_t ps,
           size_t* sz));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_congruence_widening_assign_with_tokens
PPL_PROTO((ppl_Grid_t x,
           ppl_const_Grid_t y,
           unsigned* tp));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_generator_widening_assign_with_tokens
PPL_PROTO((ppl_Grid_t x,
           ppl_const_Grid_t y,
           unsigned* tp));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_congruence_widening_assign
PPL_PROTO((ppl_Grid_t x,
           ppl_const_Grid_t y));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_generator_widening_assign
PPL_PROTO((ppl_Grid_t x,
           ppl_const_Grid_t y));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_widening_assign_with_tokens
PPL_PROTO((ppl_Grid_t x,
           ppl_const_Grid_t y,
           unsigned* tp));




/*! \relates ppl_Grid_tag */
int
ppl_Grid_widening_assign
PPL_PROTO((ppl_Grid_t x,
           ppl_const_Grid_t y));




/*! \relates ppl_Grid_tag */
int
ppl_Grid_limited_congruence_extrapolation_assign_with_tokens
PPL_PROTO((ppl_Grid_t x,
           ppl_const_Grid_t y,
           ppl_const_Congruence_System_t cs,
           unsigned* tp));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_limited_generator_extrapolation_assign_with_tokens
PPL_PROTO((ppl_Grid_t x,
           ppl_const_Grid_t y,
           ppl_const_Congruence_System_t cs,
           unsigned* tp));







/*! \relates ppl_Grid_tag */
int
ppl_Grid_limited_congruence_extrapolation_assign
PPL_PROTO((ppl_Grid_t x,
           ppl_const_Grid_t y,
           ppl_const_Congruence_System_t cs));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_limited_generator_extrapolation_assign
PPL_PROTO((ppl_Grid_t x,
           ppl_const_Grid_t y,
           ppl_const_Congruence_System_t cs));











/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_recycle_Constraint_System
PPL_PROTO((ppl_Grid_t* pph, ppl_Constraint_System_t cs));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_recycle_Congruence_System
PPL_PROTO((ppl_Grid_t* pph, ppl_Congruence_System_t cs));

/*! \relates ppl_Grid_tag */
int
ppl_new_Grid_recycle_Grid_Generator_System
PPL_PROTO((ppl_Grid_t* pph, ppl_Grid_Generator_System_t cs));






/*! \relates ppl_Grid_tag */
int
ppl_assign_Grid_from_Grid
PPL_PROTO((ppl_Grid_t dst, ppl_const_Grid_t src));





/*! \relates ppl_Grid_tag */
int
ppl_Grid_add_recycled_constraints
PPL_PROTO((ppl_Grid_t ph,
           ppl_Constraint_System_t cs));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_add_recycled_congruences
PPL_PROTO((ppl_Grid_t ph,
           ppl_Congruence_System_t cs));

/*! \relates ppl_Grid_tag */
int
ppl_Grid_add_recycled_grid_generators
PPL_PROTO((ppl_Grid_t ph,
           ppl_Grid_Generator_System_t cs));






PPL_DECLARE_IO_FUNCTIONS(Grid)

/* End of Functions Related to ppl_Grid_tag */

/*! \interface ppl_Rational_Box_tag
  \brief Types and functions for ppl_Rational_Box_tag
*/

/*! \relates ppl_Rational_Box_tag */
int
ppl_delete_Rational_Box
PPL_PROTO((ppl_const_Rational_Box_t ph));




/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_space_dimension
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_dimension_type d, int empty));





/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_C_Polyhedron
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_NNC_Polyhedron
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_Grid
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Grid_t ph));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_Rational_Box
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Rational_Box_t ph));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_BD_Shape_mpz_class
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_BD_Shape_mpz_class_t ph));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_BD_Shape_mpq_class
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_BD_Shape_mpq_class_t ph));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_Double_Box
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Double_Box_t ph));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_BD_Shape_double
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_BD_Shape_double_t ph));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_Octagonal_Shape_double
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Octagonal_Shape_double_t ph));






/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_C_Polyhedron_with_complexity
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_NNC_Polyhedron_with_complexity
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_Grid_with_complexity
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Grid_t ph, int complexity));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_Rational_Box_with_complexity
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Rational_Box_t ph, int complexity));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_BD_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_BD_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_BD_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_BD_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_Octagonal_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_Octagonal_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_Double_Box_with_complexity
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Double_Box_t ph, int complexity));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_BD_Shape_double_with_complexity
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_BD_Shape_double_t ph, int complexity));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_Octagonal_Shape_double_with_complexity
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Octagonal_Shape_double_t ph, int complexity));






/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_Constraint_System
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Constraint_System_t cs));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_Congruence_System
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Congruence_System_t cs));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_from_Generator_System
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_const_Generator_System_t cs));








/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_space_dimension
PPL_PROTO((ppl_const_Rational_Box_t ph, ppl_dimension_type* m));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_affine_dimension
PPL_PROTO((ppl_const_Rational_Box_t ph, ppl_dimension_type* m));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_relation_with_Constraint
PPL_PROTO((ppl_const_Rational_Box_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_relation_with_Generator
PPL_PROTO((ppl_const_Rational_Box_t ph,
           ppl_const_Generator_t c));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_relation_with_Congruence
PPL_PROTO((ppl_const_Rational_Box_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_get_constraints
PPL_PROTO((ppl_const_Rational_Box_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_get_congruences
PPL_PROTO((ppl_const_Rational_Box_t ph,
           ppl_const_Congruence_System_t* pcs));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_get_minimized_constraints
PPL_PROTO((ppl_const_Rational_Box_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_get_minimized_congruences
PPL_PROTO((ppl_const_Rational_Box_t ph,
           ppl_const_Congruence_System_t* pcs));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_is_empty
PPL_PROTO((ppl_const_Rational_Box_t ph));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_is_universe
PPL_PROTO((ppl_const_Rational_Box_t ph));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_is_bounded
PPL_PROTO((ppl_const_Rational_Box_t ph));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_contains_integer_point
PPL_PROTO((ppl_const_Rational_Box_t ph));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_is_topologically_closed
PPL_PROTO((ppl_const_Rational_Box_t ph));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_is_discrete
PPL_PROTO((ppl_const_Rational_Box_t ph));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_topological_closure_assign
PPL_PROTO((ppl_Rational_Box_t ph));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_bounds_from_above
PPL_PROTO((ppl_const_Rational_Box_t ph,
           ppl_const_Linear_Expression_t le));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_bounds_from_below
PPL_PROTO((ppl_const_Rational_Box_t ph,
           ppl_const_Linear_Expression_t le));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_maximize
PPL_PROTO((ppl_const_Rational_Box_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_minimize
PPL_PROTO((ppl_const_Rational_Box_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_maximize_with_point
PPL_PROTO((ppl_const_Rational_Box_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_minimize_with_point
PPL_PROTO((ppl_const_Rational_Box_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_contains_Rational_Box
PPL_PROTO((ppl_const_Rational_Box_t x,
           ppl_const_Rational_Box_t y));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_strictly_contains_Rational_Box
PPL_PROTO((ppl_const_Rational_Box_t x,
           ppl_const_Rational_Box_t y));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_is_disjoint_from_Rational_Box
PPL_PROTO((ppl_const_Rational_Box_t x,
           ppl_const_Rational_Box_t y));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_equals_Rational_Box
PPL_PROTO((ppl_const_Rational_Box_t x,
           ppl_const_Rational_Box_t y));




/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_OK
PPL_PROTO((ppl_const_Rational_Box_t ph));




/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_add_constraint
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_add_congruence
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_add_constraints
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_add_congruences
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_refine_with_constraint
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_refine_with_congruence
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_refine_with_constraints
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_refine_with_congruences
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_intersection_assign
PPL_PROTO((ppl_Rational_Box_t x,
           ppl_const_Rational_Box_t y));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_upper_bound_assign
PPL_PROTO((ppl_Rational_Box_t x,
           ppl_const_Rational_Box_t y));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_difference_assign
PPL_PROTO((ppl_Rational_Box_t x,
           ppl_const_Rational_Box_t y));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_concatenate_assign
PPL_PROTO((ppl_Rational_Box_t x,
           ppl_const_Rational_Box_t y));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_time_elapse_assign
PPL_PROTO((ppl_Rational_Box_t x,
           ppl_const_Rational_Box_t y));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_upper_bound_assign_if_exact
PPL_PROTO((ppl_Rational_Box_t x,
           ppl_const_Rational_Box_t y));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_simplify_using_context_assign
PPL_PROTO((ppl_Rational_Box_t x,
           ppl_const_Rational_Box_t y));




/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_constrains
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_unconstrain_space_dimension
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_unconstrain_space_dimensions
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_affine_image
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_affine_preimage
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_bounded_affine_image
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_bounded_affine_preimage
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_generalized_affine_image
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_generalized_affine_preimage
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_generalized_affine_image_lhs_rhs
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_generalized_affine_preimage_lhs_rhs
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_add_space_dimensions_and_embed
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_dimension_type d));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_add_space_dimensions_and_project
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_dimension_type d));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_remove_space_dimensions
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_remove_higher_space_dimensions
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_dimension_type d));




/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_expand_space_dimension
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_dimension_type d,
           ppl_dimension_type m));




/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_fold_space_dimensions
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_dimension_type ds[],
           size_t n,
           ppl_dimension_type d));




/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_map_space_dimensions
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_dimension_type maps[],
           size_t n));






/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_external_memory_in_bytes
PPL_PROTO((ppl_const_Rational_Box_t ps,
           size_t* sz));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_total_memory_in_bytes
PPL_PROTO((ppl_const_Rational_Box_t ps,
           size_t* sz));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_CC76_widening_assign_with_tokens
PPL_PROTO((ppl_Rational_Box_t x,
           ppl_const_Rational_Box_t y,
           unsigned* tp));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_CC76_widening_assign
PPL_PROTO((ppl_Rational_Box_t x,
           ppl_const_Rational_Box_t y));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_widening_assign_with_tokens
PPL_PROTO((ppl_Rational_Box_t x,
           ppl_const_Rational_Box_t y,
           unsigned* tp));




/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_widening_assign
PPL_PROTO((ppl_Rational_Box_t x,
           ppl_const_Rational_Box_t y));




/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_limited_CC76_extrapolation_assign_with_tokens
PPL_PROTO((ppl_Rational_Box_t x,
           ppl_const_Rational_Box_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));







/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_limited_CC76_extrapolation_assign
PPL_PROTO((ppl_Rational_Box_t x,
           ppl_const_Rational_Box_t y,
           ppl_const_Constraint_System_t cs));







/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_linear_partition
PPL_PROTO((ppl_const_Rational_Box_t x,
           ppl_const_Rational_Box_t y,
           ppl_Rational_Box_t* p_inters,
           ppl_Pointset_Powerset_NNC_Polyhedron_t* p_rest));





/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_recycle_Constraint_System
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_Constraint_System_t cs));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_recycle_Congruence_System
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_Congruence_System_t cs));

/*! \relates ppl_Rational_Box_tag */
int
ppl_new_Rational_Box_recycle_Generator_System
PPL_PROTO((ppl_Rational_Box_t* pph, ppl_Generator_System_t cs));






/*! \relates ppl_Rational_Box_tag */
int
ppl_assign_Rational_Box_from_Rational_Box
PPL_PROTO((ppl_Rational_Box_t dst, ppl_const_Rational_Box_t src));





/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_add_recycled_constraints
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_Constraint_System_t cs));

/*! \relates ppl_Rational_Box_tag */
int
ppl_Rational_Box_add_recycled_congruences
PPL_PROTO((ppl_Rational_Box_t ph,
           ppl_Congruence_System_t cs));






PPL_DECLARE_IO_FUNCTIONS(Rational_Box)

/* End of Functions Related to ppl_Rational_Box_tag */

/*! \interface ppl_BD_Shape_mpz_class_tag
  \brief Types and functions for ppl_BD_Shape_mpz_class_tag
*/

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_delete_BD_Shape_mpz_class
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph));




/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_space_dimension
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_dimension_type d, int empty));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_C_Polyhedron
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_NNC_Polyhedron
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_Grid
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Grid_t ph));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_Rational_Box
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Rational_Box_t ph));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_BD_Shape_mpz_class
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_BD_Shape_mpz_class_t ph));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_BD_Shape_mpq_class
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_BD_Shape_mpq_class_t ph));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_Double_Box
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Double_Box_t ph));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_BD_Shape_double
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_BD_Shape_double_t ph));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_Octagonal_Shape_double
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Octagonal_Shape_double_t ph));






/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_C_Polyhedron_with_complexity
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_NNC_Polyhedron_with_complexity
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_Grid_with_complexity
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Grid_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_Rational_Box_with_complexity
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Rational_Box_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_BD_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_BD_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_BD_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_BD_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_Octagonal_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_Octagonal_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_Double_Box_with_complexity
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Double_Box_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_BD_Shape_double_with_complexity
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_BD_Shape_double_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_Octagonal_Shape_double_with_complexity
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Octagonal_Shape_double_t ph, int complexity));






/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_Constraint_System
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_Congruence_System
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Congruence_System_t cs));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_from_Generator_System
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_const_Generator_System_t cs));








/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_space_dimension
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph, ppl_dimension_type* m));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_affine_dimension
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph, ppl_dimension_type* m));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_relation_with_Constraint
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_relation_with_Generator
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph,
           ppl_const_Generator_t c));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_relation_with_Congruence
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_get_constraints
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_get_congruences
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph,
           ppl_const_Congruence_System_t* pcs));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_get_minimized_constraints
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_get_minimized_congruences
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph,
           ppl_const_Congruence_System_t* pcs));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_is_empty
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_is_universe
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_is_bounded
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_contains_integer_point
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_is_topologically_closed
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_is_discrete
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_topological_closure_assign
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_bounds_from_above
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph,
           ppl_const_Linear_Expression_t le));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_bounds_from_below
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph,
           ppl_const_Linear_Expression_t le));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_maximize
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_minimize
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_maximize_with_point
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_minimize_with_point
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_contains_BD_Shape_mpz_class
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_strictly_contains_BD_Shape_mpz_class
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_is_disjoint_from_BD_Shape_mpz_class
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_equals_BD_Shape_mpz_class
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y));




/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_OK
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ph));




/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_add_constraint
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_add_congruence
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_add_constraints
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_add_congruences
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_refine_with_constraint
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_refine_with_congruence
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_refine_with_constraints
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_refine_with_congruences
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_intersection_assign
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_upper_bound_assign
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_difference_assign
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_concatenate_assign
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_time_elapse_assign
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_upper_bound_assign_if_exact
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_simplify_using_context_assign
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y));




/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_constrains
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_dimension_type var));




/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_unconstrain_space_dimension
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_dimension_type var));




/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_unconstrain_space_dimensions
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_affine_image
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_affine_preimage
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_bounded_affine_image
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_bounded_affine_preimage
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_generalized_affine_image
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_generalized_affine_preimage
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_generalized_affine_image_lhs_rhs
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_generalized_affine_preimage_lhs_rhs
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_add_space_dimensions_and_embed
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_dimension_type d));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_add_space_dimensions_and_project
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_dimension_type d));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_remove_space_dimensions
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_remove_higher_space_dimensions
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_dimension_type d));




/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_expand_space_dimension
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_dimension_type d,
           ppl_dimension_type m));




/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_fold_space_dimensions
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_dimension_type ds[],
           size_t n,
           ppl_dimension_type d));




/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_map_space_dimensions
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_dimension_type maps[],
           size_t n));






/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_external_memory_in_bytes
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ps,
           size_t* sz));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_total_memory_in_bytes
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t ps,
           size_t* sz));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_BHMZ05_widening_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y,
           unsigned* tp));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_H79_widening_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y,
           unsigned* tp));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_BHMZ05_widening_assign
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_H79_widening_assign
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_widening_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y,
           unsigned* tp));




/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_widening_assign
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y));




/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_limited_BHMZ05_extrapolation_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_limited_H79_extrapolation_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_limited_CC76_extrapolation_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));







/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_limited_BHMZ05_extrapolation_assign
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_limited_H79_extrapolation_assign
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_limited_CC76_extrapolation_assign
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y,
           ppl_const_Constraint_System_t cs));







/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_CC76_extrapolation_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y,
           unsigned* tp));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_CC76_extrapolation_assign
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_CC76_narrowing_assign
PPL_PROTO((ppl_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_linear_partition
PPL_PROTO((ppl_const_BD_Shape_mpz_class_t x,
           ppl_const_BD_Shape_mpz_class_t y,
           ppl_BD_Shape_mpz_class_t* p_inters,
           ppl_Pointset_Powerset_NNC_Polyhedron_t* p_rest));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_recycle_Constraint_System
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_recycle_Congruence_System
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_Congruence_System_t cs));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_new_BD_Shape_mpz_class_recycle_Generator_System
PPL_PROTO((ppl_BD_Shape_mpz_class_t* pph, ppl_Generator_System_t cs));






/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_assign_BD_Shape_mpz_class_from_BD_Shape_mpz_class
PPL_PROTO((ppl_BD_Shape_mpz_class_t dst, ppl_const_BD_Shape_mpz_class_t src));





/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_add_recycled_constraints
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_mpz_class_tag */
int
ppl_BD_Shape_mpz_class_add_recycled_congruences
PPL_PROTO((ppl_BD_Shape_mpz_class_t ph,
           ppl_Congruence_System_t cs));






PPL_DECLARE_IO_FUNCTIONS(BD_Shape_mpz_class)

/* End of Functions Related to ppl_BD_Shape_mpz_class_tag */

/*! \interface ppl_BD_Shape_mpq_class_tag
  \brief Types and functions for ppl_BD_Shape_mpq_class_tag
*/

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_delete_BD_Shape_mpq_class
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph));




/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_space_dimension
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_dimension_type d, int empty));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_C_Polyhedron
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_NNC_Polyhedron
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_Grid
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Grid_t ph));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_Rational_Box
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Rational_Box_t ph));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_BD_Shape_mpz_class
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_BD_Shape_mpz_class_t ph));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_BD_Shape_mpq_class
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_BD_Shape_mpq_class_t ph));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_Double_Box
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Double_Box_t ph));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_BD_Shape_double
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_BD_Shape_double_t ph));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_Octagonal_Shape_double
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Octagonal_Shape_double_t ph));






/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_C_Polyhedron_with_complexity
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_NNC_Polyhedron_with_complexity
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_Grid_with_complexity
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Grid_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_Rational_Box_with_complexity
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Rational_Box_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_BD_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_BD_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_BD_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_BD_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_Octagonal_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_Octagonal_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_Double_Box_with_complexity
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Double_Box_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_BD_Shape_double_with_complexity
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_BD_Shape_double_t ph, int complexity));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_Octagonal_Shape_double_with_complexity
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Octagonal_Shape_double_t ph, int complexity));






/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_Constraint_System
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_Congruence_System
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Congruence_System_t cs));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_from_Generator_System
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_const_Generator_System_t cs));








/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_space_dimension
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph, ppl_dimension_type* m));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_affine_dimension
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph, ppl_dimension_type* m));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_relation_with_Constraint
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_relation_with_Generator
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph,
           ppl_const_Generator_t c));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_relation_with_Congruence
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_get_constraints
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_get_congruences
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph,
           ppl_const_Congruence_System_t* pcs));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_get_minimized_constraints
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_get_minimized_congruences
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph,
           ppl_const_Congruence_System_t* pcs));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_is_empty
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_is_universe
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_is_bounded
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_contains_integer_point
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_is_topologically_closed
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_is_discrete
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_topological_closure_assign
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_bounds_from_above
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph,
           ppl_const_Linear_Expression_t le));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_bounds_from_below
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph,
           ppl_const_Linear_Expression_t le));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_maximize
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_minimize
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_maximize_with_point
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_minimize_with_point
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_contains_BD_Shape_mpq_class
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_strictly_contains_BD_Shape_mpq_class
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_is_disjoint_from_BD_Shape_mpq_class
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_equals_BD_Shape_mpq_class
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y));




/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_OK
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ph));




/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_add_constraint
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_add_congruence
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_add_constraints
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_add_congruences
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_refine_with_constraint
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_refine_with_congruence
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_refine_with_constraints
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_refine_with_congruences
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_intersection_assign
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_upper_bound_assign
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_difference_assign
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_concatenate_assign
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_time_elapse_assign
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_upper_bound_assign_if_exact
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_simplify_using_context_assign
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y));




/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_constrains
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_dimension_type var));




/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_unconstrain_space_dimension
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_dimension_type var));




/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_unconstrain_space_dimensions
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_affine_image
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_affine_preimage
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_bounded_affine_image
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_bounded_affine_preimage
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_generalized_affine_image
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_generalized_affine_preimage
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_generalized_affine_image_lhs_rhs
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_generalized_affine_preimage_lhs_rhs
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_add_space_dimensions_and_embed
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_dimension_type d));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_add_space_dimensions_and_project
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_dimension_type d));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_remove_space_dimensions
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_remove_higher_space_dimensions
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_dimension_type d));




/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_expand_space_dimension
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_dimension_type d,
           ppl_dimension_type m));




/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_fold_space_dimensions
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_dimension_type ds[],
           size_t n,
           ppl_dimension_type d));




/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_map_space_dimensions
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_dimension_type maps[],
           size_t n));






/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_external_memory_in_bytes
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ps,
           size_t* sz));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_total_memory_in_bytes
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t ps,
           size_t* sz));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_BHMZ05_widening_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y,
           unsigned* tp));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_H79_widening_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y,
           unsigned* tp));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_BHMZ05_widening_assign
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_H79_widening_assign
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_widening_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y,
           unsigned* tp));




/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_widening_assign
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y));




/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_limited_BHMZ05_extrapolation_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_limited_H79_extrapolation_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_limited_CC76_extrapolation_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));







/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_limited_BHMZ05_extrapolation_assign
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_limited_H79_extrapolation_assign
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_limited_CC76_extrapolation_assign
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y,
           ppl_const_Constraint_System_t cs));







/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_CC76_extrapolation_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y,
           unsigned* tp));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_CC76_extrapolation_assign
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_CC76_narrowing_assign
PPL_PROTO((ppl_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_linear_partition
PPL_PROTO((ppl_const_BD_Shape_mpq_class_t x,
           ppl_const_BD_Shape_mpq_class_t y,
           ppl_BD_Shape_mpq_class_t* p_inters,
           ppl_Pointset_Powerset_NNC_Polyhedron_t* p_rest));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_recycle_Constraint_System
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_recycle_Congruence_System
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_Congruence_System_t cs));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_new_BD_Shape_mpq_class_recycle_Generator_System
PPL_PROTO((ppl_BD_Shape_mpq_class_t* pph, ppl_Generator_System_t cs));






/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_assign_BD_Shape_mpq_class_from_BD_Shape_mpq_class
PPL_PROTO((ppl_BD_Shape_mpq_class_t dst, ppl_const_BD_Shape_mpq_class_t src));





/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_add_recycled_constraints
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_mpq_class_tag */
int
ppl_BD_Shape_mpq_class_add_recycled_congruences
PPL_PROTO((ppl_BD_Shape_mpq_class_t ph,
           ppl_Congruence_System_t cs));






PPL_DECLARE_IO_FUNCTIONS(BD_Shape_mpq_class)

/* End of Functions Related to ppl_BD_Shape_mpq_class_tag */

/*! \interface ppl_Octagonal_Shape_mpz_class_tag
  \brief Types and functions for ppl_Octagonal_Shape_mpz_class_tag
*/

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_delete_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph));




/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_space_dimension
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_dimension_type d, int empty));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_C_Polyhedron
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_NNC_Polyhedron
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_Grid
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Grid_t ph));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_Rational_Box
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Rational_Box_t ph));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_BD_Shape_mpz_class
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_BD_Shape_mpz_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_BD_Shape_mpq_class
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_BD_Shape_mpq_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_Double_Box
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Double_Box_t ph));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_BD_Shape_double
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_BD_Shape_double_t ph));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_Octagonal_Shape_double
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Octagonal_Shape_double_t ph));






/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_C_Polyhedron_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_NNC_Polyhedron_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_Grid_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Grid_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_Rational_Box_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Rational_Box_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_BD_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_BD_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_BD_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_BD_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_Octagonal_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_Octagonal_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_Double_Box_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Double_Box_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_BD_Shape_double_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_BD_Shape_double_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_Octagonal_Shape_double_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Octagonal_Shape_double_t ph, int complexity));






/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_Constraint_System
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_Congruence_System
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Congruence_System_t cs));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_from_Generator_System
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_const_Generator_System_t cs));








/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_space_dimension
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph, ppl_dimension_type* m));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_affine_dimension
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph, ppl_dimension_type* m));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_relation_with_Constraint
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_relation_with_Generator
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Generator_t c));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_relation_with_Congruence
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_get_constraints
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_get_congruences
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Congruence_System_t* pcs));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_get_minimized_constraints
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_get_minimized_congruences
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Congruence_System_t* pcs));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_is_empty
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_is_universe
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_is_bounded
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_contains_integer_point
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_is_topologically_closed
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_is_discrete
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_topological_closure_assign
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_bounds_from_above
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Linear_Expression_t le));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_bounds_from_below
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Linear_Expression_t le));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_maximize
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_minimize
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_maximize_with_point
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_minimize_with_point
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_contains_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_strictly_contains_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_is_disjoint_from_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_equals_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y));




/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_OK
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ph));




/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_add_constraint
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_add_congruence
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_add_constraints
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_add_congruences
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_refine_with_constraint
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_refine_with_congruence
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_refine_with_constraints
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_refine_with_congruences
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_intersection_assign
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_upper_bound_assign
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_difference_assign
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_concatenate_assign
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_time_elapse_assign
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_upper_bound_assign_if_exact
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_simplify_using_context_assign
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y));




/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_constrains
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_unconstrain_space_dimension
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_unconstrain_space_dimensions
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_affine_image
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_affine_preimage
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_bounded_affine_image
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_bounded_affine_preimage
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_generalized_affine_image
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_generalized_affine_preimage
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_generalized_affine_image_lhs_rhs
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_generalized_affine_preimage_lhs_rhs
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_add_space_dimensions_and_embed
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_dimension_type d));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_add_space_dimensions_and_project
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_dimension_type d));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_remove_space_dimensions
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_remove_higher_space_dimensions
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_dimension_type d));




/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_expand_space_dimension
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_dimension_type d,
           ppl_dimension_type m));




/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_fold_space_dimensions
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_dimension_type ds[],
           size_t n,
           ppl_dimension_type d));




/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_map_space_dimensions
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_dimension_type maps[],
           size_t n));






/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_external_memory_in_bytes
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ps,
           size_t* sz));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_total_memory_in_bytes
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t ps,
           size_t* sz));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_BHMZ05_widening_assign_with_tokens
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y,
           unsigned* tp));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_BHMZ05_widening_assign
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_widening_assign_with_tokens
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y,
           unsigned* tp));




/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_widening_assign
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y));




/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_limited_BHMZ05_extrapolation_assign_with_tokens
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_limited_CC76_extrapolation_assign_with_tokens
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));







/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_limited_BHMZ05_extrapolation_assign
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_limited_CC76_extrapolation_assign
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y,
           ppl_const_Constraint_System_t cs));







/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_CC76_extrapolation_assign_with_tokens
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y,
           unsigned* tp));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_CC76_extrapolation_assign
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_CC76_narrowing_assign
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_linear_partition
PPL_PROTO((ppl_const_Octagonal_Shape_mpz_class_t x,
           ppl_const_Octagonal_Shape_mpz_class_t y,
           ppl_Octagonal_Shape_mpz_class_t* p_inters,
           ppl_Pointset_Powerset_NNC_Polyhedron_t* p_rest));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_recycle_Constraint_System
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_recycle_Congruence_System
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_Congruence_System_t cs));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_new_Octagonal_Shape_mpz_class_recycle_Generator_System
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t* pph, ppl_Generator_System_t cs));






/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_assign_Octagonal_Shape_mpz_class_from_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t dst, ppl_const_Octagonal_Shape_mpz_class_t src));





/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_add_recycled_constraints
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_mpz_class_tag */
int
ppl_Octagonal_Shape_mpz_class_add_recycled_congruences
PPL_PROTO((ppl_Octagonal_Shape_mpz_class_t ph,
           ppl_Congruence_System_t cs));






PPL_DECLARE_IO_FUNCTIONS(Octagonal_Shape_mpz_class)

/* End of Functions Related to ppl_Octagonal_Shape_mpz_class_tag */

/*! \interface ppl_Octagonal_Shape_mpq_class_tag
  \brief Types and functions for ppl_Octagonal_Shape_mpq_class_tag
*/

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_delete_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph));




/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_space_dimension
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_dimension_type d, int empty));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_C_Polyhedron
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_NNC_Polyhedron
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_Grid
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Grid_t ph));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_Rational_Box
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Rational_Box_t ph));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_BD_Shape_mpz_class
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_BD_Shape_mpz_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_BD_Shape_mpq_class
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_BD_Shape_mpq_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_Double_Box
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Double_Box_t ph));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_BD_Shape_double
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_BD_Shape_double_t ph));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_Octagonal_Shape_double
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Octagonal_Shape_double_t ph));






/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_C_Polyhedron_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_NNC_Polyhedron_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_Grid_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Grid_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_Rational_Box_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Rational_Box_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_BD_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_BD_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_BD_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_BD_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_Octagonal_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_Octagonal_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_Double_Box_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Double_Box_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_BD_Shape_double_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_BD_Shape_double_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_Octagonal_Shape_double_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Octagonal_Shape_double_t ph, int complexity));






/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_Constraint_System
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_Congruence_System
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Congruence_System_t cs));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_from_Generator_System
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_const_Generator_System_t cs));








/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_space_dimension
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph, ppl_dimension_type* m));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_affine_dimension
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph, ppl_dimension_type* m));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_relation_with_Constraint
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_relation_with_Generator
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Generator_t c));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_relation_with_Congruence
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_get_constraints
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_get_congruences
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Congruence_System_t* pcs));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_get_minimized_constraints
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_get_minimized_congruences
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Congruence_System_t* pcs));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_is_empty
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_is_universe
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_is_bounded
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_contains_integer_point
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_is_topologically_closed
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_is_discrete
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_topological_closure_assign
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_bounds_from_above
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Linear_Expression_t le));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_bounds_from_below
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Linear_Expression_t le));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_maximize
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_minimize
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_maximize_with_point
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_minimize_with_point
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_contains_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_strictly_contains_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_is_disjoint_from_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_equals_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y));




/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_OK
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ph));




/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_add_constraint
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_add_congruence
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_add_constraints
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_add_congruences
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_refine_with_constraint
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_refine_with_congruence
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_refine_with_constraints
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_refine_with_congruences
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_intersection_assign
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_upper_bound_assign
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_difference_assign
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_concatenate_assign
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_time_elapse_assign
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_upper_bound_assign_if_exact
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_simplify_using_context_assign
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y));




/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_constrains
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_unconstrain_space_dimension
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_unconstrain_space_dimensions
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_affine_image
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_affine_preimage
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_bounded_affine_image
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_bounded_affine_preimage
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_generalized_affine_image
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_generalized_affine_preimage
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_generalized_affine_image_lhs_rhs
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_generalized_affine_preimage_lhs_rhs
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_add_space_dimensions_and_embed
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_dimension_type d));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_add_space_dimensions_and_project
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_dimension_type d));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_remove_space_dimensions
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_remove_higher_space_dimensions
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_dimension_type d));




/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_expand_space_dimension
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_dimension_type d,
           ppl_dimension_type m));




/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_fold_space_dimensions
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_dimension_type ds[],
           size_t n,
           ppl_dimension_type d));




/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_map_space_dimensions
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_dimension_type maps[],
           size_t n));






/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_external_memory_in_bytes
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ps,
           size_t* sz));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_total_memory_in_bytes
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t ps,
           size_t* sz));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_BHMZ05_widening_assign_with_tokens
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y,
           unsigned* tp));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_BHMZ05_widening_assign
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_widening_assign_with_tokens
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y,
           unsigned* tp));




/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_widening_assign
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y));




/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_limited_BHMZ05_extrapolation_assign_with_tokens
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_limited_CC76_extrapolation_assign_with_tokens
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));







/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_limited_BHMZ05_extrapolation_assign
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_limited_CC76_extrapolation_assign
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y,
           ppl_const_Constraint_System_t cs));







/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_CC76_extrapolation_assign_with_tokens
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y,
           unsigned* tp));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_CC76_extrapolation_assign
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_CC76_narrowing_assign
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_linear_partition
PPL_PROTO((ppl_const_Octagonal_Shape_mpq_class_t x,
           ppl_const_Octagonal_Shape_mpq_class_t y,
           ppl_Octagonal_Shape_mpq_class_t* p_inters,
           ppl_Pointset_Powerset_NNC_Polyhedron_t* p_rest));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_recycle_Constraint_System
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_recycle_Congruence_System
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_Congruence_System_t cs));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_new_Octagonal_Shape_mpq_class_recycle_Generator_System
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t* pph, ppl_Generator_System_t cs));






/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_assign_Octagonal_Shape_mpq_class_from_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t dst, ppl_const_Octagonal_Shape_mpq_class_t src));





/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_add_recycled_constraints
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_mpq_class_tag */
int
ppl_Octagonal_Shape_mpq_class_add_recycled_congruences
PPL_PROTO((ppl_Octagonal_Shape_mpq_class_t ph,
           ppl_Congruence_System_t cs));






PPL_DECLARE_IO_FUNCTIONS(Octagonal_Shape_mpq_class)

/* End of Functions Related to ppl_Octagonal_Shape_mpq_class_tag */

/*! \interface ppl_Constraints_Product_C_Polyhedron_Grid_tag
  \brief Types and functions for ppl_Constraints_Product_C_Polyhedron_Grid_tag
*/

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_delete_Constraints_Product_C_Polyhedron_Grid
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph));




/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_space_dimension
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_dimension_type d, int empty));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_C_Polyhedron
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_NNC_Polyhedron
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_Grid
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Grid_t ph));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_Rational_Box
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Rational_Box_t ph));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_BD_Shape_mpz_class
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_BD_Shape_mpz_class_t ph));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_BD_Shape_mpq_class
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_BD_Shape_mpq_class_t ph));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_Double_Box
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Double_Box_t ph));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_BD_Shape_double
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_BD_Shape_double_t ph));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_Octagonal_Shape_double
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Octagonal_Shape_double_t ph));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_Constraints_Product_C_Polyhedron_Grid
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph));






/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_C_Polyhedron_with_complexity
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_NNC_Polyhedron_with_complexity
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_Grid_with_complexity
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Grid_t ph, int complexity));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_Rational_Box_with_complexity
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Rational_Box_t ph, int complexity));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_BD_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_BD_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_BD_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_BD_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_Octagonal_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_Octagonal_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_Double_Box_with_complexity
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Double_Box_t ph, int complexity));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_BD_Shape_double_with_complexity
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_BD_Shape_double_t ph, int complexity));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_Octagonal_Shape_double_with_complexity
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Octagonal_Shape_double_t ph, int complexity));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_Constraints_Product_C_Polyhedron_Grid_with_complexity
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph, int complexity));






/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_Constraint_System
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Constraint_System_t cs));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_new_Constraints_Product_C_Polyhedron_Grid_from_Congruence_System
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t* pph, ppl_const_Congruence_System_t cs));








/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_space_dimension
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph, ppl_dimension_type* m));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_affine_dimension
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph, ppl_dimension_type* m));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_relation_with_Constraint
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_relation_with_Generator
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Generator_t c));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_relation_with_Congruence
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_is_empty
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_is_universe
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_is_bounded
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_is_topologically_closed
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_is_discrete
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_topological_closure_assign
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_bounds_from_above
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Linear_Expression_t le));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_bounds_from_below
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Linear_Expression_t le));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_maximize
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_minimize
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_maximize_with_point
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_minimize_with_point
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_contains_Constraints_Product_C_Polyhedron_Grid
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t x,
           ppl_const_Constraints_Product_C_Polyhedron_Grid_t y));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_strictly_contains_Constraints_Product_C_Polyhedron_Grid
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t x,
           ppl_const_Constraints_Product_C_Polyhedron_Grid_t y));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_is_disjoint_from_Constraints_Product_C_Polyhedron_Grid
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t x,
           ppl_const_Constraints_Product_C_Polyhedron_Grid_t y));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_equals_Constraints_Product_C_Polyhedron_Grid
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t x,
           ppl_const_Constraints_Product_C_Polyhedron_Grid_t y));




/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_OK
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ph));




/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_add_constraint
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_add_congruence
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_add_constraints
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_add_congruences
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_refine_with_constraint
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_refine_with_congruence
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_refine_with_constraints
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_refine_with_congruences
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_intersection_assign
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t x,
           ppl_const_Constraints_Product_C_Polyhedron_Grid_t y));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_upper_bound_assign
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t x,
           ppl_const_Constraints_Product_C_Polyhedron_Grid_t y));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_difference_assign
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t x,
           ppl_const_Constraints_Product_C_Polyhedron_Grid_t y));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_concatenate_assign
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t x,
           ppl_const_Constraints_Product_C_Polyhedron_Grid_t y));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_time_elapse_assign
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t x,
           ppl_const_Constraints_Product_C_Polyhedron_Grid_t y));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_upper_bound_assign_if_exact
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t x,
           ppl_const_Constraints_Product_C_Polyhedron_Grid_t y));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_constrains
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_unconstrain_space_dimension
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_unconstrain_space_dimensions
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_affine_image
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_affine_preimage
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_bounded_affine_image
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_bounded_affine_preimage
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_generalized_affine_image
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_generalized_affine_preimage
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_generalized_affine_image_lhs_rhs
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_generalized_affine_preimage_lhs_rhs
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_add_space_dimensions_and_embed
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_dimension_type d));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_add_space_dimensions_and_project
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_dimension_type d));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_remove_space_dimensions
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_remove_higher_space_dimensions
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_dimension_type d));




/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_expand_space_dimension
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_dimension_type d,
           ppl_dimension_type m));




/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_fold_space_dimensions
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_dimension_type ds[],
           size_t n,
           ppl_dimension_type d));




/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_map_space_dimensions
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t ph,
           ppl_dimension_type maps[],
           size_t n));






/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_external_memory_in_bytes
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ps,
           size_t* sz));

/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_total_memory_in_bytes
PPL_PROTO((ppl_const_Constraints_Product_C_Polyhedron_Grid_t ps,
           size_t* sz));





/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_widening_assign_with_tokens
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t x,
           ppl_const_Constraints_Product_C_Polyhedron_Grid_t y,
           unsigned* tp));




/*! \relates ppl_Constraints_Product_C_Polyhedron_Grid_tag */
int
ppl_Constraints_Product_C_Polyhedron_Grid_widening_assign
PPL_PROTO((ppl_Constraints_Product_C_Polyhedron_Grid_t x,
           ppl_const_Constraints_Product_C_Polyhedron_Grid_t y));





PPL_DECLARE_IO_FUNCTIONS(Constraints_Product_C_Polyhedron_Grid)

/* End of Functions Related to ppl_Constraints_Product_C_Polyhedron_Grid_tag */

/*! \interface ppl_Pointset_Powerset_C_Polyhedron_tag
  \brief Types and functions for ppl_Pointset_Powerset_C_Polyhedron_tag
*/

/*! \interface ppl_Pointset_Powerset_C_Polyhedron_iterator_tag
  \brief Types and functions for iterating on the disjuncts of a
  ppl_Pointset_Powerset_C_Polyhedron_tag.
*/
/*! \interface ppl_Pointset_Powerset_C_Polyhedron_const_iterator_tag
  \brief Types and functions for iterating on the disjuncts of a
  const ppl_Pointset_Powerset_C_Polyhedron_tag.
*/

PPL_TYPE_DECLARATION(Pointset_Powerset_C_Polyhedron_iterator)
PPL_TYPE_DECLARATION(Pointset_Powerset_C_Polyhedron_const_iterator)

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_iterator_tag */
int
ppl_new_Pointset_Powerset_C_Polyhedron_iterator
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_iterator_t* pit));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_const_iterator_tag */
int
ppl_new_Pointset_Powerset_C_Polyhedron_const_iterator
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_const_iterator_t* pit));




/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_delete_Pointset_Powerset_C_Polyhedron
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph));




/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_new_Pointset_Powerset_C_Polyhedron_from_space_dimension
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t* pph, ppl_dimension_type d, int empty));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_new_Pointset_Powerset_C_Polyhedron_from_Pointset_Powerset_C_Polyhedron
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t* pph, ppl_const_Pointset_Powerset_C_Polyhedron_t ph));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_new_Pointset_Powerset_C_Polyhedron_from_C_Polyhedron
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t* pph, ppl_const_Polyhedron_t ph));






/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_new_Pointset_Powerset_C_Polyhedron_from_Pointset_Powerset_C_Polyhedron_with_complexity
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t* pph, ppl_const_Pointset_Powerset_C_Polyhedron_t ph, int complexity));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_new_Pointset_Powerset_C_Polyhedron_from_C_Polyhedron_with_complexity
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t* pph, ppl_const_Polyhedron_t ph, int complexity));






/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_new_Pointset_Powerset_C_Polyhedron_from_Constraint_System
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t* pph, ppl_const_Constraint_System_t cs));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_new_Pointset_Powerset_C_Polyhedron_from_Congruence_System
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t* pph, ppl_const_Congruence_System_t cs));








/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_space_dimension
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph, ppl_dimension_type* m));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_affine_dimension
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph, ppl_dimension_type* m));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_relation_with_Constraint
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_relation_with_Generator
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Generator_t c));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_relation_with_Congruence
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_is_empty
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_is_universe
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_is_bounded
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_contains_integer_point
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_is_topologically_closed
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_is_discrete
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_topological_closure_assign
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_pairwise_reduce
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_omega_reduce
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_bounds_from_above
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_bounds_from_below
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_maximize
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_minimize
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_maximize_with_point
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_minimize_with_point
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_contains_Pointset_Powerset_C_Polyhedron
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_strictly_contains_Pointset_Powerset_C_Polyhedron
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_is_disjoint_from_Pointset_Powerset_C_Polyhedron
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_geometrically_covers_Pointset_Powerset_C_Polyhedron
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_geometrically_equals_Pointset_Powerset_C_Polyhedron
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_t y));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_equals_Pointset_Powerset_C_Polyhedron
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_t y));




/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_OK
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ph));




/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_add_constraint
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_add_congruence
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_add_constraints
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_add_congruences
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_refine_with_constraint
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_refine_with_congruence
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_refine_with_constraints
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_refine_with_congruences
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_intersection_assign
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_upper_bound_assign
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_difference_assign
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_concatenate_assign
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_time_elapse_assign
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_t y));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_upper_bound_assign_if_exact
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_t y));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_simplify_using_context_assign
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_t y));




/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_constrains
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_unconstrain_space_dimension
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_unconstrain_space_dimensions
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_affine_image
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_affine_preimage
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_bounded_affine_image
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_bounded_affine_preimage
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_generalized_affine_image
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_generalized_affine_preimage
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_generalized_affine_image_lhs_rhs
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_generalized_affine_preimage_lhs_rhs
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_add_space_dimensions_and_embed
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_dimension_type d));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_add_space_dimensions_and_project
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_dimension_type d));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_remove_space_dimensions
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_remove_higher_space_dimensions
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_dimension_type d));




/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_expand_space_dimension
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_dimension_type d,
           ppl_dimension_type m));




/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_fold_space_dimensions
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_dimension_type ds[],
           size_t n,
           ppl_dimension_type d));




/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_map_space_dimensions
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ph,
           ppl_dimension_type maps[],
           size_t n));






/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_external_memory_in_bytes
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ps,
           size_t* sz));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_total_memory_in_bytes
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ps,
           size_t* sz));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_size
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ps,
           size_t* sz));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_iterator_tag */
int
ppl_new_Pointset_Powerset_C_Polyhedron_iterator_from_iterator
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_iterator_t* px,
           ppl_const_Pointset_Powerset_C_Polyhedron_iterator_t y));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_const_iterator_tag */
int
ppl_new_Pointset_Powerset_C_Polyhedron_const_iterator_from_const_iterator
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_const_iterator_t* px,
           ppl_const_Pointset_Powerset_C_Polyhedron_const_iterator_t y));




/*! \relates ppl_Pointset_Powerset_C_Polyhedron_iterator_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_iterator_begin
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ps,
           ppl_Pointset_Powerset_C_Polyhedron_iterator_t psit));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_const_iterator_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_const_iterator_begin
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ps,
           ppl_Pointset_Powerset_C_Polyhedron_const_iterator_t psit));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_iterator_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_iterator_end
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ps,
           ppl_Pointset_Powerset_C_Polyhedron_iterator_t psit));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_const_iterator_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_const_iterator_end
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_t ps,
           ppl_Pointset_Powerset_C_Polyhedron_const_iterator_t psit));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_iterator_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_iterator_equal_test
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_iterator_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_iterator_t y));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_const_iterator_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_const_iterator_equal_test
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_const_iterator_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_const_iterator_t y));




/*! \relates ppl_Pointset_Powerset_C_Polyhedron_iterator_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_iterator_increment
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_iterator_t psit));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_const_iterator_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_const_iterator_increment
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_const_iterator_t psit));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_iterator_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_iterator_decrement
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_iterator_t psit));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_const_iterator_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_const_iterator_decrement
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_const_iterator_t psit));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_iterator_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_iterator_dereference
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_iterator_t ps,
           ppl_const_Polyhedron_t* d));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_const_iterator_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_const_iterator_dereference
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_const_iterator_t ps,
           ppl_const_Polyhedron_t* d));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_iterator_tag */
int
ppl_delete_Pointset_Powerset_C_Polyhedron_iterator
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_iterator_t psit));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_const_iterator_tag */
int
ppl_delete_Pointset_Powerset_C_Polyhedron_const_iterator
PPL_PROTO((ppl_const_Pointset_Powerset_C_Polyhedron_const_iterator_t psit));




/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_add_disjunct
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ps,
           ppl_const_Polyhedron_t d));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_drop_disjunct
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ps,
 ppl_const_Pointset_Powerset_C_Polyhedron_iterator_t cit,
 ppl_Pointset_Powerset_C_Polyhedron_iterator_t it));




/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_drop_disjuncts
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t ps,
 ppl_const_Pointset_Powerset_C_Polyhedron_iterator_t first,
 ppl_const_Pointset_Powerset_C_Polyhedron_iterator_t last));




/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_BHZ03_BHRZ03_BHRZ03_widening_assign
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_BHZ03_H79_H79_widening_assign
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_t y));





/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_BGP99_BHRZ03_extrapolation_assign
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_t y,
           int disjuncts));

/*! \relates ppl_Pointset_Powerset_C_Polyhedron_tag */
int
ppl_Pointset_Powerset_C_Polyhedron_BGP99_H79_extrapolation_assign
PPL_PROTO((ppl_Pointset_Powerset_C_Polyhedron_t x,
           ppl_const_Pointset_Powerset_C_Polyhedron_t y,
           int disjuncts));






PPL_DECLARE_IO_FUNCTIONS(Pointset_Powerset_C_Polyhedron)

/* End of Functions Related to ppl_Pointset_Powerset_C_Polyhedron_tag */

/*! \interface ppl_Pointset_Powerset_NNC_Polyhedron_tag
  \brief Types and functions for ppl_Pointset_Powerset_NNC_Polyhedron_tag
*/

/*! \interface ppl_Pointset_Powerset_NNC_Polyhedron_iterator_tag
  \brief Types and functions for iterating on the disjuncts of a
  ppl_Pointset_Powerset_NNC_Polyhedron_tag.
*/
/*! \interface ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_tag
  \brief Types and functions for iterating on the disjuncts of a
  const ppl_Pointset_Powerset_NNC_Polyhedron_tag.
*/

PPL_TYPE_DECLARATION(Pointset_Powerset_NNC_Polyhedron_iterator)
PPL_TYPE_DECLARATION(Pointset_Powerset_NNC_Polyhedron_const_iterator)

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_iterator_tag */
int
ppl_new_Pointset_Powerset_NNC_Polyhedron_iterator
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_iterator_t* pit));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_tag */
int
ppl_new_Pointset_Powerset_NNC_Polyhedron_const_iterator
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_t* pit));




/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_delete_Pointset_Powerset_NNC_Polyhedron
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph));




/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_new_Pointset_Powerset_NNC_Polyhedron_from_space_dimension
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t* pph, ppl_dimension_type d, int empty));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_new_Pointset_Powerset_NNC_Polyhedron_from_Pointset_Powerset_NNC_Polyhedron
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t* pph, ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_new_Pointset_Powerset_NNC_Polyhedron_from_NNC_Polyhedron
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t* pph, ppl_const_Polyhedron_t ph));






/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_new_Pointset_Powerset_NNC_Polyhedron_from_Pointset_Powerset_NNC_Polyhedron_with_complexity
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t* pph, ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph, int complexity));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_new_Pointset_Powerset_NNC_Polyhedron_from_NNC_Polyhedron_with_complexity
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t* pph, ppl_const_Polyhedron_t ph, int complexity));






/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_new_Pointset_Powerset_NNC_Polyhedron_from_Constraint_System
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t* pph, ppl_const_Constraint_System_t cs));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_new_Pointset_Powerset_NNC_Polyhedron_from_Congruence_System
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t* pph, ppl_const_Congruence_System_t cs));








/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_space_dimension
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph, ppl_dimension_type* m));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_affine_dimension
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph, ppl_dimension_type* m));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_relation_with_Constraint
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_relation_with_Generator
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Generator_t c));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_relation_with_Congruence
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_is_empty
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_is_universe
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_is_bounded
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_contains_integer_point
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_is_topologically_closed
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_is_discrete
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_topological_closure_assign
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_pairwise_reduce
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_omega_reduce
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_bounds_from_above
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_bounds_from_below
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_maximize
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_minimize
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_maximize_with_point
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_minimize_with_point
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_contains_Pointset_Powerset_NNC_Polyhedron
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_strictly_contains_Pointset_Powerset_NNC_Polyhedron
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_is_disjoint_from_Pointset_Powerset_NNC_Polyhedron
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_geometrically_covers_Pointset_Powerset_NNC_Polyhedron
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_geometrically_equals_Pointset_Powerset_NNC_Polyhedron
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_t y));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_equals_Pointset_Powerset_NNC_Polyhedron
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_t y));




/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_OK
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ph));




/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_add_constraint
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_add_congruence
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_add_constraints
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_add_congruences
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_refine_with_constraint
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_refine_with_congruence
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_refine_with_constraints
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_refine_with_congruences
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_intersection_assign
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_upper_bound_assign
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_difference_assign
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_concatenate_assign
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_time_elapse_assign
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_t y));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_upper_bound_assign_if_exact
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_t y));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_simplify_using_context_assign
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_t y));




/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_constrains
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_unconstrain_space_dimension
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_unconstrain_space_dimensions
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_affine_image
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_affine_preimage
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_bounded_affine_image
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_bounded_affine_preimage
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_generalized_affine_image
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_generalized_affine_preimage
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_generalized_affine_image_lhs_rhs
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_generalized_affine_preimage_lhs_rhs
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_add_space_dimensions_and_embed
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_dimension_type d));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_add_space_dimensions_and_project
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_dimension_type d));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_remove_space_dimensions
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_remove_higher_space_dimensions
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_dimension_type d));




/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_expand_space_dimension
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_dimension_type d,
           ppl_dimension_type m));




/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_fold_space_dimensions
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_dimension_type ds[],
           size_t n,
           ppl_dimension_type d));




/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_map_space_dimensions
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ph,
           ppl_dimension_type maps[],
           size_t n));






/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_external_memory_in_bytes
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ps,
           size_t* sz));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_total_memory_in_bytes
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ps,
           size_t* sz));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_size
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ps,
           size_t* sz));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_iterator_tag */
int
ppl_new_Pointset_Powerset_NNC_Polyhedron_iterator_from_iterator
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_iterator_t* px,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_iterator_t y));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_tag */
int
ppl_new_Pointset_Powerset_NNC_Polyhedron_const_iterator_from_const_iterator
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_t* px,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_const_iterator_t y));




/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_iterator_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_iterator_begin
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ps,
           ppl_Pointset_Powerset_NNC_Polyhedron_iterator_t psit));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_begin
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ps,
           ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_t psit));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_iterator_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_iterator_end
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ps,
           ppl_Pointset_Powerset_NNC_Polyhedron_iterator_t psit));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_end
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_t ps,
           ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_t psit));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_iterator_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_iterator_equal_test
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_iterator_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_iterator_t y));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_equal_test
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_const_iterator_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_const_iterator_t y));




/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_iterator_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_iterator_increment
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_iterator_t psit));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_increment
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_t psit));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_iterator_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_iterator_decrement
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_iterator_t psit));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_decrement
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_t psit));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_iterator_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_iterator_dereference
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_iterator_t ps,
           ppl_const_Polyhedron_t* d));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_dereference
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_const_iterator_t ps,
           ppl_const_Polyhedron_t* d));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_iterator_tag */
int
ppl_delete_Pointset_Powerset_NNC_Polyhedron_iterator
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_iterator_t psit));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_const_iterator_tag */
int
ppl_delete_Pointset_Powerset_NNC_Polyhedron_const_iterator
PPL_PROTO((ppl_const_Pointset_Powerset_NNC_Polyhedron_const_iterator_t psit));




/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_add_disjunct
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ps,
           ppl_const_Polyhedron_t d));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_drop_disjunct
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ps,
 ppl_const_Pointset_Powerset_NNC_Polyhedron_iterator_t cit,
 ppl_Pointset_Powerset_NNC_Polyhedron_iterator_t it));




/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_drop_disjuncts
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t ps,
 ppl_const_Pointset_Powerset_NNC_Polyhedron_iterator_t first,
 ppl_const_Pointset_Powerset_NNC_Polyhedron_iterator_t last));




/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_BHZ03_BHRZ03_BHRZ03_widening_assign
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_t y));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_BHZ03_H79_H79_widening_assign
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_t y));





/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_BGP99_BHRZ03_extrapolation_assign
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_t y,
           int disjuncts));

/*! \relates ppl_Pointset_Powerset_NNC_Polyhedron_tag */
int
ppl_Pointset_Powerset_NNC_Polyhedron_BGP99_H79_extrapolation_assign
PPL_PROTO((ppl_Pointset_Powerset_NNC_Polyhedron_t x,
           ppl_const_Pointset_Powerset_NNC_Polyhedron_t y,
           int disjuncts));






PPL_DECLARE_IO_FUNCTIONS(Pointset_Powerset_NNC_Polyhedron)

/* End of Functions Related to ppl_Pointset_Powerset_NNC_Polyhedron_tag */

/*! \interface ppl_Double_Box_tag
  \brief Types and functions for ppl_Double_Box_tag
*/

/*! \relates ppl_Double_Box_tag */
int
ppl_delete_Double_Box
PPL_PROTO((ppl_const_Double_Box_t ph));




/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_space_dimension
PPL_PROTO((ppl_Double_Box_t* pph, ppl_dimension_type d, int empty));





/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_C_Polyhedron
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_NNC_Polyhedron
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_Grid
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Grid_t ph));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_Rational_Box
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Rational_Box_t ph));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_BD_Shape_mpz_class
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_BD_Shape_mpz_class_t ph));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_BD_Shape_mpq_class
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_BD_Shape_mpq_class_t ph));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_Double_Box
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Double_Box_t ph));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_BD_Shape_double
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_BD_Shape_double_t ph));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_Octagonal_Shape_double
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Octagonal_Shape_double_t ph));






/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_C_Polyhedron_with_complexity
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_NNC_Polyhedron_with_complexity
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_Grid_with_complexity
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Grid_t ph, int complexity));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_Rational_Box_with_complexity
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Rational_Box_t ph, int complexity));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_BD_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_BD_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_BD_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_BD_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_Octagonal_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_Octagonal_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_Double_Box_with_complexity
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Double_Box_t ph, int complexity));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_BD_Shape_double_with_complexity
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_BD_Shape_double_t ph, int complexity));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_Octagonal_Shape_double_with_complexity
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Octagonal_Shape_double_t ph, int complexity));






/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_Constraint_System
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Constraint_System_t cs));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_Congruence_System
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Congruence_System_t cs));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_from_Generator_System
PPL_PROTO((ppl_Double_Box_t* pph, ppl_const_Generator_System_t cs));








/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_space_dimension
PPL_PROTO((ppl_const_Double_Box_t ph, ppl_dimension_type* m));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_affine_dimension
PPL_PROTO((ppl_const_Double_Box_t ph, ppl_dimension_type* m));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_relation_with_Constraint
PPL_PROTO((ppl_const_Double_Box_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_relation_with_Generator
PPL_PROTO((ppl_const_Double_Box_t ph,
           ppl_const_Generator_t c));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_relation_with_Congruence
PPL_PROTO((ppl_const_Double_Box_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_get_constraints
PPL_PROTO((ppl_const_Double_Box_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_get_congruences
PPL_PROTO((ppl_const_Double_Box_t ph,
           ppl_const_Congruence_System_t* pcs));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_get_minimized_constraints
PPL_PROTO((ppl_const_Double_Box_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_get_minimized_congruences
PPL_PROTO((ppl_const_Double_Box_t ph,
           ppl_const_Congruence_System_t* pcs));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_is_empty
PPL_PROTO((ppl_const_Double_Box_t ph));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_is_universe
PPL_PROTO((ppl_const_Double_Box_t ph));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_is_bounded
PPL_PROTO((ppl_const_Double_Box_t ph));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_contains_integer_point
PPL_PROTO((ppl_const_Double_Box_t ph));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_is_topologically_closed
PPL_PROTO((ppl_const_Double_Box_t ph));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_is_discrete
PPL_PROTO((ppl_const_Double_Box_t ph));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_topological_closure_assign
PPL_PROTO((ppl_Double_Box_t ph));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_bounds_from_above
PPL_PROTO((ppl_const_Double_Box_t ph,
           ppl_const_Linear_Expression_t le));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_bounds_from_below
PPL_PROTO((ppl_const_Double_Box_t ph,
           ppl_const_Linear_Expression_t le));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_maximize
PPL_PROTO((ppl_const_Double_Box_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_minimize
PPL_PROTO((ppl_const_Double_Box_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_maximize_with_point
PPL_PROTO((ppl_const_Double_Box_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_minimize_with_point
PPL_PROTO((ppl_const_Double_Box_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_contains_Double_Box
PPL_PROTO((ppl_const_Double_Box_t x,
           ppl_const_Double_Box_t y));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_strictly_contains_Double_Box
PPL_PROTO((ppl_const_Double_Box_t x,
           ppl_const_Double_Box_t y));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_is_disjoint_from_Double_Box
PPL_PROTO((ppl_const_Double_Box_t x,
           ppl_const_Double_Box_t y));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_equals_Double_Box
PPL_PROTO((ppl_const_Double_Box_t x,
           ppl_const_Double_Box_t y));




/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_OK
PPL_PROTO((ppl_const_Double_Box_t ph));




/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_add_constraint
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_add_congruence
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_add_constraints
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_add_congruences
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_refine_with_constraint
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_refine_with_congruence
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_refine_with_constraints
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_refine_with_congruences
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_intersection_assign
PPL_PROTO((ppl_Double_Box_t x,
           ppl_const_Double_Box_t y));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_upper_bound_assign
PPL_PROTO((ppl_Double_Box_t x,
           ppl_const_Double_Box_t y));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_difference_assign
PPL_PROTO((ppl_Double_Box_t x,
           ppl_const_Double_Box_t y));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_concatenate_assign
PPL_PROTO((ppl_Double_Box_t x,
           ppl_const_Double_Box_t y));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_time_elapse_assign
PPL_PROTO((ppl_Double_Box_t x,
           ppl_const_Double_Box_t y));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_upper_bound_assign_if_exact
PPL_PROTO((ppl_Double_Box_t x,
           ppl_const_Double_Box_t y));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_simplify_using_context_assign
PPL_PROTO((ppl_Double_Box_t x,
           ppl_const_Double_Box_t y));




/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_constrains
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_unconstrain_space_dimension
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_unconstrain_space_dimensions
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_affine_image
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_affine_preimage
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_bounded_affine_image
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_bounded_affine_preimage
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_generalized_affine_image
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_generalized_affine_preimage
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_generalized_affine_image_lhs_rhs
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_generalized_affine_preimage_lhs_rhs
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_add_space_dimensions_and_embed
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_dimension_type d));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_add_space_dimensions_and_project
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_dimension_type d));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_remove_space_dimensions
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_remove_higher_space_dimensions
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_dimension_type d));




/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_expand_space_dimension
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_dimension_type d,
           ppl_dimension_type m));




/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_fold_space_dimensions
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_dimension_type ds[],
           size_t n,
           ppl_dimension_type d));




/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_map_space_dimensions
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_dimension_type maps[],
           size_t n));






/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_external_memory_in_bytes
PPL_PROTO((ppl_const_Double_Box_t ps,
           size_t* sz));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_total_memory_in_bytes
PPL_PROTO((ppl_const_Double_Box_t ps,
           size_t* sz));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_CC76_widening_assign_with_tokens
PPL_PROTO((ppl_Double_Box_t x,
           ppl_const_Double_Box_t y,
           unsigned* tp));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_CC76_widening_assign
PPL_PROTO((ppl_Double_Box_t x,
           ppl_const_Double_Box_t y));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_widening_assign_with_tokens
PPL_PROTO((ppl_Double_Box_t x,
           ppl_const_Double_Box_t y,
           unsigned* tp));




/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_widening_assign
PPL_PROTO((ppl_Double_Box_t x,
           ppl_const_Double_Box_t y));




/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_limited_CC76_extrapolation_assign_with_tokens
PPL_PROTO((ppl_Double_Box_t x,
           ppl_const_Double_Box_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));







/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_limited_CC76_extrapolation_assign
PPL_PROTO((ppl_Double_Box_t x,
           ppl_const_Double_Box_t y,
           ppl_const_Constraint_System_t cs));







/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_linear_partition
PPL_PROTO((ppl_const_Double_Box_t x,
           ppl_const_Double_Box_t y,
           ppl_Double_Box_t* p_inters,
           ppl_Pointset_Powerset_NNC_Polyhedron_t* p_rest));





/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_recycle_Constraint_System
PPL_PROTO((ppl_Double_Box_t* pph, ppl_Constraint_System_t cs));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_recycle_Congruence_System
PPL_PROTO((ppl_Double_Box_t* pph, ppl_Congruence_System_t cs));

/*! \relates ppl_Double_Box_tag */
int
ppl_new_Double_Box_recycle_Generator_System
PPL_PROTO((ppl_Double_Box_t* pph, ppl_Generator_System_t cs));






/*! \relates ppl_Double_Box_tag */
int
ppl_assign_Double_Box_from_Double_Box
PPL_PROTO((ppl_Double_Box_t dst, ppl_const_Double_Box_t src));





/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_add_recycled_constraints
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_Constraint_System_t cs));

/*! \relates ppl_Double_Box_tag */
int
ppl_Double_Box_add_recycled_congruences
PPL_PROTO((ppl_Double_Box_t ph,
           ppl_Congruence_System_t cs));






PPL_DECLARE_IO_FUNCTIONS(Double_Box)

/* End of Functions Related to ppl_Double_Box_tag */

/*! \interface ppl_BD_Shape_double_tag
  \brief Types and functions for ppl_BD_Shape_double_tag
*/

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_delete_BD_Shape_double
PPL_PROTO((ppl_const_BD_Shape_double_t ph));




/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_space_dimension
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_dimension_type d, int empty));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_C_Polyhedron
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_NNC_Polyhedron
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_Grid
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Grid_t ph));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_Rational_Box
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Rational_Box_t ph));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_BD_Shape_mpz_class
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_BD_Shape_mpz_class_t ph));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_BD_Shape_mpq_class
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_BD_Shape_mpq_class_t ph));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_Double_Box
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Double_Box_t ph));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_BD_Shape_double
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_BD_Shape_double_t ph));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_Octagonal_Shape_double
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Octagonal_Shape_double_t ph));






/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_C_Polyhedron_with_complexity
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_NNC_Polyhedron_with_complexity
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_Grid_with_complexity
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Grid_t ph, int complexity));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_Rational_Box_with_complexity
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Rational_Box_t ph, int complexity));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_BD_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_BD_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_BD_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_BD_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_Octagonal_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_Octagonal_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_Double_Box_with_complexity
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Double_Box_t ph, int complexity));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_BD_Shape_double_with_complexity
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_BD_Shape_double_t ph, int complexity));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_Octagonal_Shape_double_with_complexity
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Octagonal_Shape_double_t ph, int complexity));






/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_Constraint_System
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_Congruence_System
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Congruence_System_t cs));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_from_Generator_System
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_const_Generator_System_t cs));








/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_space_dimension
PPL_PROTO((ppl_const_BD_Shape_double_t ph, ppl_dimension_type* m));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_affine_dimension
PPL_PROTO((ppl_const_BD_Shape_double_t ph, ppl_dimension_type* m));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_relation_with_Constraint
PPL_PROTO((ppl_const_BD_Shape_double_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_relation_with_Generator
PPL_PROTO((ppl_const_BD_Shape_double_t ph,
           ppl_const_Generator_t c));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_relation_with_Congruence
PPL_PROTO((ppl_const_BD_Shape_double_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_get_constraints
PPL_PROTO((ppl_const_BD_Shape_double_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_get_congruences
PPL_PROTO((ppl_const_BD_Shape_double_t ph,
           ppl_const_Congruence_System_t* pcs));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_get_minimized_constraints
PPL_PROTO((ppl_const_BD_Shape_double_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_get_minimized_congruences
PPL_PROTO((ppl_const_BD_Shape_double_t ph,
           ppl_const_Congruence_System_t* pcs));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_is_empty
PPL_PROTO((ppl_const_BD_Shape_double_t ph));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_is_universe
PPL_PROTO((ppl_const_BD_Shape_double_t ph));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_is_bounded
PPL_PROTO((ppl_const_BD_Shape_double_t ph));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_contains_integer_point
PPL_PROTO((ppl_const_BD_Shape_double_t ph));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_is_topologically_closed
PPL_PROTO((ppl_const_BD_Shape_double_t ph));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_is_discrete
PPL_PROTO((ppl_const_BD_Shape_double_t ph));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_topological_closure_assign
PPL_PROTO((ppl_BD_Shape_double_t ph));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_bounds_from_above
PPL_PROTO((ppl_const_BD_Shape_double_t ph,
           ppl_const_Linear_Expression_t le));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_bounds_from_below
PPL_PROTO((ppl_const_BD_Shape_double_t ph,
           ppl_const_Linear_Expression_t le));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_maximize
PPL_PROTO((ppl_const_BD_Shape_double_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_minimize
PPL_PROTO((ppl_const_BD_Shape_double_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_maximize_with_point
PPL_PROTO((ppl_const_BD_Shape_double_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_minimize_with_point
PPL_PROTO((ppl_const_BD_Shape_double_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_contains_BD_Shape_double
PPL_PROTO((ppl_const_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_strictly_contains_BD_Shape_double
PPL_PROTO((ppl_const_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_is_disjoint_from_BD_Shape_double
PPL_PROTO((ppl_const_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_equals_BD_Shape_double
PPL_PROTO((ppl_const_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y));




/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_OK
PPL_PROTO((ppl_const_BD_Shape_double_t ph));




/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_add_constraint
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_add_congruence
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_add_constraints
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_add_congruences
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_refine_with_constraint
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_refine_with_congruence
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_refine_with_constraints
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_refine_with_congruences
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_intersection_assign
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_upper_bound_assign
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_difference_assign
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_concatenate_assign
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_time_elapse_assign
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_upper_bound_assign_if_exact
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_simplify_using_context_assign
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y));




/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_constrains
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_dimension_type var));




/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_unconstrain_space_dimension
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_dimension_type var));




/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_unconstrain_space_dimensions
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_affine_image
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_affine_preimage
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_bounded_affine_image
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_bounded_affine_preimage
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_generalized_affine_image
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_generalized_affine_preimage
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_generalized_affine_image_lhs_rhs
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_generalized_affine_preimage_lhs_rhs
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_add_space_dimensions_and_embed
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_dimension_type d));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_add_space_dimensions_and_project
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_dimension_type d));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_remove_space_dimensions
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_remove_higher_space_dimensions
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_dimension_type d));




/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_expand_space_dimension
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_dimension_type d,
           ppl_dimension_type m));




/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_fold_space_dimensions
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_dimension_type ds[],
           size_t n,
           ppl_dimension_type d));




/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_map_space_dimensions
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_dimension_type maps[],
           size_t n));






/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_external_memory_in_bytes
PPL_PROTO((ppl_const_BD_Shape_double_t ps,
           size_t* sz));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_total_memory_in_bytes
PPL_PROTO((ppl_const_BD_Shape_double_t ps,
           size_t* sz));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_BHMZ05_widening_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y,
           unsigned* tp));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_H79_widening_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y,
           unsigned* tp));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_BHMZ05_widening_assign
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_H79_widening_assign
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_widening_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y,
           unsigned* tp));




/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_widening_assign
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y));




/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_limited_BHMZ05_extrapolation_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_limited_H79_extrapolation_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_limited_CC76_extrapolation_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));







/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_limited_BHMZ05_extrapolation_assign
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_limited_H79_extrapolation_assign
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_limited_CC76_extrapolation_assign
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y,
           ppl_const_Constraint_System_t cs));







/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_CC76_extrapolation_assign_with_tokens
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y,
           unsigned* tp));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_CC76_extrapolation_assign
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_CC76_narrowing_assign
PPL_PROTO((ppl_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_linear_partition
PPL_PROTO((ppl_const_BD_Shape_double_t x,
           ppl_const_BD_Shape_double_t y,
           ppl_BD_Shape_double_t* p_inters,
           ppl_Pointset_Powerset_NNC_Polyhedron_t* p_rest));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_recycle_Constraint_System
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_recycle_Congruence_System
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_Congruence_System_t cs));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_new_BD_Shape_double_recycle_Generator_System
PPL_PROTO((ppl_BD_Shape_double_t* pph, ppl_Generator_System_t cs));






/*! \relates ppl_BD_Shape_double_tag */
int
ppl_assign_BD_Shape_double_from_BD_Shape_double
PPL_PROTO((ppl_BD_Shape_double_t dst, ppl_const_BD_Shape_double_t src));





/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_add_recycled_constraints
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_Constraint_System_t cs));

/*! \relates ppl_BD_Shape_double_tag */
int
ppl_BD_Shape_double_add_recycled_congruences
PPL_PROTO((ppl_BD_Shape_double_t ph,
           ppl_Congruence_System_t cs));






PPL_DECLARE_IO_FUNCTIONS(BD_Shape_double)

/* End of Functions Related to ppl_BD_Shape_double_tag */

/*! \interface ppl_Octagonal_Shape_double_tag
  \brief Types and functions for ppl_Octagonal_Shape_double_tag
*/

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_delete_Octagonal_Shape_double
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph));




/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_space_dimension
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_dimension_type d, int empty));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_C_Polyhedron
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_NNC_Polyhedron
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Polyhedron_t ph));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_Grid
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Grid_t ph));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_Rational_Box
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Rational_Box_t ph));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_BD_Shape_mpz_class
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_BD_Shape_mpz_class_t ph));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_BD_Shape_mpq_class
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_BD_Shape_mpq_class_t ph));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_Octagonal_Shape_mpz_class
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_Octagonal_Shape_mpq_class
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_Double_Box
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Double_Box_t ph));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_BD_Shape_double
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_BD_Shape_double_t ph));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_Octagonal_Shape_double
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Octagonal_Shape_double_t ph));






/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_C_Polyhedron_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_NNC_Polyhedron_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Polyhedron_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_Grid_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Grid_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_Rational_Box_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Rational_Box_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_BD_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_BD_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_BD_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_BD_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_Octagonal_Shape_mpz_class_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Octagonal_Shape_mpz_class_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_Octagonal_Shape_mpq_class_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Octagonal_Shape_mpq_class_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_Double_Box_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Double_Box_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_BD_Shape_double_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_BD_Shape_double_t ph, int complexity));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_Octagonal_Shape_double_with_complexity
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Octagonal_Shape_double_t ph, int complexity));






/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_Constraint_System
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_Congruence_System
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Congruence_System_t cs));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_from_Generator_System
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_const_Generator_System_t cs));








/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_space_dimension
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph, ppl_dimension_type* m));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_affine_dimension
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph, ppl_dimension_type* m));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_relation_with_Constraint
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_relation_with_Generator
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph,
           ppl_const_Generator_t c));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_relation_with_Congruence
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_get_constraints
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_get_congruences
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph,
           ppl_const_Congruence_System_t* pcs));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_get_minimized_constraints
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph,
           ppl_const_Constraint_System_t* pcs));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_get_minimized_congruences
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph,
           ppl_const_Congruence_System_t* pcs));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_is_empty
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_is_universe
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_is_bounded
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_contains_integer_point
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_is_topologically_closed
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_is_discrete
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_topological_closure_assign
PPL_PROTO((ppl_Octagonal_Shape_double_t ph));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_bounds_from_above
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph,
           ppl_const_Linear_Expression_t le));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_bounds_from_below
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph,
           ppl_const_Linear_Expression_t le));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_maximize
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_minimize
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_maximize_with_point
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_minimize_with_point
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_contains_Octagonal_Shape_double
PPL_PROTO((ppl_const_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_strictly_contains_Octagonal_Shape_double
PPL_PROTO((ppl_const_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_is_disjoint_from_Octagonal_Shape_double
PPL_PROTO((ppl_const_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_equals_Octagonal_Shape_double
PPL_PROTO((ppl_const_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y));




/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_OK
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ph));




/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_add_constraint
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_add_congruence
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_add_constraints
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_add_congruences
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_refine_with_constraint
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_const_Constraint_t c));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_refine_with_congruence
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_const_Congruence_t c));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_refine_with_constraints
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_refine_with_congruences
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_const_Congruence_System_t cs));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_intersection_assign
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_upper_bound_assign
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_difference_assign
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_concatenate_assign
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_time_elapse_assign
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_upper_bound_assign_if_exact
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_simplify_using_context_assign
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y));




/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_constrains
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_unconstrain_space_dimension
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_dimension_type var));




/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_unconstrain_space_dimensions
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_affine_image
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_affine_preimage
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_bounded_affine_image
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_bounded_affine_preimage
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_generalized_affine_image
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_generalized_affine_preimage
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_generalized_affine_image_lhs_rhs
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_generalized_affine_preimage_lhs_rhs
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_add_space_dimensions_and_embed
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_dimension_type d));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_add_space_dimensions_and_project
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_dimension_type d));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_remove_space_dimensions
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_dimension_type ds[],
           size_t n));




/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_remove_higher_space_dimensions
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_dimension_type d));




/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_expand_space_dimension
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_dimension_type d,
           ppl_dimension_type m));




/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_fold_space_dimensions
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_dimension_type ds[],
           size_t n,
           ppl_dimension_type d));




/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_map_space_dimensions
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_dimension_type maps[],
           size_t n));






/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_external_memory_in_bytes
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ps,
           size_t* sz));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_total_memory_in_bytes
PPL_PROTO((ppl_const_Octagonal_Shape_double_t ps,
           size_t* sz));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_BHMZ05_widening_assign_with_tokens
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y,
           unsigned* tp));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_BHMZ05_widening_assign
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_widening_assign_with_tokens
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y,
           unsigned* tp));




/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_widening_assign
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y));




/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_limited_BHMZ05_extrapolation_assign_with_tokens
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_limited_CC76_extrapolation_assign_with_tokens
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y,
           ppl_const_Constraint_System_t cs,
           unsigned* tp));







/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_limited_BHMZ05_extrapolation_assign
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y,
           ppl_const_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_limited_CC76_extrapolation_assign
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y,
           ppl_const_Constraint_System_t cs));







/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_CC76_extrapolation_assign_with_tokens
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y,
           unsigned* tp));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_CC76_extrapolation_assign
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_CC76_narrowing_assign
PPL_PROTO((ppl_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_linear_partition
PPL_PROTO((ppl_const_Octagonal_Shape_double_t x,
           ppl_const_Octagonal_Shape_double_t y,
           ppl_Octagonal_Shape_double_t* p_inters,
           ppl_Pointset_Powerset_NNC_Polyhedron_t* p_rest));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_recycle_Constraint_System
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_recycle_Congruence_System
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_Congruence_System_t cs));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_new_Octagonal_Shape_double_recycle_Generator_System
PPL_PROTO((ppl_Octagonal_Shape_double_t* pph, ppl_Generator_System_t cs));






/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_assign_Octagonal_Shape_double_from_Octagonal_Shape_double
PPL_PROTO((ppl_Octagonal_Shape_double_t dst, ppl_const_Octagonal_Shape_double_t src));





/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_add_recycled_constraints
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_Constraint_System_t cs));

/*! \relates ppl_Octagonal_Shape_double_tag */
int
ppl_Octagonal_Shape_double_add_recycled_congruences
PPL_PROTO((ppl_Octagonal_Shape_double_t ph,
           ppl_Congruence_System_t cs));






PPL_DECLARE_IO_FUNCTIONS(Octagonal_Shape_double)

/* End of Functions Related to ppl_Octagonal_Shape_double_tag */

#ifdef __cplusplus
} /* extern "C" */
#endif

#undef PPL_TYPE_DECLARATION
#undef PPL_PROTO
#undef PPL_DECLARE_PRINT_FUNCTIONS
#undef PPL_DECLARE_ASCII_DUMP_LOAD_FUNCTIONS
#undef PPL_DECLARE_IO_FUNCTIONS
#undef PPL_DECLARE_AND_DOCUMENT_PRINT_FUNCTIONS
#undef PPL_DECLARE_AND_DOCUMENT_ASCII_DUMP_LOAD_FUNCTIONS
#undef PPL_DECLARE_AND_DOCUMENT_IO_FUNCTIONS

#endif /* !defined(PPL_ppl_c_h) */
