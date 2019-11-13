################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../xbee/xbee.c 

OBJS += \
./xbee/xbee.o 

C_DEPS += \
./xbee/xbee.d 


# Each subdirectory must supply rules for building sources it contributes
xbee/%.o: ../xbee/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU C Compiler'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\CMSIS -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\CMSIS\startup_gcc -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\Core16550 -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_gpio -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers\F2DSS_NVM -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_pdma -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers\mss_rtc -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_timer -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_uart -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\hal -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\hal\CortexM3 -IC:\Users\rmall\Documents\college\junior\eecs373\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\hal\CortexM3\GNU -O0 -ffunction-sections -fdata-sections -g3 -Wall -c -fmessage-length=0 -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


