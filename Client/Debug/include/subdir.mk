################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../include/s3service.pb.cc 

CC_DEPS += \
./include/s3service.pb.d 

OBJS += \
./include/s3service.pb.o 


# Each subdirectory must supply rules for building sources it contributes
include/%.o: ../include/%.cc include/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ --std=c++17 -I"/home/msys/objectPOC/POC/updated_code12_05_2023/Client/amqpcpp/include" -I"/home/msys/objectPOC/POC/updated_code12_05_2023/Client/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


