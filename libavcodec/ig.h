#ifndef AVCODEC_BLURAY_IG_H
#define AVCODEC_BLURAY_IG_H

/*
 * Blu-ray IG decoder
 * Copyright (c) 2014 Fuzhou Rockchip Electronics Co., Ltd
 * hh@rock-chips.com
 */


#define MAX_WINDOW_NUM 2
#define MAX_EFFECT_NUM  128
#define MAX_EFFECT_OBJECT_NUM 2


#define NUMBER 256 
#define PICTURENUMBER (4096)

typedef struct CompositionObject
{
    int      mObjectRefId;
    int      mWindowRefId;
    int      mForcedOnFlag;

    int      x;
    int      y;

    int      mCropFlag;
    int      mCropX;
    int      mCropY;
    int      mCropWidth;
    int      mCropHeight;
}CompositionObject ;


typedef struct Effect 
{
    unsigned int  mDuration;        /* 90kHz ticks */
    int           mPaletteRefId;

    unsigned  int  mNumberCompositionObj;
    CompositionObject *mCompositionObject;
}Effect ;


typedef struct  Window
{
    int  mId;
    int  x;
    int  y;
    int  mWidth;
    int  mHeight;
}Window;

typedef struct EffectSequence
{
    int       mNumberWidow;
    int       mNumberEffect;

    Window    *mWindow;
    Effect    *mEffect;
}EffectSequence;

/* possible states for a button */
typedef enum tag_Button_State
{
    BUTTON_DISABLED,
    BUTTON_ENABLED,
    BUTTON_SELECTED,
    BUTTON_ACTIVE,
    BUTTON_MAX
} BUTTON_STATE;

typedef struct IGCommand
{
    unsigned int   mOpCode;
    unsigned int   mDstOperand;
    unsigned int   mSrcOperand;
}IGCommand;




typedef struct IGSButton {
    unsigned short     id;
    unsigned short     numericValue;
    unsigned char      autoAction;
    unsigned short     x;
    unsigned short     y;
    unsigned short     n[4];//0:up,1:down,2:left,3:right
    unsigned short     pstart[3]; //0:normal,1:selected,2:activated
    unsigned short     pstop[3];
    unsigned short     pRepeat[3];
    unsigned int       mAnimtaionId;              // 用于指示当前button的动画图片的id
    unsigned short     cmds_count;

    unsigned char      mSelectedSoundId;
    unsigned char      mActiveSoundId;
    IGCommand          *cmds;

    BUTTON_STATE mState; /* current state of this button */
} IGSButton;


typedef struct IGSBOG {
    unsigned short     def_button;
    unsigned short     cur_button;
    int                buttons_count;
    IGSButton           *buttons;
} IGSBOG;



typedef struct IGSPage {
    int          def_button;
    int          sel_button;
    int          in_time;
    int          timeout;
    int          palette;
    int          bogs_count;
    int          id;
    IGSBOG      *bogs;

    EffectSequence* mInEffectSequnce;
    EffectSequence* mOutEffectSequnce;
} IGSPage;



typedef struct ICS
{
    int             mPages_count;

    int             mWidth;
    int             mHeight;
    int             mFrameRate;

    int             mUiMode;                    // bit7 set 1 means this is popup menu, whic is invisible default until is enable to pop up 
    unsigned long   compositionTimeoutPTS;
    unsigned long   selectionTimeoutPTS;        // 选中button显示的时间长度
    unsigned long   userTimeout;                // 用于显示没有点击，多长时间导航隐藏
    
    IGSPage        *mPages;
}ICS;


typedef struct IGSPalette {
   int id;
   unsigned int     clut[256];
} IGSPalette;


typedef struct IGSPicture {
    int                 id;
    int                 w;
    int                 h;
    unsigned int        rle_buffer_size, rle_data_len;
    unsigned int        rle_remaining_len;
    unsigned char      *rle;
    unsigned char      *picture;
} IGSPicture;


typedef struct IG
{
    // 图片数量
    int             mPicturesCount;

    // 调试板数量
    int             mPalettesCount;
    
    ICS            *mICS; 
    // 调试板数据
    IGSPalette     *mPalettes;

    // IG导航图片数据
    IGSPicture     *mPictures;    
}IG;

typedef struct IGContext
{
    // mICSDataSize,mICSDataTotalSize,mICSBuffer 用于缓冲ICS数据，拼凑成完整的ICS后再解析
    int             mICSDataSize;
    int             mICSDataTotalSize;
    unsigned char*  mICSBuffer;
    IG*             mIg;
}IGContext;


#endif
