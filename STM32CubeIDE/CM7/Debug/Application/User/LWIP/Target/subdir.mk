################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Levil/GitHub/projectPP2024/projectPP2024/CM7/LWIP/Target/ethernetif.c 

OBJS += \
./Application/User/LWIP/Target/ethernetif.o 

C_DEPS += \
./Application/User/LWIP/Target/ethernetif.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/LWIP/Target/ethernetif.o: C:/Users/Levil/GitHub/projectPP2024/projectPP2024/CM7/LWIP/Target/ethernetif.c Application/User/LWIP/Target/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DDATA_IN_D2_SRAM -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../../../CM7/LWIP/App -I../../../CM7/LWIP/Target -I../../../CM7/Core/Inc -I../../../Middlewares/Third_Party/LwIP/src/include -I../../../Middlewares/Third_Party/LwIP/system -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Drivers/BSP/Components/lan8742 -I../../../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/Third_Party/LwIP/src/include/lwip -I../../../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../../../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../../../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../../../Middlewares/Third_Party/LwIP/src/include/netif -I../../../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../../../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../../../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../../../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../../../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../../../Middlewares/Third_Party/LwIP/system/arch -I../../../Drivers/CMSIS/Include -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-LWIP-2f-Target

clean-Application-2f-User-2f-LWIP-2f-Target:
	-$(RM) ./Application/User/LWIP/Target/ethernetif.d ./Application/User/LWIP/Target/ethernetif.o ./Application/User/LWIP/Target/ethernetif.su

.PHONY: clean-Application-2f-User-2f-LWIP-2f-Target
