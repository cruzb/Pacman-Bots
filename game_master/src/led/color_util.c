#include "color_util.h"

extern uint8_t WALL_COLOR[3];

color_t hsv_to_rgb(int H, double S, double V) {
	double C = V * S;
	double H1 = (double)(H/60.0);
	double fmod_result = (fmod(H1, 2));
	double X_C = (1 - fabs(fmod_result - 1));
	double X = C * X_C;
	double m = V - C;
	double Rs, Gs, Bs;


	if(H >= 0 && H < 60) {
		Rs = C;
		Gs = X;
		Bs = 0;
	}
	else if(H >= 60 && H < 120) {
		Rs = X;
		Gs = C;
		Bs = 0;
	}
	else if(H >= 120 && H < 180) {
		Rs = 0;
		Gs = C;
		Bs = X;
	}
	else if(H >= 180 && H < 240) {
		Rs = 0;
		Gs = X;
		Bs = C;
	}
	else if(H >= 240 && H < 300) {
		Rs = X;
		Gs = 0;
		Bs = C;
	}
	else {
		Rs = C;
		Gs = 0;
		Bs = X;
	}
	return (color_t){(Rs + m) * 255, (Gs + m) * 255, (Bs + m) * 255};
}


void set_colors_hsv(led_inst_t *led_inst, grid_t* grid, size_t buf) {
    int hue = 0;
    int step = 360 / (led_inst->num_grid_cols * led_inst->num_grid_rows - grid_count(grid, '1'));
    size_t row = 0, col = 0;
    for(; row < led_inst->num_grid_rows; row++)
    {
    	for (col = 0; col < led_inst->num_grid_cols; col++)
    	{
    		if (grid_get_occupancy(grid, (cell_t){row,col}) != (uint8_t)'1')
    		{
    			size_t idx = led_grid_idx_to_strip_idx(led_inst, row, col);
    			color_t rgb = hsv_to_rgb(hue, 1.0, 1.0);
    			hue = (hue + step) % 360;
    			led_set_color(led_inst, idx, buf, rgb);
    		}
    	}
    }
}

void set_colors_hsv_range(led_inst_t *led_inst, size_t start, size_t end, size_t buf)
{
	int hue = 0;
	int step = 360 / (end - start);
	for (; start != end; start++)
	{
		color_t rgb = hsv_to_rgb(hue, 1.0, 1.0);
		hue = (hue + step) % 360;
		led_set_color(led_inst, start, buf, rgb);
	}
}

//void set_colors_hsv_with_sv(led_inst_t *led_inst, size_t buf, double saturation, double value) {
//	int hue = 0;
//	int step = 360 / (led_inst->num_grid_cols * led_inst->num_grid_rows - led_inst->grid_inst->num_occupied);
//	size_t row = 0, col = 0;
//	for(; row < led_inst->num_grid_rows; row++)
//	{
//		for (col = 0; col < led_inst->num_grid_cols; col++)
//		{
//			if (grid_get_occupancy(led_inst->grid_inst, (cell_t){row,col}) != (uint8_t)'1')
//			{
//				uint8_t rgb[3] = {0,0,0};
//				size_t idx = led_grid_idx_to_strip_idx(led_inst, row, col);
//				hsv_to_rgb(hue, saturation, value, rgb);
//				hue = (hue + step) % 360;
//				led_set_color(led_inst, idx, buf, rgb);
//			}
//		}
//	}
//}

// TODO: change to uint
void set_colors_2channel(led_inst_t *led_inst, int size) {
	int r = 0;
	int b = 0;
	int g = 0;
	int i;
	for(i = 0; i < size; i++) {
		if(i < 50) { //R
			r += (int)(255/50);
		}
		else if(i < 100) { //RG
			if(r > 245) r = 0;
			r += (int)(255/50);
			g += (int)(255/50);
		}
		else if(i < 150) { //B
			if(r > 245) r = 0;
			b += (int)(255/50);
		}
		else if(i < 200) { //RB
			if(b > 245) b = 0;
			r += (int)(255/50);
			b += (int)(255/50);
		}
		else if(i < 250) { //G
			if(g > 245) g = 0;
			g += (int)(255/50);
		}
		else { //BG
			if(b > 245) b = 0;
			b += (int)(255/50);
			g += (int)(255/50);
		}
		color_t rgb = (color_t){r,g,b};
		led_set_color(led_inst, i, 0, rgb);
		led_send(led_inst, 0, 14000);
	}
}



