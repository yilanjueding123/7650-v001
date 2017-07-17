#include "video_encoder.h"

/* global varaible */
INT32U jpeg_out_buffer_large_size;
INT32U jpeg_out_buffer_middle_size;
INT32U jpeg_out_buffer_small_size;

// Video Record buffer alloc
INT32U jpeg_out_buffer_large_cnt;
INT32U jpeg_out_buffer_middle_cnt;
INT32U jpeg_out_buffer_small_cnt;

INT32U jpeg_out_1080p_large_cnt;
INT32U jpeg_out_1080p_middle_cnt;
INT32U jpeg_out_1080p_small_cnt;

void video_encode_entrance(void)
{

    INT32S nRet;

    DBG_PRINT("=>video_encode_entrance!!!\r\n");
    {
        jpeg_out_buffer_large_size =		JPEG_OUT_BUFFER_LARGE_SIZE;
        jpeg_out_buffer_middle_size =	JPEG_OUT_BUFFER_MIDDLE_SIZE;
        jpeg_out_buffer_small_size =	JPEG_OUT_BUFFER_SMALL_SIZE;

        jpeg_out_buffer_large_cnt =		JPEG_OUT_BUFFER_LARGE_CNT;
        jpeg_out_buffer_middle_cnt =	JPEG_OUT_BUFFER_MIDDLE_CNT;
        jpeg_out_buffer_small_cnt =		JPEG_OUT_BUFFER_SMALL_CNT;

        jpeg_out_1080p_large_cnt =		JPEG_OUT_1080P_LARGE_CNT;
        jpeg_out_1080p_middle_cnt =	JPEG_OUT_1080P_MIDDLE_CNT;
        jpeg_out_1080p_small_cnt =		JPEG_OUT_1080P_SMALL_CNT;
    }

    my_avi_encode_init();

    nRet = my_avi_encode_state_task_create(AVI_ENC_PRIORITY);
    if(nRet < 0)
    {
        DBG_PRINT("avi_encode_state_task_create fail !!!");
    }

    nRet = avi_adc_record_task_create(AUD_ENC_PRIORITY);
    if(nRet < 0)
    {
        DBG_PRINT("avi_adc_record_task_create fail !!!");
    }
    else
    {
        // register UART RX for GPS function
        //avi_adc_gps_register();
        //ap_peripheral_gsensor_data_register();
    }

    nRet = avi_encode_packer_memory_alloc();
    if(nRet < 0)
    {
        DBG_PRINT("avi_encode_packer_memory_alloc fail !!!");
    }

    avi_enc_packer_init(pAviEncPacker0);
}

void video_encode_exit(void)
{
    INT32S nRet;

    DBG_PRINT("=>video_encode_exit!!!\r\n");

    video_encode_preview_stop();

    nRet = my_avi_encode_state_task_del();
    if(nRet < 0)
    {
        DBG_PRINT("avi_encode_state_task_del fail !!!");
    }

    nRet = avi_adc_record_task_del();
    if(nRet < 0)
    {
        DBG_PRINT("avi_adc_record_task_del fail !!!");
    }
}

CODEC_START_STATUS video_encode_preview_start(VIDEO_ARGUMENT arg)
{
    INT32S nRet;

    DBG_PRINT("=>video_encode_preview_start!!!\r\n");

    pAviEncAudPara->audio_format = AVI_ENCODE_AUDIO_FORMAT;
    pAviEncAudPara->channel_no = 1; //mono
    pAviEncAudPara->audio_sample_rate = arg.AudSampleRate;

    my_pAviEncVidPara->sensor_width = arg.SensorWidth;
    my_pAviEncVidPara->sensor_height = arg.SensorHeight;

    my_pAviEncVidPara->clip_width = arg.ClipWidth;
    my_pAviEncVidPara->clip_height = arg.ClipHeight;

    my_pAviEncVidPara->display_width = arg.DisplayWidth;
    my_pAviEncVidPara->display_height = arg.DisplayHeight;

    my_pAviEncVidPara->video_format = AVI_ENCODE_VIDEO_FORMAT;
    my_pAviEncVidPara->dwScale = arg.bScaler;
    my_pAviEncVidPara->dwRate = arg.VidFrameRate;
    my_pAviEncVidPara->encode_width = arg.TargetWidth;
    my_pAviEncVidPara->encode_height = arg.TargetHeight;
    my_pAviEncVidPara->sensor_do_init = arg.bSensorDoInit;
    my_pAviEncVidPara->enter_ap_mode = arg.bEnterApMode;
    //DBG_PRINT("xxxxxxxxxxxxxxxxxxxxxxxx\r\n");
    nRet = vid_enc_preview_start();
    if(nRet < 0)
    {

        //DBG_PRINT("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\r\n");
        return CODEC_START_STATUS_ERROR_MAX;
    }

    //DBG_PRINT("yyyyyyyyyyyyyyyyyyyyyyyy\r\n");
    return START_OK;
}

CODEC_START_STATUS video_encode_preview_stop(void)
{
    INT32S result;

    DBG_PRINT("=>video_encode_preview_stop!!!\r\n");

    result = vid_enc_preview_stop();
    if(result < 0)
    {
        return CODEC_START_STATUS_ERROR_MAX;
    }

    return START_OK;
}

