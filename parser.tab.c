/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 5 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex(void);
extern int yylineno;
Program* root_program = NULL;

void yyerror(const char* s) {
    fprintf(stderr, "Syntax error (line %d): %s\n", yylineno, s);
    exit(12);
}

#line 87 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_T_CLASS = 3,                    /* T_CLASS  */
  YYSYMBOL_T_ELSE = 4,                     /* T_ELSE  */
  YYSYMBOL_T_IF = 5,                       /* T_IF  */
  YYSYMBOL_T_INT = 6,                      /* T_INT  */
  YYSYMBOL_T_NEW = 7,                      /* T_NEW  */
  YYSYMBOL_T_RETURN = 8,                   /* T_RETURN  */
  YYSYMBOL_T_STRING = 9,                   /* T_STRING  */
  YYSYMBOL_T_SUPER = 10,                   /* T_SUPER  */
  YYSYMBOL_T_THIS = 11,                    /* T_THIS  */
  YYSYMBOL_T_VOID = 12,                    /* T_VOID  */
  YYSYMBOL_T_WHILE = 13,                   /* T_WHILE  */
  YYSYMBOL_T_EQ = 14,                      /* T_EQ  */
  YYSYMBOL_T_NEQ = 15,                     /* T_NEQ  */
  YYSYMBOL_T_LEQ = 16,                     /* T_LEQ  */
  YYSYMBOL_T_GEQ = 17,                     /* T_GEQ  */
  YYSYMBOL_T_AND = 18,                     /* T_AND  */
  YYSYMBOL_T_OR = 19,                      /* T_OR  */
  YYSYMBOL_T_INT_LIT = 20,                 /* T_INT_LIT  */
  YYSYMBOL_T_STRING_LIT = 21,              /* T_STRING_LIT  */
  YYSYMBOL_T_ID = 22,                      /* T_ID  */
  YYSYMBOL_23_ = 23,                       /* '('  */
  YYSYMBOL_24_ = 24,                       /* ')'  */
  YYSYMBOL_25_ = 25,                       /* ','  */
  YYSYMBOL_26_ = 26,                       /* ':'  */
  YYSYMBOL_27_ = 27,                       /* '{'  */
  YYSYMBOL_28_ = 28,                       /* '}'  */
  YYSYMBOL_29_ = 29,                       /* ';'  */
  YYSYMBOL_30_ = 30,                       /* '='  */
  YYSYMBOL_31_ = 31,                       /* '<'  */
  YYSYMBOL_32_ = 32,                       /* '>'  */
  YYSYMBOL_33_ = 33,                       /* '+'  */
  YYSYMBOL_34_ = 34,                       /* '-'  */
  YYSYMBOL_35_ = 35,                       /* '*'  */
  YYSYMBOL_36_ = 36,                       /* '/'  */
  YYSYMBOL_37_ = 37,                       /* '!'  */
  YYSYMBOL_38_ = 38,                       /* '.'  */
  YYSYMBOL_YYACCEPT = 39,                  /* $accept  */
  YYSYMBOL_program = 40,                   /* program  */
  YYSYMBOL_func_def = 41,                  /* func_def  */
  YYSYMBOL_type = 42,                      /* type  */
  YYSYMBOL_data_type = 43,                 /* data_type  */
  YYSYMBOL_prim_type = 44,                 /* prim_type  */
  YYSYMBOL_param_list = 45,                /* param_list  */
  YYSYMBOL_param_items = 46,               /* param_items  */
  YYSYMBOL_class_def = 47,                 /* class_def  */
  YYSYMBOL_class_members = 48,             /* class_members  */
  YYSYMBOL_block = 49,                     /* block  */
  YYSYMBOL_stmt_list = 50,                 /* stmt_list  */
  YYSYMBOL_var_decl = 51,                  /* var_decl  */
  YYSYMBOL_id_list = 52,                   /* id_list  */
  YYSYMBOL_stmt = 53,                      /* stmt  */
  YYSYMBOL_expr = 54,                      /* expr  */
  YYSYMBOL_logic_or_expr = 55,             /* logic_or_expr  */
  YYSYMBOL_logic_and_expr = 56,            /* logic_and_expr  */
  YYSYMBOL_eq_expr = 57,                   /* eq_expr  */
  YYSYMBOL_rel_expr = 58,                  /* rel_expr  */
  YYSYMBOL_add_expr = 59,                  /* add_expr  */
  YYSYMBOL_mul_expr = 60,                  /* mul_expr  */
  YYSYMBOL_unary_expr = 61,                /* unary_expr  */
  YYSYMBOL_postfix_expr = 62,              /* postfix_expr  */
  YYSYMBOL_primary_expr = 63,              /* primary_expr  */
  YYSYMBOL_arg_list = 64,                  /* arg_list  */
  YYSYMBOL_arg_items = 65                  /* arg_items  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   155

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  70
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  142

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   277


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    37,     2,     2,     2,     2,     2,     2,
      23,    24,    35,    33,    25,    34,    38,    36,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    26,    29,
      31,    30,    32,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    27,     2,    28,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    56,    56,    57,    58,    62,    67,    68,    72,    73,
      77,    78,    82,    83,    87,    92,   100,   109,   110,   115,
     120,   128,   132,   133,   137,   142,   147,   155,   156,   158,
     160,   162,   164,   166,   171,   175,   176,   181,   182,   187,
     188,   190,   195,   196,   198,   200,   202,   207,   208,   210,
     215,   216,   218,   223,   224,   226,   228,   233,   234,   236,
     241,   242,   243,   244,   245,   246,   247,   251,   252,   256,
     261
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "T_CLASS", "T_ELSE",
  "T_IF", "T_INT", "T_NEW", "T_RETURN", "T_STRING", "T_SUPER", "T_THIS",
  "T_VOID", "T_WHILE", "T_EQ", "T_NEQ", "T_LEQ", "T_GEQ", "T_AND", "T_OR",
  "T_INT_LIT", "T_STRING_LIT", "T_ID", "'('", "')'", "','", "':'", "'{'",
  "'}'", "';'", "'='", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'!'",
  "'.'", "$accept", "program", "func_def", "type", "data_type",
  "prim_type", "param_list", "param_items", "class_def", "class_members",
  "block", "stmt_list", "var_decl", "id_list", "stmt", "expr",
  "logic_or_expr", "logic_and_expr", "eq_expr", "rel_expr", "add_expr",
  "mul_expr", "unary_expr", "postfix_expr", "primary_expr", "arg_list",
  "arg_items", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-99)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-10)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -99,    98,   -99,   -19,   -99,   -99,   -99,   -99,   -99,   -15,
     -99,   -99,   -99,   -11,    12,    10,    19,    17,   -99,    28,
      31,    45,   -99,   -99,    39,     8,    94,   -99,   -99,    50,
      52,   -99,    55,    74,   -99,    63,    65,    20,    76,    83,
      36,   -99,   -99,    86,   -99,   -99,    89,    41,   -99,    92,
     -99,   -99,    85,   -99,    19,    19,    97,   -99,    41,   -99,
     -99,    -1,   -99,    41,    99,   102,   113,     9,    59,    35,
      57,   -99,   -22,    41,   108,   -99,    42,    41,    41,   111,
     110,   112,   -99,   114,   115,   116,   -99,   -99,    41,    41,
      41,    41,    41,    41,    41,    41,    41,    41,    41,    41,
      41,   117,   -99,   -99,   -99,   118,   119,   106,   -99,    39,
      39,    39,    41,    41,   113,     9,    59,    59,    35,    35,
      35,    35,    57,    57,   -99,   -99,   121,    39,   120,    41,
     -99,   -99,   -99,   133,   -99,   -99,   -99,   -99,   -99,   -99,
      39,   -99
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     0,    10,    11,     7,     9,     3,     0,
       6,     8,     4,     0,     0,     0,     0,     0,    12,     0,
       0,    13,    17,    14,     0,     0,     0,    22,     5,     0,
       0,    16,     0,     0,    15,     0,    25,     0,     0,     0,
       0,    64,    63,     0,    60,    61,    62,     0,    21,     0,
      27,    23,     0,    57,     0,     0,     0,    18,     0,    65,
      62,     0,    31,     0,     0,    34,    35,    37,    39,    42,
      47,    50,    53,     0,     0,    25,     0,    67,     0,     0,
       0,     0,    26,     0,    62,     0,    54,    32,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      67,     0,    66,    24,    69,     0,    68,     0,    58,     0,
       0,     0,     0,     0,    36,    38,    40,    41,    44,    46,
      43,    45,    48,    49,    51,    52,     0,     0,    59,     0,
      28,    20,    19,     0,    56,    55,    59,    30,    33,    70,
       0,    29
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -99,   -99,   -99,   -99,     1,    82,    58,   -99,   -99,   -99,
     -98,   -99,   -99,   101,   -99,   -40,   -99,    60,    62,    27,
      32,    33,   -59,   122,   -99,    46,   -99
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     8,     9,    19,    11,    20,    21,    12,    26,
      28,    33,    50,    37,    51,    74,    65,    66,    67,    68,
      69,    70,    71,    72,    53,   105,   106
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      64,   100,    10,    13,    86,     4,    39,    14,     5,    41,
      42,   131,   132,   133,     4,    15,    79,     5,    83,    44,
      45,    84,    61,    90,    91,     4,    29,    32,     5,   137,
       7,    18,    17,   101,    49,    16,    63,   104,   107,   124,
     125,     7,   141,    39,    22,    56,    41,    42,    39,    57,
      23,    41,    42,   134,   135,    24,    44,    45,    60,    61,
     104,    44,    45,    60,    61,    62,    27,    56,    96,    97,
      25,   103,    34,    63,    35,    92,    93,    36,    63,    38,
       4,    39,    40,     5,    41,    42,    54,    43,    55,   139,
      94,    95,    98,    99,    44,    45,    46,    47,     2,    58,
       4,     3,    48,     5,     4,    59,    30,     5,    77,    73,
       6,    -9,    80,    81,    75,    78,     7,   116,   117,    82,
       7,    88,    31,    79,   118,   119,   120,   121,    87,   122,
     123,    89,   102,   108,   109,   130,   110,   140,   111,   112,
     113,   127,   128,    85,   129,   136,   126,     0,   114,   138,
      76,   115,     0,     0,     0,    52
};

static const yytype_int16 yycheck[] =
{
      40,    23,     1,    22,    63,     6,     7,    22,     9,    10,
      11,   109,   110,   111,     6,    26,    38,     9,    58,    20,
      21,    22,    23,    14,    15,     6,    25,    26,     9,   127,
      22,    12,    22,    73,    33,    23,    37,    77,    78,    98,
      99,    22,   140,     7,    27,    25,    10,    11,     7,    29,
      22,    10,    11,   112,   113,    24,    20,    21,    22,    23,
     100,    20,    21,    22,    23,    29,    27,    25,    33,    34,
      25,    29,    22,    37,    22,    16,    17,    22,    37,     5,
       6,     7,     8,     9,    10,    11,    23,    13,    23,   129,
      31,    32,    35,    36,    20,    21,    22,    23,     0,    23,
       6,     3,    28,     9,     6,    22,    12,     9,    23,    23,
      12,    22,    54,    55,    22,    30,    22,    90,    91,    22,
      22,    19,    28,    38,    92,    93,    94,    95,    29,    96,
      97,    18,    24,    22,    24,    29,    24,     4,    24,    24,
      24,    24,    24,    61,    25,    24,   100,    -1,    88,    29,
      49,    89,    -1,    -1,    -1,    33
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    40,     0,     3,     6,     9,    12,    22,    41,    42,
      43,    44,    47,    22,    22,    26,    23,    22,    12,    43,
      45,    46,    27,    22,    24,    25,    48,    27,    49,    43,
      12,    28,    43,    50,    22,    22,    22,    52,     5,     7,
       8,    10,    11,    13,    20,    21,    22,    23,    28,    43,
      51,    53,    62,    63,    23,    23,    25,    29,    23,    22,
      22,    23,    29,    37,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    23,    54,    22,    52,    23,    30,    38,
      45,    45,    22,    54,    22,    44,    61,    29,    19,    18,
      14,    15,    16,    17,    31,    32,    33,    34,    35,    36,
      23,    54,    24,    29,    54,    64,    65,    54,    22,    24,
      24,    24,    24,    24,    56,    57,    58,    58,    59,    59,
      59,    59,    60,    60,    61,    61,    64,    24,    24,    25,
      29,    49,    49,    49,    61,    61,    24,    49,    29,    54,
       4,    49
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    39,    40,    40,    40,    41,    42,    42,    43,    43,
      44,    44,    45,    45,    46,    46,    47,    48,    48,    48,
      48,    49,    50,    50,    51,    52,    52,    53,    53,    53,
      53,    53,    53,    53,    54,    55,    55,    56,    56,    57,
      57,    57,    58,    58,    58,    58,    58,    59,    59,    59,
      60,    60,    60,    61,    61,    61,    61,    62,    62,    62,
      63,    63,    63,    63,    63,    63,    63,    64,    64,    65,
      65
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     6,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     4,     7,     0,     4,     7,
       7,     3,     0,     2,     3,     1,     3,     1,     4,     7,
       5,     2,     3,     5,     1,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     1,     2,     4,     4,     1,     3,     4,
       1,     1,     1,     1,     1,     2,     3,     0,     1,     1,
       3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: %empty  */
