#include "main.h"

uint16_t RunA_NumS=60;
uint16_t RunA_NumM;
uint16_t RunA_NumL=10;
uint16_t RunA_NumX;

uint16_t RunA_i=0;
uint16_t RunA_j;
uint16_t RunA_k;
uint16_t RunA_m=0;
uint16_t RunA_n=0;

uint16_t RunB_NumS=60;
uint16_t RunB_NumM;
uint16_t RunB_NumL=10;
uint16_t RunB_NumX;

uint16_t RunB_i=0;
uint16_t RunB_j;
uint16_t RunB_k;
uint16_t RunB_m=0;
uint16_t RunB_n=0;

uint16_t RunWaveA_Count=66;
uint16_t RunWaveB_Count=66;

uint16_t Wave_ChangeA;
uint16_t Wave_ChangeB;

extern uint8_t Wave_Select;

extern uint8_t Ch_Flag;////通道标志位0:A通道，1：B通道

extern uint8_t Pwr1;/////0-60档强度
extern uint8_t Pwr2;
/*******************************************************************************/
/*
*从A-I分别为：敲击、按揉、颈椎、肩部、上臂、腰部、关节、足底
*
*/
const uint16_t WaveModeA_Count[15][3] = {
{12,40,20},
{12,40,20},
{12,40,40},
{12,80,20},
{12,40,80},
{24,40,80},
{24,40,80},
{24,40,80},
{24,40,80},
{24,40,80},
{24,40,80},
{12,40,20},
{12,40,20},
{12,40,40},
{24,40,80}
};
const uint16_t WaveModeA_Hz[15][3]={
{1000,200,400},
{1000,200,400},
{1000,200,400},
{1000,200,400},
{1000,200,400},
{1000,200,400},
{1000,200,400},
{1000,200,400},
{1000,200,400},
{1000,200,400},
{1000,200,400},
{1000,200,400},
{1000,200,400},
{1000,200,400},
{1000,200,400}
};
const uint16_t WaveModeA_Cycle[15][2]={
{0,0},{2,0},{1,1},{2,0},
{1,2},{2,0},{1,3},{2,0},
{4,7},{1,0},{1,1},{2,0},
{1,2},{2,0},{2,0}
};

const uint16_t WaveModeA_Step[15][3]={
{2,2,2},
{2,2,2},
{2,2,2},
{2,2,2},
{2,2,2},
{2,2,2},
{2,2,2},
{2,2,2},
{2,2,2},
{2,2,2},
{2,2,2},
{2,2,2},
{2,2,2},
{2,2,2},
{2,2,2},
};
/*******************************************************************************/
const uint16_t WaveModeB_Count[15][3]={
{24,12,20},
{24,12,20},
{24,12,40},
{24,24,20},
{48,12,20},
{24,12,20},
{24,12,20},
{24,12,40},
{24,24,20},
{48,12,20},
{24,12,20},
{24,12,20},
{24,12,40},
{24,24,20},
{48,12,20},
};
const uint16_t WaveModeB_Hz[15][3]={
{250,500,400},
{250,500,400},
{250,500,400},
{250,500,400},
{250,500,400},
{250,500,400},
{250,500,400},
{250,500,400},
{250,500,400},
{250,500,400},
{250,500,400},
{250,500,400},
{250,500,400},
{250,500,400},
{250,500,400}
};
const uint16_t WaveModeB_Cycle[15][2]={
{2,0},{1,1},{3,0},{1,2},
{3,0},{1,3},{3,0},{4,4},
{2,0},{1,1},{3,0},{1,2},
{3,0},{1,3},{3,0}
};

const uint16_t WaveModeB_Step[15][3]={
{1,1,1},
{1,1,1},
{1,1,1},
{1,1,1},
{1,1,1},
{1,1,1},
{1,1,1},
{1,1,1},
{1,1,1},
{1,1,1},
{1,1,1},
{1,1,1},
{1,1,1},
{1,1,1},
{1,1,1},
};
/*******************************************************************************/
const uint16_t WaveModeC_Count[15][3]={
{60,155,190},
{13,2,15},
{2,3,4},
{15,30,95},
{142,663,45},
{2,4,6},
{2,190,30},
{288,60,96},
{190,60,154},
{133,48,2},
{3,4,2},
{95,15,30},
{4,6,2},
{96,284,60},
{1,1,1}
};

const uint16_t WaveModeC_Hz[15][3]={
{66,26,20},
{15,1000,40},
{2000,666,1000},
{133,66,20},
{14,20,133},
{2000,1000,666},
{1000,20,133},
{14,66,40},
{20,66,26},
{15,40,1000},
{666,1000,2000},
{200,133,66},
{1000,666,2000},
{40,14,66},
{1,1,1}
};

