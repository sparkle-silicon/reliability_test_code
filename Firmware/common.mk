
OS ?= $(shell uname)
ifeq ($(OS),Linux)
BUILD_DATE := $(shell date +'%y%m%d')#'%Y%m%d'
BUILD_TIME := $(shell date +'%H%M%S')
SHELL_TYPES := $(shell echo $$0)
else #Windows_NT
BUILD_DATE := $(shell powershell Get-Date -Format '%y%m%d')#'%Y%m%d'
BUILD_TIME := $(shell powershell Get-Date -Format '%H%M%S')
SHELL_TYPES := $(SHELL)
endif
PROGRAM ?= ec_main#project Object name
PROGRAM_FILE ?= $(PROGRAM)_$(BUILD_DATE)$(BUILD_TIME).elf
TARGET ?= $(PROGRAM_FILE)

# See LICENSE for license details.
DOWNLOAD ?= flash					#download flash
RISCV_ARCH?=rv32ec
RISCV_ABI?=ilp32e

SIZE:=riscv-nuclei-elf-size
CC:=riscv-nuclei-elf-gcc 
OBJDUMP:=riscv-nuclei-elf-objdump 

ENV_DIR = $(AE10X_DIR)/ENV
ifeq ($(DUAL_BOOT_FLAG),0)
ifeq ($(DOWNLOAD),flash) 
LINKER_SCRIPT := $(ENV_DIR)/link_flash.lds
else ifeq ($(DOWNLOAD),crypto) 
LINKER_SCRIPT := $(ENV_DIR)/link_crypto.lds
else ifeq ($(DOWNLOAD),ilm) 
LINKER_SCRIPT := $(ENV_DIR)/link_ilm.lds
endif
LDFLAGS += -T $(LINKER_SCRIPT)  -nostartfiles  -Wl,--check-sections#      函数检查 #-Wl,--gc-sections #不链接未用函数,会导致中断异常
else 
LINKER_SCRIPT1 := $(ENV_DIR)/link_flash.lds
LINKER_SCRIPT2 := $(ENV_DIR)/link_flash2.lds
LDFLAGS1 += -T $(LINKER_SCRIPT1)  -nostartfiles  -Wl,--check-sections
LDFLAGS2 += -T $(LINKER_SCRIPT2)  -nostartfiles  -Wl,--check-sections
LINK_DEPS1 			+= $(LINKER_SCRIPT1)
LINK_DEPS2 			+= $(LINKER_SCRIPT2)
endif
LDFLAGS += -L$(ENV_DIR)
# download has three option:  flashxip, flash, ilm
include $(AE10X_DIR)/Makefile
include $(KERNEL_DIR)/Makefile
include $(CUSTOM_DIR)/Makefile
include $(TEST_DIR)/Makefile
TARGET_BOOT1 = $(PROGRAM)_1_$(BUILD_DATE)$(BUILD_TIME).elf
TARGET_BOOT2 = $(PROGRAM)_2_$(BUILD_DATE)$(BUILD_TIME).elf
C_SRCS_BOOT	 += $(AE10X_DIR)/AE_INIT.c
C_SRCS_BOOT1 += $(AE10X_DIR)/AE_INIT.c
C_SRCS_BOOT2 += $(AE10X_DIR)/AE_INIT.c
C_SRCS_BOOT	 += $(C_SRCS)
C_SRCS_BOOT1 += $(C_SRCS)
C_SRCS_BOOT2 += $(C_SRCS)
# ASM_IBJS 		:= $(ASM_SRCS:.S=.i)
ASM_OBJS 		:= $(ASM_SRCS:.S=.o)
# C_IBJS 			:= $(C_SRCS_BOOT:.c=.i)
# C_SBJS 			:= $(C_SRCS_BOOT:.c=.s)
C_OBJS 			:= $(C_SRCS_BOOT:.c=.o)
LINK_OBJS 		+= $(ASM_OBJS) $(C_OBJS)
ASM_OBJS_BOOT1		:= $(ASM_OBJS:.o=1.o)
# C_IBJS_BOOT1 		:= $(C_SRCS_BOOT1:.c=1.i)
# C_SBJS_BOOT1 		:= $(C_SRCS_BOOT1:.c=1.s)
C_OBJS_BOOT1		:= $(C_SRCS_BOOT1:.c=1.o)
LINK_OBJS_BOOT1 	+= $(ASM_OBJS_BOOT1) $(C_OBJS_BOOT1) 
ASM_OBJS_BOOT2		:= $(ASM_OBJS:.o=2.o)
# C_IBJS_BOOT2 		:= $(C_SRCS_BOOT2:.c=2.i)
# C_SBJS_BOOT2 		:= $(C_SRCS_BOOT2:.c=2.s)
C_OBJS_BOOT2		:= $(C_SRCS_BOOT2:.c=2.o)
LINK_OBJS_BOOT2 	+= $(ASM_OBJS_BOOT2) $(C_OBJS_BOOT2)
LINK_DEPS 			+= $(LINKER_SCRIPT)
CLEAN_OBJS 			+=  $(LINK_OBJS) 
CLEAN_OBJS 			+= $(TARGET_BOOT1) 
CLEAN_OBJS 			+= $(TARGET_BOOT2) 
ifdef CHIP
CFLAGS += -D$(CHIP)
endif
#检查函数问题
CFLAGS += -Wall #启用常用警告信息
CFLAGS += -Wno-format -Wno-format-extra-args -Wno-strict-aliasing
# CFLAGS += -Wextra #启用额外警告
# CFLAGS += -Wabi #启用 ABI 相关的警告
# CFLAGS += -Wabi=11 #指定了要使用的 ABI 版本
# CFLAGS += -Wabsolute-value #绝对值的标准函数的可疑调用发出警告
# CFLAGS += -Waddress #有关内存地址可疑使用的警告
# CFLAGS += -Waddress-of-packed-member #尝试获取结构体或联合体中打包成员的地址
# CFLAGS += -Waggregate-return #返回结构体、联合体或数组时发出警告
# CFLAGS += -Waggressive-loop-optimizations #循环中可能触发未定义行为的情况下发出警告
# # CFLAGS += -Walloc-size-larger-than=<bytes> #用于在尝试分配内存大于指定字节数的对象时发出警告
# CFLAGS += -Walloc-zero #尝试分配零字节大小内存的情况时发出警告
# CFLAGS += -Walloca #用于在任何使用 alloca 函数时发出警告
# # CFLAGS += -Walloca-larger-than=<number> #警告未限制使用 alloca 函数的情况，并且还会警告使用了限制大小可能大于指定字节数的 alloca 函数
# # CFLAGS += -Warray-bounds #数组越界访问时发出警告,相当于级别1
# CFLAGS += -Warray-bounds=2 #级别<0,2>，数组越界访问时发出警告
# # CFLAGS += -Wattribute-alias #指出在具有别名属性的声明中的类型安全性问题、错误和不匹配,
# CFLAGS += -Wattribute-alias=2 #级别<0,2>,指出在具有别名属性的声明中的类型安全性问题、错误和不匹配
# CFLAGS += -Wattribute-warning #当使用 __attribute__((warning))声明时慧警告
# CFLAGS += -Wattributes #检测和报告代码中可能不适当的属性使用情况
# CFLAGS += -Wbad-function-cast #不兼容类型的函数进行强制类型转换的情况，并发出警告
# CFLAGS += -Wbool-compare #警告布尔表达式与 true/false 之外的整数值进行比较
# CFLAGS += -Wbool-operation #布尔表达式进行某些操作进行警告
# CFLAGS += -Wbuiltin-declaration-mismatch #内建函数的声明与其原始签名不匹配时发出警告
# CFLAGS += -Wbuiltin-macro-redefined #内建的预处理宏被重新定义或未定义时发出警告
# CFLAGS += -Wc11-c2x-compat #用于警告在 ISO C11 中不存在的但存在于 ISO C2X 中的特性
# # CFLAGS += -Wc90-c99-compat #用于警告在 ISO C90 中不存在的但存在于 ISO C99 中的特性,C90太古老了，位域和“//”注释都不支持，不适用它检查
# CFLAGS += -Wc99-c11-compat #用于警告在 ISO C99 中不存在的但存在于 ISO C11 中的特性
# CFLAGS += -Wcannot-profile #在请求进行剖析插装但无法应用于某个特定函数时发出警告
# # CFLAGS += -Wcast-align #发现代码中存在的指针强制转换增加了对齐度时发出警告,该规则不适用
# # CFLAGS += -Wcast-align=strict #更严格发现代码中存在的指针强制转换增加了对齐度时发出警告,该规则不适用
# CFLAGS += -Wcast-function-type #存在不兼容的函数类型之间的转换操作
# # CFLAGS += -Wcast-qual #在进行强制类型转换时丢弃了类型限定符
# CFLAGS += -Wchar-subscripts #警告下标类型为'char
# CFLAGS += -Wclobbered #警告可能会被 "longjmp" 或 "vfork" 更改的变量
# # CFLAGS += -Wcomment #警告可能存在嵌套的块注释以及 C++ 风格的注释跨越多个物理行的情况
# CFLAGS += -Wcomments #警告可能存在嵌套的块注释以及 C++ 风格的注释跨越多个物理行的情况
# CFLAGS += -Wconversion #警告可能改变值的隐式类型转换
# # CFLAGS +=  -Werror #警告视为错误
# # CFLAGS += -Wunused #未使用的变量和函数
# # CFLAGS += -Wuninitialized #未初始化的本地变量使用
# CFLAGS += -Wcoverage-mismatch        # Warn in case profiles in -fprofile-use do not match.
# CFLAGS += -Wcpp                      # Warn when a #warning directive is encountered.
# CFLAGS += -Wdangling-else            # Warn about dangling else.
# CFLAGS += -Wdate-time                # Warn about __TIME__, __DATE__ and __TIMESTAMP__ usage.
# CFLAGS += -Wdeclaration-after-statement# Warn when a declaration is found after a statement.
# CFLAGS += -Wdeprecated               # Warn if a deprecated compiler feature, class, method, or field is used.
# CFLAGS += -Wdeprecated-declarations  # Warn about uses of __attribute__((deprecated)) declarations.
# CFLAGS += -Wdesignated-init          # Warn about positional initialization of structs requiring designated initializers.
# CFLAGS += -Wdisabled-optimization    # Warn when an optimization pass is disabled.
# CFLAGS += -Wdiscarded-array-qualifiers# Warn if qualifiers on arrays which are pointer targets are discarded.
# CFLAGS += -Wdiscarded-qualifiers     # Warn if type qualifiers on pointers are discarded.
# CFLAGS += -Wdiv-by-zero              # Warn about compile-time integer division by zero.
# CFLAGS += -Wdouble-promotion         # Warn about implicit conversions from "float" to "double".
# CFLAGS += -Wduplicate-decl-specifier # Warn when a declaration has duplicate const, volatile, restrict or _Atomic specifier.
# CFLAGS += -Wduplicated-branches     #  Warn about duplicated branches in if-else statements.
# CFLAGS += -Wduplicated-cond          # Warn about duplicated conditions in an if-else-if chain.
# CFLAGS += -Wempty-body               # Warn about an empty body in an if or else statement.
# CFLAGS += -Wendif-labels             # Warn about stray tokens after #else and #endif.
# CFLAGS += -Wenum-compare            #  Warn about comparison of different enum types.
# CFLAGS += -Werror-implicit-function-declaration #This switch is deprecated; use -Werror=implicit-function-declaration instead.  Same as -Werror=.
# CFLAGS += -Wexpansion-to-defined     # Warn if "defined" is used outside #if.
# CFLAGS += -Wextra                   #  Print extra (possibly unwanted) warnings.
# CFLAGS += -Wfloat-conversion        #  Warn for implicit type conversions that cause loss of floating point precision.
# CFLAGS += -Wfloat-equal              # Warn if testing floating point numbers for equality.
# CFLAGS += -Wformat                  #  Warn about printf/scanf/strftime/strfmon format string anomalies.  Same as -Wformat=.
# CFLAGS += -Wformat-contains-nul     #  Warn about format strings that contain NUL bytes.
# CFLAGS += -Wformat-extra-args       #  Warn if passing too many arguments to a function for its format string.
# CFLAGS += -Wformat-nonliteral       #  Warn about format strings that are not literals.
# # CFLAGS += -Wformat-overflow<0,2>    #  Warn about function calls with format strings that write past the end of the destination region.  Same as -Wformat-overflow=1.  Same as -Wformat-overflow=.
# # CFLAGS += -Wformat-overflow=<0,2>  #   Warn about function calls with format strings that write past the end of the destination region.
# CFLAGS += -Wformat-security          # Warn about possible security problems with format functions.
# CFLAGS += -Wformat-signedness       #  Warn about sign differences with format functions.
# CFLAGS += -Wformat-truncation       #  Warn about calls to snprintf and similar functions that truncate output. Same as -Wformat-truncation=1.  Same as -Wformat-truncation=.
# # CFLAGS += -Wformat-truncation=<0,2> #  Warn about calls to snprintf and similar functions that truncate output.
# CFLAGS += -Wformat-y2k             #   Warn about strftime formats yielding 2-digit years.
# CFLAGS += -Wformat-zero-length      #  Warn about zero-length formats.
# # CFLAGS += -Wformat=<0,2>           #   Warn about printf/scanf/strftime/strfmon format string anomalies.
# CFLAGS += -Wframe-address          #   Warn when __builtin_frame_address or __builtin_return_address is used unsafely.
# # CFLAGS += -Wframe-larger-than=<byte-size># Warn if a function's stack frame requires in excess of <byte-size>.
# CFLAGS += -Wfree-nonheap-object     #  Warn when attempting to free a non-heap object.
# CFLAGS += -Whsa                    #   Warn when a function cannot be expanded to HSAIL.
# CFLAGS += -Wif-not-aligned          #  Warn when the field in a struct is not aligned.
# CFLAGS += -Wignored-attributes       # Warn whenever attributes are ignored.
# CFLAGS += -Wignored-qualifiers      #  Warn whenever type qualifiers are ignored.
# CFLAGS += -Wimplicit                #  Warn about implicit declarations.
# CFLAGS += -Wimplicit-fallthrough    #  Same as -Wimplicit-fallthrough=.  Use the latter option instead.
# # CFLAGS += -Wimplicit-fallthrough=<0,5> #Warn when a switch case falls through.
# CFLAGS += -Wimplicit-function-declaration# Warn about implicit function declarations.
# CFLAGS += -Wimplicit-int            #  Warn when a declaration does not specify a type.
# CFLAGS += -Wincompatible-pointer-types# Warn when there is a conversion between pointers that have incompatible types.
# CFLAGS += -Winit-self               #  Warn about variables which are initialized to themselves.
# CFLAGS += -Winline                 #   Warn when an inlined function cannot be inlined.
# CFLAGS += -Wint-conversion           # Warn about incompatible integer to pointer and pointer to integer conversions.
# CFLAGS += -Wint-in-bool-context     #  Warn for suspicious integer expressions in boolean context.
# CFLAGS += -Wint-to-pointer-cast    #   Warn when there is a cast to a pointer from an integer of a different size.
# CFLAGS += -Winvalid-memory-model    #  Warn when an atomic memory model parameter is known to be outside the valid range.
# CFLAGS += -Winvalid-pch            #   Warn about PCH files that are found but not used.
# CFLAGS += -Wjump-misses-init       #   Warn when a jump misses a variable initialization.
# # CFLAGS += -Wlarger-than=<byte-size> #  Warn if an object's size exceeds <byte-size>.
# CFLAGS += -Wlogical-not-parentheses #  Warn when logical not is used on the left hand side operand of a comparison.
# CFLAGS += -Wlogical-op              #  Warn when a logical operator is suspiciously always evaluating to true or false.
# CFLAGS += -Wlong-long               #  Do not warn about using "long long" when -pedantic.
# CFLAGS += -Wlto-type-mismatch      #   During link time optimization warn about mismatched types of global declarations.
# CFLAGS += -Wmain                   #   Warn about suspicious declarations of "main".
# CFLAGS += -Wmaybe-uninitialized    #   Warn about maybe uninitialized automatic variables.
# CFLAGS += -Wmemset-elt-size        #   Warn about suspicious calls to memset where the third argument contains the number of elements not multiplied by the element size.
# CFLAGS += -Wmemset-transposed-args #   Warn about suspicious calls to memset where the third argument is constant literal zero and the second is not.
# CFLAGS += -Wmisleading-indentation #   Warn when the indentation of the code does not reflect the block structure.
# CFLAGS += -Wmissing-attributes     #   Warn about declarations of entities that may be missing attributes that related entities have been declared with.
# CFLAGS += -Wmissing-braces        #    Warn about possibly missing braces around initializers.
# CFLAGS += -Wmissing-declarations   #   Warn about global functions without previous declarations.
# CFLAGS += -Wmissing-field-initializers# Warn about missing fields in struct initializers.
# CFLAGS += -Wmissing-format-attribute#  Same as -Wsuggest-attribute=format.  Use the latter option instead.
# CFLAGS += -Wmissing-include-dirs   #   Warn about user-specified include directories that do not exist.
# CFLAGS += -Wmissing-noreturn       #   Same as -Wsuggest-attribute=noreturn.  Use the latter option instead.
# CFLAGS += -Wmissing-parameter-type #   Warn about function parameters declared without a type specifier in K&R-style functions.
# CFLAGS += -Wmissing-profile        #   Warn in case profiles in -fprofile-use do not exist.
# CFLAGS += -Wmissing-prototypes      #  Warn about global functions without prototypes.
# CFLAGS += -Wmultichar             #    Warn about use of multi-character character constants.
# CFLAGS += -Wmultistatement-macros   #  Warn about unsafe macros expanding to multiple statements used as a body of a clause such as if, else, while, switch, or for.
# CFLAGS += -Wnarrowing              #   Warn about narrowing conversions within { } that are ill-formed in C++11.
# CFLAGS += -Wnested-externs         #   Warn about "extern" declarations not at file scope.
# CFLAGS += -Wno-alloc-size-larger-than # -Wno-alloc-size-larger-than Disable Walloc-size-larger-than= warning.  Equivalent to Walloc-size-larger-than=<SIZE_MAX> or larger.  Same as -Walloc-size-larger-than=.
# CFLAGS += -Wno-alloca-larger-than   #  -Wno-alloca-larger-than Disable Walloca-larger-than= warning.  Equivalent to Walloca-larger-than=<SIZE_MAX> or larger.  Same as -Walloca-larger-than=.
# CFLAGS += -Wno-vla-larger-than      #  -Wno-vla-larger-than Disable Wvla-larger-than= warning.  Equivalent to Wvla-larger-than=<SIZE_MAX> or larger.  Same as -Wvla-larger-than=.
# CFLAGS += -Wnonnull                 #  Warn about NULL being passed to argument slots marked as requiring non-NULL.
# CFLAGS += -Wnonnull-compare          # Warn if comparing pointer parameter with nonnull attribute with NULL.
# CFLAGS += -Wnormalized              #  Same as -Wnormalized=.  Use the latter option instead.
# # CFLAGS += -Wnormalized=[none|id|nfc|nfkc]# Warn about non-normalized Unicode strings.
# CFLAGS += -Wnull-dereference        #  Warn if dereferencing a NULL pointer may lead to erroneous or undefined behavior.
# CFLAGS += -Wodr                     #  Warn about some C++ One Definition Rule violations during link time optimization.
# CFLAGS += -Wold-style-declaration   #  Warn for obsolescent usage in a declaration.
# CFLAGS += -Wold-style-definition    #  Warn if an old-style parameter definition is used.
# CFLAGS += -Wopenmp-simd             #  Warn if a simd directive is overridden by the vectorizer cost model.
# CFLAGS += -Woverflow                #  Warn about overflow in arithmetic expressions.
# CFLAGS += -Woverlength-strings      #  Warn if a string is longer than the maximum portable length specified by the standard.
# CFLAGS += -Woverride-init          #   Warn about overriding initializers without side effects.
# CFLAGS += -Woverride-init-side-effects# Warn about overriding initializers with side effects.
# CFLAGS += -Wpacked                   # Warn when the packed attribute has no effect on struct layout.
# CFLAGS += -Wpacked-bitfield-compat  #  Warn about packed bit-fields whose offset changed in GCC 4.4.
# CFLAGS += -Wpacked-not-aligned      #  Warn when fields in a struct with the packed attribute are misaligned.
# CFLAGS += -Wpadded                  #  Warn when padding is required to align structure members.
# CFLAGS += -Wparentheses            #   Warn about possibly missing parentheses.
# CFLAGS += -Wpedantic                #  Issue warnings needed for strict compliance to the standard.
# CFLAGS += -Wpointer-arith          #   Warn about function pointer arithmetic.
# CFLAGS += -Wpointer-compare       #    Warn when a pointer is compared with a zero character constant.
# CFLAGS += -Wpointer-sign           #   Warn when a pointer differs in signedness in an assignment.
# CFLAGS += -Wpointer-to-int-cast    #   Warn when a pointer is cast to an integer of a different size.
# CFLAGS += -Wpragmas                #   Warn about misuses of pragmas.
# CFLAGS += -Wprio-ctor-dtor         #   Warn if constructor or destructors with priorities from 0 to 100 are used.
# CFLAGS += -Wpsabi                   #  This option lacks documentation.
# CFLAGS += -Wredundant-decls         #  Warn about multiple declarations of the same object.
# CFLAGS += -Wrestrict               #   Warn when an argument passed to a restrict-qualified parameter aliases with another argument.
# CFLAGS += -Wreturn-local-addr      #   Warn about returning a pointer/reference to a local or temporary variable.
# CFLAGS += -Wreturn-type            #   Warn whenever a function's return type defaults to "int" (C), or about inconsistent return types (C++).
# CFLAGS += -Wscalar-storage-order  #    Warn on suspicious constructs involving reverse scalar storage order.
# CFLAGS += -Wsequence-point        #    Warn about possible violations of sequence point rules.
# CFLAGS += -Wshadow                #    Warn when one variable shadows another.  Same as -Wshadow=global.
# CFLAGS += -Wshadow-compatible-local #  Same as -Wshadow=compatible-local.  Use the latter option instead.
# CFLAGS += -Wshadow-local          #    Same as -Wshadow=local.  Use the latter option instead.
# CFLAGS += -Wshadow=compatible-local #  Warn when one local variable shadows another local variable or parameter of compatible type.
# CFLAGS += -Wshadow=global          #   Warn when one variable shadows another (globally).  Same as -Wshadow.
# CFLAGS += -Wshadow=local           #   Warn when one local variable shadows another local variable or parameter.
# CFLAGS += -Wshift-count-negative  #    Warn if shift count is negative.
# CFLAGS += -Wshift-count-overflow   #   Warn if shift count >= width of type.
# CFLAGS += -Wshift-negative-value   #   Warn if left shifting a negative value.
# CFLAGS += -Wshift-overflow         #   Warn if left shift of a signed value overflows.  Same as -Wshift-overflow=.
# # CFLAGS += -Wshift-overflow=<0,2>    #  Warn if left shift of a signed value overflows.
# CFLAGS += -Wsign-compare            #  Warn about signed-unsigned comparisons.
# CFLAGS += -Wsign-conversion          # Warn for implicit type conversions between signed and unsigned integers.
# CFLAGS += -Wsizeof-array-argument   #  Warn when sizeof is applied on a parameter declared as an array.
# CFLAGS += -Wsizeof-pointer-div     #   Warn about suspicious divisions of two sizeof expressions that don't work correctly with pointers.
# CFLAGS += -Wsizeof-pointer-memaccess#  Warn about suspicious length parameters to certain string functions if the argument uses sizeof.
# CFLAGS += -Wstack-protector       #    Warn when not issuing stack smashing protection for some reason.
# # CFLAGS += -Wstack-usage=<byte-size>#   Warn if stack usage might exceed <byte-size>.
# CFLAGS += -Wstrict-aliasing       #    Warn about code which might break strict aliasing rules.
# # CFLAGS += -Wstrict-aliasing=<0,3> #    Warn about code which might break strict aliasing rules.
# CFLAGS += -Wstrict-overflow       #    Warn about optimizations that assume that signed overflow is undefined.
# # CFLAGS += -Wstrict-overflow=<0,5> #    Warn about optimizations that assume that signed overflow is undefined.
# CFLAGS += -Wstrict-prototypes      #   Warn about unprototyped function declarations.
# CFLAGS += -Wstringop-overflow       #  Warn about buffer overflow in string manipulation functions like memcpy and strcpy.  Same as -Wstringop-overflow=.
# # CFLAGS += -Wstringop-overflow=<0,4>  # Under the control of Object Size type, warn about buffer overflow in string manipulation functions like memcpy and strcpy.
# CFLAGS += -Wstringop-truncation    #   Warn about truncation in string manipulation functions like strncat and strncpy.
# CFLAGS += -Wsuggest-attribute=cold  #  Warn about functions which might be candidates for __attribute__((cold)).
# CFLAGS += -Wsuggest-attribute=const #  Warn about functions which might be candidates for __attribute__((const)).
# CFLAGS += -Wsuggest-attribute=format # Warn about functions which might be candidates for format attributes.
# CFLAGS += -Wsuggest-attribute=malloc # Warn about functions which might be candidates for __attribute__((malloc)).
# CFLAGS += -Wsuggest-attribute=noreturn# Warn about functions which might be candidates for __attribute__((noreturn)).
# CFLAGS += -Wsuggest-attribute=pure  #  Warn about functions which might be candidates for __attribute__((pure)).
# CFLAGS += -Wsuggest-final-methods  #   Warn about C++ virtual methods where adding final keyword would improve code quality.
# CFLAGS += -Wsuggest-final-types   #    Warn about C++ polymorphic types where adding final keyword would improve code quality.
# CFLAGS += -Wswitch               #     Warn about enumerated switches, with no default, missing a case.
# CFLAGS += -Wswitch-bool          #     Warn about switches with boolean controlling expression.
# CFLAGS += -Wswitch-default      #      Warn about enumerated switches missing a "default:" statement.
# CFLAGS += -Wswitch-enum         #      Warn about all enumerated switches missing a specific case.
# CFLAGS += -Wswitch-unreachable  #      Warn about statements between switch's controlling expression and the first case.
# CFLAGS += -Wsync-nand            #     Warn when __sync_fetch_and_nand and __sync_nand_and_fetch built-in functions are used.
# CFLAGS += -Wsystem-headers     #       Do not suppress warnings from system headers.
# CFLAGS += -Wtautological-compare #     Warn if a comparison always evaluates to true or false.
# CFLAGS += -Wtraditional           #    Warn about features not present in traditional C.
# CFLAGS += -Wtraditional-conversion #   Warn of prototypes causing type conversions different from what would happen in the absence of prototype.
# CFLAGS += -Wtrampolines            #   Warn whenever a trampoline is generated.
# CFLAGS += -Wtrigraphs               #  Warn if trigraphs are encountered that might affect the meaning of the program.
# CFLAGS += -Wtype-limits           #    Warn if a comparison is always true or always false due to the limited range of the data type.
# CFLAGS += -Wundef                   #  Warn if an undefined macro is used in an #if directive.
# CFLAGS += -Wuninitialized          #  Warn about uninitialized automatic variables.
# CFLAGS += -Wunknown-pragmas         #  Warn about unrecognized pragmas.
# CFLAGS += -Wunreachable-code        #  Does nothing. Preserved for backward compatibility.
# CFLAGS += -Wunsafe-loop-optimizations# Does nothing. Preserved for backward compatibility.
# CFLAGS += -Wunsuffixed-float-constants# Warn about unsuffixed float constants.
# CFLAGS += -Wunused                  #  Enable all -Wunused- warnings.
# CFLAGS += -Wunused-but-set-parameter # Warn when a function parameter is only set, otherwise unused.
# CFLAGS += -Wunused-but-set-variable #  Warn when a variable is only set, otherwise unused.
# CFLAGS += -Wunused-const-variable   #  Warn when a const variable is unused.  Same as -Wunused-const-variable=.
# # CFLAGS += -Wunused-const-variable=<0,2># Warn when a const variable is unused.
# CFLAGS += -Wunused-function        #   Warn when a function is unused.
# CFLAGS += -Wunused-label          #    Warn when a label is unused.
# CFLAGS += -Wunused-local-typedefs    # Warn when typedefs locally defined in a function are not used.
# CFLAGS += -Wunused-macros           #  Warn about macros defined in the main file that are not used.
# CFLAGS += -Wunused-parameter      #    Warn when a function parameter is unused.
# CFLAGS += -Wunused-result          #   Warn if a caller of a function, marked with attribute warn_unused_result, does not use its return value.
# CFLAGS += -Wunused-value          #    Warn when an expression value is unused.
# CFLAGS += -Wunused-variable       #    Warn when a variable is unused.
# CFLAGS += -Wvarargs                #   Warn about questionable usage of the macros used to retrieve variable arguments.
# CFLAGS += -Wvariadic-macros         #  Warn about using variadic macros.
# CFLAGS += -Wvector-operation-performance #Warn when a vector operation is compiled outside the SIMD.
# CFLAGS += -Wvla                     #  Warn if a variable length array is used.
# # CFLAGS += -Wvla-larger-than=<number>#  Warn on unbounded uses of variable-length arrays, and on bounded uses of variable-length arrays whose bound can be larger than <number> bytes. <number> bytes.
# CFLAGS += -Wvolatile-register-var   #  Warn when a register variable is declared volatile.
# CFLAGS += -Wwrite-strings           #  In C++, nonzero means warn about deprecated conversion from string literals to 'char *'.  In C, similar warning, except that the conversion is of course not deprecated by the ISO C standard.
#检查函数问题
#把W替换Wno-即可
#不检查问题
CFLAGS += -fsigned-char #riscv default (-funsigned-char) you chan set shell  `riscv-nuclei-elf-gcc -dM -E - < /dev/null|grep  CHAR_UNSIGNED` know char default,
CFLAGS += -msave-restore  -fno-unroll-loops  -Os   -flto
CFLAGS += -g 
CFLAGS += -march=$(RISCV_ARCH)
CFLAGS += -mabi=$(RISCV_ABI)
CFLAGS += -mcmodel=medlow 
CFLAGS += -ffunction-sections -fdata-sections -fno-common
CFLAGS += -D$(DOWNLOAD)
# # CFLAGS += -fsanitize=address
# CFLAGS +=  -pedantic
# CFLAGS += -fstack-protector-strong #强化栈保护	栈缓冲区溢出
# CFLAGS += -D_FORTIFY_SOURCE=s #运行时缓冲区检查	内存越界访问
# CFLAGS += -Wl,-z,relro,-z,now #只读重定位	GOT 覆盖攻击
# CFLAGS += -fPIE #-pie #位置无关可执行	代码复用攻击,pie无效
# #CFLAGS += -ftrivial-auto-var-init=pattern	#初始化变量
# #CFLAGS += -fcf-protection=full	#控制流完整性保护
# Makefile 安全基线配置
# CFLAGS += -Wall -Wextra -Wpedantic -Wconversion -Werror
# LDFLAGS += -Wl,-z,defs -Wl,-z,noexecstack
ifeq ($(SAVE_TEMPS_FILE),1) 
CFLAGS += -save-temps=obj
endif
# CFLAGS += -save-temps#保存中間
# $(C_OBJS): %.o: %.c $(HEADERS)
# 	@$(CC) $(CFLAGS) -include sys/cdefs.h -Wp,-MD,$(idep_file) -E $< -o $@
#CFLAGS +=  -std=gnu99 -std=c99
ifeq ($(USER_RISCV_LIBC_A),0) 
CFLAGS += -fno-builtin-printf 
CFLAGS += -fno-builtin-memset
CFLAGS += -DUSER_AE10X_LIBC_A 
ifeq ($(OS),Linux)
ifneq ($(wildcard $(AE10X_DIR)/libLinuxAE10X.a),)
LDFLAGS += -L$(AE10X_DIR) -lLinuxAE10X
endif
else #Windows or other commands
ifneq ($(wildcard $(AE10X_DIR)/libWinAE10X.a),)
LDFLAGS += -L $(AE10X_DIR) -lWinAE10X
endif
endif