CODEC_START_STATUS video_encode_start(MEDIA_SOURCE src, INT16S txt_handle)
{
    INT32S nRet;

    DBG_PRINT("=>video_encode_start, txt_handle = %d!!!\r\n", txt_handle);

    if(src.type == SOURCE_TYPE_FS)
    {
        pAviEncPara->source_type = SOURCE_TYPE_FS;
    }
    else if(src.type == SOURCE_TYPE_USER_DEFINE)
    {
        pAviEncPara->source_type = SOURCE_TYPE_USER_DEFINE;
    }
    else
    {
        return RESOURCE_WRITE_ERROR;
    }
	
    if(src.type_ID.FileHandle < 0)
    {
        return RESOURCE_NO_FOUND_ERROR;
    }
	
    avi_encode_set_curworkmem((void *)pAviEncPacker0);
    nRet = avi_encode_set_file_handle_and_caculate_free_size(pAviEncPara->AviPackerCur, src.type_ID.FileHandle, txt_handle);
    if(nRet < 0)
    {
        return RESOURCE_WRITE_ERROR;
    }
	
    //start avi packer
    nRet = avi_enc_packer_start(pAviEncPara->AviPackerCur);
    if(nRet < 0)
    {
        return CODEC_START_STATUS_ERROR_MAX;
    }
	
    //start avi encode
    nRet = avi_enc_start();
    if(nRet < 0)
    {
        return CODEC_START_STATUS_ERROR_MAX;
    }
	
    return START_OK;
}

CODEC_START_STATUS video_encode_stop(void)
{
    INT32S nRet, nTemp;

    DBG_PRINT("=>video_encode_stop!!!\r\n");

#if C_AUTO_DEL_FILE==CUSTOM_ON
    unlink_step_flush();
#endif

    nRet = avi_enc_stop();
    nTemp = avi_enc_packer_stop(pAviEncPara->AviPackerCur);
    avi_encode_stop();
    if(nRet < 0 || nTemp < 0)
    {
        return CODEC_START_STATUS_ERROR_MAX;
    }
    return START_OK;
}


CODEC_START_STATUS video_encode_fast_stop_and_start(MEDIA_SOURCE src, INT16S next_txt_handle)
{
    INT32S nRet;

    DBG_PRINT("=>video_encode_fast_stop_and_start!!!\r\n");

    if(src.type == SOURCE_TYPE_FS)
        pAviEncPara->source_type = SOURCE_TYPE_FS;
    else if(src.type == SOURCE_TYPE_USER_DEFINE)
        pAviEncPara->source_type = SOURCE_TYPE_USER_DEFINE;
    else
        return RESOURCE_WRITE_ERROR;

    if(src.type_ID.FileHandle < 0)
        return RESOURCE_NO_FOUND_ERROR;

    nRet = avi_enc_packer_switch_file(pAviEncPara->AviPackerCur, src.type_ID.FileHandle, next_txt_handle);
    if(nRet < 0)
    {
        return CODEC_START_STATUS_ERROR_MAX;
    }

    return START_OK;
}


CODEC_START_STATUS video_encode_auto_switch_csi_frame(void)
{
    DBG_PRINT("=>video_encode_auto_switch_csi_frame!!!\r\n");
    return START_OK;
}

CODEC_START_STATUS video_encode_auto_switch_csi_fifo_end(INT8U flag)
{
    DBG_PRINT("=>video_encode_auto_switch_csi_fifo_end!!!\r\n");
    return START_OK;
}

CODEC_START_STATUS video_encode_auto_switch_csi_frame_end(INT8U flag)
{
    DBG_PRINT("=>video_encode_auto_switch_csi_frame_end!!!\r\n");
    return START_OK;
}

CODEC_START_STATUS video_encode_set_zoom_scaler(FP32 zoom_ratio)
{
    DBG_PRINT("=>video_encode_set_zoom_scaler!!!\r\n");
    return START_OK;
}

CODEC_START_STATUS video_encode_capture_picture(MEDIA_SOURCE src)
{
    DBG_PRINT("=>video_encode_capture_picture!!!\r\n");

    if(src.type_ID.FileHandle < 0)
    {
        return CODEC_START_STATUS_ERROR_MAX;
    }

    pAviEncPara->AviPackerCur->file_handle = src.type_ID.FileHandle;

    if(avi_enc_save_jpeg() < 0)
    {
        close(src.type_ID.FileHandle);
        return CODEC_START_STATUS_ERROR_MAX;
    }

    return START_OK;
}

CODEC_START_STATUS video_encode_fast_switch_stop_and_start(MEDIA_SOURCE src)
{
    DBG_PRINT("=>video_encode_fast_switch_stop_and_start!!!\r\n");
    return START_OK;
}

/****************************************************************************/
/*
	video_encode_preview_off
 */
CODEC_START_STATUS video_encode_preview_off(void)
{
    if(vid_enc_preview_buf_to_dummy() < 0)
    {
        return CODEC_START_STATUS_ERROR_MAX;
    }

    return START_OK;
}

/****************************************************************************/
/*
	video_encode_preview_on
 */
CODEC_START_STATUS video_encode_preview_on(void)
{
    if(vid_enc_preview_buf_to_display() < 0)
    {
        return CODEC_START_STATUS_ERROR_MAX;
    }

    return START_OK;
}

