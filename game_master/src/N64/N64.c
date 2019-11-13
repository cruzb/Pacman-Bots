#include "N64.h"

#define N64_BAUD_RATE	1000000 //4 x 250,000

#define UART_ADDR 0x40050000
#define EN_N64_TO_UART_ADDR 0x40051000

#define N64_00				0x04
#define N64_01				0x34
#define N64_10				0x07
#define N64_11				0x37

#define N64_CONSOLE_STOP	0xFF
#define N64_CONTROLLER_STOP	0xFC

#define BUTTON_MASK_A	0x80000000
#define BUTTON_MASK_B	0x40000000
#define BUTTON_MASK_Z	0x20000000
#define BUTTON_MASK_S	0x10000000
#define BUTTON_MASK_DU	0x08000000
#define BUTTON_MASK_DD	0x04000000
#define BUTTON_MASK_DL	0x02000000
#define BUTTON_MASK_DR	0x01000000
#define BUTTON_MASK_L	0x00800000
#define BUTTON_MASK_R	0x00400000
#define BUTTON_MASK_CU	0x00200000
#define BUTTON_MASK_CD	0x00100000
#define BUTTON_MASK_CL	0x00080000
#define BUTTON_MASK_CR	0x00040000
#define BUTTON_MASK_AX	0x0000FF00
#define BUTTON_MASK_AY	0x000000FF

button_t buttons[NUM_BUTTONS] = {
			{"A", BUTTON_MASK_A},
			{"B", BUTTON_MASK_B},
			{"Z", BUTTON_MASK_Z},
			{"S", BUTTON_MASK_S},
			{"DU", BUTTON_MASK_DU},
			{"DD", BUTTON_MASK_DD},
			{"DL", BUTTON_MASK_DL},
			{"DR", BUTTON_MASK_DR},
			{"L", BUTTON_MASK_L},
			{"R", BUTTON_MASK_R},
			{"CU", BUTTON_MASK_CU},
			{"CD", BUTTON_MASK_CD},
			{"CL", BUTTON_MASK_CL},
			{"CR", BUTTON_MASK_CR},
			{"AX", BUTTON_MASK_AX},
			{"AY", BUTTON_MASK_AY}
};

uart_16550_instance_t g_uart;
uint8_t rx_buff[16];

void print_state(uint32_t state, int inline_print) {
	if (inline_print)
		printf("\r");
	int i = 0;
	for(;i < NUM_BUTTONS; ++i) {
		if(state & buttons[i].mask) {
			printf("%s ", buttons[i].name);
		}
		else {
			printf(" ");
		}
	}
	if (inline_print)
		fflush(stdout);
	else
		printf("\r\n");
}

uint32_t read_poll(uint8_t* buf, uint32_t len) {
	uint32_t out = 0;
	uint32_t i = 0;
	for(;i < len; ++i) {
		uint8_t bits = 0;
		switch(buf[i]) {
		case 0x04:
			bits = 0x0;
			break;
		case 0x34:
			bits = 0x01;
			break;
		case 0x07:
			bits = 0x02;
			break;
		case 0x37:
			bits = 0x03;
			break;
		default:
			break;
			printf("\rUnk: %u\n", buf[i]);
		}

		out |= (bits << (30 - (i * 2)));
	}
	return out;
}

void enable_n64_to_uart(uint32_t addr) {
	uint32_t* ptr = (uint32_t*)addr;
	*ptr = 1;
}

void disable_n64_to_uart(uint32_t addr) {
	uint32_t* ptr = (uint32_t*)addr;
	*ptr = 0;
}

void N64Init(N64Inst_t *inst, volatile uint32_t *addr)
{
	inst->base_addr = addr;
	inst->state = 0;
	UART_16550_init(&g_uart, UART_ADDR, 6, UART_16550_DATA_6_BITS | UART_16550_NO_PARITY | UART_16550_ONE_STOP_BIT);
	N64Enable(inst);
}

void N64Enable(N64Inst_t *inst)
{
	inst->state = 0;
	NVIC_EnableIRQ(Fabric_IRQn);
	enable_n64_to_uart(EN_N64_TO_UART_ADDR);
}

void N64Disable(N64Inst_t *inst)
{
	disable_n64_to_uart(EN_N64_TO_UART_ADDR);
	NVIC_DisableIRQ(Fabric_IRQn);
	inst->state = 0;
}

void N64GetState(N64Inst_t *inst)
{
	inst->state = read_poll(rx_buff, sizeof(rx_buff));
	UART_16550_get_rx(&g_uart, rx_buff, sizeof(rx_buff));
}

int N64ButtonPressed(N64Inst_t *inst, button_type_t b)
{
	return inst->state & buttons[b].mask;
}

void N64PrintState(N64Inst_t *inst, int inline_print)
{
	print_state(inst->state, inline_print);
}

direction_t N64ADir(N64Inst_t *inst) {
	int8_t x = (int8_t)(inst->state & BUTTON_MASK_AX);
	int8_t y = (int8_t)(inst->state & BUTTON_MASK_AY);
	if(x < -30) {
		return LEFT;
	}
	if(x > 30) {
		return RIGHT;
	}
	if(y < -80) {
		return DOWN;
	}
	if(y > 80) {
		return UP;
	}
	return NONE;
}

direction_t N64DDir(N64Inst_t *inst) {
	if(inst->state & BUTTON_MASK_DL) {
		return LEFT;
	}
	if(inst->state & BUTTON_MASK_DR) {
		return RIGHT;
	}
	if(inst->state & BUTTON_MASK_DD) {
		return DOWN;
	}
	if(inst->state & BUTTON_MASK_DU) {
		return UP;
	}
	return NONE;
}
