#include "../sleep/sleep.h"
#include "drivers/mss_gpio/mss_gpio.h"
#include <stdlib.h>

void sfled_init(mss_gpio_id_t mss_gpio_id);

void sfled_on(mss_gpio_id_t mss_gpio_id);

void sfled_toggle(mss_gpio_id_t mss_gpio_id);

void sfled_off(mss_gpio_id_t mss_gpio_id);

void sfled_flash(mss_gpio_id_t mss_gpio_id, uint32_t num_flashes, uint32_t usecs);
