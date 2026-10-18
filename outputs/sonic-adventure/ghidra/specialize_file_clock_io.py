"""Add explicit host register access to selected recovered input routines."""
from pathlib import Path
import re

root=Path('outputs/sonic-adventure/source')
helpers='''static uint32_t rd(const struct sa_input_io *io,uint8_t *m,uint32_t a,unsigned n){uint32_t physical=a&0x1fffffffu;if(physical<0x0c000000u||physical>=0x0d000000u)return io->read(io->user,a,n);uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(const struct sa_input_io *io,uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(io,m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(const struct sa_input_io *io,uint8_t *m,uint32_t a,uint32_t v,unsigned n){uint32_t physical=a&0x1fffffffu;if(physical<0x0c000000u||physical>=0x0d000000u){io->write(io->user,a,v,n);return;}for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
'''
for name in ['sa_file_clock_get']:
 source=(root/f'{name}.c').read_text()
 start=source.index('static uint32_t rd(');end=source.index('static int gt(',start)
 body=source[end:]
 body=re.sub(r'\b(rd|sx|wr)\(m,',r'\1(io,m,',body)
 body=body.replace(f'int {name}(',f'int {name}_io(',1)
 old=',void *ctx,int (*call)'
 assert old in body,name
 body=body.replace(old,',const struct sa_input_io *io,void *ctx,int (*call)',1)
 (root/f'{name}_io.c').write_text('#include "input_io.h"\n'+source[:start]+helpers+body)
 print(name,'IO specialization generated')
