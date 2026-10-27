"""Declared coherent input/packet contexts; no request result or completion supplied."""
import types
import write_watch_channel_request as fast
import verify_movie_maple as packet
v=fast.old
CTX=packet.CTX;FRAME=packet.FRAME;LOCK=0x8cc7f003
CONFIGS=((0,0),(1,0x400000a1),(4,0x600000a1),(7,0x400080a1))

def put(m,a,value,n=4):m[a&0xffffff:(a&0xffffff)+n]=(value&((1<<(8*n))-1)).to_bytes(n,'little')

def setup(ram,entry,args,mode=0,sr=0x400000a1):
 c=v.Input(dict(entry=entry,mode=mode,sr=sr,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234,channel_request_arguments=args,channel_io_arguments=args))
 m,s,f=v.prepare(ram,c);s.sr=sr;f.fpscr=0x40001|(mode<<19)
 for j,x in enumerate(args):s.r[4+j]=x&0xffffffff
 for bank in range(2):
  for reg in range(16):f.banks[bank][reg]=(0x7fa12345^((bank+1)*0x11111111)^((reg+1)*0x01020304))&0xffffffff
 return c,m,s,f

def configure(m,unit,lock=0,busy=0,serial=1,order=0,padding=0,previous=0,bank=0,context=CTX):
 # Retail pointer cells and unit mapping remain untouched; object bodies are
 # explicit fixture resources, not claimed to be physically acquired hardware.
 put(m,v.load(m,0x8c65f65c),context)
 m[context&0xffffff:(context&0xffffff)+0x2000]=bytes(0x2000)
 m[FRAME&0xffffff:(FRAME&0xffffff)+512]=bytes([0x5a])*512
 code=m[(v.load(m,0x8c65f960)+unit)&0xffffff];put(m,FRAME,((code&0xc0)>>6)<<16|0x1234)
 for off,value in ((4044,FRAME),(4056,previous),(4060,0x20),(4064,serial),(4276,bank),(4292,order),(4300,padding)):put(m,context+off,value)
 for b in range(2):
  for i in range(24):put(m,context+4072+b*96+i*4,0x8ce00000+b*24576+i*1024)
 put(m,context+0x2e+unit*44,busy,1)
 put(m,v.load(m,0x8c660610),LOCK)
 m[(LOCK-3)&0xffffff:(LOCK&0xffffff)+5]=bytes([0xa5])*8;put(m,LOCK,lock,1)
 return dict(context=context,unit=unit,lock=LOCK,packet_code=code)

def expected_packet(m,s,entry,stack_arg,context=CTX):
 # Reuse the separately established packet-format oracle, supplying the two
 # command words from the request API rather than its stack implementation.
 copied=bytearray(m);pseudo=type(s).from_buffer_copy(s);payload=0x8cc7e000;sp=payload+32
 command=0xa if entry==0x8c660494 else 0xb
 second=(s.r[6]<<24)&0xffffffff
 if command==0xb:second|=((s.r[7]<<16)|stack_arg)&0xffffffff
 put(copied,payload,s.r[5]);put(copied,payload+4,second);put(copied,sp,0);put(copied,sp+4,0)
 pseudo.r[5]=command;pseudo.r[6]=payload;pseudo.r[7]=2;pseudo.r[15]=sp
 oracle=types.FunctionType(packet.packet_oracle.__code__,dict(packet.packet_oracle.__globals__,CTX=context),'independent_packet_oracle')
 expected,result=oracle(copied,pseudo,dict(entry=packet.COMMAND,second_words=0))
 return expected,result,command,[s.r[5],second]
