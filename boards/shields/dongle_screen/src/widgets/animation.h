#pragma once

#include <lvgl.h>

struct zmk_widget_animation {
    lv_obj_t *animimg;
};

int zmk_widget_animation_init(struct zmk_widget_animation *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_animation_obj(struct zmk_widget_animation *widget);