#line 56 "parser.y"
                 { root_program = ast_program_new(); (yyval.prog) = root_program; }
#line 1233 "parser.tab.c"
    break;

  case 3: /* program: program func_def  */
#line 57 "parser.y"
                       { ast_program_add_func((yyvsp[-1].prog), (yyvsp[0].func)); (yyval.prog) = (yyvsp[-1].prog); }
#line 1239 "parser.tab.c"
    break;

  case 4: /* program: program class_def  */
#line 58 "parser.y"
                        { ast_program_add_class((yyvsp[-1].prog), (yyvsp[0].cls)); (yyval.prog) = (yyvsp[-1].prog); }
#line 1245 "parser.tab.c"
    break;

  case 5: /* func_def: type T_ID '(' param_list ')' block  */
#line 63 "parser.y"
      { (yyval.func) = ast_func_new(yylineno, (yyvsp[-5].strVal), (yyvsp[-4].strVal), (yyvsp[-2].paramList), (yyvsp[0].stmtList)); }
#line 1251 "parser.tab.c"
    break;

  case 6: /* type: data_type  */
#line 67 "parser.y"
                { (yyval.strVal) = (yyvsp[0].strVal); }
#line 1257 "parser.tab.c"
    break;

  case 7: /* type: T_VOID  */
