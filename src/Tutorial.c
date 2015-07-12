#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_meridiem_layer;

static void update_minute(struct tm *tick_time) {
    // Create a long-lived buffer
    static char time_buffer[] = "00:00";
    
    // Write the current hours and minutes into the buffer
    if(clock_is_24h_style() == true) {
        //Use 24h hour format
        strftime(time_buffer, sizeof(time_buffer), "%H:%M", tick_time);
    } else {
        //Use 12 hour format
        strftime(time_buffer, sizeof(time_buffer), "%I:%M", tick_time);
        //Remove leading 0
        if (time_buffer[0] == '0') {
            memmove(time_buffer, &time_buffer[1], sizeof(time_buffer) - 1);
        }
    }
    
    // Display this time on the TextLayer
    text_layer_set_text(s_time_layer, time_buffer);
}

static void update_meridiem(struct tm *tick_time) {
    // Create a long-lived buffer
    static char meridiem_buffer[] = "PM";
    
    // Write the current hours and minutes into the buffer
    if(clock_is_24h_style() == false) {
        //Display AM/PM
        strftime(meridiem_buffer, sizeof(meridiem_buffer), "%p", tick_time);
    }
    
    // Display this time on the TextLayer
    text_layer_set_text(s_meridiem_layer, meridiem_buffer);
}

static void update_time() {
    // Get a tm structure
    time_t temp = time(NULL);
    struct tm *current_time = localtime(&temp);
    update_minute(current_time);
    update_meridiem(current_time);
}

static void main_window_load(Window *window) {
#ifdef PBL_COLOR
    window_set_background_color(s_main_window, GColorImperialPurple);
#else
    window_set_background_color(s_main_window, GColorWhite);
#endif
    // Create time TextLayer
    s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
#ifdef PBL_COLOR
    text_layer_set_text_color(s_time_layer, GColorJaegerGreen);
#else
    text_layer_set_text_color(s_time_layer, GColorBlack);
#endif
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text(s_time_layer, "00:00");
    
    // Create meridiem TextLayer
    s_meridiem_layer = text_layer_create(GRect(80, 95, 40, 20));
#ifdef PBL_COLOR
    text_layer_set_text_color(s_meridiem_layer, GColorOrange);
#else
    text_layer_set_text_color(s_meridiem_layer, GColorBlack);
#endif
    text_layer_set_background_color(s_meridiem_layer, GColorClear);
    text_layer_set_text(s_meridiem_layer, "PM");
    
    // Improve the layout to be more like a watchface
    text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
    text_layer_set_font(s_meridiem_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    
    // Add it as a child layer to the Window's root layer
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_meridiem_layer));
    
    // Make sure the time is displayed from the start
    update_time();
}

static void main_window_unload(Window *window) {
    // Destroy TextLayer
    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_meridiem_layer);
}

static void minute_tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_minute(tick_time);
}

static void hour_tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_meridiem(tick_time);
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
    tick_timer_service_subscribe(MINUTE_UNIT, minute_tick_handler);
    tick_timer_service_subscribe(HOUR_UNIT, hour_tick_handler);
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