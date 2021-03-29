/*
	Application template for Amazfit Bip BipOS
	(C) Maxim Volkov  2019 <Maxim.N.Volkov@ya.ru>
	
	Application template loader for the BipOS
	
*/

#include <libbip.h>

#include "metronome.h"
#include "metronome_utils.h"
#include "metronome_beat_tap.h"

//	screen menu structure - each screen has its own
struct regmenu_ screen_data = {
  55, //	main screen number, value 0-255, for custom windows it is better to take from 50 and above
  1, //	auxiliary screen number (usually 1)
  0, //	0
  dispatch_screen, //	pointer to the function handling touch, swipe, long press
  key_press_screen, //	pointer to the function handling pressing the button
  screen_job, //	pointer to the callback function of the timer  
  0, //	0
  show_screen, //	pointer to the screen display function
  0, //	
  0 //	long press of the button
};
int main(int param0, char ** argv) { //	here the variable argv is not defined
  show_screen((void * ) param0);
}

void show_screen(void * param0) {
  struct app_data_ ** app_data_p = get_ptr_temp_buf_2(); //	pointer to a pointer to screen data
  struct app_data_ * app_data; //	pointer to screen data

  Elf_proc_ * proc;

  // check the source at the procedure launch
  if ((param0 == * app_data_p) && get_var_menu_overlay()) { // return from the overlay screen (incoming call, notification, alarm, target, etc.)

    app_data = * app_data_p; //	the data pointer must be saved for the deletion 
    //	release memory function reg_menu
    * app_data_p = NULL; //	reset the pointer to pass it to the function reg_menu		

    // 	create a new screen when the pointer temp_buf_2 is equal to 0 and the memory is not released		
    reg_menu( &screen_data, 0); // 	menu_overlay=0

    * app_data_p = app_data; //	restore the data pointer after the reg_menu function

    //   here we perform actions when returning from the overlay screen: restore data, etc.

  } else { // if the function is started for the first time i.e. from the menu 

    // create a screen (register in the system) 
    reg_menu( &screen_data, 0);

    // allocate the necessary memory and place the data in it (the memory by the pointer stored at temp_buf_2 is released automatically by the function reg_menu of another screen)
    * app_data_p = (struct app_data_ * ) pvPortMalloc(sizeof(struct app_data_));
    app_data = * app_data_p; //	data pointer

    // clear the memory for data
    _memclr(app_data, sizeof(struct app_data_));

    //	param0 value contains a pointer to the data of the running process structure Elf_proc_
    proc = param0;

    // remember the address of the pointer to the function you need to return to after finishing this screen
    if (param0 && proc->ret_f) //	if the return pointer is passed, then return to it
      app_data->ret_f = proc->elf_finish;
    else //	if not, to the watchface
      app_data->ret_f = show_watchface;

    // here we perform actions that are necessary if the function is launched for the first time from the menu: filling all data structures, etc.
    app_data->bpm = 60;
    app_data->is_playing = 0;
    app_data->is_on_metronome = 1;
    for (int i = 0; i < BEAT_TAP_MAX; i++) app_data->ticks[i] = 0;
  }

  // here we do the interface drawing, there is no need to update (move to video memory) the screen
  draw_screen(app_data);

  // if necessary, set the call timer screen_job in ms
  set_update_period(0, 0);

  // prevent exiting the app after a period
  set_display_state_value(8, 1);
  set_display_state_value(2, 1);
}

void key_press_screen() {
  struct app_data_ ** app_data_p = get_ptr_temp_buf_2(); //	pointer to a pointer to screen data 
  struct app_data_ * app_data = * app_data_p; //	pointer to screen data

  // call the return function (usually this is the start menu), specify the address of the function of our application as a parameter
  show_menu_animate(app_data->ret_f, (unsigned int) show_watchface, ANIMATE_RIGHT);
};

void screen_job() {
  // if necessary, you can use the screen data in this function
  struct app_data_ ** app_data_p = get_ptr_temp_buf_2(); //	pointer to pointer to screen data  
  struct app_data_ * app_data = * app_data_p; //	pointer to screen data

  if (app_data->is_playing && app_data->is_on_metronome) {
    vibrate(1, 40, 0);
    set_bpm(app_data->bpm);
  } else {
    set_bpm(0);
  }
}

