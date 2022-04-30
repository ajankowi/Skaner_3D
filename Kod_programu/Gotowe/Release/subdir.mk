################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../diskio.c \
../ff.c \
../ffunicode_avr.c \
../main.c \
../mmc_avr_spi.c 

OBJS += \
./diskio.o \
./ff.o \
./ffunicode_avr.o \
./main.o \
./mmc_avr_spi.o 

C_DEPS += \
./diskio.d \
./ff.d \
./ffunicode_avr.d \
./main.d \
./mmc_avr_spi.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -IC:/ProGFX/include -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


