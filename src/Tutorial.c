#include <pebble.h>
#include <pebble_fonts.h>
#include <num2words-en.h>

#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1
#define BUFFER_SIZE 44

static Window *s_main_window;

static TextLayer *s_time_layer1,
                 *s_time_layer2,
                 *s_time_layer3,
                 *s_meridiem_layer,
                 *s_weather_layer,
                 *s_date_layer;
                 //*s_top_day_layer,
                 //*s_bottom_day_layer;

static GRect *display_layer1,
             *off_right_layer1,
             *off_left_layer1,
             *display_layer2,
             *off_right_layer2,
             *off_left_layer2,
             *display_layer3,
             *off_right_layer3,
             *off_left_layer3,
             *display_meridiem,
             *off_right_meridiem,
             *off_left_meridiem,
             *display_date;

	// check if need to update day or date
//	if(strcmp(topDayLayer.text, WEEKDAYS[t->tm_wday]) != 0) {
//    		text_layer_set_text(&topDayLayer, WEEKDAYS[t->tm_wday]);
//    		text_layer_set_text(&bottomDayLayer, DAYS[t->tm_mday]);
//	}
//}

void on_animation_stopped(Animation *anim, bool finished, void *context)
{
    //Free the memory used by the Animation
    property_animation_destroy((PropertyAnimation*) anim);
}

void animate_layer(Layer *layer, GRect *start, GRect *finish, int duration, int delay)
{
    //Declare animation
    PropertyAnimation *anim = property_animation_create_layer_frame(layer, start, finish);
 
    //Set characteristics
    animation_set_duration((Animation*) anim, duration);
    animation_set_delay((Animation*) anim, delay);
 
    //Start animation!
    animation_schedule((Animation*) anim);
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *current_time = localtime(&temp);
  
  // Create a long-lived buffer
  static char time_buffer[] = "00:00",
              meridiem_buffer[3],
              date_buffer[BUFFER_SIZE],
              text_line1[BUFFER_SIZE],
              text_line2[BUFFER_SIZE],
              text_line3[BUFFER_SIZE];
    
  // Write the current hours and minutes into the buffer
//  if(clock_is_24h_style() == true) {
    //Use 24h hour format
//    strftime(time_buffer, sizeof(time_buffer), "%H:%M", current_time);
//  } else {
    //Use 12 hour format
    time_to_3words(current_time->tm_hour, current_time->tm_min, text_line1, text_line2, text_line3, BUFFER_SIZE);
    //Display AM/PM
    strftime(meridiem_buffer, sizeof(meridiem_buffer), "%p", current_time);
//    strftime(time_buffer, sizeof(time_buffer), "%I:%M", current_time);
    //Remove leading 0
//    if (time_buffer[0] == '0') {
//      memmove(time_buffer, &time_buffer[1], sizeof(time_buffer) - 1);
//    }
//  }
  
  // Get Date
  date_to_chars(current_time->tm_mon, current_time->tm_mday, date_buffer, BUFFER_SIZE);
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer1, text_line1);//time_buffer);
  text_layer_set_text(s_time_layer2, text_line2);
  text_layer_set_text(s_time_layer3, text_line3);
  text_layer_set_text(s_meridiem_layer, meridiem_buffer);
  text_layer_set_text(s_date_layer, date_buffer);
}

static void update_time_animate() {
  update_time();
	// Slide onscreen from the left
	animate_layer(text_layer_get_layer(s_time_layer1), off_left_layer1, display_layer1, 250, 400);
  animate_layer(text_layer_get_layer(s_time_layer2), off_left_layer2, display_layer2, 300, 500);
  animate_layer(text_layer_get_layer(s_time_layer3), off_left_layer3, display_layer3, 350, 600);
  animate_layer(text_layer_get_layer(s_meridiem_layer), off_left_meridiem, display_meridiem, 400, 700);
}

static void main_window_load(Window *window) {
#ifdef PBL_COLOR
  window_set_background_color(s_main_window, GColorOxfordBlue);
#else
  window_set_background_color(s_main_window, GColorWhite);
#endif
  // Create time TextLayer
  display_layer1 = &GRect(0, 35, 144, 168);
  off_right_layer1 = &GRect(144, 35, 144, 168);
  off_left_layer1 = &GRect(-144, 35, 144, 168);

  display_layer2 = &GRect(0, 70, 144, 168);
  off_right_layer2 = &GRect(144, 70, 144, 168);
  off_left_layer2 = &GRect(-144, 70, 144, 168);

  display_layer3 = &GRect(0, 105, 144, 168);
  off_right_layer3 = &GRect(144, 105, 144, 168);
  off_left_layer3 = &GRect(-144, 105, 144, 168);

  display_meridiem = &GRect(80, 145, 144, 168);
  off_right_meridiem = &GRect(144, 145, 144, 168);
  off_left_meridiem = &GRect(-144, 145, 144, 168);
  
  
  s_time_layer1 = text_layer_create(*off_right_layer1);
  s_time_layer2 = text_layer_create(*off_right_layer2);
  s_time_layer3 = text_layer_create(*off_right_layer3);
#ifdef PBL_COLOR
  text_layer_set_text_color(s_time_layer1, GColorWhite);
  text_layer_set_text_color(s_time_layer2, GColorWhite);
  text_layer_set_text_color(s_time_layer3, GColorWhite);
#else
  text_layer_set_text_color(s_time_layer1, GColorWhite);
  text_layer_set_text_color(s_time_layer2, GColorWhite);
  text_layer_set_text_color(s_time_layer3, GColorWhite);
#endif
  text_layer_set_background_color(s_time_layer1, GColorClear);
  text_layer_set_background_color(s_time_layer2, GColorClear);
  text_layer_set_background_color(s_time_layer3, GColorClear);
    
  // Create meridiem TextLayer
  s_meridiem_layer = text_layer_create(*off_right_meridiem);
#ifdef PBL_COLOR
  text_layer_set_text_color(s_meridiem_layer, GColorOrange);
#else
  text_layer_set_text_color(s_meridiem_layer, GColorWhite);
#endif
  text_layer_set_background_color(s_meridiem_layer, GColorClear);
    
  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer1, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_font(s_time_layer2, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_font(s_time_layer3, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(s_time_layer1, GTextAlignmentLeft);
  text_layer_set_text_alignment(s_time_layer2, GTextAlignmentLeft);
  text_layer_set_text_alignment(s_time_layer3, GTextAlignmentLeft);
  text_layer_set_font(s_meridiem_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
    
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer1));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer2));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer3));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_meridiem_layer));
    
    
  // Create weather Layer
  s_weather_layer = text_layer_create(GRect(0, 0, 144, 25));
  text_layer_set_background_color(s_weather_layer, GColorClear);