const uint16_t WaveModeC_Cycle[15][2]={
{10,0},{20,1},{12,2},{30,3},
{20,4},{6,5},{15,6},{10,7},
{10,8},{20,9},{112,10},{20,11},
{10,12},{10,13},{1,1}
};

const uint16_t WaveModeC_Step[15][3]={
{4,4,4},
{2,2,2},
{2,2,2},
{4,4,4},
{4,4,4},
{9,2,2},
{9,2,7},
{4,4,4},
{4,4,4},
{2,2,2},
{2,2,2},
{4,4,4},
{2,2,9},
{4,4,4},
{1,1,1}
};

/*******************************************************************************/
const uint16_t WaveModeD_Count[15][3]={
{96,140,190},
{3,2,77},
{190,4,6},
{144,30,15},

{15,48,308},
{96,6,2},
{95,30,6},
{190,96,132},

{77,3,2},
{190,4,6},
{144,30,15},
{15,48,308},

{96,6,2},
{95,30,6},
{1,1,1}
};

const uint16_t WaveModeD_Hz[15][3]={
{41,28,200},
{666,1000,26},
{100,1000,666},

{14,66,133},
{133,40,26},
{40,666,2000},
{200,66,666},
{200,40,30},

{30,666,1000},
{200,1000,666},
{14,66,133},
{133,40,26},

{40,666,2000},
{200,133,666},
{1,1,1}
};

const uint16_t WaveModeD_Cycle[15][2]={
{10,0},{10,1},{10,2},{30,3},
{30,4},{6,5},{15,6},{10,7},
{10,8},{20,9},{10,10},{20,11},
{10,12},{10,13},{1,1}
};

const uint16_t WaveModeD_Step[15][3]={
{1,1,1},
{2,2,9},
{9,2,2},
{4,4,4},
{4,4,4},
{2,2,2},
{4,4,2},
{1,1,1},
{9,2,2},
{9,2,2},
{2,2,2},
{2,2,2},
{2,2,2},
{4,4,4},
{1,1,1}
};
/*******************************************************************************/
const uint16_t WaveModeE_Count[15][3]={
{475,150,50},
{48,1,1},
{60,60,47},
{150,240,50},

{190,60,235},
{30,48,10},
{1,1,2},
{10,150,475},

{50,240,120},
{475,150,10},
{48,1,1},
{60,60,47},

{30,48,10},
{10,150,475},
{1,1,1}
};

const uint16_t WaveModeE_Hz[15][3]={
{200,66,200},
{40,2000,2000},
{4000,4000,200},

{66,40,200},
{200,66,40},
{200,2000,2000},
{4000,4000,666},

{200,40,666},
{30,666,1000},
{200,1000,666},
{66,40,200},
{200,66,40},

{200,2000,2000},
{4000,4000,666},
{1,1,1}
};

const uint16_t WaveModeE_Cycle[15][2]={
{6,0},{20,1},{30,2},{10,3},
{15,4},{6,5},{15,6},{20,7},
{6,8},{15,9},{20,10},{6,11},
{10,12},{15,13},{1,1}
};

const uint16_t WaveModeE_Step[15][3]={
{5,7,7},
{5,2,5},
{5,2,7},
{5,7,5},
{7,5,5},
{7,5,7},
{5,7,7},
{5,2,5},
{7,2,5},
{5,7,5},
{7,5,5},
{7,5,7},
{5,7,7},
{5,2,5},
{1,1,1}
};

/*******************************************************************************/
const uint16_t WaveModeF_Count[15][3]={
{190,140,96},
{190,48,77},
{2,4,6},
{144,30,15},

{2,48,3},
{95,154,30},
{77,95,30},
{2,48,77},

{144,30,15},
{3,2,48},
{30,95,154},
{30,77,95},

{30,95,154},
{30,77,95},
{1,1,1}
};

const uint16_t WaveModeF_Hz[15][3]={
{200,26,400},
{200,40,26},
{1000,1000,666},
{14,66,133},

{2000,40,666},
{200,26,133},
{26,200,33},
{26,40,200},

{1000,40,26},
{666,1000,1000},
{666,1000,1000},
{14,33,133},

{26,200,33},
{26,40,200},
{1,1,1}
};

const uint16_t WaveModeF_Cycle[15][2]={
{10,0},{20,1},{15,2},{30,3},
{10,4},{20,5},{15,6},{30,7},
{10,8},{20,9},{15,10},{30,11},
{10,12},{15,13},{1,1}
};

