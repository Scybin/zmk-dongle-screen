#include <lvgl.h>
#include <stdbool.h>
#include <stdint.h>
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

#define FRAME_DELAY_MS      250U
#define FRAME_ZOOM          512
#define FRAME_COUNT         (sizeof(animation_frames) / sizeof(animation_frames[0]))
#define CYCLE_DURATION_MS   (FRAME_DELAY_MS * FRAME_COUNT)

static struct zmk_widget_animation *s_anim_widget;
static bool s_typing_active;
static bool s_anim_running;
static uint32_t s_anim_start_tick;
static lv_timer_t *s_stop_timer;

static void start_anim_if_needed(void)
{
    if (!s_anim_widget || !s_anim_widget->animimg) {
        return;
    }

    if (s_stop_timer) {
        lv_timer_del(s_stop_timer);
        s_stop_timer = NULL;
    }

    if (s_anim_running) {
        return;
    }

    lv_animimg_set_repeat_count(s_anim_widget->animimg, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(s_anim_widget->animimg);

    s_anim_start_tick = lv_tick_get();
    s_anim_running = true;
}

static void stop_anim_timer_cb(lv_timer_t *timer)
{
    (void)timer;

    if (!s_anim_widget || !s_anim_widget->animimg) {
        return;
    }

    lv_anim_del(s_anim_widget->animimg, NULL);
    lv_img_set_src(s_anim_widget->animimg, (const void *)animation_frames[0]);

    s_anim_running = false;

    if (s_stop_timer) {
        lv_timer_del(s_stop_timer);
        s_stop_timer = NULL;
    }
}

static void schedule_stop_after_current_cycle(void)
{
    if (!s_anim_running) {
        if (s_anim_widget && s_anim_widget->animimg) {
            lv_img_set_src(s_anim_widget->animimg, (const void *)animation_frames[0]);
        }
        return;
    }

    uint32_t now     = lv_tick_get();
    uint32_t elapsed = now - s_anim_start_tick;
    uint32_t offset  = elapsed % CYCLE_DURATION_MS;
    uint32_t remain  = CYCLE_DURATION_MS - offset;

    if (remain == 0U) {
        remain = CYCLE_DURATION_MS;
    }

    if (!s_stop_timer) {
        s_stop_timer = lv_timer_create(stop_anim_timer_cb, remain, NULL);
    } else {
        lv_timer_set_period(s_stop_timer, remain);
        lv_timer_reset(s_stop_timer);
    }

    lv_timer_set_repeat_count(s_stop_timer, 1);
}

int zmk_widget_animation_init(struct zmk_widget_animation *widget, lv_obj_t *parent)
{
    if (!widget || !parent) {
        return -1;
    }

    widget->animimg = lv_animimg_create(parent);

    lv_animimg_set_src(widget->animimg, (const void **)animation_frames, FRAME_COUNT);
    lv_animimg_set_duration(widget->animimg, FRAME_DELAY_MS * FRAME_COUNT);
    lv_animimg_set_repeat_count(widget->animimg, LV_ANIM_REPEAT_INFINITE);
    lv_img_set_zoom(widget->animimg, FRAME_ZOOM);
    lv_img_set_src(widget->animimg, (const void *)animation_frames[0]);

    s_anim_widget   = widget;
    s_anim_running  = false;
    s_typing_active = false;

    if (s_stop_timer) {
        lv_timer_del(s_stop_timer);
        s_stop_timer = NULL;
    }

    return 0;
}

lv_obj_t *zmk_widget_animation_obj(struct zmk_widget_animation *widget)
{
    return widget ? widget->animimg : NULL;
}

void zmk_widget_animation_on_wpm_changed(uint16_t wpm)
{
    bool typing_now = (wpm > 0U);

    if (typing_now && !s_typing_active) {
        start_anim_if_needed();
    } else if (!typing_now && s_typing_active) {
        schedule_stop_after_current_cycle();
    }

    s_typing_active = typing_now;
}
