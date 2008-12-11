################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/drivers/video/TextVideo.cpp \
../src/drivers/video/VBoxVideo.cpp \
../src/drivers/video/VesaVideo.cpp 

OBJS += \
./src/drivers/video/TextVideo.o \
./src/drivers/video/VBoxVideo.o \
./src/drivers/video/VesaVideo.o 

CPP_DEPS += \
./src/drivers/video/TextVideo.d \
./src/drivers/video/VBoxVideo.d \
./src/drivers/video/VesaVideo.d 


# Each subdirectory must supply rules for building sources it contributes
src/drivers/video/%.o: ../src/drivers/video/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/kllrnohj/workspace/kllrix2/src" -O0 -Wall -c -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-rtti -fno-builtin -fno-exceptions -fomit-frame-pointer -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