#line 68 "parser.y"
                { (yyval.strVal) = strdup("void"); }
#line 1263 "parser.tab.c"
    break;

  case 8: /* data_type: prim_type  */
#line 72 "parser.y"
                { (yyval.strVal) = (yyvsp[0].strVal); }
#line 1269 "parser.tab.c"
    break;

  case 9: /* data_type: T_ID  */
#line 73 "parser.y"
                { (yyval.strVal) = (yyvsp[0].strVal); }
#line 1275 "parser.tab.c"
    break;

  case 10: /* prim_type: T_INT  */
#line 77 "parser.y"
                { (yyval.strVal) = strdup("int"); }
#line 1281 "parser.tab.c"
    break;

  case 11: /* prim_type: T_STRING  */
#line 78 "parser.y"
                { (yyval.strVal) = strdup("string"); }
#line 1287 "parser.tab.c"
    break;

  case 12: /* param_list: T_VOID  */
#line 82 "parser.y"
                  { (yyval.paramList) = (ParamList){NULL, 0, 0}; }
#line 1293 "parser.tab.c"
    break;

  case 13: /* param_list: param_items  */
#line 83 "parser.y"
                  { (yyval.paramList) = (yyvsp[0].paramList); }
#line 1299 "parser.tab.c"
    break;

  case 14: /* param_items: data_type T_ID  */
