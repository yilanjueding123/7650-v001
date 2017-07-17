
static INT8U csi_register_init[][2] =
{
    {0x12, 0x80},	// Reset all registers to default values
    {0x2d, 0x40},	// dummy byte
    {0x2e, 0x00},

#if 1	// pclk
    {0x6b, 0x0a},

    {0x11, 0x00},	// 15 FPS
    //	{0x11, 0x01},	// 7.5 FPS
    //	{0x11, 0x02},	// 5 FPS
    //	{0x11, 0x03},	// 3.75 FPS
    //	{0x11, 0x04},	// 3 FPS
    //	{0x11, 0x05},	// 2.5 FPS
    //	{0x11, 0x06},	// 2.14 FPS
    //	{0x11, 0x07},	// 1.87 FPS
    //	{0x11, 0x08},	// 1.67 FPS
    //	{0x11, 0x09},	// 1.5 FPS
    //	{0x11, 0x0E},	// 1 FPS
    //	{0x11, 0x13},	// 0.75 FPS
    //	{0x11, 0x17},	// 0.62 FPS
    //	{0x11, 0x1D},	// 0.5 FPS
#elif 1		// pclk*4
    {0x6b, 0x4a},

    {0x11, 0x01},	// 30.35 FPS
    //	{0x11, 0x02},	// 22.50 FPS
    //	{0x11, 0x03},	// 15 FPS
    //	{0x11, 0x04},	// 12 FPS
    //	{0x11, 0x05},	// 10 FPS
    //	{0x11, 0x06},	// 8.57 FPS
    //	{0x11, 0x07},	// 7.5 FPS
    //	{0x11, 0x08},	// 6.67 FPS
    //	{0x11, 0x09},	// 6 FPS
    //	{0x11, 0x0E},	// 4 FPS
    //	{0x11, 0x13},	// 3 FPS
    //	{0x11, 0x17},	// 2.5 FPS
    //	{0x11, 0x1D},	// 2 FPS
#elif 1	// pclk*6
    {0x6b, 0x8a},

    {0x11, 0x02},	// 33.76 FPS
    //	{0x11, 0x03},	// 23.58 FPS
    //	{0x11, 0x04},	// 20.25 FPS
    //	{0x11, 0x05},	// 15 FPS
    //	{0x11, 0x06},	// 12.85 FPS
    //	{0x11, 0x07},	// 11.25 FPS
    //	{0x11, 0x08},	// 10 FPS
    //	{0x11, 0x09},	// 9 FPS
    //	{0x11, 0x0E},	// 6 FPS
    //	{0x11, 0x13},	// 4.5 FPS
    //	{0x11, 0x17},	// 3.75 FPS
    //	{0x11, 0x1D},	// 3 FPS
#elif 1		// pclk*8
    {0x6b, 0xca},

    //	{0x11, 0x03},	// 36.58 FPS
    //	{0x11, 0x04},	// 25.33 FPS
    //	{0x11, 0x05},	// 22.5 FPS
    //	{0x11, 0x06},	// 17.14 FPS
    //	{0x11, 0x07},	// 15 FPS
    //	{0x11, 0x08},	// 13.33 FPS
    //	{0x11, 0x09},	// 12.10 FPS
    //	{0x11, 0x0E},	// 8 FPS
    //	{0x11, 0x13},	// 6 FPS
    //	{0x11, 0x17},	// 5 FPS
    //	{0x11, 0x1D},	// 4 FPS
#endif
    {0x09, 0x01},		// TBD: save power here
    {0x3a, 0x04},		// bit[4]: Force U and V output value for testing
    {0x67, 0x77},		// Force U value when testing
    {0x68, 0x88},		// Force V value when testing
    {0x12, 0x00},
    {0x17, 0x13},
    {0x18, 0x01},
    {0x32, 0xb6},
    {0x19, 0x02},
    {0x1a, 0x7a},
    {0x03, 0x0a},
    {0x0c, 0x00},
    {0x3e, 0x00},
    {0x70, 0x3a},
    {0x71, 0x35},
    {0x72, 0x11},
    {0x73, 0xf0},
    {0xa2, 0x02},

    {0x7a, 0x24},
    {0x7b, 0x04},
    {0x7c, 0x0a},
    {0x7d, 0x17},
    {0x7e, 0x32},
    {0x7f, 0x3f},
    {0x80, 0x4c},
    {0x81, 0x58},
    {0x82, 0x64},
    {0x83, 0x6f},
    {0x84, 0x7a},
    {0x85, 0x8c},
    {0x86, 0x9e},
    {0x87, 0xbb},
    {0x88, 0xd2},
    {0x89, 0xe5},

    {0x13, 0xe0},
    {0x00, 0x00},
    {0x10, 0x00},
    {0x0d, 0x40},
    {0x14, 0x18},
    {0xa5, 0x02},
    {0xab, 0x03},
    {0x24, 0x95},
    {0x25, 0x33},
    {0x26, 0xe3},
    {0x9f, 0x78},
    {0xa0, 0x68},
    {0xa1, 0x03},
    {0xa6, 0xd8},
    {0xa7, 0xd8},
    {0xa8, 0xf0},
    {0xa9, 0x90},
    {0xaa, 0x94},
    {0x13, 0xe5},

    {0x0e, 0x61},
    {0x0f, 0x4b},
    {0x16, 0x02},
    {0x1e, 0x3f}, 	// Flip (bit4) & Mirror (bit5)
    {0x21, 0x02},
    {0x22, 0x91},
    {0x29, 0x07},
    {0x33, 0x0b},
    {0x35, 0x0b},
    {0x37, 0x1d},
    {0x38, 0x71},
    {0x39, 0x2a},
    {0x3c, 0x78},
    {0x4d, 0x40},
    {0x4e, 0x20},
    {0x69, 0x00},

    {0x74, 0x10},
    {0x8d, 0x4f},
    {0x8e, 0x00},
    {0x8f, 0x00},
    {0x90, 0x00},
    {0x91, 0x00},

    {0x96, 0x00},
    {0x9a, 0x80},
    {0xb0, 0x84},
    {0xb1, 0x0c},
    {0xb2, 0x0e},
    {0xb3, 0x82},
    {0xb8, 0x0a},

    {0x43, 0x0a},
    {0x44, 0xf0},
    {0x45, 0x44},
    {0x46, 0x7a},
    {0x47, 0x27},
    {0x48, 0x3c},
    {0x59, 0xbc},
    {0x5a, 0xde},
    {0x5b, 0x54},
    {0x5c, 0x8a},
    {0x5d, 0x4b},
    {0x5e, 0x0f},
    {0x6c, 0x0a},
    {0x6d, 0x55},
    {0x6e, 0x11},
    {0x6f, 0x9e},

    {0x6a, 0x40},
    {0x01, 0x40},
    {0x02, 0x40},
    {0x13, 0xe7},

    {0x4f, 0x80},
    {0x50, 0x80},
    {0x51, 0x00},
    {0x52, 0x22},
    {0x53, 0x5e},
    {0x54, 0x80},
    {0x58, 0x9e},

    {0x62, 0x08},
    {0x63, 0x8f},
    {0x65, 0x00},
    {0x64, 0x08},
    {0x94, 0x08},
    {0x95, 0x0c},
    {0x66, 0x05},

    {0x41, 0x08},
    {0x3f, 0x00},
    {0x75, 0x05},
    {0x76, 0xe1},
    {0x4c, 0x00},
    {0x77, 0x01},
    {0x3d, 0xc2},
    {0x4b, 0x09},
    {0xc9, 0x60},
    {0x41, 0x38},
    {0x56, 0x40},

    {0x34, 0x11},
    {0x3b, 0xca},	// enable night mode
    //	{0x3b, 0x4a},	// disable night mode

    {0xa4, 0x88},
    {0x96, 0x00},
    {0x97, 0x30},
    {0x98, 0x20},
    {0x99, 0x30},
    {0x9a, 0x84},
    {0x9b, 0x29},
    {0x9c, 0x03},
    {0x9d, 0x98},
    {0x9e, 0x7f},
    {0x78, 0x04},

    {0x79, 0x01},
    {0xc8, 0xf0},
    {0x79, 0x0f},
    {0xc8, 0x00},
    {0x79, 0x10},
    {0xc8, 0x7e},
    {0x79, 0x0a},
    {0xc8, 0x80},
    {0x79, 0x0b},
    {0xc8, 0x01},
    {0x79, 0x0c},
    {0xc8, 0x0f},
    {0x79, 0x0d},
    {0xc8, 0x20},
    {0x79, 0x09},
    {0xc8, 0x80},
    {0x79, 0x02},
    {0xc8, 0xc0},
    {0x79, 0x03},
    {0xc8, 0x40},
    {0x79, 0x05},
    {0xc8, 0x30},
    {0x79, 0x26},

    {0x3b, 0x00},	// 60Hz
    //	{0x3b, 0x08}	// 50Hz
};