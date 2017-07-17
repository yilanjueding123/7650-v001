#include "ap_state_config.h"
#include "state_video_record.h"
#include "ap_state_handling.h"
#include "ap_video_record.h"
#include "ap_display.h"
#include "avi_encoder_app.h"

#include "LDWs.h"
#if (Enable_Lane_Departure_Warning_System)
INT8U LDWS_Enable_Flag = 0;
INT32S	*LDWs_workmem = NULL;
extern LDWsParameter LDWPar;
extern INT8U ap_LDW_get_from_config(INT8U LDW_choice);
#endif

//	prototypes
INT8S state_video_record_init(INT32U state);
void state_video_record_exit(void);

extern void ap_video_record_md_icon_update(INT8U sts);
extern void ap_video_record_md_icon_clear_all(void);
extern void ap_video_capture_mode_switch(INT8U DoSensorInit, INT16U EnterAPMode);

extern STOR_SERV_FILEINFO curr_file_info;
extern STOR_SERV_FILEINFO next_file_info;

extern INT8U g_lock_current_file_flag;
extern INT8U g_cycle_record_continuing_flag;

static INT8U video_key_flag;

#if (Enable_Lane_Departure_Warning_System)

LDW_DISPLAY ldw_dis;
INT32U red_line_postion;
INT32U ldw_line_postion_L_x;
INT32U ldw_line_postion_L_y;
INT32U ldw_line_postion_R_x;
INT32U ldw_line_postion_R_y;
void ap_get_ldw_display_coordinate(void)
{
    //coordinate transform
    ldw_dis.ldw_dis_LT_alarmP_X = divFun((LDWPar.LT_alarmP_X * LDWPar.scaleRate_F), 10) + LDWPar.ROI.x;
    ldw_dis.ldw_dis_LT_alarmP_Y = divFun((LDWPar.LT_alarmP_Y * LDWPar.scaleRate_F), 10) + LDWPar.ROI.y + LDWPar.ROI_initY; // + 10;
    ldw_dis.ldw_dis_LB_alarmP_X = divFun((LDWPar.LB_alarmP_X * LDWPar.scaleRate_F), 10) + LDWPar.ROI.x;
    ldw_dis.ldw_dis_LB_alarmP_Y = divFun((LDWPar.LB_alarmP_Y * LDWPar.scaleRate_F), 10) + LDWPar.ROI.y + LDWPar.ROI_initY; //+ 10;
    ldw_dis.ldw_dis_LTP_X = divFun((LDWPar.LTP_X * LDWPar.scaleRate_F), 10) + LDWPar.ROI.x;
    ldw_dis.ldw_dis_LTP_Y = divFun((LDWPar.LTP_Y * LDWPar.scaleRate_F), 10) + LDWPar.ROI.y + LDWPar.ROI_initY; //+ 10;
    ldw_dis.ldw_dis_LBP_X = divFun((LDWPar.LBP_X * LDWPar.scaleRate_F), 10) + LDWPar.ROI.x;
    ldw_dis.ldw_dis_LBP_Y = divFun((LDWPar.LBP_Y * LDWPar.scaleRate_F), 10) + LDWPar.ROI.y + LDWPar.ROI_initY; //+ 10;

    ldw_dis.ldw_dis_RT_alarmP_X = divFun((LDWPar.RT_alarmP_X * LDWPar.scaleRate_F), 10) + LDWPar.ROI.x;
    ldw_dis.ldw_dis_RT_alarmP_Y = divFun((LDWPar.RT_alarmP_Y * LDWPar.scaleRate_F), 10) + LDWPar.ROI.y + LDWPar.ROI_initY; //+ 10;
    ldw_dis.ldw_dis_RB_alarmP_X = divFun((LDWPar.RB_alarmP_X * LDWPar.scaleRate_F), 10) + LDWPar.ROI.x;
    ldw_dis.ldw_dis_RB_alarmP_Y = divFun((LDWPar.RB_alarmP_Y * LDWPar.scaleRate_F), 10) + LDWPar.ROI.y + LDWPar.ROI_initY; //+ 10;
    ldw_dis.ldw_dis_RTP_X = divFun((LDWPar.RTP_X * LDWPar.scaleRate_F), 10) + LDWPar.ROI.x;
    ldw_dis.ldw_dis_RTP_Y = divFun((LDWPar.RTP_Y * LDWPar.scaleRate_F), 10) + LDWPar.ROI.y + LDWPar.ROI_initY; //+ 10;
    ldw_dis.ldw_dis_RBP_X = divFun((LDWPar.RBP_X * LDWPar.scaleRate_F), 10) + LDWPar.ROI.x;
    ldw_dis.ldw_dis_RBP_Y = divFun((LDWPar.RBP_Y * LDWPar.scaleRate_F), 10) + LDWPar.ROI.y + LDWPar.ROI_initY; //+ 10;


    ldw_dis.ldw_dis_LT_alarmP_X = ldw_dis.ldw_dis_LT_alarmP_X * TFT_WIDTH / 1280;
    ldw_dis.ldw_dis_LT_alarmP_Y = ldw_dis.ldw_dis_LT_alarmP_Y * TFT_HEIGHT / 720;
    ldw_dis.ldw_dis_LB_alarmP_X = ldw_dis.ldw_dis_LB_alarmP_X * TFT_WIDTH / 1280;
    ldw_dis.ldw_dis_LB_alarmP_Y = ldw_dis.ldw_dis_LB_alarmP_Y * TFT_HEIGHT / 720;
    ldw_dis.ldw_dis_LTP_X = ldw_dis.ldw_dis_LTP_X * TFT_WIDTH / 1280;
    ldw_dis.ldw_dis_LTP_Y = ldw_dis.ldw_dis_LTP_Y * TFT_HEIGHT / 720;
    ldw_dis.ldw_dis_LBP_X = ldw_dis.ldw_dis_LBP_X * TFT_WIDTH / 1280;
    ldw_dis.ldw_dis_LBP_Y = ldw_dis.ldw_dis_LBP_Y * TFT_HEIGHT / 720;

    ldw_dis.ldw_dis_RT_alarmP_X = ldw_dis.ldw_dis_RT_alarmP_X * TFT_WIDTH / 1280;
    ldw_dis.ldw_dis_RT_alarmP_Y = ldw_dis.ldw_dis_RT_alarmP_Y * TFT_HEIGHT / 720;
    ldw_dis.ldw_dis_RB_alarmP_X = ldw_dis.ldw_dis_RB_alarmP_X * TFT_WIDTH / 1280;
    ldw_dis.ldw_dis_RB_alarmP_Y = ldw_dis.ldw_dis_RB_alarmP_Y * TFT_HEIGHT / 720;
    ldw_dis.ldw_dis_RTP_X = ldw_dis.ldw_dis_RTP_X * TFT_WIDTH / 1280;
    ldw_dis.ldw_dis_RTP_Y = ldw_dis.ldw_dis_RTP_Y * TFT_HEIGHT / 720;
    ldw_dis.ldw_dis_RBP_X = ldw_dis.ldw_dis_RBP_X * TFT_WIDTH / 1280;
    ldw_dis.ldw_dis_RBP_Y = ldw_dis.ldw_dis_RBP_Y * TFT_HEIGHT / 720;

#if USE_PANEL_SCREEN_PROPOTION == PANEL_SCREEN_PROPOTION_4_3	//4:3
    ldw_dis.ldw_dis_LT_alarmP_Y = ldw_dis.ldw_dis_LT_alarmP_Y * 3 / 4 + 30;
    ldw_dis.ldw_dis_LB_alarmP_Y = ldw_dis.ldw_dis_LB_alarmP_Y * 3 / 4 + 30;
    ldw_dis.ldw_dis_RT_alarmP_Y = ldw_dis.ldw_dis_RT_alarmP_Y * 3 / 4 + 30;
    ldw_dis.ldw_dis_RB_alarmP_Y = ldw_dis.ldw_dis_RB_alarmP_Y * 3 / 4 + 30;

    ldw_dis.ldw_dis_LTP_Y = ldw_dis.ldw_dis_LTP_Y * 3 / 4 + 30;
    ldw_dis.ldw_dis_LBP_Y = ldw_dis.ldw_dis_LBP_Y * 3 / 4 + 30;
    ldw_dis.ldw_dis_RTP_Y = ldw_dis.ldw_dis_RTP_Y * 3 / 4 + 30;
    ldw_dis.ldw_dis_RBP_Y = ldw_dis.ldw_dis_RBP_Y * 3 / 4 + 30;

#endif
}

