#include <pebble.h>

static Window *main_window;
static TextLayer *time_layer;
static TextLayer *date_layer;
static BitmapLayer *bitmap_layer;
static GBitmap *pic_bitmap;

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

static void main_window_load(Window *window) {
  Layer  *root_layer = window_get_root_layer(window);
  GRect layer_bounds = layer_get_bounds(root_layer);
  
  pic_bitmap = gbitmap_create_with_resource(RESOURCE_ID_WIFEY_PIC);
  bitmap_layer = bitmap_layer_create(GRect(PBL_IF_ROUND_ELSE(52, 35), 10, 72, 72));
  bitmap_layer_set_bitmap(bitmap_layer, pic_bitmap);
  
  time_layer = text_layer_create(
    GRect(0, PBL_IF_ROUND_ELSE(80,80), layer_bounds.size.w, layer_bounds.size.h));
  date_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(125, 125), layer_bounds.size.w, layer_bounds.size.h));
  
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_background_color(date_layer, GColorClear);
  
  text_layer_set_text_color(time_layer, GColorBlack);
  
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  
  text_layer_set_overflow_mode(date_layer, GTextOverflowModeWordWrap);

  layer_add_child(root_layer, bitmap_layer_get_layer(bitmap_layer));
  layer_add_child(root_layer, text_layer_get_layer(time_layer));
  layer_add_child(root_layer, text_layer_get_layer(date_layer));
  
  update_time();
}

static void main_window_unload(Window *window) {
  text_layer_destroy(time_layer);
  text_layer_destroy(date_layer);
  gbitmap_destroy(pic_bitmap);
  bitmap_layer_destroy(bitmap_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

void handle_init(void) {
	// Create a window and text layer
	main_window = window_create();
  
  window_set_window_handlers(main_window, (WindowHandlers) { 
   .load = main_window_load,
   .unload = main_window_unload
  });
	
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Registering time service");
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Push the window
	window_stack_push(main_window, true);
}	
	
void handle_deinit(void) {
	// Destroy the window
	window_destroy(main_window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
