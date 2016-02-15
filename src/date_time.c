#include <pebble.h>
#include "date_time.h"

static TextLayer *time_layer;
static TextLayer *date_layer;

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char s_buffer_time[8];
  static char s_buffer_date[20];
  strftime(s_buffer_time, sizeof(s_buffer_time), clock_is_24h_style() ?
                                        "%H:%M" : "%I:%M", tick_time);
  text_layer_set_text(time_layer, s_buffer_time);

  strftime(s_buffer_date, sizeof(s_buffer_date), "%a, %b %d", tick_time);
  text_layer_set_text(date_layer, s_buffer_date);
}


static void time_layer_load(Layer *root_layer) {
  GRect layer_bounds = layer_get_bounds(root_layer);
  time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(80,80), layer_bounds.size.w, layer_bounds.size.h));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorBlack);
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  layer_add_child(root_layer, text_layer_get_layer(time_layer));
}

static void date_layer_load(Layer *root_layer) {
  GRect layer_bounds = layer_get_bounds(root_layer);
  date_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(125, 125), layer_bounds.size.w, layer_bounds.size.h));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(date_layer, GTextOverflowModeWordWrap);
  
  layer_add_child(root_layer, text_layer_get_layer(date_layer));
}

void date_time_load(Layer *root_layer) {
  time_layer_load(root_layer);
  date_layer_load(root_layer);
  update_time();
}

static void time_callback(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

void date_time_unload(){
  text_layer_destroy(time_layer);
  text_layer_destroy(date_layer);
}

void date_time_init(){
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Registering time service");
  tick_timer_service_subscribe(MINUTE_UNIT, time_callback);
}