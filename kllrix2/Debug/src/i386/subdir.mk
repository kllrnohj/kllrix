################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/i386/gdt.cpp \
../src/i386/idt.cpp \
../src/i386/irq.cpp \
../src/i386/isrs.cpp \
../src/i386/kbd.cpp \
../src/i386/timer.cpp 

ASM_SRCS += \
../src/i386/misc_s.asm 

OBJS += \
./src/i386/gdt.o \
./src/i386/idt.o \
./src/i386/irq.o \
./src/i386/isrs.o \
./src/i386/kbd.o \
./src/i386/misc_s.o \
./src/i386/timer.o 

CPP_DEPS += \
./src/i386/gdt.d \
./src/i386/idt.d \
./src/i386/irq.d \
./src/i386/isrs.d \
./src/i386/kbd.d \
./src/i386/timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/i386/%.o: ../src/i386/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/kllrnohj/workspace/kllrix2/src" -O0 -Wall -c -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-rtti -fno-builtin -fno-exceptions -fomit-frame-pointer -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/i386/%.o: ../src/i386/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	nasm -f elf -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