LDFLAGS += -nostdlib  -nodefaultlibs
endif
ifeq ($(USER_RISCV_LIBC_A),1) 
CFLAGS += -DUSER_RISCV_LIBC_A 
CFLAGS += -D_COMPILING_NEWLIB
LDFLAGS += --specs=nano.specs --specs=nosys.specs
endif
LINK_OBJS_WITHOUT_COMMENT := $(notdir $(LINK_OBJS))
dep_files := $(foreach f,$(LINK_OBJS_WITHOUT_COMMENT), $(f).d)
dep_files := $(wildcard $(dep_files))
CFLAGS1 += -DROM_QIDONG
CFLAGS1 += -DDUBLE_FIRMWARE1
CFLAGS1 += $(CFLAGS)
CFLAGS2 += -DDUBLE_FIRMWARE2
CFLAGS2 += $(CFLAGS)
ifneq ($(dep_files),)
  include $(dep_files)
endif
LDFLAGS1 += $(LDFLAGS)
LDFLAGS2 += $(LDFLAGS)

LDFLAGS += -Wl,-Map=$(PROGRAM).map
LDFLAGS1 += -Wl,-Map=$(PROGRAM)_1.map
LDFLAGS2 += -Wl,-Map=$(PROGRAM)_2.map
.PHONY: Target
#########################
ifeq ($(DUAL_BOOT_FLAG),0)
Target: $(TARGET)
$(TARGET): $(LINK_OBJS) $(LINK_DEPS)
	@$(CC) $(CFLAGS) $(LINK_OBJS) -o $@ $(LDFLAGS)
	@echo CC  	$@
