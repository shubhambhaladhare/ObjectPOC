################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/consumer.cpp \
../src/publisher.cpp \
../src/auto.cpp

OBJS += \
./src/consumer.o \
./src/publisher.o \
./src/auto.o

CPP_DEPS += \
./src/consumer.d \
./src/publisher.d \
./src/auto.d

# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ --std=c++17 -I"/home/msys/objectPOC/POC/updated_code12_05_2023/Client/amqpcpp/include" -I"/home/msys/objectPOC/POC/updated_code12_05_2023/Client/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


