/* No CRT formatting implementation or original SDK bodies are embedded here.
 * The genuine formatter/decimal helpers are separately compiled private inputs.
 * This is value/buffer handling, not an FPSCR flags/traps emulator.
 */
#include "jp_native_print_format.h"
#include <string.h>
struct _iobuf;
extern int jp_sdk_fmtout(int,int (*)(int,struct _iobuf *),void *,const char *,jp_print_va_list);
extern volatile int jp_sdk_errno;
static struct {uintptr_t first,last;int active,error;} bounds;
static int format_status;
static int reject(int value){format_status=value;if(bounds.active)bounds.error=1;return -1;}
int jp_print_format_status(void){return format_status;}
int jp_print_format_failed(void){return bounds.error;}
size_t jp_print_format_strlen(const char *s)
{
    if(!s){reject(JP_FORMAT_ARGUMENT);return 0;}
    for(size_t i=0;i<=32767;i++)if(!s[i])return i;
    reject(JP_FORMAT_ARGUMENT);return 0;
}
int jp_print_format_span(const void *pointer,int count)
{
    uintptr_t at=(uintptr_t)pointer;
    if(!bounds.active || count<0 || at<bounds.first || at>bounds.last ||
       (size_t)count>bounds.last-at){reject(JP_FORMAT_BOUNDS);return 0;}
    return !bounds.error;
}
int jp_print_vformat(char *buffer,size_t capacity,const char *format,jp_print_va_list args)
{
    if(bounds.active)return reject(JP_FORMAT_STATE);
    format_status=JP_FORMAT_OK;
    if(!buffer || !capacity || capacity>32768 || capacity-1>UINTPTR_MAX-(uintptr_t)buffer)
        return reject(JP_FORMAT_ARGUMENT);
    bounds.first=(uintptr_t)buffer;bounds.last=bounds.first+capacity-1;
    bounds.active=1;bounds.error=0;buffer[0]=0;jp_sdk_errno=0;
    (void)jp_print_format_strlen(format);
    int length=bounds.error?-1:jp_sdk_fmtout(1,NULL,buffer,format,args);
    if(bounds.error || jp_sdk_errno || length<0){
        if(format_status==JP_FORMAT_OK)format_status=JP_FORMAT_DIRECTIVE;
        length=-1;buffer[0]=0;
    }
    bounds.active=0;return length;
}
int jp_print_format(char *buffer,size_t capacity,const char *format,...)
{
    jp_print_va_list args;__builtin_va_start(args,format);
    int length=jp_print_vformat(buffer,capacity,format,args);__builtin_va_end(args);return length;
}
uint64_t jp_print_format_widen_word(uint32_t word)
{
    uint64_t sign=(uint64_t)(word>>31)<<63;
    uint32_t absolute=word&0x7fffffffu,exp=absolute>>23,frac=absolute&0x7fffffu;
    if(!exp)return sign; /* original FCNVSD DN=1 */
    if(exp==255)return frac?UINT64_C(0x7ff7ffffffffffff):sign|UINT64_C(0x7ff0000000000000);
    return sign|((uint64_t)(exp+896u)<<52)|((uint64_t)frac<<29);
}
uint32_t jp_print_format_narrow_word(uint64_t word)
{
    uint32_t sign=(uint32_t)(word>>63)<<31;
    unsigned exp=(unsigned)((word>>52)&2047u);uint64_t frac=word&UINT64_C(0xfffffffffffff);
    if(exp==2047)return frac?0x7fbfffffu:sign|0x7f800000u;
    if(exp<897)return sign; /* fixed DN=1, including all single subnormals */
    if(exp>1150)return sign|0x7f7fffffu; /* RZ finite overflow */
    return sign|((uint32_t)(exp-896u)<<23)|(uint32_t)(frac>>29);
}
double jp_print_format_argument(double value)
{
    uint64_t word;memcpy(&word,&value,8);
    word=jp_print_format_widen_word(jp_print_format_narrow_word(word));memcpy(&value,&word,8);return value;
}
int jp_print_format_scaled_exponent(int exp)
{
    /* _log10 sees only widened single inputs. frexp's exponent is therefore
     * -125..128 after DN=1. For every integer in that finite domain, truncation
     * of original binary64 RZ (exp*0.301L) equals this exact rational result.
     * The original _log10 body still performs its real splits around it. */
    if(exp< -125 || exp>128){reject(JP_FORMAT_ARGUMENT);return 0;}
    return exp*301/1000;
}
int jp_print_fixed_arguments(float *value,int digit)
{
    uint32_t word;memcpy(&word,value,4);
    if((word&0x7fffffffu)<0x800000u)word&=0x80000000u;
    memcpy(value,&word,4);
    if((word&0x7fffffffu)>=0x7f800000u || digit<1 || digit>13)return reject(JP_FORMAT_ARGUMENT);
    int negative=(word>>31)!=0 && (word&0x7fffffffu)!=0;
    uint64_t promoted=jp_print_format_widen_word(word&0x7fffffffu);double magnitude;memcpy(&magnitude,&promoted,8);
    char buffer[20];int length=jp_print_format(buffer,sizeof buffer,"%0.*f",5,magnitude);
    if(length<0)return -1;
    int point=0;while(point<length && buffer[point]!='.')++point;
    int pad=digit-point;
    if(point==length || pad<0 || length+pad>=20)return reject(JP_FORMAT_BOUNDS);
    return length+pad+negative;
}