ifeq ($(OS),Linux) #Linux
	@echo "**********************************************************************************"
	@echo " Embedded Controller  SPK32$(CHIP) Series Firmware SIZE:"
	@echo ""
	@$(SIZE) $@
	@echo ""
	@echo "Copyright ©2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved."
	@echo "**********************************************************************************"
else #Windows or other commands
	@echo **********************************************************************************
	@echo  Embedded Controller  SPK32$(CHIP) Series Firmware SIZE:
	@$(SIZE) $@
	@echo Copyright (c)2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
	@echo **********************************************************************************
endif
# idep_file =$(@:.i=.d)
odep_file =$(@:.o=.d)
# $(C_OBJS): %.o: %.s  $(HEADERS)
# 	@$(CC) $(CFLAGS)   -c $< -o $@ 
# 	@$(OBJDUMP) -D $@ > $(<:.s=.asm)
# 	@echo CC 	$(notdir $@)
# $(C_SBJS): %.s: %.i  $(HEADERS)
# 	@$(CC) $(CFLAGS)   -S $< -o $@
# $(C_IBJS): %.i: %.c $(HEADERS)
# 	@$(CC) $(CFLAGS) -include sys/cdefs.h -Wp,-MD,$(idep_file) -E $< -o $@
$(C_OBJS): %.o: %.c $(HEADERS)
	@$(CC) $(CFLAGS) $(CINCLUDES) -include sys/cdefs.h -Wp,-MD,$(odep_file) -c $< -o $@ 
	@echo CC    $(notdir $@) 
