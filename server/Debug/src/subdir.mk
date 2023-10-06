################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/accesskeys.cpp \
../src/accounts.cpp \
../src/bucket.cpp \
../src/consumer.cpp \
../src/handler.cpp \
../src/ldapserver.cpp \
../src/mssql.cpp \
../src/object.cpp \
../src/publisher.cpp \
../src/user.cpp \
../src/credential.cpp

OBJS += \
./src/accesskeys.o \
./src/accounts.o \
./src/bucket.o \
./src/consumer.o \
./src/handler.o \
./src/ldapserver.o \
./src/mssql.o \
./src/object.o \
./src/publisher.o \
./src/user.o \
./src/credential.o

CPP_DEPS += \
./src/accesskeys.d \
./src/accounts.d \
./src/bucket.d \
./src/consumer.d \
./src/handler.d \
./src/ldapserver.d \
./src/mssql.d \
./src/object.d \
./src/publisher.d \
./src/user.d \
./src/credential.d


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/msys/objectPOC/POC/updated_code12_05_2023/S3Server/amqpcpp/include" -I"/home/msys/objectPOC/POC/updated_code12_05_2023/S3Server/include" -O0 -g3 -Wall -c -fmessage-length=0 -DBOOST_NO_CXX11_SCOPED_ENUMS -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"  
	@echo 'Finished building: $<'
	@echo ' '


