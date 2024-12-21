################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/KE16_Flash.c \
../source/bootloader.c \
../source/clock.c \
../source/main.c \
../source/parsing_srec.c \
../source/port.c \
../source/uart.c 

C_DEPS += \
./source/KE16_Flash.d \
./source/bootloader.d \
./source/clock.d \
./source/main.d \
./source/parsing_srec.d \
./source/port.d \
./source/uart.d 

OBJS += \
./source/KE16_Flash.o \
./source/bootloader.o \
./source/clock.o \
./source/main.o \
./source/parsing_srec.o \
./source/port.o \
./source/uart.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MKE16Z64VLF4 -DCPU_MKE16Z64VLF4_cm0plus -DFRDM_KE16Z -DFREEDOM -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\source" -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\utilities" -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\drivers" -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\device" -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\component\uart" -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\component\lists" -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\CMSIS" -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\board" -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\frdmke16z\driver_examples\lpuart\interrupt" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/KE16_Flash.d ./source/KE16_Flash.o ./source/bootloader.d ./source/bootloader.o ./source/clock.d ./source/clock.o ./source/main.d ./source/main.o ./source/parsing_srec.d ./source/parsing_srec.o ./source/port.d ./source/port.o ./source/uart.d ./source/uart.o

.PHONY: clean-source