$(ASM_OBJS): %.o: %.S $(HEADERS)
	@$(CC) $(CFLAGS) -Wp,-MD,$(odep_file) -c $< -o $@
	@$(OBJDUMP) -D $@ > $(@:.o=.asm)
	@echo CC 	$(notdir $@)
#########################
#########################
else ifeq ($(DUAL_BOOT_FLAG),1)
Target: $(TARGET_BOOT1) $(TARGET_BOOT2)
$(TARGET_BOOT1): $(LINK_OBJS_BOOT1) $(LINK_DEPS1)
	@$(CC) $(CFLAGS1) $(LINK_OBJS_BOOT1) -o $@ $(LDFLAGS1)
	@echo CC  	$@
ifeq ($(OS),Linux) #Linux
	@echo "**********************************************************************************"
	@echo " Embedded Controller  SPK32$(CHIP) Series Firmware SIZE:"
	@echo ""
	@$(SIZE) $@
	@echo ""
	@echo "Copyright ©2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved."
	@echo "**********************************************************************************"
else #Windows or other commands
	@echo **********************************************************************************
	@echo  Embedded Controller  SPK32$(CHIP) Series Firmware SIZE:
	@$(SIZE) $@
	@echo Copyright (c)2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
	@echo **********************************************************************************
endif
$(TARGET_BOOT2): $(LINK_OBJS_BOOT2) $(LINK_DEPS2)
	@$(CC) $(CFLAGS2) $(LINK_OBJS_BOOT2) -o $@ $(LDFLAGS2)
	@echo CC  	$@