void ap_get_ldw_display_freeze_coordinate(void)
{
    red_line_postion = 437 * TFT_HEIGHT / 720;

#if USE_PANEL_SCREEN_PROPOTION == PANEL_SCREEN_PROPOTION_16_9
#else
    red_line_postion = red_line_postion * 3 / 4 + 30;
#endif

#if USE_PANEL_NAME == PANEL_T43
    ldw_line_postion_L_x = 123;
    ldw_line_postion_L_y = 226;
    ldw_line_postion_R_x = TFT_WIDTH - ldw_line_postion_L_x;
    ldw_line_postion_R_y = 226;
#elif USE_PANEL_NAME == PANEL_400X240_I80
    ldw_line_postion_L_x = 103;
    ldw_line_postion_L_y = 210;
    ldw_line_postion_R_x = TFT_WIDTH - ldw_line_postion_L_x;
    ldw_line_postion_R_y = 210;
#elif USE_PANEL_NAME == PANEL_T27P05_ILI8961
    ldw_line_postion_L_x = 82;
    ldw_line_postion_L_y = 210;
    ldw_line_postion_R_x = TFT_WIDTH - ldw_line_postion_L_x;
    ldw_line_postion_R_y = 210;
#elif USE_PANEL_NAME == PANEL_T40P00_ILI9342C
    ldw_line_postion_L_x = 46;
    ldw_line_postion_L_y = 210;
    ldw_line_postion_R_x = TFT_WIDTH - ldw_line_postion_L_x;
    ldw_line_postion_R_y = 210;
#else
    ldw_line_postion_L_x = 46;
    ldw_line_postion_L_y = 210;
    ldw_line_postion_R_x = TFT_WIDTH - ldw_line_postion_L_x;
    ldw_line_postion_R_y = 210;
#endif



}

