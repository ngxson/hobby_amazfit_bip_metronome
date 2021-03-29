#ifndef __BEAT_TAP_H__
#define __BEAT_TAP_H__

#include <libbip.h>
#include "metronome.h"

unsigned int beat_tap_calc_bpm(struct app_data_ *app_data) {
  unsigned int sum = 0;
  unsigned char n = 0;
  unsigned char i;
  for (i = 1; i < BEAT_TAP_MAX; i++) {
    if (app_data->ticks[i] != 0 && app_data->ticks[i-1] != 0) {
      sum += (app_data->ticks[i] - app_data->ticks[i-1]) * TIME_UNIT_BETWEEN_TICKS;
      n++;
    }
  }
  return (n != 0) ? (TIME_UNIT_ONE_MINUTE / (sum / n)) : 0;
}

void beat_tap_draw_texts(struct app_data_ *app_data) {
  char buf[20];
  unsigned int new_bpm = beat_tap_calc_bpm(app_data);
  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);

  text_out("<", 5, 5);
  text_out_center("Beat Tap", 88, 88 - 10);
  beat_tap_calc_bpm(app_data);
  _sprintf(buf, "%d BPM", new_bpm);
  text_out_center(buf, 88, 88 + 10);

  for (int i = 0; i < app_data->tick_index; i++) buf[i] = '*';
  buf[app_data->tick_index] = '\0';
  set_fg_color(COLOR_GREEN);
  text_out_center(buf, 88, 176-5-20);

  if (new_bpm >= 10 && new_bpm <= 200) {
    app_data->bpm = new_bpm;
  }
}

#endif