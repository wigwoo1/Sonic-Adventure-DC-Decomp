"""Run after translate_tikal_helper.py for the interrupt constructor."""
from pathlib import Path
import re,sys
name=sys.argv[1] if len(sys.argv)>1 else 'sa_interrupt_callback_create'
entry=sys.argv[2] if len(sys.argv)>2 else '8c64594c'
p=Path('outputs/sonic-adventure/source')/(name+'.c')
s=p.read_text()
a=s.index('static uint32_t rd(')
b=s.index('SA_STAGE_EXPORT',a)
s=s[:a]+'''#include "interrupt_io.h"
/* Retail interrupt callback constructor, 8c64594c..8c645af8.
 * Hardware accesses use explicit callbacks; VBR is supplied by the host. */
static uint32_t rd(uint8_t *m,struct sa_interrupt_io *io,uint32_t a,unsigned n){
 uint32_t physical=a&0x1fffffffu,v=0;
 if(io->status)return 0;
 if(physical>=0x0c000000u && physical<=0x0d000000u-n){
  for(unsigned i=0;i<n;i++)v|=(uint32_t)m[physical-0x0c000000u+i]<<(i*8);
 }else if(n==4 && io->read32)io->status=io->read32(io->context,a,&v);
 else io->status=-3;
 return v;
}
static uint32_t sx(uint8_t *m,struct sa_interrupt_io *io,uint32_t a,unsigned n){
 uint32_t v=rd(m,io,a,n);
 return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;
}
static void wr(uint8_t *m,struct sa_interrupt_io *io,uint32_t a,uint32_t v,unsigned n){
 uint32_t physical=a&0x1fffffffu;
 if(io->status)return;
 if(physical>=0x0c000000u && physical<=0x0d000000u-n){
  for(unsigned i=0;i<n;i++)m[physical-0x0c000000u+i]=(uint8_t)(v>>(i*8));
 }else if(n==4 && io->write32)io->status=io->write32(io->context,a,v);
 else io->status=-3;
}
'''+s[b:]
s=s.replace('uint32_t *macl,void *ctx','uint32_t *macl,struct sa_interrupt_io *io,void *ctx')
s=s.replace('(void)gt;(void)eq;','')
s=s.replace(' goto L'+entry+';',' if(!io)return -3;io->status=0;uint32_t vbr=io->vbr;\n goto L'+entry+';')
a=s.index('L'+entry+':')
s=s[:a]+s[a:].replace('rd(m,','rd(m,io,').replace('sx(m,','sx(m,io,').replace('wr(m,','wr(m,io,').replace('return 0;','return io->status;')
s=re.sub(r'(L[0-9a-f]+:) ',r'\1 if(io->status)return io->status; ',s)
if name!='sa_interrupt_callback_create':s=s.replace('Retail interrupt callback constructor, 8c64594c..8c645af8.','Retail interrupt callback removal, 8c645c54..8c645dbc.')
p.write_text(s)