void exit_app() {
  Elf_proc_* proc = get_proc_by_addr(main);
  proc->ret_f = NULL;
  elf_finish(main);
}

void adjust_bpm(struct app_data_ *app_data, int amount) {
  int new_bpm = app_data->bpm + amount;
  if (new_bpm >= 10 && new_bpm <= 200) {
    app_data->bpm = new_bpm;
    set_bpm(new_bpm);
    draw_screen(app_data);
  }
}

void handle_gest_metronome_screen(struct app_data_ *app_data, struct gesture_ *gest) {
  if (gest->gesture == GESTURE_CLICK) {
    if (gest->touch_pos_y >= BTN_ROW_FROM_Y && gest->touch_pos_y <= BTN_ROW_TO_Y) {
      if (gest->touch_pos_x >= BTN_MINUS_10_FROM_X && gest->touch_pos_x <= BTN_MINUS_10_TO_X) {
        adjust_bpm(app_data, -10);
      }

      else if (gest->touch_pos_x >= BTN_MINUS_1_FROM_X && gest->touch_pos_x <= BTN_MINUS_1_TO_X) {
        adjust_bpm(app_data, -1);
      }

      else if (gest->touch_pos_x >= BTN_PLUS_1_FROM_X && gest->touch_pos_x <= BTN_PLUS_1_TO_X) {
        adjust_bpm(app_data, 1);
      }

      else if (gest->touch_pos_x >= BTN_PLUS_10_FROM_X && gest->touch_pos_x <= BTN_PLUS_10_TO_X) {
        adjust_bpm(app_data, 10);
      }
    }

    else if (
      gest->touch_pos_y >= BTN_RUN_FROM_Y && gest->touch_pos_y <= BTN_RUN_TO_Y
      && gest->touch_pos_x >= BTN_RUN_FROM_X && gest->touch_pos_x <= BTN_RUN_TO_X
    ) {
      if (app_data->is_playing) {
        app_data->is_playing = 0;
        set_bpm(0);
      } else {
        app_data->is_playing = 1;
        set_bpm(app_data->bpm);
      }
      vibrate(1, 100, 0);
      draw_screen(app_data);
    }
  } else if (gest->gesture == GESTURE_SWIPE_LEFT) {
    app_data->is_playing = 0;
    app_data->is_on_metronome = 0;
    set_bpm(0);
    draw_screen(app_data);
  }
}

void handle_gest_beat_tap_screen(struct app_data_ *app_data, struct gesture_ *gest) {
  if (gest->gesture == GESTURE_CLICK) {
    int idx = app_data->tick_index;
    if (idx < BEAT_TAP_MAX) {
      app_data->ticks[idx] = get_tick_count();
      app_data->tick_index++;
    } else {
      for (int i = 0; i < BEAT_TAP_MAX; i++) app_data->ticks[i] = 0;
      app_data->tick_index = 0;
    }
    draw_screen(app_data);
  } else if (gest->gesture == GESTURE_SWIPE_RIGHT) {
    app_data->is_on_metronome = 1;
    draw_screen(app_data);
  }
}

int dispatch_screen(void * param) {
  struct app_data_ ** app_data_p = get_ptr_temp_buf_2(); //	pointer to a pointer to screen data 
  struct app_data_ * app_data = * app_data_p; //	pointer to screen data

  // in case of rendering the interface, update (transfer to video memory) the screen

  struct gesture_ * gest = param;

  if (app_data->is_on_metronome) {
    handle_gest_metronome_screen(app_data, gest);
  } else {
    handle_gest_beat_tap_screen(app_data, gest);
  }

  return 0;
};

// custom function
void draw_screen(struct app_data_ *app_data) {
  set_bg_color(COLOR_BLACK);
  fill_screen_bg();
  set_graph_callback_to_ram_1();
  load_font();

  // draw contents
  if (app_data->is_on_metronome) {
    metronome_draw_texts(app_data->bpm);
    metronome_draw_button_row();
    metronome_draw_button_play_pause(app_data->is_playing);
  } else {
    beat_tap_draw_texts(app_data);
  }
  repaint_screen_lines(0, 176);
};