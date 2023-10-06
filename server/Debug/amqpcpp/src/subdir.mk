################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../amqpcpp/src/AMQP.cpp \
../amqpcpp/src/AMQPBase.cpp \
../amqpcpp/src/AMQPException.cpp \
../amqpcpp/src/AMQPExchange.cpp \
../amqpcpp/src/AMQPMessage.cpp \
../amqpcpp/src/AMQPQueue.cpp 

O_SRCS += \
../amqpcpp/src/AMQP.o \
../amqpcpp/src/AMQPBase.o \
../amqpcpp/src/AMQPException.o \
../amqpcpp/src/AMQPExchange.o \
../amqpcpp/src/AMQPMessage.o \
../amqpcpp/src/AMQPQueue.o 

OBJS += \
./amqpcpp/src/AMQP.o \
./amqpcpp/src/AMQPBase.o \
./amqpcpp/src/AMQPException.o \
./amqpcpp/src/AMQPExchange.o \
./amqpcpp/src/AMQPMessage.o \
./amqpcpp/src/AMQPQueue.o 

CPP_DEPS += \
./amqpcpp/src/AMQP.d \
./amqpcpp/src/AMQPBase.d \
./amqpcpp/src/AMQPException.d \
./amqpcpp/src/AMQPExchange.d \
./amqpcpp/src/AMQPMessage.d \
./amqpcpp/src/AMQPQueue.d 


# Each subdirectory must supply rules for building sources it contributes
amqpcpp/src/%.o: ../amqpcpp/src/%.cpp amqpcpp/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/msys/objectPOC/POC/updated_code12_05_2023/S3Server/amqpcpp/include" -I"/home/msys/objectPOC/POC/updated_code12_05_2023/S3Server/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


