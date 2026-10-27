#include "console_sysinfo_status.h"
int sa_console_status_service(struct sa_console_service *state,uint8_t *ram,
 uint32_t channel,uint32_t selector,uint32_t first,uint32_t second,uint32_t count,
 uint32_t *result,uint32_t *write_result) {
 int status=sa_console_service(state,ram,channel,selector,first,second,count,result,write_result);
 if(!status && channel==0u && selector==0u){*result=0;*write_result=1;}
 return status;
}