#line 88 "parser.y"
      {
          (yyval.paramList) = (ParamList){NULL, 0, 0};
          param_list_push(&(yyval.paramList), (yyvsp[-1].strVal), (yyvsp[0].strVal));
      }
#line 1308 "parser.tab.c"
    break;

  case 15: /* param_items: param_items ',' data_type T_ID  */
#line 93 "parser.y"
      {
          param_list_push(&(yyvsp[-3].paramList), (yyvsp[-1].strVal), (yyvsp[0].strVal));
          (yyval.paramList) = (yyvsp[-3].paramList);
      }
#line 1317 "parser.tab.c"
    break;

  case 16: /* class_def: T_CLASS T_ID ':' T_ID '{' class_members '}'  */
#line 101 "parser.y"
      {
          (yyvsp[-1].cls)->name = (yyvsp[-5].strVal);
          (yyvsp[-1].cls)->super_name = (yyvsp[-3].strVal);
          (yyval.cls) = (yyvsp[-1].cls);
      }
#line 1327 "parser.tab.c"
    break;

  case 17: /* class_members: %empty  */
#line 109 "parser.y"
                 { (yyval.cls) = ast_class_new(yylineno); }
#line 1333 "parser.tab.c"
    break;

  case 18: /* class_members: class_members data_type id_list ';'  */
