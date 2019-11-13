################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include xbee/subdir.mk
-include sleep/subdir.mk
-include sfled/subdir.mk
-include robot/subdir.mk
-include rgb_sensor/subdir.mk
-include pwm/subdir.mk
-include ps/subdir.mk
-include motor/subdir.mk
-include hbridge/subdir.mk
-include encoder/subdir.mk
-include clk/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
MEMORYMAP += \
memory-map.xml \

IHEXFILES += \
robot_mss_MSS_CM3_0_app.hex \

SRECFILES += \
robot_mss_MSS_CM3_0_app.srec \

LISTINGS += \
robot_mss_MSS_CM3_0_app.lst \


# All Target
all: robot_mss_MSS_CM3_0_app secondary-outputs

# Tool invocations
robot_mss_MSS_CM3_0_app: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU C Linker'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -L"C:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\Debug" -TC:\Users\Brandon\Desktop\robot\SoftConsole\robot_mss_MSS_CM3_0\robot_mss_MSS_CM3_0_hw_platform\CMSIS\startup_gcc\debug-in-actel-smartfusion-envm.ld -Wl,-Map=$(EXECUTABLE).map -Xlinker -gc-sections -o"robot_mss_MSS_CM3_0_app" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

memory-map.xml: $(EXECUTABLE)
	@echo 'Invoking: Memory map generator'
	actel-map -o "memory-map.xml" $(EXECUTABLE) $(BUILDCMD)
	@echo 'Finished building: $@'
	@echo ' '

robot_mss_MSS_CM3_0_app.hex: $(EXECUTABLE)
	@echo 'Invoking: GNU Intel Hex File Generator'
	arm-none-eabi-objcopy -O ihex $(EXECUTABLE) "robot_mss_MSS_CM3_0_app.hex"
	@echo 'Finished building: $@'
	@echo ' '

robot_mss_MSS_CM3_0_app.srec: $(EXECUTABLE)
	@echo 'Invoking: GNU S-Record Generator'
	arm-none-eabi-objcopy -O srec $(EXECUTABLE) "robot_mss_MSS_CM3_0_app.srec"
	@echo 'Finished building: $@'
	@echo ' '

robot_mss_MSS_CM3_0_app.lst: $(EXECUTABLE)
	@echo 'Invoking: GNU Listing Generator'
	arm-none-eabi-objdump -h -S $(EXECUTABLE) > "robot_mss_MSS_CM3_0_app.lst"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean::
	-$(RM) $(MEMORYMAP)$(OBJS)$(C_DEPS)$(SRECFILES)$(IHEXFILES)$(LISTINGS)$(EXECUTABLE) robot_mss_MSS_CM3_0_app
	-@echo ' '

secondary-outputs: $(MEMORYMAP) $(IHEXFILES) $(SRECFILES) $(LISTINGS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
