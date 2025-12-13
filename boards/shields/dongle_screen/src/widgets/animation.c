#include <lvgl.h>
#include "animation.h"

extern const lv_img_dsc_t gif_frame1_img;
extern const lv_img_dsc_t gif_frame2_img;
extern const lv_img_dsc_t gif_frame3_img;
extern const lv_img_dsc_t gif_frame4_img;

static const lv_img_dsc_t *animation_frames[] = {
    &gif_frame1_img,
    &gif_frame2_img,
    &gif_frame3_img,
    &gif_frame4_img,
    &gif_frame3_img,
    &gif_frame2_img,
    &gif_frame1_img,
};

#define FRAME_DELAY_MS 250
#define FRAME_ZOOM 512

int zmk_widget_animation_init(struct zmk_widget_animation *widget, lv_obj_t *parent)
{
    if (!widget || !parent) {
        return -1;
    }

    widget->animimg = lv_animimg_create(parent);

    const uint32_t frame_count = sizeof(animation_frames) / sizeof(animation_frames[0]);

    lv_animimg_set_src(widget->animimg, (const void **)animation_frames, frame_count);
    lv_animimg_set_duration(widget->animimg, FRAME_DELAY_MS * frame_count);
    lv_animimg_set_repeat_count(widget->animimg, LV_ANIM_REPEAT_INFINITE);
    lv_img_set_zoom(widget->animimg, FRAME_ZOOM);
    lv_animimg_start(widget->animimg);

    return 0;
}

lv_obj_t *zmk_widget_animation_obj(struct zmk_widget_animation *widget)
{
    return widget ? widget->animimg : NULL;
}