const uint16_t WaveModeF_Step[15][3]={
{4,4,4},
{4,2,2},
{2,2,2},
{4,8,9},
{9,2,2},
{4,2,2},
{5,2,7},
{4,3,4},
{2,2,2},
{2,2,9},
{3,4,4},
{2,9,2},
{4,7,4},
{2,9,2},
{1,1,1}
};
/*******************************************************************************/
const uint16_t WaveModeG_Count[15][3]={
{1776,100,440},
{196,60,20},
{3,4,1},
{2,4,6},

{2,6,4},
{30,60,196},
{196,30,15},
{2220,546,656},

{1,4,4},
{1776,100,440},
{10,196,60},
{1,3,4},

{4,6,2},
{30,60,200},
{1,1,1}
};

const uint16_t WaveModeG_Hz[15][3]={
{14,100,14},
{200,66,200},
{666,1000,2000},

{666,1000,2000},
{2000,1000,666},
{2000,666,1000},
{133,33,200},
{200,66,133},

{14,26,200},
{2000,1000,1000},
{14,100,14},
{200,100,66},

{66,1000,2000},
{133,33,20},
{1,1,1}
};

const uint16_t WaveModeG_Cycle[15][2]={
{3,0},{10,1},{15,2},{10,3},
{10,4},{10,5},{12,6},{20,7},
{4,8},{12,9},{3,10},{10,11},
{15,12},{10,13},{1,1}
};

const uint16_t WaveModeG_Step[15][3]={
{7,7,7},
{2,2,2},
{8,8,7},
{2,2,2},
{8,7,4},
{7,7,7},
{7,7,7},
{2,2,2},
{8,8,4},
{7,7,7},
{2,2,2},
{7,8,8},
{2,2,2},
{7,4,8},
{1,1,1}
};
/*******************************************************************************/
const uint16_t WaveModeH_Count[15][3]={
{285,140,15},
{77,3,2},
{284,4,6},
{95,30,15},

{45,96,624},
{96,6,4},
{154,30,95},
{6,95,30},

{15,288,140},
{77,3,2},
{284,4,6},
{15,95,30},

{616,45,96},
{6,95,30},
{1,1,1}
};

const uint16_t WaveModeH_Hz[15][3]={
{200,28,133},
{28,666,1000},
{14,1000,666},
{200,66,133},

{133,40,26},
{40,666,1000},
{26,666,1000},
{26,133,200},

{666,200,66},
{133,200,28},
{26,666,1000},
{14,1000,666},

{133,200,66},
{26,133,40},
{1,1,1}
};

const uint16_t WaveModeH_Cycle[15][2]={
{10,0},{20,1},{10,2},{30,3},
{20,4},{5,5},{10,6},{12,7},
{15,8},{10,9},{20,10},{10,11},
{20,12},{4,13},{1,1}
};

const uint16_t WaveModeH_Step[15][3]={
{1,1,1},
{1,1,1},
{1,2,2},
{1,2,2},
{1,1,1},
{1,2,2},
{9,9,4},
{2,4,4},
{1,1,1},
{1,1,1},
{1,2,2},
{2,1,2},
{1,1,1},
{1,2,2},
{1,1,1}
};
/*******************************************************************************/
const uint16_t WaveModeI_Count[15][3]={
{15,120,40},
{40,15,10},
{15,40,64},
{15,120,40},

{15,120,40},
{40,15,10},
{15,120,40},
{40,15,10},

{15,40,64},
{15,120,40},
{40,15,10},
{15,40,64},

{15,120,40},
{40,15,10},
{1,1,1}
};

const uint16_t WaveModeI_Hz[15][3]={
{400,200,250},
{500,666,1000},
{666,500,250},

{400,200,250},
{500,666,1000},
{666,500,250},

{400,200,250},
{500,666,1000},
{666,500,250},

{400,200,250},
{500,666,1000},
{666,500,250},

{400,200,250},
{500,666,1000},
{1,1,1}
};

const uint16_t WaveModeI_Cycle[15][2]={
{18,0},{18,1},{18,2},{18,3},
{18,4},{18,5},{18,6},{18,7},
{18,8},{18,9},{18,10},{18,11},
{18,12},{18,13},{1,1}
};

const uint16_t WaveModeI_Step[15][3]={
{4,4,4},
{4,4,4},
{4,4,4},
{4,4,4},
{4,4,4},
{4,4,4},
{4,4,4},
{4,4,4},
{4,4,4},
{4,4,4},
{4,4,4},
{4,4,4},
{4,4,4},
{4,4,4},
{1,1,1}
};
/*******************************************************************************/
extern uint8_t Wave_SelectA;
extern uint8_t Wave_SelectB;