#ifdef PBL_COLOR
  text_layer_set_text_color(s_weather_layer, GColorWhite);
#else
  text_layer_set_text_color(s_weather_layer, GColorBlack);
#endif
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
  text_layer_set_text(s_weather_layer, "Loading...");
  text_layer_set_font(s_weather_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
  
  // Create date Layer
  display_date = &GRect(0, 20, 144, 168);
  s_date_layer = text_layer_create(*display_date);
  text_layer_set_background_color(s_date_layer, GColorClear);
#ifdef PBL_COLOR
  text_layer_set_text_color(s_date_layer, GColorWhite);
#else
  text_layer_set_text_color(s_date_layer, GColorBlack);
#endif
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentLeft);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  
  // Animate time for first load
  update_time_animate();
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer1);
  text_layer_destroy(s_time_layer2);
  text_layer_destroy(s_time_layer3);
  text_layer_destroy(s_meridiem_layer);
  text_layer_destroy(s_weather_layer);
  text_layer_destroy(s_date_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  int seconds = tick_time->tm_sec;
  int minutes = tick_time->tm_min;
  int hours = tick_time->tm_hour;
  
//	if(seconds == 59) {
		// Slide offscreen to the right
//	  animate_layer(text_layer_get_layer(s_time_layer3), display_layer3, off_right_layer3, 250, 400);
//	  animate_layer(text_layer_get_layer(s_time_layer2), display_layer2, off_right_layer2, 300, 500);
//    APP_LOG(APP_LOG_LEVEL_INFO, "Attempted animating off screen of %c %c %c at %d:%d:%d", 
//            *text_layer_get_text(s_time_layer1), *text_layer_get_text(s_time_layer2), *text_layer_get_text(s_time_layer3),
//            hours, minutes, seconds);
//    if (minutes == 59) {
//      animate_layer(text_layer_get_layer(s_time_layer1), display_layer1, off_right_layer1, 350, 600);
//      if (hours == 11) {
//        animate_layer(text_layer_get_layer(s_meridiem_layer), display_meridiem, off_right_meridiem, 400, 700);
//      }
//    }
//	}
/*	else*/ if(seconds == 0) {
    update_time();
    // Slide onscreen from the left
//    if (minutes == 0) {
//      if (hours == 12) {
//        animate_layer(text_layer_get_layer(s_meridiem_layer), off_left_meridiem, display_meridiem, 400, 700);
//      }
//		  animate_layer(text_layer_get_layer(s_time_layer1), off_left_layer1, display_layer1, 250, 400);
//    }
//	  animate_layer(text_layer_get_layer(s_time_layer2), off_left_layer2, display_layer2, 300, 500);
//	  animate_layer(text_layer_get_layer(s_time_layer3), off_left_layer3, display_layer3, 350, 600);
//   APP_LOG(APP_LOG_LEVEL_INFO, "Attempted animating to display of %c %c %c at %d:%d:%d", 
//           *text_layer_get_text(s_time_layer1), *text_layer_get_text(s_time_layer2), *text_layer_get_text(s_time_layer3),
//           hours, minutes, seconds);
	}
  
  
  // Get weather update every 30 minutes
  if(minutes % 30 == 0 && seconds == 0) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
 
    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);
 
    // Send the message!
    app_message_outbox_send();
  }
//  APP_LOG(APP_LOG_LEVEL_INFO, "Exiting tick handler at %d:%d:%d",
//            hours, minutes, seconds);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char temperature_buffer[8];
  static char conditions_buffer[32];
  static char weather_layer_buffer[32];
  
  // Read first item
  Tuple *t = dict_read_first(iterator);
 
  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
    case KEY_TEMPERATURE:
      snprintf(temperature_buffer, sizeof(temperature_buffer), "%dºF", (int)t->value->int32);
      break;
    case KEY_CONDITIONS:
      snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }
 
    // Look for next item
    t = dict_read_next(iterator);
  }
  
  // Assemble full string and display
  snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
  text_layer_set_text(s_weather_layer, weather_layer_buffer);
}
 
static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}
 
static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}
 
static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();
    
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
    
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
    
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}