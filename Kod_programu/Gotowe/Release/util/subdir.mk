################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../util/HD44780.c \
../util/StepMotor.c \
../util/VL53L0X.c \
../util/i2cmaster.c \
../util/millis.c 

OBJS += \
./util/HD44780.o \
./util/StepMotor.o \
./util/VL53L0X.o \
./util/i2cmaster.o \
./util/millis.o 

C_DEPS += \
./util/HD44780.d \
./util/StepMotor.d \
./util/VL53L0X.d \
./util/i2cmaster.d \
./util/millis.d 


# Each subdirectory must supply rules for building sources it contributes
util/%.o: ../util/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -IC:/ProGFX/include -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