extern uint8_t Mode1;
extern uint8_t Mode2;
/*******************************************************************************/
/*通道A预装载数组*/
uint16_t 	RunModeA_Count[15][3];
uint16_t	RunModeA_Cycle[15][2];
uint16_t	RunModeA_Step[15][3];
uint16_t	RunModeA_Hz[15][3];
/*通道B预装载数组*/
uint16_t 	RunModeB_Count[15][3];
uint16_t	RunModeB_Cycle[15][2];
uint16_t	RunModeB_Step[15][3];
uint16_t	RunModeB_Hz[15][3];

//void Mode_Select(void)
//{	
//					switch(Mode1)
//					{
//						case 2:/*敲击*/
//								
//								memcpy(RunModeA_Count,WaveModeA_Count, sizeof(WaveModeA_Count));
//								memcpy(RunModeA_Hz,WaveModeA_Hz, sizeof(WaveModeA_Hz));
//								memcpy(RunModeA_Cycle,WaveModeA_Cycle,sizeof(WaveModeA_Cycle));
//								memcpy(RunModeA_Step,WaveModeA_Step,sizeof(WaveModeA_Step));

//								RunA_NumL = RunModeA_Cycle[0][0];/*每组循环次数*/
//								RunA_m = RunModeA_Cycle[0][1];/*下一组参数*/
//								RunA_NumS = RunModeA_Count[0][0];/*组内每个调制波运行次数*/
//								Wave_SelectA = RunModeA_Step[0][0];/*调制波种类切换*/
//								RunWaveA_Count = RunModeA_Hz[0][0]*2;/*调制波频率切换*/
//								
//								
//							break;
//						case 0:/*按压*/
//								
//								memcpy(RunModeA_Count,WaveModeB_Count, sizeof(WaveModeB_Count));
//								memcpy(RunModeA_Hz,WaveModeB_Hz, sizeof(WaveModeB_Hz));
//								memcpy(RunModeA_Cycle,WaveModeB_Cycle,sizeof(WaveModeB_Cycle));
//								memcpy(RunModeA_Step,WaveModeB_Step,sizeof(WaveModeB_Step));

//								RunA_NumL = RunModeA_Cycle[0][0];/*每组循环次数*/
//								RunA_m = RunModeA_Cycle[0][1];/*下一组参数*/
//								RunA_NumS = RunModeA_Count[0][0];/*组内每个调制波运行次数*/
//								Wave_SelectA = RunModeA_Step[0][0];/*调制波种类切换*/
//								RunWaveA_Count = RunModeA_Hz[0][0]*2;/*调制波频率切换*/
//							break;
//						case 1:/*揉捏*/
//								
//								memcpy(RunModeA_Count,WaveModeC_Count, sizeof(WaveModeC_Count));
//								memcpy(RunModeA_Hz,WaveModeC_Hz, sizeof(WaveModeC_Hz));
//								memcpy(RunModeA_Cycle,WaveModeC_Cycle,sizeof(WaveModeC_Cycle));
//								memcpy(RunModeA_Step,WaveModeC_Step,sizeof(WaveModeC_Step));

//								RunA_NumL = RunModeA_Cycle[0][0];/*每组循环次数*/
//								RunA_m = RunModeA_Cycle[0][1];/*下一组参数*/
//								RunA_NumS = RunModeA_Count[0][0];/*组内每个调制波运行次数*/
//								Wave_SelectA = RunModeA_Step[0][0];/*调制波种类切换*/
//								RunWaveA_Count = RunModeA_Hz[0][0]*2;/*调制波频率切换*/
//							break;
//						case 6:/*颈椎*/
//								
//								memcpy(RunModeA_Count,WaveModeD_Count, sizeof(WaveModeD_Count));
//								memcpy(RunModeA_Hz,WaveModeD_Hz, sizeof(WaveModeD_Hz));
//								memcpy(RunModeA_Cycle,WaveModeD_Cycle,sizeof(WaveModeD_Cycle));
//								memcpy(RunModeA_Step,WaveModeD_Step,sizeof(WaveModeD_Step));

