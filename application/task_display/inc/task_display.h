#ifndef __TASK_DISPLAY_H__
#define __TASK_DISPLAY_H__

#include "application.h"
#include "ap_display.h"

//	Display task frame buffer source
#define DISPLAY_BUFF_SRC_SENSOR			0
#define DISPLAY_BUFF_SRC_JPEG			1
#define DISPLAY_BUFF_SRC_MJPEG			2
#define DISPLAY_BUFF_SRC_WAV_TIME		3
#define DISPLAY_BUFF_SRC_USB_SETTING	4

#define DISPLAY_NONE_EFFECT			0
#define DISPLAY_PIC_EFFECT			1
#define DISPLAY_PIC_PREVIEW_EFFECT	2
#define DISPLAY_STRING_EFFECT		3

#define DISPLAY_MP3_INDEX_NONE_EFFECT		0
#define DISPLAY_MP3_INDEX_SHOW				1
#define DISPLAY_MP3_TOTAL_INDEX_SHOW		2
#define DISPLAY_MP3_VOLUME_SHOW				4
#define DISPLAY_MP3_FM_CHANNEL_SHOW			8

#define DISPLAY_MP3_INPUT_NUM_NONE_EFFECT		0
#define DISPLAY_MP3_INPUT_NUM_SHOW				1

extern OS_EVENT *DisplayTaskQ;
extern void task_display_init(void);

extern void task_display_entry(void *para);
extern void ap_display_init(void);
extern void ap_display_tv_init(void);
extern void ap_display_tv_uninit(void);
extern void ap_display_hdmi_init(void);
extern void ap_display_hdmi_uninit(void);
extern void ap_display_setting_frame_buff_set(INT32U frame_buff);
extern void ap_display_effect_init(void);
extern void ap_display_icon_move(DISPLAY_ICONMOVE *icon);
extern void ap_display_icon_sts_set(INT32U msg);
extern void ap_display_icon_sts_clear(INT32U msg);
extern void ap_display_buff_copy_and_draw(INT32U buff_addr, INT16U src_type);
extern void ap_display_hdmi_buff_copy_and_draw(INT32U buff_addr, INT16U src_type);
extern void ap_display_timer(void);
extern void ap_display_effect_sts_set(INT8U type);
extern void ap_display_string_draw(STR_ICON *str_info);
extern void ap_display_video_preview_end(void);
extern void ap_display_mp3_index_sts_set(INT8U type);
extern void ap_display_mp3_input_num_sts_set(INT8U type);
extern void ap_display_left_rec_time_draw(INT32U time, INT8U flag);

#define TRANSPARENT_COLOR			0x8c71
#ifdef HDMI_JPG_DECODE_AS_GP420
#define TRANSPARENT_COLOR_YUYV		0x8c808c80
#elif defined(HDMI_JPG_DECODE_AS_YUV422)
#define TRANSPARENT_COLOR_YUYV		0x8c80
#endif

typedef struct
{
    INT8U h;
    INT8U m;
    INT8U s;
} DISPLAY_REC_TIME;

extern INT16U icon_battery0[];			//1
extern INT16U icon_battery1[];			//2
extern INT16U icon_battery2[];			//3
extern INT16U icon_battery3[];			//4
//extern INT16U icon_battery_red[];		//5
extern INT16U icon_capture[];			//6
extern INT16U icon_video[];				//7
extern INT16U icon_review[];			//8
extern INT16U icon_base_setting[];
extern INT16U icon_red_light[];			//9
extern INT16U icon_play[];				//10
extern INT16U icon_pause[];				//11
extern INT16U icon_motion_detect[];		//12
extern INT16U icon_mic_off[];			//13
extern INT16U icon_ir_on[];				//14
extern INT16U icon_video1[];			//15
extern INT16U icon_focus[];				//16
extern INT16U icon_sd_card[];						//17
extern INT16U icon_internal_memory[];
extern INT16U icon_no_sd_card[];			//17
extern INT16U icon_video_record_cyc_1minute[];
extern INT16U icon_video_record_cyc_2minute[];
extern INT16U icon_video_record_cyc_3minute[];
extern INT16U icon_video_record_cyc_5minute[];
extern INT16U icon_video_record_cyc_10minute[];
extern INT16U icon_video_EV6[];
extern INT16U icon_video_EV5[];
extern INT16U icon_video_EV4[];
extern INT16U icon_video_EV3[];
extern INT16U icon_video_EV2[];
extern INT16U icon_video_EV1[];
extern INT16U icon_video_EV0[];
extern INT16U icon_video_EV_1[];
extern INT16U icon_video_EV_2[];
extern INT16U icon_video_EV_3[];
extern INT16U icon_video_EV_4[];
extern INT16U icon_video_EV_5[];
extern INT16U icon_video_EV_6[];
extern INT16U icon_video_ldw_active[];

