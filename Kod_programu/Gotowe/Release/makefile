################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include util/subdir.mk
-include .metadata/.plugins/org.eclipse.cdt.make.core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Gotowe.lss \

FLASH_IMAGE += \
Gotowe.hex \

EEPROM_IMAGE += \
Gotowe.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: Gotowe.elf secondary-outputs

# Tool invocations
Gotowe.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Gotowe.map -mmcu=atmega32 -o "Gotowe.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Gotowe.lss: Gotowe.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Gotowe.elf  >"Gotowe.lss"
	@echo 'Finished building: $@'
	@echo ' '

Gotowe.hex: Gotowe.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex Gotowe.elf  "Gotowe.hex"
	@echo 'Finished building: $@'
	@echo ' '

Gotowe.eep: Gotowe.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex Gotowe.elf  "Gotowe.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Gotowe.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 Gotowe.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) Gotowe.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