//								RunA_NumL = RunModeA_Cycle[0][0];/*每组循环次数*/
//								RunA_m = RunModeA_Cycle[0][1];/*下一组参数*/
//								RunA_NumS = RunModeA_Count[0][0];/*组内每个调制波运行次数*/
//								Wave_SelectA = RunModeA_Step[0][0];/*调制波种类切换*/
//								RunWaveA_Count = RunModeA_Hz[0][0]*2;/*调制波频率切换*/
//							break;
//						case 3:/*肩部*/
//								
//								memcpy(RunModeA_Count,WaveModeE_Count, sizeof(WaveModeE_Count));
//								memcpy(RunModeA_Hz,WaveModeE_Hz, sizeof(WaveModeE_Hz));
//								memcpy(RunModeA_Cycle,WaveModeE_Cycle,sizeof(WaveModeE_Cycle));
//								memcpy(RunModeA_Step,WaveModeE_Step,sizeof(WaveModeE_Step));

//								RunA_NumL = RunModeA_Cycle[0][0];/*每组循环次数*/
//								RunA_m = RunModeA_Cycle[0][1];/*下一组参数*/
//								RunA_NumS = RunModeA_Count[0][0];/*组内每个调制波运行次数*/
//								Wave_SelectA = RunModeA_Step[0][0];/*调制波种类切换*/
//								RunWaveA_Count = RunModeA_Hz[0][0]*2;/*调制波频率切换*/
//							break;
//						case 8:/*综合*/
//								
//								memcpy(RunModeA_Count,WaveModeF_Count, sizeof(WaveModeF_Count));
//								memcpy(RunModeA_Hz,WaveModeF_Hz, sizeof(WaveModeF_Hz));
//								memcpy(RunModeA_Cycle,WaveModeF_Cycle,sizeof(WaveModeF_Cycle));
//								memcpy(RunModeA_Step,WaveModeF_Step,sizeof(WaveModeF_Step));

//								RunA_NumL = RunModeA_Cycle[0][0];/*每组循环次数*/
//								RunA_m = RunModeA_Cycle[0][1];/*下一组参数*/
//								RunA_NumS = RunModeA_Count[0][0];/*组内每个调制波运行次数*/
//								Wave_SelectA = RunModeA_Step[0][0];/*调制波种类切换*/
//								RunWaveA_Count = RunModeA_Hz[0][0]*2;/*调制波频率切换*/
//							break;
//						case 4:/*腰部*/
//								
//								memcpy(RunModeA_Count,WaveModeG_Count, sizeof(WaveModeG_Count));
//								memcpy(RunModeA_Hz,WaveModeG_Hz, sizeof(WaveModeG_Hz));
//								memcpy(RunModeA_Cycle,WaveModeG_Cycle,sizeof(WaveModeG_Cycle));
//								memcpy(RunModeA_Step,WaveModeG_Step,sizeof(WaveModeG_Step));

//								RunA_NumL = RunModeA_Cycle[0][0];/*每组循环次数*/
//								RunA_m = RunModeA_Cycle[0][1];/*下一组参数*/
//								RunA_NumS = RunModeA_Count[0][0];/*组内每个调制波运行次数*/
//								Wave_SelectA = RunModeA_Step[0][0];/*调制波种类切换*/
//								RunWaveA_Count = RunModeA_Hz[0][0]*2;/*调制波频率切换*/
//							break;
//						case 7:/*关节*/
//								
//								memcpy(RunModeA_Count,WaveModeH_Count, sizeof(WaveModeH_Count));
//								memcpy(RunModeA_Hz,WaveModeH_Hz, sizeof(WaveModeH_Hz));
//								memcpy(RunModeA_Cycle,WaveModeH_Cycle,sizeof(WaveModeH_Cycle));
//								memcpy(RunModeA_Step,WaveModeH_Step,sizeof(WaveModeH_Step));

//								RunA_NumL = RunModeA_Cycle[0][0];/*每组循环次数*/
//								RunA_m = RunModeA_Cycle[0][1];/*下一组参数*/
//								RunA_NumS = RunModeA_Count[0][0];/*组内每个调制波运行次数*/
//								Wave_SelectA = RunModeA_Step[0][0];/*调制波种类切换*/
//								RunWaveA_Count = RunModeA_Hz[0][0]*2;/*调制波频率切换*/
//							break;
//						case 5:/*足底*/
//								
//								memcpy(RunModeA_Count,WaveModeI_Count, sizeof(WaveModeI_Count));
//								memcpy(RunModeA_Hz,WaveModeI_Hz, sizeof(WaveModeI_Hz));
//								memcpy(RunModeA_Cycle,WaveModeI_Cycle,sizeof(WaveModeI_Cycle));
//								memcpy(RunModeA_Step,WaveModeI_Step,sizeof(WaveModeI_Step));

