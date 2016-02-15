#include <pebble.h>
#include "battery.h"

#define BATTERY_METER_THICKNESS 4

static Layer *battery_layer;
static int battery_level;

static void battery_callback(BatteryChargeState state) {
  battery_level = state.charge_percent;
  layer_mark_dirty(battery_layer);
}

static void update_battery(Layer *layer, GContext *context) {
  GRect layer_bounds = layer_get_bounds(layer);
  
  int bar_width = (int)(((float)battery_level / 100.0F) * layer_bounds.size.w);

  graphics_context_set_fill_color(context, GColorVividCerulean);
  graphics_fill_rect(context, GRect(layer_bounds.origin.x, layer_bounds.origin.y,
                                    bar_width, layer_bounds.size.h), 0, GCornerNone);
  
}

void battery_load(Layer *root_layer) {
  GRect layer_bounds = layer_get_bounds(root_layer);

  battery_layer = layer_create(GRect( 0, layer_bounds.size.h - 4, layer_bounds.size.w, BATTERY_METER_THICKNESS));
  layer_set_update_proc(battery_layer, update_battery);
  layer_add_child(root_layer, battery_layer);
  
  // Ensure battery level is displayed from the start
  battery_callback(battery_state_service_peek());
}

void battery_unload() {
  layer_destroy(battery_layer);
}

void battery_init() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Registering battery service");
  battery_state_service_subscribe(battery_callback);
}