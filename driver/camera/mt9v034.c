#include "system.h"

/*!
* @brief 写MT9V034寄存器
* ----------------------
* @param RegAdd    寄存器地址
* @param Data      16位数据
*/
static unsigned char mt9v_write(unsigned char RegAdd, uint16_t Data)  //I2C 写16位寄存器函数
{
  uint8_t buf[2];
  buf[0]=(uint8_t)(Data >> 8);
  buf[1]=(uint8_t)(Data);
  unsigned char result = IIC_WriteData(LPI2C1, MTV_I2C_ADDR, 8, RegAdd, buf, 2);
  return result;
}


/*!
* @brief 读MT9V034寄存器
* ----------------------
* @param RegAdd    寄存器地址
* @param Data      16位数据
*/
//I2C 读16位寄存器函数  address：要读寄存器   Data：读出值的缓冲区
static unsigned char mt9v_read(unsigned char RegAdd, uint16_t *Data)
{  
  uint8_t buf[2];
  unsigned char result = IIC_ReadData(LPI2C1, MTV_I2C_ADDR, 8, RegAdd, buf, 2);		//I2C1读16位寄存器函数
  *Data= ((buf[0]<<8)| buf[1]);
  return result;
}


status_t mt9v_deinit(camera_device_handle_t *handle)    
{
  ((mt9v_resource_t *)(handle->resource))->pullPowerDownPin(true);
  return kStatus_Success;
}

status_t mt9v_control(camera_device_handle_t *handle, camera_device_cmd_t cmd, int32_t arg)
{   
  return kStatus_InvalidArgument;
}

status_t mt9v_start(camera_device_handle_t *handle)
{
  return kStatus_Success;
}

status_t mt9v_stop(camera_device_handle_t *handle)
{
  return kStatus_Success;
}

status_t mt9v_initext(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig)
{
  return mt9v_init(handle, config);
}

