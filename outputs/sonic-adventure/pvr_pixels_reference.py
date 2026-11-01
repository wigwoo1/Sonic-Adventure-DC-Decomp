"""Independent recursive texture layout and rational integer raster oracle.

This defines the tested host profile, not physical PowerVR precision or timing.
"""
import struct
from fractions import Fraction as F
def float_value(word):return F(struct.unpack('<f',int(word).to_bytes(4,'little'))[0])
def rounded(value):return (value.numerator*2+value.denominator)//(2*value.denominator) if value>=0 else -rounded(-value)
def texture_info(tsp,tcw,level):
 w=2**(3+((tsp//8)%8));h=2**(3+(tsp%8));fmt=(tcw//(1<<27))%8;linear=(tcw//(1<<26))%2;mip=tcw//(1<<31)
 if fmt>2 or tcw&(1<<30) or tcw&(1<<25):return -2,None
 if mip and linear:return -2,None
 if (not mip and level) or level>w.bit_length()-1:return -3,None
 if mip:h=w
 w//=2**level;h=max(1,h//(2**level));offset=0
 if mip:
  offset=6;side=1
  while side<w:offset+=2*side*side;side*=2
 return 0,(w,h,(tcw%(1<<25))*8+offset,fmt,linear)
def square_order(side,x=0,y=0):
 if side==1:yield x,y;return
 half=side//2
 for dx,dy in ((0,0),(0,half),(half,0),(half,half)):yield from square_order(half,x+dx,y+dy)
def texture_order(w,h,linear):
 if linear:
  for y in range(h):
   for x in range(w):yield x,y
 else:
  side=min(w,h)
  for y in range(0,h,side):
   for x in range(0,w,side):yield from square_order(side,x,y)
def expand(v,bits):return int((format(v,'0'+str(bits)+'b')*2)[:8],2)
def unpack(word,fmt):
 if fmt==0:a=255 if word>=32768 else 0;r=expand((word//1024)%32,5);g=expand((word//32)%32,5);b=expand(word%32,5)
 elif fmt==1:a=255;r=expand((word//2048)%32,5);g=expand((word//32)%64,6);b=expand(word%32,5)
 else:a=((word//4096)%16)*17;r=((word//256)%16)*17;g=((word//16)%16)*17;b=(word%16)*17
 return a*2**24+r*2**16+g*256+b
def decode(vram,tsp,tcw,level):
 rc,info=texture_info(tsp,tcw,level)
 if rc:return rc,None,info
 w,h,base,fmt,linear=info
 if len(vram)>8388608 or base+2*w*h>len(vram):return -3,None,info
 out=[0]*(w*h)
 for i,(x,y) in enumerate(texture_order(w,h,linear)):out[y*w+x]=unpack(int.from_bytes(vram[base+2*i:base+2*i+2],'little'),fmt)
 return 0,out,info
def channels(word):return [(word//(256**i))%256 for i in range(4)]
def packed(v):return sum(x*256**i for i,x in enumerate(v))
def mul(a,b):return (a*b+127)//255
def fragment(col,tex,tsp):
 c=channels(col);t=channels(tex)
 if tsp&(1<<19):t[3]=255
 mode=(tsp//64)%4
 if mode==0:return packed(t)
 rgb=[(t[i]*t[3]+c[i]*(255-t[3])+127)//255 for i in range(3)] if mode==2 else [mul(c[i],t[i]) for i in range(3)]
 return packed(rgb+[t[3] if mode==1 else mul(c[3],t[3]) if mode==3 else c[3]])
def blend(src,dst,tsp):
 s=channels(src);d=channels(dst)
 def factors(mode,is_source):
  other=d if is_source else s
  return ([0]*4,[255]*4,other,[255-x for x in other],[s[3]]*4,[255-s[3]]*4,[d[3]]*4,[255-d[3]]*4)[mode]
 a=factors(tsp//2**29,True);b=factors((tsp//2**26)%8,False)
 return packed([min(255,(s[i]*a[i]+d[i]*b[i]+127)//255) for i in range(4)])
def coordinate(v,size,clamp,flip):
 if clamp:return min(size-1,max(0,v))
 n=v%(size*(2 if flip else 1));return 2*size-1-n if flip and n>=size else n
def determinant(a,b,p):return (b['x']-a['x'])*(p[1]-a['y'])-(b['y']-a['y'])*(p[0]-a['x'])
def included_zero(a,b):return b['y']<a['y'] or (b['y']==a['y'] and b['x']>a['x'])
def draw(words,vram,o,pixels=None,depth=None):
 pixels=list(pixels or [0]*(o.width*o.height));depth=list(depth or [0]*len(pixels));stats=[0]*5;triangles=[];vertices=[];h=None
 if len(words)%8 or not words or len(words)>512:return -3,pixels,depth,tuple(stats)
 try:
  for offset in range(0,len(words),8):
   w=words[offset:offset+8];kind=w[0]//2**29
   if kind==4:
    if vertices:raise ValueError(-3)
    pcw,isp,tsp,tcw=w[:4];color=(pcw//16)%4;list_type=(pcw//2**24)%8
    if pcw&0x300c5 or list_type not in (0,2,4) or color not in (0,2) or tsp&0x3000000:raise ValueError(-2)
    if pcw&8 and (tsp//2**22)%4!=2:raise ValueError(-2)
    face=[rounded(float_value(x)*255) for x in w[4:]] if color==2 else [0]*4
    if color==2 and any(float_value(x)<0 or float_value(x)>1 for x in w[4:]):raise ValueError(-3)
    texture=None;info=None
    if pcw&8 and o.textures:
     if (tsp//8192)%4:raise ValueError(-2)
     rc,texture,info=decode(vram,tsp,tcw,o.mip_level)
     if rc:raise ValueError(rc)
    h=dict(pcw=pcw,isp=isp,tsp=tsp,face=face,color=color,list_type=list_type,texture=texture,info=info)
   elif kind==0:
    if w[0] or vertices or h is None:raise ValueError(-3)
    h=None
   elif kind==7:
    if h is None:raise ValueError(-3)
    x,y,z,u,v=map(float_value,w[1:6]);x=x*o.width/o.input_width;y=y*o.height/o.input_height
    if not(-16384<=x<=16384 and -16384<=y<=16384 and 0<z<=16):raise ValueError(-3)
    textured=h['texture'] is not None
    if textured and not(-16<=u<=16 and -16<=v<=16):raise ValueError(-3)
    if h['color']==2:
     intensity=float_value(w[6])
     if not 0<=intensity<=1:raise ValueError(-3)
     col=packed([mul(c,rounded(intensity*255)) for c in h['face'][::-1]])
    else:col=w[6]
    vertex=dict(x=rounded(x*16),y=rounded(y*16),z=rounded(z*65536),u=rounded(u*z*65536) if textured else 0,v=rounded(v*z*65536) if textured else 0,color=col)
    if vertex['z']==0:raise ValueError(-3)
    vertices.append(vertex)
    if len(vertices)>=3:
     a,b,c=vertices[-3:]
     if len(vertices)%2==0:a,b=b,a
     triangles.append((a,b,c,h))
    if w[0]&(1<<28):
     if len(vertices)<3:raise ValueError(-3)
     vertices=[]
   else:raise ValueError(-2)
  if vertices:raise ValueError(-3)
 except (ValueError,OverflowError) as error:
  return error.args[0] if error.args and isinstance(error.args[0],int) else -3,pixels,depth,tuple(stats)
 for a,b,c,h in triangles:
  area=determinant(a,b,(c['x'],c['y']));cull=(h['isp']//2**27)%4
  if not area or cull==2 and area<0 or cull==3 and area>0:continue
  if area<0:a,b=b,a;area=-area
  if cull==1 and area<=o.cull_area:continue
  stats[0]+=1
  # Brute-force reference visits the full viewport; native uses a bounding box.
  for y in range(o.height):
   for x in range(o.width):
    p=(x*16+8,y*16+8);weights=[determinant(b,c,p),determinant(c,a,p),determinant(a,b,p)]
    if any(w<0 for w in weights) or any(w==0 and not included_zero(q,r) for w,(q,r) in zip(weights,((b,c),(c,a),(a,b)))):continue
    stats[1]+=1;vs=(a,b,c)
    def interpolate(key):return int(F(sum(w*v[key] for w,v in zip(weights,vs)),area))
    z=interpolate('z');index=y*o.width+x;old=depth[index];mode=h['isp']//2**29
    if not (False,z<old,z==old,z<=old,z>old,z!=old,z>=old,True)[mode]:continue
    stats[2]+=1;col=c['color']
    if h['pcw']&2:col=packed([sum(w*channels(v['color'])[k] for w,v in zip(weights,vs))//area for k in range(4)])
    if not h['tsp']&(1<<20):col|=0xff000000
    if h['texture'] is not None:
     tw,th,_,_,_=h['info'];sx=coordinate((interpolate('u')*tw)//z,tw,(h['tsp']//2**16)%2,(h['tsp']//2**18)%2);sy=coordinate((interpolate('v')*th)//z,th,(h['tsp']//2**15)%2,(h['tsp']//2**17)%2)
     col=fragment(col,h['texture'][sy*tw+sx],h['tsp']);stats[3]+=1
    if h['list_type']==4 and col//2**24<o.alpha_ref:continue
    pixels[index]=blend(col,pixels[index],h['tsp']) if h['list_type']==2 else col
    if not h['isp']&(1<<26):depth[index]=z
    stats[4]+=1
 return 0,pixels,depth,tuple(stats)