extern INT16U icon_capture_white_balance_daylight[];
extern INT16U icon_capture_white_balance_cloudy[];
extern INT16U icon_capture_white_balance_tungsten[];
extern INT16U icon_capture_white_balance_fluorescent[];

extern INT16U icon_capture_ISO_auto[];
extern INT16U icon_capture_ISO_100[];
extern INT16U icon_capture_ISO_200[];
extern INT16U icon_capture_ISO_400[];

extern INT16U icon_capture_quality_fine[];
extern INT16U icon_capture_quality_normal[];
extern INT16U icon_capture_quality_economy[];

extern INT16U icon_playback_play[];
extern INT16U icon_playback_pause[];
extern INT16U icon_playback_movie[];

extern INT16U icon_motion_detect_start[];	//18
extern INT16U icon_usb_connect[];			//19
extern INT16U icon_scroll_bar[];			//20
extern INT16U icon_scroll_bar_idx[];		//21
extern INT16U icon_md_sts0[];				//22
extern INT16U icon_md_sts1[];				//23
extern INT16U icon_md_sts2[];				//24
extern INT16U icon_md_sts3[];				//25
extern INT16U icon_md_sts4[];				//26
extern INT16U icon_md_sts5[];				//27
extern INT16U icon_battery_charged[];
extern INT16U icon_locked[];
extern INT16U icon_audio_record[];
extern INT16U icon_2x[];					//41
extern INT16U icon_4x[];					//42
extern INT16U icon_forward[];				//45
extern INT16U icon_backward[];				//46

extern INT16U ui_up[];
extern INT16U ui_left[];
extern INT16U ui_right[];
extern INT16U ui_down[];

extern INT16U icon_background_mode[];
extern INT16U icon_background_up[];
extern INT16U icon_background_down[];
extern INT16U icon_background_left[];
extern INT16U icon_background_right[];
extern INT16U icon_background_ok[];

extern INT16U ui_text_0[];
extern INT16U ui_text_1[];
extern INT16U ui_text_2[];
extern INT16U ui_text_3[];
extern INT16U ui_text_4[];
extern INT16U ui_text_5[];
extern INT16U ui_text_6[];
extern INT16U ui_text_7[];
extern INT16U ui_text_8[];
extern INT16U ui_text_9[];
extern INT16U ui_text_klino	[];	//	"/"
extern INT16U ui_text_colon[];	//	":"

extern INT8U hdmi_ui_text_0[];
extern INT8U hdmi_ui_text_1[];
extern INT8U hdmi_ui_text_2[];
extern INT8U hdmi_ui_text_3[];
extern INT8U hdmi_ui_text_4[];
extern INT8U hdmi_ui_text_5[];
extern INT8U hdmi_ui_text_6[];
extern INT8U hdmi_ui_text_7[];
extern INT8U hdmi_ui_text_8[];
extern INT8U hdmi_ui_text_9[];
extern INT8U hdmi_ui_text_klino	[];	//	"/"
extern INT8U hdmi_ui_text_colon[];	//	":"

// For state setting
extern INT16U icon_manu_record_resolution[];
extern INT16U icon_manu_record_looprecording[];
extern INT16U icon_manu_record_EV[];
extern INT16U icon_manu_record_motiondetect[];
extern INT16U icon_manu_record_date_stemp[];
extern INT16U icon_manu_record_gsensor[];
extern INT16U icon_manu_record_ldw[];