ifeq ($(OS),Linux) #Linux
	@echo "**********************************************************************************"
	@echo " Embedded Controller  SPK32$(CHIP) Series Firmware SIZE:"
	@echo ""
	@$(SIZE) $@
	@echo ""
	@echo "Copyright ©2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved."
	@echo "**********************************************************************************"
else #Windows or other commands
	@echo **********************************************************************************
	@echo  Embedded Controller  SPK32$(CHIP) Series Firmware SIZE:
	@$(SIZE) $@
	@echo Copyright (c)2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
	@echo **********************************************************************************
endif
# idep_file1 =$(@:1.i=1.d)
odep_file1 =$(@:1.o=1.d)
# idep_file2 =$(@:2.i=2.d)
odep_file2 =$(@:2.o=2.d)

$(C_OBJS_BOOT1): %1.o: %.c $(HEADERS)
	@$(CC) $(CFLAGS1) $(CINCLUDES) -include sys/cdefs.h -Wp,-MD,$(odep_file1) -c $< -o $@ 
	@echo CC    $(notdir $@) 
$(ASM_OBJS_BOOT1): %1.o: %.S $(HEADERS)
	@$(CC) $(CFLAGS1) -Wp,-MD,$(odep_file1) -c -o $@ $<
	@$(OBJDUMP) -D $@ > $(@:.o=.asm)
	@echo CC  	$(subst $(shell pwd)/,,$@)
