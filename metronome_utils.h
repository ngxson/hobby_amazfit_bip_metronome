
#ifndef __UTILS_H__
#define __UTILS_H__

#include <libbip.h>

#define BTN_ROW_FROM_Y 82
#define BTN_ROW_TO_Y 113

#define BTN_MINUS_10_FROM_X 6
#define BTN_MINUS_10_TO_X 40

#define BTN_MINUS_1_FROM_X 51
#define BTN_MINUS_1_TO_X 81

#define BTN_PLUS_1_FROM_X 94
#define BTN_PLUS_1_TO_X 125

#define BTN_PLUS_10_FROM_X 135
#define BTN_PLUS_10_TO_X 169

#define BTN_RUN_FROM_X 6
#define BTN_RUN_FROM_Y 134
#define BTN_RUN_TO_X 169
#define BTN_RUN_TO_Y 167

void set_bpm(int bpm) {
  if (bpm > 0) set_update_period(1, 60000 / bpm);
  else set_update_period(0, 0);
}

void draw_button(char* name, int from_x, int from_y, int to_x, int to_y, long text_color, long bg_color) {
  set_bg_color(bg_color);
  set_fg_color(text_color);
  draw_rect(from_x, from_y, to_x, to_y);
  draw_filled_rect_bg(from_x+1, from_y+1, to_x-1, to_y-1);
  text_out_center(name, (from_x + to_x) / 2, (from_y + to_y) / 2 - 8);
}

void metronome_draw_button_row() {
  draw_button(
    "-10",
    BTN_MINUS_10_FROM_X,
    BTN_ROW_FROM_Y,
    BTN_MINUS_10_TO_X,
    BTN_ROW_TO_Y,
    COLOR_WHITE,
    COLOR_BLACK
  );

  draw_button(
    "-1",
    BTN_MINUS_1_FROM_X,
    BTN_ROW_FROM_Y,
    BTN_MINUS_1_TO_X,
    BTN_ROW_TO_Y,
    COLOR_WHITE,
    COLOR_BLACK
  );

  draw_button(
    "+1",
    BTN_PLUS_1_FROM_X,
    BTN_ROW_FROM_Y,
    BTN_PLUS_1_TO_X,
    BTN_ROW_TO_Y,
    COLOR_WHITE,
    COLOR_BLACK
  );

  draw_button(
    "+10",
    BTN_PLUS_10_FROM_X,
    BTN_ROW_FROM_Y,
    BTN_PLUS_10_TO_X,
    BTN_ROW_TO_Y,
    COLOR_WHITE,
    COLOR_BLACK
  );
}

void metronome_draw_button_play_pause(char is_playing) {
  if (is_playing) {
    draw_button(
      "Pause",
      BTN_RUN_FROM_X,
      BTN_RUN_FROM_Y,
      BTN_RUN_TO_X,
      BTN_RUN_TO_Y,
      COLOR_WHITE,
      COLOR_RED
    );
  } else {
    draw_button(
      "Play",
      BTN_RUN_FROM_X,
      BTN_RUN_FROM_Y,
      BTN_RUN_TO_X,
      BTN_RUN_TO_Y,
      COLOR_BLACK,
      COLOR_GREEN
    );
  }
}

void metronome_draw_texts(char bpm) {
  char buf[20];
  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  text_out(">", 179-18-5, 5);
  text_out_center("Metronome", 88, 8);
  text_out_center("(c) ngxson.com", 88, 29);
  _sprintf(buf, "%d BPM", bpm);
  text_out_center(buf, 88, 50);
}

#endif