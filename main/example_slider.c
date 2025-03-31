void create_slider_and_bar() 
{
    /*Create a slider as example*/
    slider1 = lv_slider_create(lv_screen_active());

    /*Create an Arc*/
    arc = lv_arc_create(lv_screen_active());
    lv_arc_set_range(arc, 0, 100);
    lv_obj_set_size(arc, 200, 200); // Example size: 200x200 pixels
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
    lv_obj_center(arc);

    lv_obj_set_width(slider1, 150);                          /*Set the width*/
    lv_obj_center(slider1);                                  /*Align to the center of the parent (screen)*/
}
