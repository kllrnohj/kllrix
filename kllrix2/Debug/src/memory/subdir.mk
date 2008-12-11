################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/memory/KVirtualAlloc.cpp \
../src/memory/StackAllocator.cpp \
../src/memory/kmem.cpp 

C_SRCS += \
../src/memory/liballoc.c 

OBJS += \
./src/memory/KVirtualAlloc.o \
./src/memory/StackAllocator.o \
./src/memory/kmem.o \
./src/memory/liballoc.o 

C_DEPS += \
./src/memory/liballoc.d 

CPP_DEPS += \
./src/memory/KVirtualAlloc.d \
./src/memory/StackAllocator.d \
./src/memory/kmem.d 


# Each subdirectory must supply rules for building sources it contributes
src/memory/%.o: ../src/memory/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/kllrnohj/workspace/kllrix2/src" -O0 -Wall -c -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-rtti -fno-builtin -fno-exceptions -fomit-frame-pointer -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/memory/%.o: ../src/memory/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/kllrnohj/workspace/kllrix2/src" -O0 -Wall -c -fmessage-length=0 -Wextra -nostdlib -nostartfiles -nodefaultlibs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


