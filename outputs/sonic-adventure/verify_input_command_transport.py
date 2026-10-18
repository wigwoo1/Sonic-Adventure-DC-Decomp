import ctypes,json,struct
from verify_startup import ROOT

class Request(ctypes.Structure):
 _fields_=[(x,ctypes.c_uint32) for x in ['port','command','destination','source','payload_words','response_address','packet_mode']]+[('payload',ctypes.POINTER(ctypes.c_uint8))]
Exchange=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.POINTER(Request),ctypes.POINTER(ctypes.c_uint8),ctypes.c_uint32)

def load_transport():
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_command_transport.dll')).sa_input_command_transport
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.c_uint32,ctypes.c_uint32,ctypes.c_uint32,ctypes.c_uint32,ctypes.c_void_p,Exchange]
 return fn

def main():
 fn=load_transport();count=0
 lists=[
  ([0x80000000,0x0ca50000,0x2001],[(1,0x20,0,0,0x0ca50000)]),
  ([1,0x0ca50400,0x01002009,1,0x700,0x700,0x700,0x700,0x700,0x80000000,0x0ca60400,0x101],[(9,0x20,0,1,0x0ca50400),(1,1,0,0,0x0ca60400)]),
  ([2,0x0ca60000,0x0200010a,0x02000000,0,0x700,0x700,0x700,0x700,0x700,0x80000001,0x0ca50000,0x01002009,1],[(10,1,0,2,0x0ca60000),(9,0x20,0,1,0x0ca50000)])]
 for words,expected in lists:
  for mode in [0,1]:
   encoded=list(words);pos=0
   while pos<len(encoded):
    control=encoded[pos]
    if control==0x700:pos+=1;continue
    if mode:encoded[pos+2]=int.from_bytes(encoded[pos+2].to_bytes(4,'little'),'big')
    pos+=3+(control&255)
   ram=bytearray(16*1024*1024);struct.pack_into('<'+'I'*len(encoded),ram,0xa40000,*encoded)
   view=(ctypes.c_uint8*len(ram)).from_buffer(ram);seen=[];errors=[]
   @Exchange
   def exchange(ctx,request,response,capacity):
    r=request.contents;seen.append((r.command,r.destination,r.source,r.payload_words,r.response_address))
    if capacity!=1024 or r.port!=0 or r.packet_mode!=mode:errors.append('request')
    for i,value in enumerate([r.command,r.destination,r.source,r.payload_words]):response[i]=value
    return 4
   assert fn(view,len(ram),0x0ca40000,len(encoded)*4,mode,None,exchange)==len(expected)
   assert seen==expected and not errors
   for command,dest,source,payload,address in expected:assert ram[address&0xffffff:(address&0xffffff)+4]==bytes([command,dest,source,payload])
   count+=1
 malformed=[([0x80000000],-2),([0,0x0ca50000,0x2001],-2),([0x80000400,0x0ca50000,0x2001],-2),([0x80000001,0x0ca50000,0x2001,0],-2),([0x80000000,0xa05f6c00,0x2001],-3)]
 for words,result in malformed:
  ram=bytearray(16*1024*1024);struct.pack_into('<'+'I'*len(words),ram,0xa40000,*words);before=bytes(ram);seen=[]
  @Exchange
  def reject(ctx,request,response,capacity):seen.append(1);return 0
  view=(ctypes.c_uint8*len(ram)).from_buffer(ram)
  assert fn(view,len(ram),0x0ca40000,len(words)*4,0,None,reject)==result and not seen and bytes(ram)==before
  count+=1
 (ROOT/'outputs/sonic-adventure/analysis/input_command_transport_verification.json').write_text(json.dumps(dict(cases_passed=count,scope='Independent captured command-list vectors for discovery, padding, multiple commands and active VMU request; both header modes. Independent decoded request/response-address checks and malformed-list no-exchange/no-write checks. Host adapter, not recovered game code or complete device emulation.'),indent=2)+'\n')
 print(count,'host command transport comparisons passed')

if __name__=='__main__':main()
