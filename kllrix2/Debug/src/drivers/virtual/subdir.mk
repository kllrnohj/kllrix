################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/drivers/virtual/VMMDriver.cpp 

OBJS += \
./src/drivers/virtual/VMMDriver.o 

CPP_DEPS += \
./src/drivers/virtual/VMMDriver.d 


# Each subdirectory must supply rules for building sources it contributes
src/drivers/virtual/%.o: ../src/drivers/virtual/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/kllrnohj/workspace/kllrix2/src" -O0 -Wall -c -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-rtti -fno-builtin -fno-exceptions -fomit-frame-pointer -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


