#include <pebble.h>

static Window *main_window;
static TextLayer *time_layer;
static BitmapLayer *bitmap_layer;
static GBitmap *pic_bitmap;

static void update_time() {
  
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char s_buffer[100];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                        "%H:%M%n%a, %b %d" : "%I:%M%n%a, %b %d", tick_time);
  text_layer_set_text(time_layer, s_buffer);
  
}

static void main_window_load(Window *window) {
  Layer  *root_layer = window_get_root_layer(window);
  GRect layer_bounds = layer_get_bounds(root_layer);
  
  pic_bitmap = gbitmap_create_with_resource(RESOURCE_ID_WIFEY_PIC);
  bitmap_layer = bitmap_layer_create(GRect(5, 5, 48, 48));
  bitmap_layer_set_bitmap(bitmap_layer, pic_bitmap);
  
//   time_layer = text_layer_create(
//     GRect(0, PBL_IF_ROUND_ELSE(58,52), layer_bounds.size.w, layer_bounds.size.h));
  
//   text_layer_set_background_color(time_layer, GColorClear);
//   text_layer_set_text_color(time_layer, GColorBlack);
//   text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28 ));
//   text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
//   text_layer_set_overflow_mode(time_layer, GTextOverflowModeWordWrap);

  layer_add_child(root_layer, bitmap_layer_get_layer(bitmap_layer));
  //layer_add_child(bitmap_layer_get_layer(bitmap_layer), text_layer_get_layer(time_layer));
  
  //update_time();
}

static void main_window_unload(Window *window) {
  text_layer_destroy(time_layer);
  gbitmap_destroy(pic_bitmap);
  bitmap_layer_destroy(bitmap_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  //update_time();
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
