#pragma once

#include <lvgl.h>
#include <stdbool.h>
#include <stdint.h>

struct zmk_widget_animation {
    lv_obj_t *animimg;
};

int zmk_widget_animation_init(struct zmk_widget_animation *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_animation_obj(struct zmk_widget_animation *widget);

void zmk_widget_animation_on_wpm_changed(uint16_t wpm);