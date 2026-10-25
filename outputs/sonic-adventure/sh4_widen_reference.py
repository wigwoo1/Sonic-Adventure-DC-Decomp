"""Independent manual-based FCNVSD and double comparison test oracle."""
import struct
MANUAL='https://www.renesas.com/us/en/node/735531'
def widen(f,raw,n):
 if n>14 or n&1 or not f.fpscr&0x80000:return -1
 old=f.fpscr;status=old&~0x3f000;absraw=raw&0x7fffffff
 if 0<absraw<0x800000 and not old&0x40000:f.fpscr=status|0x20000;return 1
 if absraw>0x7f800000:
  if raw&0x400000:
   status|=0x10040
   if old&0x800:f.fpscr=status;return 1
  encoded=0x7ff7ffffffffffff
 else:
  if absraw<0x800000:raw&=0x80000000
  value=struct.unpack('>f',raw.to_bytes(4,'big'))[0]
  encoded=int.from_bytes(struct.pack('>d',value),'big')
 bank=old>>21&1;f.banks[bank][n]=encoded>>32;f.banks[bank][n+1]=encoded&0xffffffff;f.fpscr=status;return 0

def compare(f,sr,n,m,greater):
 if n>15 or m>15:return -1,sr
 status=f.fpscr;double=bool(status&0x80000)
 if double and (n|m)&1:return -1,sr
 bank=status>>21&1;bits=[];width=64 if double else 32
 for index in (n,m):bits.append((f.banks[bank][index]<<32)|f.banks[bank][index+1] if double else f.banks[bank][index])
 frac=52 if double else 23;infinity=0x7ff0000000000000 if double else 0x7f800000;mask=(1<<(width-1))-1
 nan=any((x&mask)>infinity for x in bits);signaling=any((x&mask)>infinity and x&(1<<(frac-1)) for x in bits)
 status&=~0x3f000
 if signaling or greater and nan:
  status|=0x10040;f.fpscr=status
  return (1,sr) if status&0x800 else (0,sr&~1)
 if nan:f.fpscr=status;return 0,sr&~1
 if not double and not status&0x40000 and any(0<(x&mask)<1<<frac for x in bits):return -1,sr
 if status&0x40000:bits=[x&~mask if x&mask<1<<frac else x for x in bits]
 values=[struct.unpack('>d' if double else '>f',x.to_bytes(width//8,'big'))[0] for x in bits]
 f.fpscr=status;condition=values[0]>values[1] if greater else values[0]==values[1]
 return 0,(sr&~1)|int(condition)
