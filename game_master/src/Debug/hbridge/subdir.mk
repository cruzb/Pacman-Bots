################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hbridge/hbridge.c 

OBJS += \
./hbridge/hbridge.o 

C_DEPS += \
./hbridge/hbridge.d 


# Each subdirectory must supply rules for building sources it contributes
hbridge/%.o: ../hbridge/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU C Compiler'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\CMSIS -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\CMSIS\startup_gcc -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\Core16550 -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_gpio -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers\F2DSS_NVM -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_pdma -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers\mss_rtc -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_timer -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\drivers\mss_uart -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\hal -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\hal\CortexM3 -IC:\Users\rmallya\Documents\game_master\SoftConsole\GM_MSS_MSS_CM3_0\GM_MSS_MSS_CM3_0_hw_platform\hal\CortexM3\GNU -O0 -ffunction-sections -fdata-sections -g3 -Wall -c -fmessage-length=0 -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