#line 111 "parser.y"
      {
          ast_class_add_field((yyvsp[-3].cls), ast_stmt_var_decl(yylineno, (yyvsp[-2].strVal), (yyvsp[-1].idList)));
          (yyval.cls) = (yyvsp[-3].cls);
      }
#line 1342 "parser.tab.c"
    break;

  case 19: /* class_members: class_members data_type T_ID '(' param_list ')' block  */
#line 116 "parser.y"
      {
          ast_class_add_method((yyvsp[-6].cls), ast_func_new(yylineno, (yyvsp[-5].strVal), (yyvsp[-4].strVal), (yyvsp[-2].paramList), (yyvsp[0].stmtList)));
          (yyval.cls) = (yyvsp[-6].cls);
      }
#line 1351 "parser.tab.c"
    break;

  case 20: /* class_members: class_members T_VOID T_ID '(' param_list ')' block  */
#line 121 "parser.y"
      {
          ast_class_add_method((yyvsp[-6].cls), ast_func_new(yylineno, strdup("void"), (yyvsp[-4].strVal), (yyvsp[-2].paramList), (yyvsp[0].stmtList)));
          (yyval.cls) = (yyvsp[-6].cls);
      }
#line 1360 "parser.tab.c"
    break;

  case 21: /* block: '{' stmt_list '}'  */
#line 128 "parser.y"
                        { (yyval.stmtList) = (yyvsp[-1].stmtList); }
#line 1366 "parser.tab.c"
    break;

  case 22: /* stmt_list: %empty  */
#line 132 "parser.y"
                 { (yyval.stmtList) = (StmtList){NULL, 0, 0}; }
#line 1372 "parser.tab.c"
    break;

  case 23: /* stmt_list: stmt_list stmt  */
