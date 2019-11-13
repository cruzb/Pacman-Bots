################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../motor/motor.c 

OBJS += \
./motor/motor.o 

C_DEPS += \
./motor/motor.d 


# Each subdirectory must supply rules for building sources it contributes
motor/%.o: ../motor/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU C Compiler'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\CMSIS -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\CMSIS\startup_gcc -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\drivers -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\drivers\mss_ace -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\drivers\mss_gpio -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\drivers\mss_i2c -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\drivers\mss_nvm -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers\F2DSS_NVM -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\drivers\mss_pdma -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\drivers\mss_rtc -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers\mss_rtc -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\drivers\mss_timer -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\drivers\mss_uart -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\drivers_config -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\drivers_config\mss_ace -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\hal -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\hal\CortexM3 -IC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\hal\CortexM3\GNU -O0 -ffunction-sections -fdata-sections -g3 -Wall -c -fmessage-length=0 -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


