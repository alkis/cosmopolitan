#-*-mode:makefile-gmake;indent-tabs-mode:t;tab-width:8;coding:utf-8-*-┐
#── vi: set noet ft=make ts=8 sw=8 fenc=utf-8 :vi ────────────────────┘

PKGS += THIRD_PARTY_COMPILER_RT

THIRD_PARTY_COMPILER_RT_ARTIFACTS += THIRD_PARTY_COMPILER_RT_A
THIRD_PARTY_COMPILER_RT = $(THIRD_PARTY_COMPILER_RT_A_DEPS) $(THIRD_PARTY_COMPILER_RT_A)
THIRD_PARTY_COMPILER_RT_A = o/$(MODE)/third_party/compiler_rt/compiler_rt.a
THIRD_PARTY_COMPILER_RT_A_FILES :=				\
	$(wildcard third_party/compiler_rt/*)			\
	$(wildcard third_party/compiler_rt/nexgen32e/*)
THIRD_PARTY_COMPILER_RT_A_HDRS = $(filter %.h,$(THIRD_PARTY_COMPILER_RT_A_FILES))
THIRD_PARTY_COMPILER_RT_A_INCS = $(filter %.inc,$(THIRD_PARTY_COMPILER_RT_A_FILES))
THIRD_PARTY_COMPILER_RT_A_SRCS_S = $(filter %.S,$(THIRD_PARTY_COMPILER_RT_A_FILES))
THIRD_PARTY_COMPILER_RT_A_SRCS_C = $(filter %.c,$(THIRD_PARTY_COMPILER_RT_A_FILES))

THIRD_PARTY_COMPILER_RT_A_SRCS =				\
	$(THIRD_PARTY_COMPILER_RT_A_SRCS_S)			\
	$(THIRD_PARTY_COMPILER_RT_A_SRCS_C)

THIRD_PARTY_COMPILER_RT_A_OBJS =				\
	$(THIRD_PARTY_COMPILER_RT_A_SRCS_S:%.S=o/$(MODE)/%.o)	\
	$(THIRD_PARTY_COMPILER_RT_A_SRCS_C:%.c=o/$(MODE)/%.o)

THIRD_PARTY_COMPILER_RT_A_CHECKS =				\
	$(THIRD_PARTY_COMPILER_RT_A).pkg			\
	$(THIRD_PARTY_COMPILER_RT_A_HDRS:%=o/$(MODE)/%.ok)

THIRD_PARTY_COMPILER_RT_A_DIRECTDEPS =				\
	LIBC_INTRIN						\
	LIBC_NEXGEN32E						\
	LIBC_SYSV

THIRD_PARTY_COMPILER_RT_A_DEPS :=				\
	$(call uniq,$(foreach x,$(THIRD_PARTY_COMPILER_RT_A_DIRECTDEPS),$($(x))))

$(THIRD_PARTY_COMPILER_RT_A):					\
		third_party/compiler_rt/			\
		$(THIRD_PARTY_COMPILER_RT_A).pkg		\
		$(THIRD_PARTY_COMPILER_RT_A_OBJS)

$(THIRD_PARTY_COMPILER_RT_A).pkg:				\
		$(THIRD_PARTY_COMPILER_RT_A_OBJS)		\
		$(foreach x,$(THIRD_PARTY_COMPILER_RT_A_DIRECTDEPS),$($(x)_A).pkg)

$(THIRD_PARTY_COMPILER_RT_A_OBJS): private			\
		DEFAULT_CFLAGS +=				\
			$(OLD_CODE)				\
			-DCRT_HAS_128BIT			\
			-fno-sanitize=address

# these assembly files are safe to build on aarch64
o/$(MODE)/third_party/compiler_rt/comprt.o: third_party/compiler_rt/comprt.S
	@$(COMPILE) -AOBJECTIFY.S $(OBJECTIFY.S) $(OUTPUT_OPTION) -c $<

THIRD_PARTY_COMPILER_RT_LIBS = $(foreach x,$(THIRD_PARTY_COMPILER_RT_ARTIFACTS),$($(x)))
THIRD_PARTY_COMPILER_RT_SRCS = $(foreach x,$(THIRD_PARTY_COMPILER_RT_ARTIFACTS),$($(x)_SRCS))
THIRD_PARTY_COMPILER_RT_HDRS = $(foreach x,$(THIRD_PARTY_COMPILER_RT_ARTIFACTS),$($(x)_HDRS))
THIRD_PARTY_COMPILER_RT_INCS = $(foreach x,$(THIRD_PARTY_COMPILER_RT_ARTIFACTS),$($(x)_INCS))
THIRD_PARTY_COMPILER_RT_CHECKS = $(foreach x,$(THIRD_PARTY_COMPILER_RT_ARTIFACTS),$($(x)_CHECKS))
THIRD_PARTY_COMPILER_RT_OBJS = $(foreach x,$(THIRD_PARTY_COMPILER_RT_ARTIFACTS),$($(x)_OBJS))

.PHONY: o/$(MODE)/third_party/compiler_rt
o/$(MODE)/third_party/compiler_rt: $(THIRD_PARTY_COMPILER_RT_CHECKS)