#line 133 "parser.y"
                     { stmt_list_push(&(yyvsp[-1].stmtList), (yyvsp[0].stmt)); (yyval.stmtList) = (yyvsp[-1].stmtList); }
#line 1378 "parser.tab.c"
    break;

  case 24: /* var_decl: data_type id_list ';'  */
#line 138 "parser.y"
      { (yyval.stmt) = ast_stmt_var_decl(yylineno, (yyvsp[-2].strVal), (yyvsp[-1].idList)); }
#line 1384 "parser.tab.c"
    break;

  case 25: /* id_list: T_ID  */
#line 143 "parser.y"
      {
          (yyval.idList) = (StringList){NULL, 0, 0};
          string_list_push(&(yyval.idList), (yyvsp[0].strVal));
      }
#line 1393 "parser.tab.c"
    break;

  case 26: /* id_list: id_list ',' T_ID  */
#line 148 "parser.y"
      {
          string_list_push(&(yyvsp[-2].idList), (yyvsp[0].strVal));
          (yyval.idList) = (yyvsp[-2].idList);
      }
#line 1402 "parser.tab.c"
    break;

  case 27: /* stmt: var_decl  */
#line 155 "parser.y"
               { (yyval.stmt) = (yyvsp[0].stmt); }
#line 1408 "parser.tab.c"
    break;

  case 28: /* stmt: postfix_expr '=' expr ';'  */
