################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/drivers/dummy/PCIDriver.cpp \
../src/drivers/dummy/VideoDriver.cpp 

OBJS += \
./src/drivers/dummy/PCIDriver.o \
./src/drivers/dummy/VideoDriver.o 

CPP_DEPS += \
./src/drivers/dummy/PCIDriver.d \
./src/drivers/dummy/VideoDriver.d 


# Each subdirectory must supply rules for building sources it contributes
src/drivers/dummy/%.o: ../src/drivers/dummy/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/kllrnohj/Programming/kllrix/kllrix2/src" -O0 -Wall -c -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-rtti -fno-builtin -fno-exceptions -fomit-frame-pointer -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


