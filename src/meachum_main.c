#include <pebble.h>
#include "battery.h"
#include "bitmap.h"
#include "date_time.h"

static Window *main_window;

static void main_window_load(Window *window) {
  Layer  *root_layer = window_get_root_layer(window);
  
  bitmap_load(root_layer);
  date_time_load(root_layer);
  battery_load(root_layer);
}

static void main_window_unload(Window *window) {
  date_time_unload();
  battery_unload();
  bitmap_unload();
}


void initialize(void) {
	main_window = window_create();
  
  window_set_window_handlers(main_window, (WindowHandlers) { 
   .load = main_window_load,
   .unload = main_window_unload
  });

  date_time_init();
  battery_init();
  
	window_stack_push(main_window, true);
}	
	
void deinitialize(void) {
	window_destroy(main_window);
}

int main(void) {
	initialize();
	app_event_loop();
	deinitialize();
}