extern INT16U icon_manu_capture_sequence[];
extern INT16U icon_manu_capture_quality[];
extern INT16U icon_manu_capture_sharpness[];
extern INT16U icon_manu_capture_white_balance[];
extern INT16U icon_manu_capture_iso[];
extern INT16U icon_manu_capture_anti_shaking[];
extern INT16U icon_manu_capture_quick_review[];

extern INT16U icon_manu_play_delete[];
extern INT16U icon_manu_play_protect[];

extern INT16U icon_manu_base_date_time[];
extern INT16U icon_manu_base_auto_power_off[];
extern INT16U icon_manu_base_screen_saver[];
extern INT16U icon_manu_base_delay_power_on[];
extern INT16U icon_manu_base_beep_sound[];
extern INT16U icon_manu_base_language[];
extern INT16U icon_manu_base_TV_mode[];
extern INT16U icon_manu_base_frequency[];
extern INT16U icon_manu_base_format[];
extern INT16U icon_manu_base_default_setting[];
extern INT16U icon_manu_base_version[];

extern INT16U icon_playback_volume_title[];
extern INT16U icon_playback_volume_red[];
extern INT16U icon_playback_volume_white[];



//--------Thumbnail--------------//
extern INT16U icon_playback_movie[];
//extern INT16U thumbnail_video_icon[];
extern INT16U thumbnail_cursor_3x3_96x64[];
extern INT16U thumbnail_cursor_3x3_black_96x64[];
extern INT16U thumbnail_lock_icon[];

//-------------MP3---------------//
extern INT16U icon_mp3_play[];
extern INT16U icon_mp3_pause[];
extern INT16U icon_mp3_play_one[];
extern INT16U icon_mp3_play_all[];
extern INT16U icon_mp3_index[];
extern INT16U icon_mp3_volume[];
extern INT16U icon_mp3_mute[];
extern INT16U icon_mp3_stop[];

extern INT16U mp3_index_0[];
extern INT16U mp3_index_1[];
extern INT16U mp3_index_2[];
extern INT16U mp3_index_3[];
extern INT16U mp3_index_4[];
extern INT16U mp3_index_5[];
extern INT16U mp3_index_6[];
extern INT16U mp3_index_7[];
extern INT16U mp3_index_8[];
extern INT16U mp3_index_9[];
extern INT16U mp3_index_dot[];
extern INT16U mp3_index_MHz[];

extern INT16U mp3_input_0[];
extern INT16U mp3_input_1[];
extern INT16U mp3_input_2[];
extern INT16U mp3_input_3[];
extern INT16U mp3_input_4[];
extern INT16U mp3_input_5[];
extern INT16U mp3_input_6[];
extern INT16U mp3_input_7[];
extern INT16U mp3_input_8[];
extern INT16U mp3_input_9[];

extern INT8U display_mp3_volume;
extern INT16U display_mp3_play_index;
extern INT16U display_mp3_total_index;
extern INT16U display_mp3_FM_channel;
extern INT16U display_mp3_input_num;
extern void timer_counter_force_display(INT8U force_en);

//-------------HDMI--------------//
extern const INT16U ALIGN4 hdmi_bat0[];
extern const INT16U ALIGN4 hdmi_bat1[];
extern const INT16U ALIGN4 hdmi_bat2[];
extern const INT16U ALIGN4 hdmi_bat3[];

extern const INT16U ALIGN4 hdmi_charging[];

extern const INT16U ALIGN4 hdmi_avi_file[];
extern const INT16U ALIGN4 hdmi_jpg_file[];
extern const INT16U ALIGN4 hdmi_key[];
extern const INT16U ALIGN4 hdmi_pause[];
extern const INT16U ALIGN4 hdmi_play[];

extern const INT16U ALIGN4 hdmi_red_cu[];
extern const INT16U ALIGN4 hdmi_white_cu[];
extern const INT16U ALIGN4 hdmi_speaker[];


#endif //__TASK_DISPLAY_H__

