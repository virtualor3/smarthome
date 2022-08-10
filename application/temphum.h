#ifndef __TEMPHUM__
#define __TEMPHUM__


#define GET_SI7006_TMP _IOR('r',0,int)
#define GET_SI7006_HUM _IOR('r',1,int)

#define GET_CMD_SIZE(cmd)  ((cmd>>16)&0x3fff)
#define TMP_ADDR 0xe3
#define HUM_ADDR 0xe5



#endif