//								RunA_NumL = RunModeA_Cycle[0][0];/*每组循环次数*/
//								RunA_m = RunModeA_Cycle[0][1];/*下一组参数*/
//								RunA_NumS = RunModeA_Count[0][0];/*组内每个调制波运行次数*/
//								Wave_SelectA = RunModeA_Step[0][0];/*调制波种类切换*/
//								RunWaveA_Count = RunModeA_Hz[0][0]*2;/*调制波频率切换*/
//							break;

//						default:
//							break;
//					}


//						switch(Mode2)
//						{
//							case 2:
//									
//									memcpy(RunModeB_Count,WaveModeA_Count, sizeof(WaveModeA_Count));
//									memcpy(RunModeB_Hz,WaveModeA_Hz, sizeof(WaveModeA_Hz));
//									memcpy(RunModeB_Cycle,WaveModeA_Cycle,sizeof(WaveModeA_Cycle));
//									memcpy(RunModeB_Step,WaveModeA_Step,sizeof(WaveModeA_Step));


//									RunB_NumL = RunModeB_Cycle[0][0];/*每组循环次数*/
//									RunB_m = RunModeB_Cycle[0][1];/*下一组参数*/
//									RunB_NumS = RunModeB_Count[0][0];/*组内每个调制波运行次数*/
//									Wave_SelectB = RunModeB_Step[0][0];/*调制波种类切换*/
//									RunWaveB_Count = RunModeB_Hz[0][0];/*调制波频率切换*/
//									
//								break;
//							case 0:
//									
//									memcpy(RunModeB_Count,WaveModeB_Count, sizeof(WaveModeB_Count));
//									memcpy(RunModeB_Hz,WaveModeB_Hz, sizeof(WaveModeB_Hz));
//									memcpy(RunModeB_Cycle,WaveModeB_Cycle,sizeof(WaveModeB_Cycle));
//									memcpy(RunModeB_Step,WaveModeB_Step,sizeof(WaveModeB_Step));

//									RunB_NumL = RunModeB_Cycle[0][0];/*每组循环次数*/
//									RunB_m = RunModeB_Cycle[0][1];/*下一组参数*/
//									RunB_NumS = RunModeB_Count[0][0];/*组内每个调制波运行次数*/
//									Wave_SelectB = RunModeB_Step[0][0];/*调制波种类切换*/
//									RunWaveB_Count = RunModeB_Hz[0][0];/*调制波频率切换*/
//								break;
//							case 1:
//									
//									memcpy(RunModeB_Count,WaveModeC_Count, sizeof(WaveModeC_Count));
//									memcpy(RunModeB_Hz,WaveModeC_Hz, sizeof(WaveModeC_Hz));
//									memcpy(RunModeB_Cycle,WaveModeC_Cycle,sizeof(WaveModeC_Cycle));
//									memcpy(RunModeB_Step,WaveModeC_Step,sizeof(WaveModeC_Step));

//									RunB_NumL = RunModeB_Cycle[0][0];/*每组循环次数*/
//									RunB_m = RunModeB_Cycle[0][1];/*下一组参数*/
//									RunB_NumS = RunModeB_Count[0][0];/*组内每个调制波运行次数*/
//									Wave_SelectB = RunModeB_Step[0][0];/*调制波种类切换*/
//									RunWaveB_Count = RunModeB_Hz[0][0];/*调制波频率切换*/
//								break;
//							case 6:
//									
//									memcpy(RunModeB_Count,WaveModeD_Count, sizeof(WaveModeD_Count));
//									memcpy(RunModeB_Hz,WaveModeD_Hz, sizeof(WaveModeD_Hz));
//									memcpy(RunModeB_Cycle,WaveModeD_Cycle,sizeof(WaveModeD_Cycle));
//									memcpy(RunModeB_Step,WaveModeD_Step,sizeof(WaveModeD_Step));

//									RunB_NumL = RunModeB_Cycle[0][0];/*每组循环次数*/
//									RunB_m = RunModeB_Cycle[0][1];/*下一组参数*/
//									RunB_NumS = RunModeB_Count[0][0];/*组内每个调制波运行次数*/
//									Wave_SelectB = RunModeB_Step[0][0];/*调制波种类切换*/
//									RunWaveB_Count = RunModeB_Hz[0][0];/*调制波频率切换*/

//								break;
//							case 3:
//									
//									memcpy(RunModeB_Count,WaveModeE_Count, sizeof(WaveModeE_Count));
//									memcpy(RunModeB_Hz,WaveModeE_Hz, sizeof(WaveModeE_Hz));
//									memcpy(RunModeB_Cycle,WaveModeE_Cycle,sizeof(WaveModeE_Cycle));
//									memcpy(RunModeB_Step,WaveModeE_Step,sizeof(WaveModeE_Step));

