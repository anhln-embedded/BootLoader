################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/uart/fsl_adapter_lpuart.c 

C_DEPS += \
./component/uart/fsl_adapter_lpuart.d 

OBJS += \
./component/uart/fsl_adapter_lpuart.o 


# Each subdirectory must supply rules for building sources it contributes
component/uart/%.o: ../component/uart/%.c component/uart/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MKE16Z64VLF4 -DCPU_MKE16Z64VLF4_cm0plus -DFRDM_KE16Z -DFREEDOM -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\source" -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\utilities" -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\drivers" -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\device" -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\component\uart" -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\component\lists" -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\CMSIS" -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\board" -I"F:\Embedded_fpt\KE16\MOCK\UART_MAIN\lpuart_interrupt\BootLoader\frdmke16z\driver_examples\lpuart\interrupt" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-uart

clean-component-2f-uart:
	-$(RM) ./component/uart/fsl_adapter_lpuart.d ./component/uart/fsl_adapter_lpuart.o

.PHONY: clean-component-2f-uart