/**
*  MT9V034默认的寄存器配置
*  ----------------
*  曝光时间最小
*  [A: Min_Exposure (CSW=0, FSW=260) B: (CSW=1)]
*  配置文件MT9V034-REV4-COLOR.ini
*/
static void mt9v_default_settings(void)   //神眼摄像头默认配置
{
  mt9v_write(0x01, 0x0001);   //COL_WINDOW_START_CONTEXTA_REG
  mt9v_write(0x02, 0x0004);   //ROW_WINDOW_START_CONTEXTA_REG
  mt9v_write(0x03, 0x01E0);   //ROW_WINDOW_SIZE_CONTEXTA_REG
  mt9v_write(0x04, 0x02F0);   //COL_WINDOW_SIZE_CONTEXTA_REG
  mt9v_write(0x05, 0x005E);   //HORZ_BLANK_CONTEXTA_REG
  mt9v_write(0x06, 0x002D);   //VERT_BLANK_CONTEXTA_REG
  mt9v_write(0x07, 0x0188);   //CONTROL_MODE_REG
  mt9v_write(0x08, 0x01BB);   //COARSE_SHUTTER_WIDTH_1_CONTEXTA
  mt9v_write(0x09, 0x01D9);   //COARSE_SHUTTER_WIDTH_2_CONTEXTA
  mt9v_write(0x0A, 0x0164);   //SHUTTER_WIDTH_CONTROL_CONTEXTA
  mt9v_write(0x0B, 0x0000);   //COARSE_SHUTTER_WIDTH_TOTAL_CONTEXTA
  mt9v_write(0x0C, 0x0000);   //RESET_REG
  mt9v_write(0x0D, 0x0300);   //READ_MODE_REG
  mt9v_write(0x0E, 0x0000);   //READ_MODE2_REG
  mt9v_write(0x0F, 0x0000);   //PIXEL_OPERATION_MODE
  mt9v_write(0x10, 0x0040);   //RAMP_START_DELAY
  mt9v_write(0x11, 0x8042);   //OFFSET_CONTROL
  mt9v_write(0x12, 0x0022);   //AMP_RESET_BAR_CONTROL
  mt9v_write(0x13, 0x2D2E);   //5T_PIXEL_RESET_CONTROL
  mt9v_write(0x14, 0x0E02);   //4T_PIXEL_RESET_CONTROL
  mt9v_write(0x15, 0x0E32);   //TX_CONTROL
  mt9v_write(0x16, 0x2802);   //5T_PIXEL_SHS_CONTROL
  mt9v_write(0x17, 0x3E38);   //4T_PIXEL_SHS_CONTROL
  mt9v_write(0x18, 0x3E38);   //5T_PIXEL_SHR_CONTROL
  mt9v_write(0x19, 0x2802);   //4T_PIXEL_SHR_CONTROL
  mt9v_write(0x1A, 0x0428);   //COMPARATOR_RESET_CONTROL
  mt9v_write(0x1B, 0x0000);   //LED_OUT_CONTROL
  mt9v_write(0x1C, 0x0302);   //DATA_COMPRESSION
  mt9v_write(0x1D, 0x0040);   //ANALOG_TEST_CONTROL
  mt9v_write(0x1E, 0x0000);   //SRAM_TEST_DATA_ODD
  mt9v_write(0x1F, 0x0000);   //SRAM_TEST_DATA_EVEN
  mt9v_write(0x20, 0x03C7);   //BOOST_ROW_EN
  mt9v_write(0x21, 0x0020);   //I_VLN_CONTROL
  mt9v_write(0x22, 0x0020);   //I_VLN_AMP_CONTROL
  mt9v_write(0x23, 0x0010);   //I_VLN_CMP_CONTROL
  mt9v_write(0x24, 0x001B);   //I_OFFSET_CONTROL
  //MTV_IICWriteReg1G=0x25, 0x0000); // I_BANDGAP_CONTROL - TRIMMED PER DIE
  mt9v_write(0x26, 0x0004);   //I_VLN_VREF_ADC_CONTROL
  mt9v_write(0x27, 0x000C);   //I_VLN_STEP_CONTROL
  mt9v_write(0x28, 0x0010);   //I_VLN_BUF_CONTROL
  mt9v_write(0x29, 0x0010);   //I_MASTER_CONTROL
  mt9v_write(0x2A, 0x0020);   //I_VLN_AMP_60MHZ_CONTROL
  mt9v_write(0x2B, 0x0004);   //VREF_AMP_CONTROL
  mt9v_write(0x2C, 0x0004);   //VREF_ADC_CONTROL
  mt9v_write(0x2D, 0x0004);   //VBOOST_CONTROL
  mt9v_write(0x2E, 0x0007);   //V_HI_CONTROL
  mt9v_write(0x2F, 0x0003);   //V_LO_CONTROL
  mt9v_write(0x30, 0x0003);   //V_AMP_CAS_CONTROL
  mt9v_write(0x31, 0x0027);   //V1_CONTROL_CONTEXTA
  mt9v_write(0x32, 0x001A);   //V2_CONTROL_CONTEXTA
  mt9v_write(0x33, 0x0005);   //V3_CONTROL_CONTEXTA
  mt9v_write(0x34, 0x0003);   //V4_CONTROL_CONTEXTA
  mt9v_write(0x35, 0x0010);   //GLOBAL_GAIN_CONTEXTA_REG
  mt9v_write(0x36, 0x8010);   //GLOBAL_GAIN_CONTEXTB_REG
  mt9v_write(0x37, 0x0000);   //VOLTAGE_CONTROL
  mt9v_write(0x38, 0x0000);   //IDAC_VOLTAGE_MONITOR
  mt9v_write(0x39, 0x0027);   //V1_CONTROL_CONTEXTB
  mt9v_write(0x3A, 0x0026);   //V2_CONTROL_CONTEXTB
  mt9v_write(0x3B, 0x0005);   //V3_CONTROL_CONTEXTB
  mt9v_write(0x3C, 0x0003);   //V4_CONTROL_CONTEXTB
  mt9v_write(0x40, 0x0080);   //DARK_AVG_THRESHOLDS
  mt9v_write(0x46, 0x231D);   //CALIB_CONTROL_REG (AUTO)
  mt9v_write(0x47, 0x0080);   //STEP_SIZE_AVG_MODE
  mt9v_write(0x48, 0x0020);   //ROW_NOISE_CONTROL
  mt9v_write(0x4C, 0x0002);   //NOISE_CONSTANT
  mt9v_write(0x60, 0x0000);   //PIXCLK_CONTROL
  mt9v_write(0x67, 0x0000);   //TEST_DATA
  mt9v_write(0x6C, 0x0000);   //TILE_X0_Y0
  mt9v_write(0x70, 0x0000);   //TILE_X1_Y0
  mt9v_write(0x71, 0x002A);   //TILE_X2_Y0
  mt9v_write(0x72, 0x0000);   //TILE_X3_Y0
  mt9v_write(0x7F, 0x0000);   //TILE_X4_Y0
  mt9v_write(0x99, 0x0000);   //TILE_X0_Y1
  mt9v_write(0x9A, 0x0096);   //TILE_X1_Y1
  mt9v_write(0x9B, 0x012C);   //TILE_X2_Y1
  mt9v_write(0x9C, 0x01C2);   //TILE_X3_Y1
  mt9v_write(0x9D, 0x0258);   //TILE_X4_Y1
  mt9v_write(0x9E, 0x02F0);   //TILE_X0_Y2
  mt9v_write(0x9F, 0x0000);   //TILE_X1_Y2
  mt9v_write(0xA0, 0x0060);   //TILE_X2_Y2
  mt9v_write(0xA1, 0x00C0);   //TILE_X3_Y2
  mt9v_write(0xA2, 0x0120);   //TILE_X4_Y2
  mt9v_write(0xA3, 0x0180);   //TILE_X0_Y3
  mt9v_write(0xA4, 0x01E0);   //TILE_X1_Y3
  mt9v_write(0xA5, 0x003A);   //TILE_X2_Y3
  mt9v_write(0xA6, 0x0002);   //TILE_X3_Y3
  mt9v_write(0xA8, 0x0000);   //TILE_X4_Y3
  mt9v_write(0xA9, 0x0002);   //TILE_X0_Y4
  mt9v_write(0xAA, 0x0002);   //TILE_X1_Y4
  mt9v_write(0xAB, 0x0040);   //TILE_X2_Y4
  mt9v_write(0xAC, 0x0001);   //TILE_X3_Y4
  mt9v_write(0xAD, 0x01E0);   //TILE_X4_Y4
  mt9v_write(0xAE, 0x0014);   //X0_SLASH5
  mt9v_write(0xAF, 0x0000);   //X1_SLASH5
  mt9v_write(0xB0, 0xABE0);   //X2_SLASH5
  mt9v_write(0xB1, 0x0002);   //X3_SLASH5
  mt9v_write(0xB2, 0x0010);   //X4_SLASH5
  mt9v_write(0xB3, 0x0010);   //X5_SLASH5
  mt9v_write(0xB4, 0x0000);   //Y0_SLASH5
  mt9v_write(0xB5, 0x0000);   //Y1_SLASH5
  mt9v_write(0xB6, 0x0000);   //Y2_SLASH5
  mt9v_write(0xB7, 0x0000);   //Y3_SLASH5
  mt9v_write(0xBF, 0x0016);   //Y4_SLASH5
  mt9v_write(0xC0, 0x000A);   //Y5_SLASH5
  mt9v_write(0xC2, 0x18D0);   //DESIRED_BIN
  mt9v_write(0xC3, 0x007F);   //EXP_SKIP_FRM_H
  mt9v_write(0xC4, 0x007F);   //EXP_LPF
  mt9v_write(0xC5, 0x007F);   //GAIN_SKIP_FRM
  mt9v_write(0xC6, 0x0000);   //GAIN_LPF_H
  mt9v_write(0xC7, 0x4416);   //MAX_GAIN
  mt9v_write(0xC8, 0x4421);   //MIN_COARSE_EXPOSURE
  mt9v_write(0xC9, 0x0001);   //MAX_COARSE_EXPOSURE
  mt9v_write(0xCA, 0x0004);   //BIN_DIFF_THRESHOLD
  mt9v_write(0xCB, 0x01E0);   //AUTO_BLOCK_CONTROL
  mt9v_write(0xCC, 0x02F0);   //PIXEL_COUNT
  mt9v_write(0xCD, 0x005E);   //LVDS_MASTER_CONTROL
  mt9v_write(0xCE, 0x002D);   //LVDS_SHFT_CLK_CONTROL
  mt9v_write(0xCF, 0x01DE);   //LVDS_DATA_CONTROL
  mt9v_write(0xD0, 0x01DF);   //LVDS_DATA_STREAM_LATENCY
  mt9v_write(0xD1, 0x0164);   //LVDS_INTERNAL_SYNC
  mt9v_write(0xD2, 0x0001);   //LVDS_USE_10BIT_PIXELS
  mt9v_write(0xD3, 0x0000);   //STEREO_ERROR_CONTROL
  mt9v_write(0xD4, 0x0000);   //INTERLACE_FIELD_VBLANK
  mt9v_write(0xD5, 0x0104);   //IMAGE_CAPTURE_NUM
  mt9v_write(0xD6, 0x0000);   //ANALOG_CONTROLS
  mt9v_write(0xD7, 0x0000);   //AB_PULSE_WIDTH_REG
  mt9v_write(0xD8, 0x0000);   //TX_PULLUP_PULSE_WIDTH_REG
  mt9v_write(0xD9, 0x0000);   //RST_PULLUP_PULSE_WIDTH_REG
  mt9v_write(0xF0, 0x0000);   //NTSC_FV_CONTROL
  mt9v_write(0xFE, 0xBEEF);   //NTSC_HBLANK
}    
/**
*  MT9V034分辨率、帧率修改
*  ----------------
*  752 * 480   max60fps
*  376 * 240   120fps      112fps
*  188 * 120   max240fps   避免消隐区最大193fps 
*  行和列都可以四分频  当行分频时，输出帧率会倍频
*  垂直翻转、水平翻转。
* 
*/
static void mt9v_frame(uint16_t height,uint16_t width, uint8_t fps)
{
  uint16_t data = 0;
  uint16_t frameRate = 0;
  if((height*4)<=MAX_IMAGE_HEIGHT )   //判断行是否4分频
  {
    height *= 4;
    data |= MT9V034_READ_MODE_ROW_BIN_4;
    if(fps > 193)
      fps = 193;    //最大帧率 193
    if(fps < 1)
      fps = 1;        //最小帧率  1
    
    if(fps > 132)  //fps 过高，不适合一起算
    {
      frameRate = (uint16_t)(-2.0 * fps + 504);  //估算的，不精确
    }
    else
    {
      frameRate = (uint16_t)(132.0 / fps * 240);
    }
  }
  else if((height*2)<=MAX_IMAGE_HEIGHT )  //判断行是否2分频
  {
    height *= 2;
    data |= MT9V034_READ_MODE_ROW_BIN_2;
    
    if(fps > 112)
      fps = 112;    //最大帧率 112
    if(fps < 1)
      fps = 1;        //最小帧率  1
    if(fps > 66)  //fps 过高，不适合一起算
    {
      frameRate = (uint16_t)(-5.2 * fps + 822);
    }
    else
    {
      frameRate = (uint16_t)(66.0 / fps * 480);
    }
  }
  else 
  {
    if(fps > 60) 
      fps = 60;    //最大帧率 60
    if(fps < 1) 
      fps = 1;        //最小帧率  1
    frameRate = (uint16_t)(60.0 / fps * 480);
  }
  if((width*4)<=MAX_IMAGE_WIDTH )                                             //判断列是否4分频   
  {
    width *= 4;
    data |= MT9V034_READ_MODE_COL_BIN_4;
  }
  else if((width*2)<=MAX_IMAGE_WIDTH )                                        //判断列是否2分频
  {
    width *= 2;
    data |= MT9V034_READ_MODE_COL_BIN_2;
  }
  //         水平翻转                     垂直翻转
  data |= (MT9V034_READ_MODE_ROW_FLIP|MT9V034_READ_MODE_COLUMN_FLIP);         //需要翻转的可以打开注释，或者后面PXP转换时翻转也可以  
  
  mt9v_write(MT9V034_READ_MODE, data);                                 //写寄存器，配置行分频
  mt9v_write(MT9V034_WINDOW_WIDTH,  width);                            //读取图像的列数  改变此处也可改变图像输出大小，不过会丢失视角
  mt9v_write(MT9V034_WINDOW_HEIGHT, height);                           //读取图像的行数  改变此处也可改变图像输出大小，不过会丢失视角
  mt9v_write(MT9V034_COLUMN_START, MT9V034_COLUMN_START_MIN);          //列开始
  mt9v_write(MT9V034_ROW_START, MT9V034_ROW_START_MIN);                //行开始
  mt9v_write(MT9V034_TOTAL_SHUTTER_WIDTH,frameRate);                   //0x0B 曝光时间 越长帧率越小
}