//									RunB_NumL = RunModeB_Cycle[0][0];/*每组循环次数*/
//									RunB_m = RunModeB_Cycle[0][1];/*下一组参数*/
//									RunB_NumS = RunModeB_Count[0][0];/*组内每个调制波运行次数*/
//									Wave_SelectB = RunModeB_Step[0][0];/*调制波种类切换*/
//									RunWaveB_Count = RunModeB_Hz[0][0];/*调制波频率切换*/
//								break;
//							case 8:
//									
//									memcpy(RunModeB_Count,WaveModeF_Count, sizeof(WaveModeF_Count));
//									memcpy(RunModeB_Hz,WaveModeF_Hz, sizeof(WaveModeF_Hz));
//									memcpy(RunModeB_Cycle,WaveModeF_Cycle,sizeof(WaveModeF_Cycle));
//									memcpy(RunModeB_Step,WaveModeF_Step,sizeof(WaveModeF_Step));

//									RunB_NumL = RunModeB_Cycle[0][0];/*每组循环次数*/
//									RunB_m = RunModeB_Cycle[0][1];/*下一组参数*/
//									RunB_NumS = RunModeB_Count[0][0];/*组内每个调制波运行次数*/
//									Wave_SelectB = RunModeB_Step[0][0];/*调制波种类切换*/
//									RunWaveB_Count = RunModeB_Hz[0][0];/*调制波频率切换*/
//								break;
//							case 4:
//									
//									memcpy(RunModeB_Count,WaveModeG_Count, sizeof(WaveModeG_Count));
//									memcpy(RunModeB_Hz,WaveModeG_Hz, sizeof(WaveModeG_Hz));
//									memcpy(RunModeB_Cycle,WaveModeG_Cycle,sizeof(WaveModeG_Cycle));
//									memcpy(RunModeB_Step,WaveModeG_Step,sizeof(WaveModeG_Step));

//									RunB_NumL = RunModeB_Cycle[0][0];/*每组循环次数*/
//									RunB_m = RunModeB_Cycle[0][1];/*下一组参数*/
//									RunB_NumS = RunModeB_Count[0][0];/*组内每个调制波运行次数*/
//									Wave_SelectB = RunModeB_Step[0][0];/*调制波种类切换*/
//									RunWaveB_Count = RunModeB_Hz[0][0];/*调制波频率切换*/
//								break;
//							case 7:
//									
//									memcpy(RunModeB_Count,WaveModeH_Count, sizeof(WaveModeH_Count));
//									memcpy(RunModeB_Hz,WaveModeH_Hz, sizeof(WaveModeH_Hz));
//									memcpy(RunModeB_Cycle,WaveModeH_Cycle,sizeof(WaveModeH_Cycle));
//									memcpy(RunModeB_Step,WaveModeH_Step,sizeof(WaveModeH_Step));

//									RunB_NumL = RunModeB_Cycle[0][0];/*每组循环次数*/
//									RunB_m = RunModeB_Cycle[0][1];/*下一组参数*/
//									RunB_NumS = RunModeB_Count[0][0];/*组内每个调制波运行次数*/
//									Wave_SelectB = RunModeB_Step[0][0];/*调制波种类切换*/
//									RunWaveB_Count = RunModeB_Hz[0][0];/*调制波频率切换*/
//								break;
//							case 5:
//									
//									memcpy(RunModeB_Count,WaveModeI_Count, sizeof(WaveModeI_Count));
//									memcpy(RunModeB_Hz,WaveModeI_Hz, sizeof(WaveModeI_Hz));
//									memcpy(RunModeB_Cycle,WaveModeI_Cycle,sizeof(WaveModeI_Cycle));
//									memcpy(RunModeB_Step,WaveModeI_Step,sizeof(WaveModeI_Step));

//									RunB_NumL = RunModeB_Cycle[0][0];/*每组循环次数*/
//									RunB_m = RunModeB_Cycle[0][1];/*下一组参数*/
//									RunB_NumS = RunModeB_Count[0][0];/*组内每个调制波运行次数*/
//									Wave_SelectB = RunModeB_Step[0][0];/*调制波种类切换*/
//									RunWaveB_Count = RunModeB_Hz[0][0];/*调制波频率切换*/
//								break;

//							default:
//								break;

//						}
//			
//			

//}







