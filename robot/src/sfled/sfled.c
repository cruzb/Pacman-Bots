#include "../sleep/sleep.h"
#include "drivers/mss_gpio/mss_gpio.h"
#include <stdlib.h>

void sfled_init(mss_gpio_id_t mss_gpio_id) {
	MSS_GPIO_init();
	MSS_GPIO_config( mss_gpio_id, MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_set_output(mss_gpio_id, 0xff);
}

void sfled_on(mss_gpio_id_t mss_gpio_id) {
	MSS_GPIO_set_output(mss_gpio_id, 0x00);
}

void sfled_toggle(mss_gpio_id_t mss_gpio_id) {
	MSS_GPIO_set_outputs(MSS_GPIO_get_outputs() ^ mss_gpio_id);
}

void sfled_off(mss_gpio_id_t mss_gpio_id) {
	MSS_GPIO_set_outputs(MSS_GPIO_get_outputs() | mss_gpio_id);
}

void sfled_flash(mss_gpio_id_t mss_gpio_id, uint32_t num_flashes, uint32_t usecs)
{
	sfled_off(mss_gpio_id);
	size_t i = 0;
	for(;i < 2 * num_flashes; ++i) {
		sfled_toggle(mss_gpio_id);
		usleep(usecs);
	}
	sfled_off(mss_gpio_id);
}
