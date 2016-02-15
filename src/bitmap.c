#include <pebble.h>
#include "bitmap.h"

static BitmapLayer *bitmap_layer;
static GBitmap *pic_bitmap;

void bitmap_load(Layer *root_layer) {
  pic_bitmap = gbitmap_create_with_resource(RESOURCE_ID_WIFEY_PIC);
  bitmap_layer = bitmap_layer_create(GRect(PBL_IF_ROUND_ELSE(52, 35), 10, 72, 72));
  bitmap_layer_set_bitmap(bitmap_layer, pic_bitmap);
  
  layer_add_child(root_layer, bitmap_layer_get_layer(bitmap_layer));
}

void bitmap_unload() {
  gbitmap_destroy(pic_bitmap);
  bitmap_layer_destroy(bitmap_layer);
}