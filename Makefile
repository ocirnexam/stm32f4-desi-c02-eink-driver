BINARY = test

BUILD_DIR = ./build

LDSCRIPT = stm32f446ret.ld

# You should use linker script generation! Specify device!
LIBNAME		= opencm3_stm32f4
DEFS		+= -DSTM32F4
DEFS		+= -I$(OPENCM3_DIR)/include

FP_FLAGS	?= -mfloat-abi=hard -mfpu=fpv4-sp-d16
ARCH_FLAGS	= -mthumb -mcpu=cortex-m4 $(FP_FLAGS)


include rules.mk