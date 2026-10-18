#include "stage00_task.h"
static void flag(uint32_t *sr,int condition){*sr=(*sr&~1u)|(uint32_t)condition;}
SA_STAGE_EXPORT uint32_t sa_model_angle_unwrap_8c055d70(uint32_t *r,uint32_t *sr){
 r[7]=65536u;r[3]=(r[5]&65535u)-(r[4]&65535u);r[6]=r[3];r[0]=(uint32_t)(int32_t)(int16_t)r[6];
 flag(sr,(int32_t)r[0]>0);
 if(*sr&1u){
  r[6]=r[5];flag(sr,(int32_t)r[4]>(int32_t)r[6]);
  if(*sr&1u){do{r[6]+=r[7];flag(sr,(int32_t)r[4]>(int32_t)r[6]);}while(*sr&1u);}
 }else{
  flag(sr,(int32_t)r[0]>=0);
  if(*sr&1u)r[6]=r[4];
  else{r[6]=r[5];flag(sr,(int32_t)r[4]>=(int32_t)r[6]);
   if(!(*sr&1u)){do{r[6]-=r[7];flag(sr,(int32_t)r[4]>=(int32_t)r[6]);}while(!(*sr&1u));}
  }
 }
 r[0]=r[6];return r[0];
}
