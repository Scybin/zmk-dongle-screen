#include <lvgl.h>
#include "animation.h"

/* Frames provided by assets/gif.c */
extern const lv_img_dsc_t gif_frame1_img;
extern const lv_img_dsc_t gif_frame2_img;

/* Local frame array for lv_animimg */
static const lv_img_dsc_t *animation_frames[] = {
    &gif_frame1_img,
    &gif_frame2_img,
};

int zmk_widget_animation_init(struct zmk_widget_animation *widget, lv_obj_t *parent)
{
    if (!widget || !parent) {
        return -1;
    }

    widget->animimg = lv_animimg_create(parent);

    lv_animimg_set_src(
        widget->animimg,
        (const void **)animation_frames,
        sizeof(animation_frames) / sizeof(animation_frames[0])
    );

    /* 500 ms per full cycle; tweak as you like */
    lv_animimg_set_duration(widget->animimg, 500);
    lv_animimg_set_repeat_count(widget->animimg, LV_ANIM_REPEAT_INFINITE);

    /* Always running, no WPM logic */
    lv_animimg_start(widget->animimg);

    return 0;
}

lv_obj_t *zmk_widget_animation_obj(struct zmk_widget_animation *widget)
{
    return widget ? widget->animimg : NULL;
}
