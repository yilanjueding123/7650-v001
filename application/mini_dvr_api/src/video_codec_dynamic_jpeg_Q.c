#include "my_avi_encoder_state.h"


#if ENABLE_DYNAMIC_TUNING_JPEG_Q
extern INT32S current_Y_Q_value;
extern INT32S target_Y_Q_value;

extern INT32S current_UV_Q_value;
extern INT32S target_UV_Q_value;

extern INT32U current_VLC_size;
extern INT32U max_VLC_size;
#endif
/****************************************************************************/
/*
 *	Dynamic_Tune_Q
 */
void Dynamic_Tune_Q(INT32U jpeg_size, INT32U full_size_flag)
{

    if ( (my_pAviEncVidPara->encode_width == AVI_WIDTH_1080FHD) || (my_pAviEncVidPara->encode_width == AVI_WIDTH_1080P) )
    {
        if (full_size_flag)
        {
            current_Y_Q_value -= 15;
        }
        else if (jpeg_size > (max_VLC_size - (30 * 1024))) //330kb
        {
            current_Y_Q_value -= 5;
        }
        else if (jpeg_size < (max_VLC_size - (60 * 1024))) //300kb
        {
            current_Y_Q_value += 5;
        }
    }
    else	 // other size(720p)
    {
        if (full_size_flag)
        {
            current_Y_Q_value -= 15;
        }
        else if (jpeg_size > (max_VLC_size - (30 * 1024)))
        {
            current_Y_Q_value -= 5;
        }
        else if (jpeg_size < (max_VLC_size - (60 * 1024)))
        {
            current_Y_Q_value += 5;
        }
    }
    //+++++++++++++++++++++++++++++ 20150629 xiangyong
    if(current_Y_Q_value >= 50)
    {
        current_Y_Q_value = 50;
    }

    if(current_Y_Q_value <= 30)
    {
        current_Y_Q_value = 30;
    }

    //current_Y_Q_value = 25; //only test
    //----------------------------- 20150629 xiangyong
}