status_t mt9v_init(camera_device_handle_t *handle, const camera_config_t *config)
{     
  uint16_t data = 0; 
  mt9v_read(MT9V034_CHIP_VERSION, &data);   //读取ID
  if(data != MT9V034_CHIP_ID)                      //ID不正确
  {
    printf("mt9v034 error\n");    
  }
  
  mt9v_default_settings();                                                    //最小曝光时间配置  注意神眼摄像头的寄存器掉电不会丢失，修改前最好记得备份当前配置
  /*改变摄像头的输出图像大小，修改后CSI  和 PXP 都需要自行修改*/
  mt9v_frame(IMAGEH, IMAGEW, config->framePerSec);                            //设置摄像头图像分频输出,BIT4,5镜像设置:上下左右镜像   
  
  mt9v_write(0x2C, 0x0004);                                            //参考电压设置   1.4v 
  mt9v_write(MT9V034_ANALOG_CTRL, MT9V034_ANTI_ECLIPSE_ENABLE);        //反向腐蚀
  mt9v_write(MT9V034_MAX_GAIN_REG, 10);                                //0xAB  最大模拟增益     64
  mt9v_write(MT9V034_AGC_AEC_PIXEL_COUNT_REG, 0xB0);                   //0xB0  用于AEC/AGC直方图像素数目,最大44000   IMAGEH*IMAGEW  
  mt9v_write(MT9V034_ADC_RES_CTRL_REG, 0x0303);                        //0x1C  here is the way to regulate darkness :)    
  mt9v_write(0x13,0x2D2E);                                             //We also recommended using R0x13 = 0x2D2E with this setting for better column FPN.  
  mt9v_write(0x20,0x03c7);                                             //0x01C7对比度差，发白；0x03C7对比度提高 Recommended by design to improve performance in HDR mode and when frame rate is low.
  mt9v_write(0x24,0x0010);                                             //Corrects pixel negative dark offset when global reset in R0x20[9] is enabled.
  mt9v_write(0x2B,0x0003);                                             //Improves column FPN.
  mt9v_write(0x2F,0x0003);                                             //Improves FPN at near-saturation.  
  mt9v_write(MT9V034_SHUTTER_WIDTH_CONTROL,0x0164);                    //0x0A Coarse Shutter IMAGEW Control 
  mt9v_write(MT9V034_V2_CTRL_REG_A, 0x001A);                           //0x32   0x001A  
  mt9v_write(MT9V034_HDR_ENABLE_REG,0x0103);                           //0x0F High Dynamic Range enable,bit is set (R0x0F[1]=1), the sensor uses black level correction values from one green plane, which are applied to all colors.
  mt9v_write(MT9V034_AGC_AEC_DESIRED_BIN_REG, 40);                     //0xA5  图像亮度  60  1-64
  mt9v_write(MT9V034_ANALOG_GAIN,0x8010);
  mt9v_write(MT9V034_RESET, 0x03);                                     //0x0c  复位 
  
  /*    自动曝光配置  移植K60的程序
  MT9V034_SetFrameResolution(IMAGEH, IMAGEW, config->framePerSec);//设置摄像头图像4*4分频输出PCLK, 27/4 = 6.75M ,BIT4,5镜像设置:上下左右均镜像   
  MT9V034_SetAutoExposure(1);  
  SCCB_RegWrite(MT9V034_I2C_ADDR, 0xAC, 0x0001);
  SCCB_RegWrite(MT9V034_I2C_ADDR, 0xAD, 0x01E0);  
  SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2C, 0x0004);
  
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x7F, 0x3000);           // test pattern
  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ANALOG_CTRL, MT9V034_ANTI_ECLIPSE_ENABLE);
  SCCB_RegWrite(MT9V034_I2C_ADDR, 0x0F, 0x0000);  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_HDR_ENABLE_REG, 0x0101); // 0x0F bit8:1HDR,0linear; bit1:1彩色,0灰度;bit0:1HDR,0linear
  //MT9V034_WriteReg16(MT9V034_HDR_ENABLE_REG, 0x0103);     // 0x0F bit8:1HDR,0linear; bit1:1彩色,0灰度;bit0:1HDR,0linear
  //0x07 Chip Control bit2-0:0逐行扫描,1无效，2场，3单场；bit5:1立体视觉使能,bit7:1并口使能；bit8:1同步模式;bit9:1正常操作；bit15:0A/1B
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_CHIP_CONTROL, 0x0188);          //Context A  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ROW_NOISE_CORR_CTRL_REG, 0);   //0x70  0x0000 
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE_REG, 0x0303);   //0xAF  AEC/AGC A~bit0:1AE;bit1:1AG/B~bit2:1AE;bit3:1AG
  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MIN_EXPOSURE_REG, 0x0001);     //0xAC  最小粗快门宽度   0x0001
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_EXPOSURE_REG, 0x01E0);     //0xAD  最大醋快门宽度   0x01E0-480
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 50);             //0xAB  最大模拟增益     64
  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AGC_AEC_PIXEL_COUNT_REG, 188*120);//0xB0  用于AEC/AGC直方图像素数目,最大44000   4096=320*240  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ADC_RES_CTRL_REG, 0x0303);     //0x1C  here is the way to regulate darkness :)    
  
  SCCB_RegWrite(MT9V034_I2C_ADDR,0x13,0x2D2E);//We also recommended using R0x13 = 0x2D2E with this setting for better column FPN.  
  SCCB_RegWrite(MT9V034_I2C_ADDR,0x20,0x03C7);//0x01C7对比度差，发白；0x03C7对比度提高 Recommended by design to improve performance in HDR mode and when frame rate is low.
  SCCB_RegWrite(MT9V034_I2C_ADDR,0x24,0x0010);//Corrects pixel negative dark offset when global reset in R0x20[9] is enabled.
  SCCB_RegWrite(MT9V034_I2C_ADDR,0x2B,0x0003);//Improves column FPN.
  SCCB_RegWrite(MT9V034_I2C_ADDR,0x2F,0x0003);//Improves FPN at near-saturation.  
  
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x08, 0x03D4);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x09, 0x03E7);
  //100DB //设置曝光时间
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_SHUTTER_WIDTH1,0x01BB);        //0x08 Coarse Shutter IMAGEW 1
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_SHUTTER_WIDTH2,0x01D9);        //0x09 Coarse Shutter IMAGEW 2
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_SHUTTER_WIDTH_CONTROL,0x0064); //0x0A Coarse Shutter IMAGEW Control 
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_V2_CTRL_REG_A, 0x001A);        //0x32   0x001A  
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_TOTAL_SHUTTER_WIDTH,0x0320);   //0x0B Coarse Shutter IMAGEW Total
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_HDR_ENABLE_REG,0x0103);        //0x0F High Dynamic Range enable,bit is set (R0x0F[1]=1), the sensor uses black level correction values from one green plane, which are applied to all colors.
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AGC_AEC_DESIRED_BIN_REG, 60); //0xA5  图像亮度  50  1-64
  SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_ANALOG_GAIN,0x8010);           //0x35
  
  //80dB HDR
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x08, 0x03CA);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x09, 0x03DE);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x0A, 0x0064);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x0B, 0x03E8);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x0F, 0x0103);
  //SCCB_RegWrite(MT9V034_I2C_ADDR, 0x35, 0x8010);   
  
  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_RESET, 0x03);          //0x0c  复位 
  //    delayms(1000);
  */
  return kStatus_Success;
}

/* 结构体中保存了函数的指针 */
const camera_device_operations_t mt9v_ops = {
  .init    = mt9v_init,   
  .deinit  = mt9v_deinit,
  .start   = mt9v_start,
  .stop    = mt9v_stop,
  .control = mt9v_control,
  .init_ext= mt9v_initext,
};

