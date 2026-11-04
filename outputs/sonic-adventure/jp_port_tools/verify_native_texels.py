"""Independent synthetic texel fixtures, exhaustive 16-bit colors and bounds.
Input fixtures represent encoded surfaces, not retail file loading or pixels
measured on a Dreamcast. Original assets and texture-loader integration remain.
"""
import argparse,ctypes as C,hashlib,json,struct
from pathlib import Path
from native_packet_test_support import bind
U=C.c_uint32;S=C.c_size_t;P=C.c_void_p;I=C.c_int
class Level(C.Structure):_fields_=[(n,U) for n in ['width','height','format','mip_count']]+[(n,S) for n in ['encoded_offset','required_source_bytes','rgba_bytes']]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def positions(w,h,ox=0,oy=0):
    # Recursive rectangles, independently of the consumer's bit interleaver.
    if w==h==1:yield ox,oy
    elif w>h:
        yield from positions(w//2,h,ox,oy);yield from positions(w//2,h,ox+w//2,oy)
    elif h>w:
        yield from positions(w,h//2,ox,oy);yield from positions(w,h//2,ox,oy+h//2)
    else:
        for dx,dy in [(0,0),(0,h//2),(w//2,0),(w//2,h//2)]:yield from positions(w//2,h//2,ox+dx,oy+dy)
def rgba(value,fmt):
    if fmt==0:a=255 if value>=32768 else 0;r=(value//1024)%32;g=(value//32)%32;b=value%32
    elif fmt==1:a=255;r=value//2048;g=(value//32)%64;b=value%32
    else:return bytes([((value//256)%16)*17,((value//16)%16)*17,(value%16)*17,(value//4096)*17])
    return bytes([(r*255)//31,(g*255)//(63 if fmt==1 else 31),(b*255)//31,a])
def expanded(value,fmt):
    # Bit replication is the defined conversion; rounding a normalized rational
    # is a different policy. Build the 5/6-bit lookup tables independently.
    if fmt==2:return rgba(value,fmt)
    five=[int((format(v,'05b')*2)[:8],2) for v in range(32)]
    six=[int((format(v,'06b')*2)[:8],2) for v in range(64)]
    if fmt==0:return bytes([five[(value//1024)%32],five[(value//32)%32],five[value%32],255 if value>=32768 else 0])
    return bytes([five[value//2048],six[(value//32)%64],five[value%32],255])
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();assert not a.report.exists()
    b=json.loads((d/'build_verification.json').read_text());assert b['exit_code']==b['diagnostic_bytes']==0
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    dll=Path(b['DLL']);assert sha(dll)==b['DLL_sha256'];lib=C.CDLL(str(dll))
    describe=bind(lib,'jp_texel_describe',I,U,U,U,U,P);decode=bind(lib,'jp_texel_decode',I,U,U,U,U,P,S,P,S,P,S,P)
    checks=pixels=cases=0;groups={};palette=(U*1024)(*[((j%256)<<24)|(((j*3)%256)<<16)|(((j*5)%256)<<8)|((j*7)%256) for j in range(1024)])
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    def test(tsp,tcw,level,stride,encoded,expected,description=None):
        nonlocal pixels,cases
        info=Level();check(describe(tsp,tcw,level,stride,C.byref(info))==0,'surface layout')
        if description:check(tuple(getattr(info,n) for n in ['width','height','encoded_offset','required_source_bytes'])==description,'independent layout bounds')
        src=C.create_string_buffer(bytes(encoded));out=C.create_string_buffer(b'G'*16+b'X'*len(expected)+b'H'*16);result=Level()
        check(decode(tsp,tcw,level,stride,src,len(encoded),palette,1024,C.addressof(out)+16,len(expected),C.byref(result))==0,'decode valid surface')
        check(out.raw[:16]==b'G'*16 and out.raw[16+len(expected):16+len(expected)+16]==b'H'*16,'output guards')
        got=out.raw[16:16+len(expected)];check(got==expected,'independent expected pixels');pixels+=len(expected)//4;cases+=1
        check(C.string_at(C.byref(info),C.sizeof(info))==C.string_at(C.byref(result),C.sizeof(result)),'describe and decode metadata')
        for source_size,output_size,entries,want in [(info.required_source_bytes-1,len(expected),1024,3),(len(encoded),len(expected)-1,1024,4)]+([(len(encoded),len(expected),0,5)] if info.format>=5 else []):
            oldout=out.raw;C.memset(C.byref(result),0x5a,C.sizeof(result));oldinfo=bytes(result)
            check(decode(tsp,tcw,level,stride,src,source_size,palette,entries,C.addressof(out)+16,output_size,C.byref(result))==want,'short span rejected')
            check(out.raw==oldout and bytes(result)==oldinfo,'failure preserves output/info')
    colors={fmt:[expanded(value,fmt) for value in range(65536)] for fmt in range(3)}
    # Exhaust every 16-bit value in both linear and twiddled 256x256 surfaces.
    for fmt in range(3):
        expected=b''.join(colors[fmt]);linear=struct.pack('<65536H',*range(65536))
        twiddled=struct.pack('<65536H',*(y*256+x for x,y in positions(256,256)))
        for plain,raw in [(True,linear),(False,twiddled)]:test(45,fmt<<27|(0x04000000 if plain else 0),0,0,raw,expected,(256,256,0,131072))
    groups['exhaustive_rgb1555_565_4444']=checks
    for w,h in [(8,16),(16,8),(8,64),(64,8),(16,32),(32,16)]:
        u=w.bit_length()-4;v=h.bit_length()-4;tsp=(u<<3)|v
        for fmt in range(3):
            values=[(j*1297+fmt*999)%65536 for j in range(w*h)];expected=b''.join(colors[fmt][j] for j in values)
            raw=struct.pack('<'+'H'*(w*h),*(values[y*w+x] for x,y in positions(w,h)));test(tsp,fmt<<27,0,0,raw,expected,(w,h,0,w*h*2))
            stride=32 if w<=32 else 96;rows=bytearray(stride*h*2)
            for y in range(h):struct.pack_into('<'+'H'*w,rows,y*stride*2,*values[y*w:(y+1)*w])
            test(tsp,(fmt<<27)|0x06000000,0,stride,rows,expected,(w,h,0,((h-1)*stride+w)*2))
    groups['rectangles_and_stride']=checks-sum(groups.values())
    offsets={0:[6,8,16,48,176,688,2736,10928,43696,174768,699056],1:[6,8,16,48,176,688,2736,10928,43696,174768,699056],2:[6,8,16,48,176,688,2736,10928,43696,174768,699056],5:[1,2,4,12,44,172,684,2732,10924,43692,174764],6:[3,4,8,24,88,344,1368,5464,21848,87384,349528]}
    # Every SDK mip offset, including sub-8x8 levels; palette bank bits overlap
    # the non-paletted stride/linear flags and must remain palette selectors.
    for log in range(3,11):
        for fmt in [0,1,2,5,6]:
            banks=[0,63] if fmt==5 else [0,3] if fmt==6 else [0]
            for bank in banks:
                tcw=0x80000000|(fmt<<27)|(bank<<(21 if fmt==5 else 25))
                chain=bytearray(offsets[fmt][log]+(((1<<(2*log))+1)//2 if fmt==5 else (1<<(2*log)) if fmt==6 else (2<<(2*log))))
                for k in range(log+1):
                    size=1<<k;offset=offsets[fmt][k]
                    if fmt<3:
                        value=(k*8931+fmt*201)%65536;chain[offset:offset+size*size*2]=struct.pack('<H',value)*(size*size);expected=colors[fmt][value]*(size*size)
                    else:
                        index=(k*7+3)%(16 if fmt==5 else 256);chain[offset:offset+max(1,size*size//2) if fmt==5 else offset+size*size]=bytes([index*17 if fmt==5 else index])*(max(1,size*size//2) if fmt==5 else size*size)
                        packed=palette[bank*(16 if fmt==5 else 256)+index];expected=bytes([(packed>>16)&255,(packed>>8)&255,packed&255,packed>>24])*(size*size)
                    # Only this level is checked now, so other chain levels may
                    # remain unfilled. Required prefix is a strict read bound.
                    test(((log-3)<<3)|(log-3),tcw,log-k,0,chain,expected)
    groups['all_sdk_mip_offsets_and_palette_banks']=checks-sum(groups.values())
    for fmt in [0,1,2]:
        for log in range(3,9):
            size=1<<log;vqends=[1,2,6,22,86,342,1366,5462,21846,87382,349526]
            for mip in [False,True]:
                for k in ([log] if not mip else range(1,log+1)):
                    side=1<<k;offset=2048+(vqends[k-1] if mip else 0);raw=bytearray(offset+side*side//4)
                    for code in range(256):struct.pack_into('<4H',raw,code*8,*[(code*199+corner*7919)%65536 for corner in range(4)])
                    indices=[(j*31+7)%256 for j in range(side*side//4)]
                    coords=list(positions(side//2,side//2));codes={xy:indices[j] for j,xy in enumerate(coords)};raw[offset:]=bytes(indices)
                    expected=b''.join(colors[fmt][(codes[(x//2,y//2)]*199+((x%2)*2+y%2)*7919)%65536] for y in range(side) for x in range(side))
                    test(((log-3)<<3)|(log-3),(fmt<<27)|0x40000000|(0x80000000 if mip else 0),log-k if mip else 0,0,raw,expected,(side,side,offset,len(raw)))
    groups['vq_codebooks_indices_and_mips_above_1x1']=checks-sum(groups.values())
    # Explicit unsupported formats and bad layout arguments, no metadata writes.
    for tsp,tcw,level,stride,want in [(0,3<<27,0,0,2),(0,4<<27,0,0,2),(0,7<<27,0,0,2),(0,0xe8000000,0,0,2),(0,0xc8000000,3,0,2),(0,0x06000000,0,0,1),(0,0x06000000,0,31,1),(0,0x06000000,0,1024,1),(1,0x80000000,0,0,2),(0,0,1,0,1)]:
        info=Level();C.memset(C.byref(info),0x5a,C.sizeof(info));old=bytes(info);check(describe(tsp,tcw,level,stride,C.byref(info))==want and bytes(info)==old,'unsupported/invalid atomic metadata')
    src=C.create_string_buffer(bytes(256));info=Level();before=src.raw
    check(decode(0,0,0,0,src,256,None,0,src,256,C.byref(info))==1 and src.raw==before,'overlapping input/output rejected')
    groups['unsupported_and_aliasing_failures']=checks-sum(groups.values())
    report=dict(status='authored_native_texture_decoder_verified',checks=checks,pixels_compared=pixels,valid_surface_cases=cases,groups=groups,
        exhaustive_16bit_values_per_format=65536,formats=['ARGB1555','RGB565','ARGB4444','PAL4','PAL8'],input='encoded GPU surfaces, not PVR files',
        qualified_against='independent constructed channel/address fixtures and pinned SDK layout tables; not physical Dreamcast captures',
        VQ_1x1_YUV_bump_and_palette_VQ_supported=False,asset_loader_integrated=False,palette_state_captured_in_frames=False,
        build_report_sha256=sha(d/'build_verification.json'),DLL_sha256=sha(dll),verifier_sha256=sha(Path(__file__)),game_frame_executed=False,playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
if __name__=='__main__':main()