#line 157 "parser.y"
      { (yyval.stmt) = ast_stmt_assign(yylineno, (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 1414 "parser.tab.c"
    break;

  case 29: /* stmt: T_IF '(' expr ')' block T_ELSE block  */
#line 159 "parser.y"
      { (yyval.stmt) = ast_stmt_if(yylineno, (yyvsp[-4].expr), (yyvsp[-2].stmtList), (yyvsp[0].stmtList)); }
#line 1420 "parser.tab.c"
    break;

  case 30: /* stmt: T_WHILE '(' expr ')' block  */
#line 161 "parser.y"
      { (yyval.stmt) = ast_stmt_while(yylineno, (yyvsp[-2].expr), (yyvsp[0].stmtList)); }
#line 1426 "parser.tab.c"
    break;

  case 31: /* stmt: T_RETURN ';'  */
#line 163 "parser.y"
      { (yyval.stmt) = ast_stmt_return(yylineno, NULL); }
#line 1432 "parser.tab.c"
    break;

  case 32: /* stmt: T_RETURN expr ';'  */
#line 165 "parser.y"
      { (yyval.stmt) = ast_stmt_return(yylineno, (yyvsp[-1].expr)); }
#line 1438 "parser.tab.c"
    break;

  case 33: /* stmt: postfix_expr '(' arg_list ')' ';'  */
#line 167 "parser.y"
      { (yyval.stmt) = ast_stmt_expr(yylineno, ast_expr_call(yylineno, (yyvsp[-4].expr), (yyvsp[-2].exprList))); }
#line 1444 "parser.tab.c"
    break;

  case 34: /* expr: logic_or_expr  */
#line 171 "parser.y"
                    { (yyval.expr) = (yyvsp[0].expr); }
#line 1450 "parser.tab.c"
    break;

  case 35: /* logic_or_expr: logic_and_expr  */
#line 175 "parser.y"
                     { (yyval.expr) = (yyvsp[0].expr); }
#line 1456 "parser.tab.c"
    break;

  case 36: /* logic_or_expr: logic_or_expr T_OR logic_and_expr  */
#line 177 "parser.y"
      { (yyval.expr) = ast_expr_binary(yylineno, strdup("||"), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1462 "parser.tab.c"
    break;

  case 37: /* logic_and_expr: eq_expr  */
#line 181 "parser.y"
              { (yyval.expr) = (yyvsp[0].expr); }
#line 1468 "parser.tab.c"
    break;

  case 38: /* logic_and_expr: logic_and_expr T_AND eq_expr  */
#line 183 "parser.y"
      { (yyval.expr) = ast_expr_binary(yylineno, strdup("&&"), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1474 "parser.tab.c"
    break;

  case 39: /* eq_expr: rel_expr  */
#line 187 "parser.y"
               { (yyval.expr) = (yyvsp[0].expr); }
#line 1480 "parser.tab.c"
    break;

  case 40: /* eq_expr: eq_expr T_EQ rel_expr  */
#line 189 "parser.y"
      { (yyval.expr) = ast_expr_binary(yylineno, strdup("=="), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1486 "parser.tab.c"
    break;

  case 41: /* eq_expr: eq_expr T_NEQ rel_expr  */
#line 191 "parser.y"
      { (yyval.expr) = ast_expr_binary(yylineno, strdup("!="), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1492 "parser.tab.c"
    break;

  case 42: /* rel_expr: add_expr  */
#line 195 "parser.y"
               { (yyval.expr) = (yyvsp[0].expr); }
#line 1498 "parser.tab.c"
    break;

  case 43: /* rel_expr: rel_expr '<' add_expr  */
#line 197 "parser.y"
      { (yyval.expr) = ast_expr_binary(yylineno, strdup("<"), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1504 "parser.tab.c"
    break;

  case 44: /* rel_expr: rel_expr T_LEQ add_expr  */
#line 199 "parser.y"
      { (yyval.expr) = ast_expr_binary(yylineno, strdup("<="), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1510 "parser.tab.c"
    break;

  case 45: /* rel_expr: rel_expr '>' add_expr  */
#line 201 "parser.y"
      { (yyval.expr) = ast_expr_binary(yylineno, strdup(">"), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1516 "parser.tab.c"
    break;

  case 46: /* rel_expr: rel_expr T_GEQ add_expr  */
#line 203 "parser.y"
      { (yyval.expr) = ast_expr_binary(yylineno, strdup(">="), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1522 "parser.tab.c"
    break;

  case 47: /* add_expr: mul_expr  */
#line 207 "parser.y"
               { (yyval.expr) = (yyvsp[0].expr); }
#line 1528 "parser.tab.c"
    break;

  case 48: /* add_expr: add_expr '+' mul_expr  */
#line 209 "parser.y"
      { (yyval.expr) = ast_expr_binary(yylineno, strdup("+"), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1534 "parser.tab.c"
    break;

  case 49: /* add_expr: add_expr '-' mul_expr  */
#line 211 "parser.y"
      { (yyval.expr) = ast_expr_binary(yylineno, strdup("-"), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1540 "parser.tab.c"
    break;

  case 50: /* mul_expr: unary_expr  */
#line 215 "parser.y"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 1546 "parser.tab.c"
    break;

  case 51: /* mul_expr: mul_expr '*' unary_expr  */
#line 217 "parser.y"
      { (yyval.expr) = ast_expr_binary(yylineno, strdup("*"), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1552 "parser.tab.c"
    break;

  case 52: /* mul_expr: mul_expr '/' unary_expr  */
#line 219 "parser.y"
      { (yyval.expr) = ast_expr_binary(yylineno, strdup("/"), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1558 "parser.tab.c"
    break;

  case 53: /* unary_expr: postfix_expr  */
#line 223 "parser.y"
                   { (yyval.expr) = (yyvsp[0].expr); }
#line 1564 "parser.tab.c"
    break;

  case 54: /* unary_expr: '!' unary_expr  */
#line 225 "parser.y"
      { (yyval.expr) = ast_expr_unary(yylineno, strdup("!"), (yyvsp[0].expr)); }
#line 1570 "parser.tab.c"
    break;

  case 55: /* unary_expr: '(' prim_type ')' unary_expr  */
#line 227 "parser.y"
      { (yyval.expr) = ast_expr_cast(yylineno, (yyvsp[-2].strVal), (yyvsp[0].expr)); }
#line 1576 "parser.tab.c"
    break;

  case 56: /* unary_expr: '(' T_ID ')' unary_expr  */
#line 229 "parser.y"
      { (yyval.expr) = ast_expr_cast(yylineno, (yyvsp[-2].strVal), (yyvsp[0].expr)); }
#line 1582 "parser.tab.c"
    break;

  case 57: /* postfix_expr: primary_expr  */
#line 233 "parser.y"
                   { (yyval.expr) = (yyvsp[0].expr); }
#line 1588 "parser.tab.c"
    break;

  case 58: /* postfix_expr: postfix_expr '.' T_ID  */
#line 235 "parser.y"
      { (yyval.expr) = ast_expr_member(yylineno, (yyvsp[-2].expr), (yyvsp[0].strVal)); }
#line 1594 "parser.tab.c"
    break;

  case 59: /* postfix_expr: postfix_expr '(' arg_list ')'  */
#line 237 "parser.y"
      { (yyval.expr) = ast_expr_call(yylineno, (yyvsp[-3].expr), (yyvsp[-1].exprList)); }
#line 1600 "parser.tab.c"
    break;

  case 60: /* primary_expr: T_INT_LIT  */
#line 241 "parser.y"
                   { (yyval.expr) = ast_expr_int(yylineno, (yyvsp[0].intVal)); }
#line 1606 "parser.tab.c"
    break;

  case 61: /* primary_expr: T_STRING_LIT  */
#line 242 "parser.y"
                   { (yyval.expr) = ast_expr_str(yylineno, (yyvsp[0].strVal)); }
#line 1612 "parser.tab.c"
    break;

  case 62: /* primary_expr: T_ID  */
#line 243 "parser.y"
                   { (yyval.expr) = ast_expr_id(yylineno, (yyvsp[0].strVal)); }
#line 1618 "parser.tab.c"
    break;

  case 63: /* primary_expr: T_THIS  */
#line 244 "parser.y"
                   { (yyval.expr) = ast_expr_id(yylineno, strdup("this")); }
#line 1624 "parser.tab.c"
    break;

  case 64: /* primary_expr: T_SUPER  */
#line 245 "parser.y"
                   { (yyval.expr) = ast_expr_id(yylineno, strdup("super")); }
#line 1630 "parser.tab.c"
    break;

  case 65: /* primary_expr: T_NEW T_ID  */
#line 246 "parser.y"
                   { (yyval.expr) = ast_expr_new(yylineno, (yyvsp[0].strVal)); }
#line 1636 "parser.tab.c"
    break;

  case 66: /* primary_expr: '(' expr ')'  */
#line 247 "parser.y"
                   { (yyval.expr) = (yyvsp[-1].expr); }
#line 1642 "parser.tab.c"
    break;

  case 67: /* arg_list: %empty  */
#line 251 "parser.y"
                 { (yyval.exprList) = (ExprList){NULL, 0, 0}; }
#line 1648 "parser.tab.c"
    break;

  case 68: /* arg_list: arg_items  */
#line 252 "parser.y"
                 { (yyval.exprList) = (yyvsp[0].exprList); }
#line 1654 "parser.tab.c"
    break;

  case 69: /* arg_items: expr  */
#line 257 "parser.y"
      {
          (yyval.exprList) = (ExprList){NULL, 0, 0};
          expr_list_push(&(yyval.exprList), (yyvsp[0].expr));
      }
#line 1663 "parser.tab.c"
    break;

  case 70: /* arg_items: arg_items ',' expr  */
#line 262 "parser.y"
      {
          expr_list_push(&(yyvsp[-2].exprList), (yyvsp[0].expr));
          (yyval.exprList) = (yyvsp[-2].exprList);
      }
#line 1672 "parser.tab.c"
    break;


#line 1676 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 268 "parser.y"