# $(C_OBJS_BOOT1): %1.o: %1.s $(HEADERS)
# 	@$(CC) $(CFLAGS1)  -c $< -o $@ 
# 	@echo CC  	$(subst $(shell pwd)/,,$@)
# $(C_SBJS_BOOT1): %1.s: %1.i  $(HEADERS)
# 	@$(CC) $(CFLAGS1)   -S $< -o $@
# $(C_IBJS_BOOT1): %1.i: %.c $(HEADERS)
# 	@$(CC) $(CFLAGS1) -include sys/cdefs.h -Wp,-MD,$(idep_file1) -E $< -o $@

$(C_OBJS_BOOT2): %2.o: %.c $(HEADERS)
	@$(CC) $(CFLAGS2) $(CINCLUDES) -include sys/cdefs.h  -Wp,-MD,$(odep_file2) -c $< -o $@ 
	@echo CC    $(notdir $@) 
$(ASM_OBJS_BOOT2): %2.o: %.S $(HEADERS)
	@$(CC) $(CFLAGS2) -Wp,-MD,$(odep_file2) -c -o $@ $<
	@$(OBJDUMP) -D $@ > $(@:.o=.asm)
	@echo CC  	$(subst $(shell pwd)/,,$@)
# $(C_OBJS_BOOT2): %2.o: %2.s $(HEADERS)
# 	@$(CC) $(CFLAGS2)  -c $< -o $@ 
# 	@echo CC  	$(subst $(shell pwd)/,,$@)
# $(C_SBJS_BOOT2): %2.s: %2.i  $(HEADERS)
# 	@$(CC) $(CFLAGS2)   -S $< -o $@
# $(C_IBJS_BOOT2): %2.i: %.c $(HEADERS)
# 	@$(CC) $(CFLAGS2) -include sys/cdefs.h -Wp,-MD,$(idep_file2) -E $< -o $@
endif
#########################
.PHONY: clean
clean:
ifeq ($(OS),Linux) #Linux
	@rm -f $(CLEAN_OBJS)
	@echo RM -F CLEAN_OBJS
else #Windows or other commands
	@del /q /f /s $(CLEAN_OBJS)
endif
