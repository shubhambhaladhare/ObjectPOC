################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../subscriber.cpp 

OBJS += \
./subscriber.o 

CPP_DEPS += \
./subscriber.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/msys/objectPOC/POC/updated_code12_05_2023/S3Server/amqpcpp/include" -I"/home/msys/objectPOC/POC/updated_code12_05_2023/S3Server/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