//void Wave_ModeChange(void)
//{
//		if(Wave_ChangeA)/*跑完相应调制波数*/
//		{
//			if(RunA_n<2)
//			{
//				RunA_n++;															
//				RunA_NumS=RunModeA_Count[RunA_m][RunA_n];/*组内每个调制波运行次数*/
//				Wave_SelectA = RunModeA_Step[RunA_m][RunA_n];/*调制波种类切换*/
//				RunWaveA_Count = RunModeA_Hz[RunA_m][RunA_n]*2;/*调制波频率切换*/
//			}
//			else/*一组调制波已运行完成*/
//			{
//				RunA_NumL--;/*调制波组循环次数计数器*/
//				RunA_n=0;
//				RunA_NumS=RunModeA_Count[RunA_m][RunA_n];/*组内每个调制波运行次数*/
//				Wave_SelectA = RunModeA_Step[RunA_m][RunA_n];/*调制波种类切换*/
//				RunWaveA_Count = RunModeA_Hz[RunA_m][RunA_n]*2;/*调制波频率切换*/
//				if(RunA_NumL==0)/*调制波组循环次数完成*/
//				{
//					RunA_i++;/*切换至下一个调制波组*/
//					RunA_NumL = RunModeA_Cycle[RunA_i][0];/*每组循环次数*/
//					RunA_m = RunModeA_Cycle[RunA_i][1];/*下一组参数*/
//					RunA_NumS = RunModeA_Count[RunA_m][RunA_n];/*组内每个调制波运行次数*/
//					Wave_SelectA = RunModeA_Step[RunA_m][RunA_n];/*调制波种类切换*/
//					RunWaveA_Count = RunModeA_Hz[RunA_m][RunA_n]*2;/*调制波频率切换*/
//					if(RunA_i>13)/*整模式完成一个循环*/
//					{
//						RunA_i=0;
//						RunA_NumS=0;
//						RunA_NumL = RunModeA_Cycle[RunA_i][0];
//						RunA_m = RunModeA_Cycle[RunA_i][1];
//						RunA_NumS=RunModeA_Count[RunA_i][RunA_n];
//						Wave_SelectA = RunModeA_Step[RunA_i][RunA_n];
//						RunWaveA_Count = RunModeA_Hz[RunA_i][RunA_n]*2;
//				
//					}
//				}
//			}
//				Wave_ChangeA=0;
//		}

//	if(Wave_ChangeB)
//		{
//			if(RunB_n<2)
//			{					
//				RunB_n++;															
//				RunB_NumS=RunModeB_Count[RunB_m][RunB_n];/*组内每个调制波运行次数*/
//				Wave_SelectB = RunModeB_Step[RunB_m][RunB_n];/*调制波种类切换*/
//				RunWaveB_Count = RunModeB_Hz[RunB_m][RunB_n]*2;/*调制波频率切换*/
//			}else
//			{
//				RunB_NumL--;/*调制波组循环次数计数器*/
//				RunB_n=0;
//				RunB_NumS=RunModeB_Count[RunB_m][RunB_n];/*组内每个调制波运行次数*/
//				Wave_SelectB = RunModeB_Step[RunB_m][RunB_n];/*调制波种类切换*/
//				RunWaveB_Count = RunModeB_Hz[RunB_m][RunB_n]*2;/*调制波频率切换*/
//				if(RunB_NumL==0)/*调制波组循环次数完成*/
//				{
//					RunB_i++;/*切换至下一个调制波组*/
//					RunB_NumL = RunModeB_Cycle[RunB_i][0];/*每组循环次数*/
//					RunB_m = RunModeB_Cycle[RunB_i][1];/*下一组参数*/
//					RunB_NumS = RunModeB_Count[RunB_m][RunB_n];/*组内每个调制波运行次数*/
//					Wave_SelectB = RunModeB_Step[RunB_m][RunB_n];/*调制波种类切换*/
//					RunWaveB_Count = RunModeB_Hz[RunB_m][RunB_n]*2;/*调制波频率切换*/
//					if(RunB_i>13)/*整模式完成一个循环*/
//					{
//						RunB_i=0;
//						RunB_NumS=1;
//						RunB_NumL = RunModeB_Cycle[RunB_i][0];
//						RunB_m = RunModeB_Cycle[RunB_i][1];
//						RunB_NumS=RunModeB_Count[RunB_m][RunB_n];
//						Wave_SelectB = RunModeB_Step[RunB_m][RunB_n];
//						RunWaveB_Count = RunModeB_Hz[RunB_m][RunB_n]*2;
//				
//					}
//			}
//		}
//		Wave_ChangeB=0;
//	}
//	
//}







