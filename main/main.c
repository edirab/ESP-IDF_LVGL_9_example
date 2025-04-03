#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "st7789.h"
#include "driver/gptimer.h"
#include "utime.h"
#include "esp_timer.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "uart1.h"

#define LV_TICK_PERIOD_MS 1


lv_obj_t * slider1;
lv_obj_t * arc;

int slide_val = 0;

static void lv_tick_task(void *arg) {
    (void) arg;

    lv_tick_inc(LV_TICK_PERIOD_MS);
}

void start_1ms_timer() 
{
    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));
}



void create_battery_icon() 
{
    /*Create battery indicator*/
    static lv_style_t style_indic;
    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_LIGHT_GREEN));

    lv_obj_t* bar = lv_bar_create(lv_scr_act());
    lv_obj_set_size(bar, 60, 15);
    lv_obj_align(bar,LV_ALIGN_TOP_RIGHT,-5,2);
    lv_bar_set_value(bar, 85, LV_ANIM_ON);
    lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);

    lv_obj_t* label_bat = lv_label_create(lv_scr_act());
    lv_obj_align(label_bat, LV_ALIGN_TOP_RIGHT, -15, 2);
    lv_label_set_text(label_bat, "85%");
}

void create_label() 
{
    lv_obj_t *label = lv_label_create(lv_scr_act());

    // Set the text of the label
    lv_label_set_text(label, "ESP-IDF + LVGL 9.2.2 \xB0C");

    // Align the label to the bottom center of the parent
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, 0);

    static lv_style_t st;
    lv_style_init(&st);
    lv_style_set_text_font(&st, &lv_font_montserrat_14);

    // Apply the style to the label
    lv_obj_add_style(label, &st, 0);

}

void gui_task(void *pvParameters) {

    lv_init();

    lv_display_t *display = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);

    static lv_color_t buf1[DISP_BUF_SIZE / 10];                        /*Declare a buffer for 1/10 screen size*/
    static lv_color_t buf2[DISP_BUF_SIZE / 10];                        /*Declare a buffer for 1/10 screen size*/
    lv_display_set_buffers(display, buf1, buf2, sizeof(buf1),LV_DISPLAY_RENDER_MODE_PARTIAL );  /*Initialize the display buffer.*/

    lv_display_set_flush_cb(display, st7789_flush);

    start_1ms_timer();

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x003a57), LV_PART_MAIN);

    create_battery_icon();

    //
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, &lv_font_montserrat_24);
    lv_style_set_text_color(&style, lv_color_hex(0xe0e0e0)  /*lv_palette_main(LV_PALETTE_BLUE)*/);

    lv_obj_t *label_1 = lv_label_create(lv_scr_act());

    // Set the text of the label
    lv_label_set_text(label_1, "temp: 25 *C");
    lv_obj_set_x(label_1, 20);
    lv_obj_set_y(label_1, 20);

    lv_obj_t *label_2 = lv_label_create(lv_scr_act());
    lv_label_set_text(label_2, "hum: 40 %");
    lv_obj_set_x(label_2, 100);
    lv_obj_set_y(label_2, 50);

    lv_obj_t *label_3 = lv_label_create(lv_scr_act());
    lv_label_set_text(label_3, "soil: 3096");
    lv_obj_set_x(label_3, 20);
    lv_obj_set_y(label_3, 80);

    lv_obj_t *label_4 = lv_label_create(lv_scr_act());
    lv_label_set_text(label_4, "lum: 1024");
    lv_obj_set_x(label_4, 100);
    lv_obj_set_y(label_4, 110);

    // Apply the style to the label
    lv_obj_add_style(label_1, &style, 0);
    lv_obj_add_style(label_2, &style, 0);
    lv_obj_add_style(label_3, &style, 0);
    lv_obj_add_style(label_4, &style, 0);




    while(1) 
    {
        // ESP_LOGI(__FUNCTION__, "gui task while loop");

        // lv_slider_set_value(slider1, slide_val, LV_ANIM_ON);
        // lv_arc_set_value(arc, slide_val);
    	
    	slide_val ++;
    	if(slide_val > 100)
        {
    		slide_val = 0;
    	}

    	vTaskDelay(pdMS_TO_TICKS(10));
        lv_task_handler();
    }
}

void app_main(void)
{
    spi_display_init();
    st7789_init();
    setup_uart1();
	
    // Create the Hello World task
    xTaskCreate(gui_task,       "lvgl_task",      18*1024, NULL, 2, NULL);
    // xTaskCreate(update_sliders, "update_sliders", 2*1024, NULL, 2, NULL);

    xTaskCreate(uart_echo_task, "uart_echo_task", ECHO_TASK_STACK_SIZE, NULL, 2, NULL);
}
