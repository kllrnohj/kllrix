################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/video/FBConsole.cpp \
../src/video/Font6x11.cpp \
../src/video/Font7x14.cpp \
../src/video/Font8x16.cpp \
../src/video/Font8x8.cpp \
../src/video/FontMini4x6.cpp \
../src/video/VBEDriver.cpp 

OBJS += \
./src/video/FBConsole.o \
./src/video/Font6x11.o \
./src/video/Font7x14.o \
./src/video/Font8x16.o \
./src/video/Font8x8.o \
./src/video/FontMini4x6.o \
./src/video/VBEDriver.o 

CPP_DEPS += \
./src/video/FBConsole.d \
./src/video/Font6x11.d \
./src/video/Font7x14.d \
./src/video/Font8x16.d \
./src/video/Font8x8.d \
./src/video/FontMini4x6.d \
./src/video/VBEDriver.d 


# Each subdirectory must supply rules for building sources it contributes
src/video/%.o: ../src/video/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/kllrnohj/Programming/kllrix/kllrix2/src" -O0 -Wall -c -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-rtti -fno-builtin -fno-exceptions -fomit-frame-pointer -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