#endif

INT8S state_video_record_init(INT32U state)
{
    DBG_PRINT("video_record state init enter\r\n");
    if (ap_video_record_init(state) == STATUS_OK)
    {
        return STATUS_OK;
    }
    else
    {
        return STATUS_FAIL;
    }
}
INT8U cyc_restart_flag = 0;
//extern INT8U  File_creat_ok;

//INT8U event_stop_flag=0;
extern INT8U   card_insert_autorecord;
void state_video_record_entry(void *para, INT32U state)
{
    EXIT_FLAG_ENUM exit_flag = EXIT_RESUME;
    INT32U msg_id, led_type;
    INT8U power_on_flag, type = 0;
    STAudioConfirm *audio_temp;
    INT32S 	nRet ;
    //+++
#if (Enable_Lane_Departure_Warning_System)
    INT32S	workingMemSzie;
    INT32S	workingImg_W = 288;
    INT32S	workingImg_H = 88;
#endif

    INT8U	send_active = 0;

#if C_MOTION_DETECTION == CUSTOM_ON
    INT8U md_check_tick, switch_flag;

    md_check_tick = switch_flag = 0;
#endif

	__msg("state_video_record_entry\n");
    video_key_flag = 0;

    power_on_flag = 0;

    if(ap_state_handling_storage_id_get() == NO_STORAGE)
    {
        led_type = LED_NO_SDC;
        msgQSend(PeripheralTaskQ, MSG_PERIPHERAL_TASK_LED_SET, &led_type, sizeof(INT32U), MSG_PRI_NORMAL);
    }
    else
    {
        led_type = LED_WAITING_RECORD;
    }

    if(*(INT32U *) para == STATE_STARTUP)   
    {
        //if(ap_state_handling_storage_id_get()!=NO_STORAGE)
        {
            DBG_PRINT("MSG_PERIPHERAL_TASK_USBD_DETECT_INIT\n");
            msgQSend(PeripheralTaskQ, MSG_PERIPHERAL_TASK_USBD_DETECT_INIT, NULL, NULL, MSG_PRI_NORMAL);
            power_on_flag = 1;
        }

    }

    else if((*(INT32U *) para == STATE_SETTING) || \
		(*(INT32U *) para == STATE_VIDEO_PREVIEW) || \
		(*(INT32U *) para == STATE_AUDIO_RECORD))
    {
        ap_video_capture_mode_switch(0, STATE_VIDEO_RECORD);
    }

    state_video_record_init(state);

    //+++
#if Enable_Lane_Departure_Warning_System
    LDWS_Enable_Flag = 0;
    if(ap_LDW_get_from_config(LDW_ON_OFF) && ((ap_state_config_video_resolution_get() == 0) || (ap_state_config_video_resolution_get() == 2)))
    {
        LDWS_Enable_Flag = 1;
        workingMemSzie = LDWs_get_memorySize(workingImg_W, workingImg_H, &LDWPar);
        LDWs_workmem = (INT32S *)gp_malloc_align(workingMemSzie, 8);
        LDWPar.carClassMode = ap_LDW_get_from_config(LDW_CAR_TYPE);   // 0 = 轎車, 1 = 休旅車, 2 = 特高車
        LDWPar.countryMode = ap_LDW_get_from_config(LDW_AREA_CHOICE);    // 0 = 台灣, 1 = 大陸
        LDWPar.sensitiveMode = ap_LDW_get_from_config(LDW_SENSITIVITY);  // 0 = 一般, 1 = 高感度
        LDWPar.turnOnSpeedMode = ap_LDW_get_from_config(LDW_START_SPEED);  // 0 = high, 1 = low
        LDWsInit(LDWs_workmem, &LDWPar);

        ap_get_ldw_display_freeze_coordinate();
    }
    drvl2_sdc_card_long_polling_set(SD_USED_NUM , LDWS_Enable_Flag);
#endif
    //---
    while (exit_flag == EXIT_RESUME)
    {
        if (msgQReceive(ApQ, &msg_id, (void *) ApQ_para, AP_QUEUE_MSG_MAX_LEN) == STATUS_FAIL)
        {
            continue;
        }

        switch (msg_id)
        {
        case EVENT_APQ_ERR_MSG:
            audio_temp = (STAudioConfirm *)ApQ_para;
            if ((audio_temp->result == AUDIO_ERR_DEC_FINISH) && (audio_temp->source_type == AUDIO_SRC_TYPE_APP_RS))
            {
                //gpio_write_io(SPEAKER_EN, DATA_LOW);
            }
            else if((audio_temp->result == AUDIO_ERR_DEC_FINISH) && (audio_temp->source_type == AUDIO_SRC_TYPE_FS))
            {
                audio_confirm_handler((STAudioConfirm *)ApQ_para);
            }
            else
            {
                audio_confirm_handler((STAudioConfirm *)ApQ_para);
            }
            break;
        case MSG_STORAGE_SERVICE_MOUNT:
            ap_state_handling_storage_id_set(ApQ_para[0]);
            ap_state_handling_str_draw_exit();
            ap_state_handling_icon_clear_cmd(ICON_INTERNAL_MEMORY, NULL, NULL);
            ap_state_handling_icon_show_cmd(ICON_SD_CARD, NULL, NULL);
            ap_video_record_sts_set(~VIDEO_RECORD_UNMOUNT);
            video_calculate_left_recording_time_enable();
            if (ap_state_config_md_get())
            {
                __msg("MSG_PERIPHERAL_TASK_MOTION_DETECT_START\n");
                ap_state_handling_icon_show_cmd(ICON_MOTION_DETECT, NULL, NULL);
                msgQSend(PeripheralTaskQ, MSG_PERIPHERAL_TASK_MOTION_DETECT_START, NULL, NULL, MSG_PRI_NORMAL);
                ap_video_record_sts_set(VIDEO_RECORD_MD);
            }
            DBG_PRINT("[Video Record Mount OK]\r\n");
            break;
        case MSG_STORAGE_SERVICE_NO_STORAGE:
            led_type = LED_NO_SDC;
			__msg("led_type = %d\n", led_type);
            msgQSend(PeripheralTaskQ, MSG_PERIPHERAL_TASK_LED_SET, &led_type, sizeof(INT32U), MSG_PRI_NORMAL);
            ap_state_handling_storage_id_set(ApQ_para[0]);

            msgQSend(StorageServiceQ, MSG_STORAGE_SERVICE_AUTO_DEL_LOCK, NULL, NULL, MSG_PRI_NORMAL);
            ap_video_record_error_handle(NULL);	//if it is recording, stop it
#if C_MOTION_DETECTION == CUSTOM_ON
            if(ap_video_record_sts_get() & VIDEO_RECORD_MD)
            {
                ap_video_record_md_icon_clear_all();
                ap_video_record_md_disable();
                ap_state_config_md_set(0);
            }
#endif
            video_calculate_left_recording_time_disable();
            ap_state_handling_str_draw_exit();
            ap_state_handling_icon_clear_cmd(ICON_SD_CARD, NULL, NULL);
            ap_state_handling_icon_show_cmd(ICON_INTERNAL_MEMORY, NULL, NULL);

            g_lock_current_file_flag = 0;
            g_cycle_record_continuing_flag = 0;
            ap_state_handling_icon_clear_cmd(ICON_LOCKED, NULL, NULL);
            ap_state_handling_icon_clear_cmd(ICON_VIDEO_LDW_SART, NULL, NULL);

            ap_video_record_sts_set(VIDEO_RECORD_UNMOUNT);
            power_on_flag = 0;
            DBG_PRINT("[Video Record Mount FAIL]\r\n");
            break;

#if C_MOTION_DETECTION == CUSTOM_ON
        case MSG_APQ_MOTION_DETECT_TICK:
            ap_video_record_md_tick(&md_check_tick, NULL);
            break;
        case MSG_APQ_MOTION_DETECT_ICON_UPDATE:
            ap_video_record_md_icon_update(ApQ_para[0]);
            break;
        case MSG_APQ_MOTION_DETECT_ACTIVE:
			__msg("MSG_APQ_MOTION_DETECT_ACTIVE\n");
            ap_video_record_md_active(&md_check_tick, NULL);
            break;
#endif

        case MSG_APQ_MD_SET:
			__msg("MSG_APQ_MD_SET\n");
            ap_peripheral_auto_off_force_disable_set(1);
		
            led_type = LED_MONTION_WAIT;
            msgQSend(PeripheralTaskQ, MSG_PERIPHERAL_TASK_LED_SET, &led_type, sizeof(INT32U), MSG_PRI_NORMAL);

			ap_state_config_md_set(1);
            msgQSend(PeripheralTaskQ, MSG_PERIPHERAL_TASK_MOTION_DETECT_START, NULL, NULL, MSG_PRI_NORMAL);
            ap_video_record_sts_set(VIDEO_RECORD_MD);
			
            break;

        case MSG_APQ_FUNCTION_KEY_ACTIVE:
        case MSG_APQ_VIDEO_RECORD_ACTIVE:
			__msg("MSG_APQ_VIDEO_RECORD_ACTIVE, video_status:%d\n", ap_video_record_sts_get());
            nRet = 0;
            if(nRet == 0)
            {
                power_on_flag = 0;				
                ap_video_record_func_key_active(msg_id);
            }
            
            break;

        case MSG_APQ_POWER_ON_AUTO_RECORD:
#if C_MOTION_DETECTION == CUSTOM_ON
            if(ap_video_record_sts_get() & VIDEO_RECORD_MD)
            {
                power_on_flag = 0;
            }
            else
#endif

                if(power_on_flag )
                {
                    if(usb_state_get())
                    {
                        if((ap_video_record_sts_get() & VIDEO_RECORD_BUSY))
                            break;
                        else if((ap_video_record_sts_get() & VIDEO_RECORD_MD))
                            break;

                    }
                    power_on_flag = 0;

                    ap_video_record_func_key_active(msg_id);
                    DBG_PRINT("AUTO_Record_start!!!!");
                }
            break;

        case MSG_APQ_POWER_KEY_ACTIVE:
			__msg("MSG_APQ_POWER_KEY_ACTIVE\n");
            if ((ap_video_record_sts_get() & VIDEO_RECORD_BUSY))
            {

                if(ap_video_record_func_key_active(msg_id))
                {
                    break;
                }
            }
            OSTimeDly(5);
            video_encode_exit();
            video_calculate_left_recording_time_disable();
            ap_state_handling_power_off();
            break;
        case MSG_APQ_QUICK_RECORD:
            if(!(ap_video_record_sts_get() & VIDEO_RECORD_BUSY))
            {

                ap_video_record_func_key_active(msg_id);
            }
            break;

        case MSG_APQ_MENU_KEY_ACTIVE:
            if ((ap_video_record_sts_get() & VIDEO_RECORD_BUSY))
            {
                break;
                if(ap_video_record_func_key_active(msg_id))
                {
                    break;
                }
            }
#if C_MOTION_DETECTION == CUSTOM_ON
            if(ap_video_record_sts_get() & VIDEO_RECORD_MD)
            {
                ap_video_record_md_icon_clear_all();
                ap_video_record_md_disable();
                ap_state_config_md_set(0);
            }
#endif
            ap_state_handling_str_draw_exit();
            OSTimeDly(10);	//wait to update display
            power_on_flag = 0;

            vid_enc_disable_sensor_clock();
            OSQPost(StateHandlingQ, (void *) STATE_SETTING);
            exit_flag = EXIT_BREAK;
            break;
        case MSG_APQ_AUDO_ACTIVE:
            if ((ap_video_record_sts_get() & VIDEO_RECORD_BUSY))
            {
                send_active = 0;
                if(ap_video_record_func_key_active(msg_id))
                {
                    break;
                }
            }
            else
                send_active = 1;
            video_calculate_left_recording_time_disable();
            OSTimeDly(3);
            power_on_flag = 0;

            vid_enc_disable_sensor_clock();
            OSQPost(StateHandlingQ, (void *) STATE_AUDIO_RECORD);
            if(send_active)
                msgQSend(ApQ, MSG_APQ_AUDO_ACTIVE, NULL, NULL, MSG_PRI_NORMAL);
            exit_flag = EXIT_BREAK;
            break;

        case MSG_APQ_CAPTURE_KEY_ACTIVE:
        case MSG_APQ_CAPTUER_ACTIVE:
        case MSG_APQ_MODE:
            if ((ap_video_record_sts_get() & VIDEO_RECORD_BUSY))
            {
                send_active = 0;
				led_type = LED_RECORD;
                msgQSend(PeripheralTaskQ, MSG_PERIPHERAL_TASK_LED_SET, &led_type, sizeof(INT32U), MSG_PRI_NORMAL);
				break;
#if 0
                if(ap_video_record_func_key_active(msg_id))
                {
                    video_key_flag = 0;
                    break;
                }
#endif
            }
            else
            {
                send_active = 1;
            }
#if C_MOTION_DETECTION == CUSTOM_ON
            if(ap_video_record_sts_get() & VIDEO_RECORD_MD)
            {
                ap_video_record_md_icon_clear_all();
                ap_video_record_md_disable();
                ap_state_config_md_set(0);
            }
#endif
#if GPDV_BOARD_VERSION != GPCV1237A_Aerial_Photo
            ap_state_handling_str_draw_exit();
#endif
            video_calculate_left_recording_time_disable();
            OSTimeDly(3);
            power_on_flag = 0;

            vid_enc_disable_sensor_clock();
            OSQPost(StateHandlingQ, (void *) STATE_VIDEO_PREVIEW);
#if GPDV_BOARD_VERSION == GPCV1237A_Aerial_Photo
            if(send_active)
            {
                msgQSend(ApQ, MSG_APQ_CAPTUER_ACTIVE, NULL, NULL, MSG_PRI_NORMAL);
            }
            else
            {
                led_type = LED_WAITING_CAPTURE;
                msgQSend(PeripheralTaskQ, MSG_PERIPHERAL_TASK_LED_SET, &led_type, sizeof(INT32U), MSG_PRI_NORMAL);
            }
#endif
            exit_flag = EXIT_BREAK;
            break;

#if C_AUTO_DEL_FILE == CUSTOM_ON
        case MSG_APQ_FREE_FILESIZE_CHECK_REPLY:
            DBG_PRINT("--CYC_0---\r\n");
            if(*((INT32S *) ApQ_para) == STATUS_OK)
            {
                ap_video_record_start();
            }
            else
            {
                ap_state_handling_str_draw_exit();
                ap_state_handling_str_draw(STR_SD_FULL, WARNING_STR_COLOR);
                video_calculate_left_recording_time_disable();
                led_type = LED_SDC_FULL;
                msgQSend(PeripheralTaskQ, MSG_PERIPHERAL_TASK_LED_SET, &led_type, sizeof(INT32U), MSG_PRI_NORMAL);
            }
            break;
#endif
        case MSG_STORAGE_SERVICE_VID_REPLY:
            DBG_PRINT("MSG_STORAGE_SERVICE_VID_REPLY\n");
            if ((ap_video_record_sts_get() & VIDEO_RECORD_BUSY) == 0)
            {

                STOR_SERV_FILEINFO *file_info_ptr = (STOR_SERV_FILEINFO *) ApQ_para;
                INT8U type = FALSE;

                DBG_PRINT("Not VIDEO_RECORD_BUSY\n");
                msgQSend(StorageServiceQ, MSG_STORAGE_SERVICE_VID_CYCLIC_REQ, &type, sizeof(INT8U), MSG_PRI_NORMAL);
                close(file_info_ptr->file_handle);
                unlink((CHAR *) file_info_ptr->file_path_addr);
                file_info_ptr->file_handle = -1;
#if	GPS_TXT
                close(file_info_ptr->txt_handle);
                unlink((CHAR *) file_info_ptr->txt_path_addr);
                file_info_ptr->txt_handle = -1;
#endif
            }
            else
            {

                DBG_PRINT("video busy status\r\n");
                ap_video_record_reply_action((STOR_SERV_FILEINFO *) ApQ_para);
#if C_MOTION_DETECTION == CUSTOM_ON
                switch_flag = 0;
#endif
                if(ap_state_handling_file_creat_get())
                {
                    if(ap_video_record_sts_get() & VIDEO_RECORD_MD)
                    {
                        led_type = LED_MOTION_DETECTION;
                    }
                    else
                    {
                        led_type = LED_RECORD;
                    }
					
                    msgQSend(PeripheralTaskQ, MSG_PERIPHERAL_TASK_LED_SET, &led_type, sizeof(INT32U), MSG_PRI_NORMAL);
                }

            }

            break;

#if C_CYCLIC_VIDEO_RECORD == CUSTOM_ON
        case MSG_STORAGE_SERVICE_VID_CYCLIC_REPLY:
            if ((ap_video_record_sts_get() & VIDEO_RECORD_BUSY) == 0)
            {
                STOR_SERV_FILEINFO *file_info_ptr = (STOR_SERV_FILEINFO *) ApQ_para;
                INT8U type = FALSE;
                DBG_PRINT("video cyclic status\n");

                msgQSend(StorageServiceQ, MSG_STORAGE_SERVICE_VID_CYCLIC_REQ, &type, sizeof(INT8U), MSG_PRI_NORMAL);
                close(file_info_ptr->file_handle);
                unlink((CHAR *) file_info_ptr->file_path_addr);
                file_info_ptr->file_handle = -1;
#if GPS_TXT
                close(file_info_ptr->txt_handle);
                unlink((CHAR *) file_info_ptr->txt_path_addr);
                file_info_ptr->txt_handle = -1;
#endif
            }
            else
            {

                DBG_PRINT("cyclic video status busy\n");
                ap_video_record_cycle_reply_open((STOR_SERV_FILEINFO *) ApQ_para);
            }
            break;

        case MSG_APQ_CYCLIC_VIDEO_RECORD:

            DBG_PRINT("--CYC_6---\r\n");
            ap_video_record_cycle_reply_action();
            break;
#endif

        case MSG_APQ_NEXT_KEY_ACTIVE:
            if((ap_video_record_sts_get() & VIDEO_RECORD_BUSY))
            {
#if KEY_TYPE != KEY_TYPE2 && KEY_TYPE != KEY_TYPE4
                msgQSend(ApQ, MSG_APQ_SOS_KEY_ACTIVE, NULL, NULL, MSG_PRI_NORMAL);
#endif
            }
            else
            {
                ap_video_record_zoom_inout(0);
            }
            break;

        case MSG_APQ_PREV_KEY_ACTIVE:
        {
            INT8U temp;
            temp = ap_state_config_voice_record_switch_get();
            if(temp)
            {
                ap_state_config_voice_record_switch_set(0);
            }
            else
            {
                ap_state_config_voice_record_switch_set(1);
            }
            if(ap_state_config_voice_record_switch_get() != 0)
            {
                ap_state_handling_icon_clear_cmd(ICON_MIC_OFF, NULL, NULL);
            }
            else
            {
                ap_state_handling_icon_show_cmd(ICON_MIC_OFF, NULL, NULL);
            }

            ap_video_record_zoom_inout(1);
        }
        break;

        case MSG_APQ_PARK_MODE_SET:
            if(ap_state_config_park_mode_G_sensor_get())
            {
                ap_state_config_park_mode_G_sensor_set(0);
                ap_video_record_clear_park_mode();
                DBG_PRINT("PARK MODE OFF\r\n");
            }
            else
            {
                ap_state_config_park_mode_G_sensor_set(1);
                ap_video_record_show_park_mode();
                DBG_PRINT("PARK MODE ON\r\n");
            }
            break;

        case MSG_APQ_CONNECT_TO_PC:
            DBG_PRINT("------usb_connect-------\r\n");
#if GPDV_BOARD_VERSION != GPCV1237A_Aerial_Photo
            if(ap_display_get_device() != DISP_DEV_TFT) break;
            msgQSend(PeripheralTaskQ, MSG_PERIPHERAL_TV_POLLING_STOP, NULL, NULL, MSG_PRI_NORMAL);
            OSTimeDly(3);
#endif
            power_on_flag = 0;

#if C_MOTION_DETECTION == CUSTOM_ON			
            if(ap_video_record_sts_get() & VIDEO_RECORD_MD)
            {
                ap_video_record_md_icon_clear_all();
                ap_video_record_md_disable();
                ap_state_config_md_set(0);
            }
#endif
            if ((ap_video_record_sts_get() & VIDEO_RECORD_BUSY) != 0)
            {

                ap_video_record_func_key_active(NULL);
            }

            video_calculate_left_recording_time_disable();
            ap_state_handling_str_draw_exit();

            vid_enc_disable_sensor_clock();
            video_encode_preview_off();

            //+++
#if Enable_Lane_Departure_Warning_System
            LDWS_Enable_Flag = 0;
            drvl2_sdc_card_long_polling_set(SD_USED_NUM , LDWS_Enable_Flag);
#endif
            //---
            ap_state_handling_connect_to_pc(STATE_VIDEO_RECORD);

            break;

        case MSG_APQ_DISCONNECT_TO_PC:
            DBG_PRINT("---message usb disconnect pc---\r\n");
#if GPDV_BOARD_VERSION != GPCV1237A_Aerial_Photo
            msgQSend(PeripheralTaskQ, MSG_PERIPHERAL_TV_POLLING_START, NULL, NULL, MSG_PRI_NORMAL);
#endif
            power_on_flag = 0;

            ap_state_handling_disconnect_to_pc();
            OSTimeDly(50);
            if(ap_state_handling_storage_id_get() != NO_STORAGE)
            {
                video_calculate_left_recording_time_enable();
            }

            vid_enc_disable_sensor_clock();
            video_encode_preview_on();

            ap_video_capture_mode_switch(0, STATE_VIDEO_RECORD);
            //+++
#if Enable_Lane_Departure_Warning_System
            if(ap_LDW_get_from_config(LDW_ON_OFF) && ((ap_state_config_video_resolution_get() == 0) || (ap_state_config_video_resolution_get() == 2)))
            {
                LDWS_Enable_Flag = 1;
                drvl2_sdc_card_long_polling_set(SD_USED_NUM , LDWS_Enable_Flag);
            }
#endif
            break;

        case MSG_APQ_RECORD_SWITCH_FILE:
#if C_MOTION_DETECTION == CUSTOM_ON
            if (!switch_flag)
            {
                switch_flag = 1;

                ap_video_record_func_key_active(msg_id);
                ap_video_record_func_key_active(msg_id);
            }
#endif
            break;

        case MSG_APQ_VDO_REC_RESTART:
            DBG_PRINT("VIDEO_REC_RESTART\r\n");

            cyc_restart_flag = 1;
            ap_video_record_func_key_active(msg_id);
            cyc_restart_flag = 0;
        case MSG_APQ_VDO_REC_STOP:
            ap_video_record_func_key_active(msg_id);
            if (msg_id == MSG_APQ_VDO_REC_STOP)
            {
                ap_state_handling_str_draw_exit();
                ap_state_handling_str_draw(STR_SD_FULL, WARNING_STR_COLOR);
                video_calculate_left_recording_time_disable();
                led_type = LED_SDC_FULL;
                msgQSend(PeripheralTaskQ, MSG_PERIPHERAL_TASK_LED_SET, &led_type, sizeof(INT32U), MSG_PRI_NORMAL);
            }
            break;

        case MSG_APQ_AVI_PACKER_ERROR:
            ap_video_record_error_handle(NULL);

            // Dominant add. restart AVI record again.
            if(drvl2_sdc_live_response() == 0)    // Card exist
            {
                if (ap_state_config_md_get())
                {
                    break;  // motion detect mode..... no need restart
                }
                else
                {
                    INT64U  disk_free_size;

                    disk_free_size = vfsFreeSpace(MINI_DVR_STORAGE_TYPE) >> 20;
                    if (disk_free_size < CARD_FULL_SIZE_RECORD)
                    {
                        ap_state_handling_str_draw_exit();
                        ap_state_handling_str_draw(STR_SD_FULL, WARNING_STR_COLOR);
                        video_calculate_left_recording_time_disable();
                        led_type = LED_SDC_FULL;
                        msgQSend(PeripheralTaskQ, MSG_PERIPHERAL_TASK_LED_SET, &led_type, sizeof(INT32U), MSG_PRI_NORMAL);
                        msgQSend(StorageServiceQ, MSG_STORAGE_SERVICE_TIMER_START, NULL, NULL, MSG_PRI_NORMAL);
                    }
                    else
                    {
                        msgQSend(ApQ, MSG_APQ_FUNCTION_KEY_ACTIVE, NULL, NULL, MSG_PRI_NORMAL); //restart
                    }
                }
            }
            else
            {
#if C_MOTION_DETECTION == CUSTOM_ON
                if(ap_video_record_sts_get() & VIDEO_RECORD_MD)
                {
                    ap_video_record_md_icon_clear_all();
                    ap_video_record_md_disable();
                    ap_state_config_md_set(0);
                }
#endif
                msgQSend(StorageServiceQ, MSG_STORAGE_SERVICE_STORAGE_CHECK, NULL, NULL, MSG_PRI_NORMAL);
            }
            break;

#if C_BATTERY_DETECT == CUSTOM_ON
        case MSG_APQ_BATTERY_LVL_SHOW:
            ap_state_handling_battery_icon_show(ApQ_para[0]);
            break;
        case MSG_APQ_BATTERY_CHARGED_SHOW:
            ap_state_handling_charge_icon_show(1);
            break;
        case MSG_APQ_BATTERY_CHARGED_CLEAR:
            ap_state_handling_charge_icon_show(0);
            break;
        case MSG_APQ_BATTERY_LOW_SHOW:
            if ((ap_video_record_sts_get() & VIDEO_RECORD_BUSY))
            {
                if(ap_video_record_func_key_active(msg_id))
                {
                    break;
                }
            }
            OSTimeDly(5);
            ap_state_handling_clear_all_icon();
            ap_state_handling_str_draw_exit();
            ap_state_handling_str_draw(STR_BATTERY_LOW, WARNING_STR_COLOR);
            msgQSend(PeripheralTaskQ, MSG_PERIPHERAL_TASK_BATTERY_LOW_BLINK_START, NULL, NULL, MSG_PRI_NORMAL);
            break;
#endif

        case MSG_APQ_NIGHT_MODE_KEY:
            audio_effect_play(EFFECT_CLICK);
            ap_state_handling_night_mode_switch();
            break;

        case MSG_APQ_AUDIO_EFFECT_UP:
        case MSG_APQ_AUDIO_EFFECT_DOWN:
            break;

        case MSG_APQ_USER_CONFIG_STORE:
            ap_state_config_store();
            break;

#if GPDV_BOARD_VERSION != GPCV1237A_Aerial_Photo
        case MSG_APQ_SOS_KEY_ACTIVE:
            if ((ap_video_record_sts_get() & VIDEO_RECORD_BUSY))
            {
                ap_video_record_lock_current_file();
                ap_state_handling_icon_show_cmd(ICON_LOCKED, NULL, NULL);
            }
            break;
#endif
        case MSG_APQ_HDMI_PLUG_IN:
            if ((ap_video_record_sts_get() & VIDEO_RECORD_BUSY))
            {
                if(ap_video_record_func_key_active(msg_id))
                {
                    break;
                }
            }
#if C_MOTION_DETECTION == CUSTOM_ON
            if(ap_video_record_sts_get() & VIDEO_RECORD_MD)
            {
                ap_video_record_md_icon_clear_all();
                ap_video_record_md_disable();
                ap_state_config_md_set(0);
            }
#endif
            ap_state_handling_str_draw_exit();
            video_calculate_left_recording_time_disable();
            power_on_flag = 0;

            vid_enc_disable_sensor_clock();
            OSQPost(StateHandlingQ, (void *) STATE_VIDEO_PREVIEW);
            msgQSend(ApQ, MSG_APQ_HDMI_PLUG_IN, NULL, NULL, MSG_PRI_NORMAL);
            exit_flag = EXIT_BREAK;
            break;

        //+++ TV_OUT_D1
        case MSG_APQ_TV_PLUG_OUT:
        case MSG_APQ_TV_PLUG_IN:
            if ((ap_video_record_sts_get() & VIDEO_RECORD_BUSY))
            {
                if(ap_video_record_func_key_active(msg_id))
                {
                    break;
                }
            }
#if C_MOTION_DETECTION == CUSTOM_ON
            if(ap_video_record_sts_get() & VIDEO_RECORD_MD)
            {
                ap_video_record_md_icon_clear_all();
                ap_video_record_md_disable();
            }
#endif
            ap_state_handling_str_draw_exit();
            ap_video_record_clear_resolution_str();
            ap_video_record_clear_park_mode();

            vid_enc_disable_sensor_clock();
            if(msg_id == MSG_APQ_TV_PLUG_IN)
            {
                ap_state_handling_tv_init();
            }
            else
            {
                ap_state_handling_tv_uninit();
            }
            ap_video_capture_mode_switch(0, STATE_VIDEO_RECORD);
            ap_video_record_resolution_display();
            if(ap_state_config_park_mode_G_sensor_get())
            {
                ap_video_record_show_park_mode();
            }
            power_on_flag = 0;
            break;
        //---
        default:
            if(ap_video_record_sts_get() & VIDEO_RECORD_BUSY)
            {
                if((msg_id == MSG_APQ_AUDIO_EFFECT_OK) || (msg_id == MSG_APQ_AUDIO_EFFECT_MODE))
                {
                    ap_state_common_handling(msg_id);
                }
            }
            else
            {
                ap_state_common_handling(msg_id);
            }
            break;
        }
    }
    state_video_record_exit();
}

void state_video_record_exit(void)
{
    //+++
#if (Enable_Lane_Departure_Warning_System)
    if(LDWS_Enable_Flag)
    {
        if(LDWs_workmem != NULL)
        {
            gp_free((void *) LDWs_workmem);
            LDWs_workmem = NULL;
        }
    }
    LDWS_Enable_Flag = 0;
    drvl2_sdc_card_long_polling_set(SD_USED_NUM , LDWS_Enable_Flag);
#endif

    ap_video_record_exit();
    DBG_PRINT("Exit video_record state\r\n");
}
