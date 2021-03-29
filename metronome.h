/*
	Application template for Amazfit Bip BipOS
	(C) Maxim Volkov  2019 <Maxim.N.Volkov@ya.ru>
	
	Application template, header file

*/

#ifndef __APP_TEMPLATE_H__
#define __APP_TEMPLATE_H__

#define BEAT_TAP_MAX 16
#define TIME_UNIT_BETWEEN_TICKS 196
#define TIME_UNIT_ONE_MINUTE 60 * 1000 * 100

// the data structure for our screen
struct app_data_ {
	void* 	         ret_f;					//	the address of the return function
	unsigned int     bpm;
	char             is_playing;
	char             is_on_metronome;
	unsigned int     ticks[BEAT_TAP_MAX];
	unsigned char    tick_index;
};

// template.c
void 	show_screen (void *return_screen);
void 	key_press_screen();
int 	dispatch_screen (void *param);
void 	screen_job();
void  draw_screen(struct app_data_ *a);
#endif