void set_colors_factored(led_inst_t *led_inst) {
	/*int max_color = 0xFFFFFF;
	int interval = (int)(max_color / 150);
	int color = 20;
	int i;
	for(i = 0; i < 150; i++){
		int r = color & 0xFF0000;
		int g = color & 0x00FF00;
		int b = color & 0x0000FF;
		led_set_color(led_inst, i, r, g, b);
		led_send(led_inst, 14000);
		color += interval;
	}*/
	int r = 25;
	int g = 0;
	int b = 0;
	int led = 0;
	int i, j, k;
	int i_step = (int)(255/5);
	int j_step = (int)(255/5);
	int k_step = (int)(255/6);
	for(i = 0; i < 6; i++){
		for(j = 0; j < 5; j++){
			for(k = 0; k < 5; k++){
				color_t rgb = (color_t){r,g,b};
				led_set_color(led_inst, 0, led, rgb);
				led_send(led_inst, 0, 14000);
				led++;
				b += k_step;
			}
			g += j_step;
		}
		r += i_step;
	}
	while(1);
}

int sin_to_val(float phase, float freq) {
	float s = sin(freq + phase);
	float r = round(s * 127) + 128;

	return (int)r;
}

//void set_colors_rainbow(led_inst_t* led_inst) {
//	int size = 150;
//	int i;
//	for(i = 0; i < size; i++) {
//		int r = sin_to_val(.05, 1.666 * i);
//		int g = sin_to_val(.05, 2.666 * i);
//		int b = sin_to_val(.05,3.666 * i);
//
//		//led_set_color(led_inst, i, r, g, b);
//		//led_send(led_inst, 14000);
//	}
//
//	while(1);
//}




void get(float x, float* r, float* g, float* b) {
    *r = 0.0;
    *g = 0.0;
    *b = 1.0;
    if (x >= 0.0 && x < 0.2) {
        x = x / 0.2;
        *r = 0.0;
        *g = x;
        *b = 1.0;
    } else if (x >= 0.2 && x < 0.4) {
        x = (x - 0.2) / 0.2;
        *r = 0.0;
        *g = 1.0;
        *b = 1.0 - x;
    } else if (x >= 0.4 && x < 0.6) {
        x = (x - 0.4) / 0.2;
        *r = x;
        *g = 1.0;
        *b = 0.0;
    } else if (x >= 0.6 && x < 0.8) {
        x = (x - 0.6) / 0.2;
        *r = 1.0;
        *g = 1.0 - x;
        *b = 0.0;
    } else if (x >= 0.8 && x <= 1.0) {
        x = (x - 0.8) / 0.2;
        *r = 1.0;
        *g = 0.0;
        *b = x;
    }
}

void set_colors_cube(led_inst_t* led_inst) {
	float r,g,b;
	float dx = 1/(float)(150-1);
	int i;
	for(i = 0; i < 150; i++) {
		get(i*dx, &r,&g,&b);
		//led_set_color(led_inst, i, (int)r*255, (int)g*255, (int)b*255);
		//led_send(led_inst, 14000);
	}
}

uint8_t are_colors_equal(uint8_t* rgb1, uint8_t* rgb2) {
	return(rgb1[0] == rgb2[0] && rgb1[1] == rgb2[1] && rgb1[2] == rgb2[2]);
}

uint8_t is_wall(uint8_t* rgb) {
	return are_colors_equal(rgb, WALL_COLOR);
}
