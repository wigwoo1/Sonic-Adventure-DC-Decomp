"""Compare all real JP music loop headers with the original native header reader.

Read-only oracle/metadata inventory, never a production file service. Publishes
no retail header bytes, audio, compiled game code or substitute loop points.
"""
import argparse,ctypes as C,hashlib,json,struct
from pathlib import Path
from adx_media_fixtures import Disc
from verify_native_audio import bind
I,H,P=C.c_int32,C.c_int16,C.c_void_p
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    lib=C.CDLL(b['game_sound_DLL']);decode=bind(lib,'ADX_DecodeInfoExLoop',I,P,I,*([P]*7));disc=Disc(a.track.resolve())
    rows=[];labels=[];declared=[]
    def check(ok,label):assert ok,label;labels.append(label)
    for index in range(110):
        text=C.create_string_buffer(32)
        check(bind(lib,'peer_game_stream_music_file',I,C.c_uint32,P,C.c_uint32)(index,text,32)==0,'actual original declared music ID '+str(index))
        declared.append(text.value.decode())
    try:
        for name,(fad,size) in sorted(disc.files.items()):
            if not name.endswith('.ADX'):continue
            header=disc.read(name,0,128);storage=C.create_string_buffer(header);values=[I(),H(),H(),I(),I(),I(),I()]
            ret=decode(storage,len(header),*[C.byref(v) for v in values]);offset=struct.unpack_from('>H',header,2)[0]+4
            check(header[:2]==b'\x80\0' and header[4:8]==bytes((3,18,4,2)),'actual original stereo ADX format '+name)
            independent_loop=offset>=48 and struct.unpack_from('>h',header,22)[0]==1
            row=dict(file=name,bytes=size,fad=fad,header_sha256=hashlib.sha256(header).hexdigest(),data_offset=offset,declared_samples=struct.unpack_from('>I',header,12)[0],rate=struct.unpack_from('>I',header,8)[0],original_loop_header_status=ret,looping=independent_loop)
            check((ret==0)==independent_loop,'native original loop classification equals independent big-endian header '+name)
            if independent_loop:
                expected=[struct.unpack_from('>h',header,20)[0],1,struct.unpack_from('>h',header,26)[0],*struct.unpack_from('>iiii',header,28)]
                check([v.value for v in values]==expected,'every original loop field matches independent byte decoding '+name)
                row.update(zip(('inserted_samples','loops','type','start_sample','start_byte','end_sample','end_byte'),expected))
                check(0<=row['start_sample']<row['end_sample']<=row['declared_samples'],'real loop samples lie within declared source '+name)
                check(row['start_byte']%2048==0 and row['start_sample']%32==0,'real loop starts align to original sector/block assumptions '+name)
                check(row['start_byte']==offset+row['start_sample']//32*36 and row['end_byte']==offset+(row['end_sample']+31)//32*36,'real loop byte offsets match original interleaved stereo blocks '+name)
                check(row['end_byte']<=size,'real loop end remains within file '+name)
            else:check(values[1].value==0,'original nonloop reader reports no loop '+name)
            rows.append(row)
    finally:disc.close()
    looping=[x for x in rows if x['looping']];looping.sort(key=lambda x:x['end_sample'])
    missing=sorted(set(declared)-{x['file'] for x in rows})
    check(len(rows)==109 and len(looping)==79,'all109 present Japanese music files classified with79 looping tracks')
    check(len(declared)==110 and missing==['NIGHTS_K.ADX'],'actual110 declared IDs include one filename absent from this Japanese disc inventory')
    result=dict(status='all_Japanese_music_loop_headers_independently_match_original_native_reader_metadata_only',runtime_checks=len(labels),assertions=labels,music_files=len(rows),looping_tracks=len(looping),tracks=rows,shortest_real_loop_candidate=looping[0],
        declared_music_IDs=len(declared),missing_declared_music_files=missing,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),production_code_changed=False,all79_runtime_loop_playback_verified=False,retail_data_included=False,playable=False,overall_completion_percent=10)
    d.mkdir();(d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(labels),'checks',flush=True)
if __name__=='__main__':main()
