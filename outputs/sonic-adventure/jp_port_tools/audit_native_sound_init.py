"""Trace pinned sound initialization and inspect actual private JP audio inputs.

Uses the qualified native data transport/original ISO parser, independently
checks the selected payloads against raw-disc reads, and publishes metadata
only. Does not execute, replace, or claim to port the game's audio driver.
"""
import argparse
import ctypes as C
import hashlib
import json
import re
import struct
import subprocess
from pathlib import Path
from build_native_gdfs import functions
from verify_native_audio import bind,sha

PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'

def file_sha(path):
    h=hashlib.sha256()
    with Path(path).open('rb') as f:
        for raw in iter(lambda:f.read(8<<20),b''):h.update(raw)
    return h.hexdigest()

def digest(raw):return hashlib.sha256(raw).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','media','iso','media_check','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    m=json.loads((a.media/'build_verification.json').read_bytes());b=json.loads((a.iso/'build_verification.json').read_bytes());v=json.loads((a.media_check/'verification.json').read_bytes())
    assert sha(a.media/'build_verification.json')==v['build_verification_sha256'] and sha(a.iso/'build_verification.json')==v['ISO_build_verification_sha256']
    assert sha(m['DLL'])==m['DLL_sha256']==b['media_DLL_sha256'] and sha(b['DLL'])==b['DLL_sha256']
    for name,h in b['original_source_sha256'].items():assert sha(u/name)==h,name
    track=Path(v['media']['data_track']);assert file_sha(track)==v['media']['data_track_sha256']
    git='C:/Program Files/Git/cmd/git.exe';tree=subprocess.check_output([git,'-C',str(u),'ls-tree','-r',PIN],text=True)
    objects={line.split('\t',1)[1]:line.split()[2] for line in tree.splitlines()}
    source_paths=[u/'src/sound/sound.c',u/'src/system/loader.h',u/'sdk/include/shinobi.h',u/'sdk/shinobi/sd_work.h']+sorted((u/'sdk/shinobi').glob('sd_*.c'))
    originals={};providers={};bodies={}
    for path in source_paths:
        name=path.relative_to(u).as_posix();raw=path.read_bytes()
        assert hashlib.sha1(b'blob '+str(len(raw)).encode()+b'\0'+raw).hexdigest()==objects[name],name
        originals[name]=digest(raw)
        if path.suffix=='.c':
            for function,body in functions(raw).items():
                key=name+'::'+function;bodies[key]=body;providers.setdefault(function,[]).append(key)
    start='src/sound/sound.c::sound_system_init';queue=[start];reachable={};external=set()
    controls={'if','for','while','switch','sizeof','return','__asm__','asm'}
    while queue:
        key=queue.pop()
        if key in reachable:continue
        body=bodies[key];code=re.sub(rb'/\*.*?\*/|//[^\r\n]*|"(?:\\.|[^"\\])*"',b'',body.split(b'{',1)[1],flags=re.S)
        calls=sorted(set(re.findall(rb'\b([A-Za-z_]\w*)\s*\(',code)))
        calls=[x.decode() for x in calls if x.decode() not in controls]
        edges={name:providers.get(name,[]) for name in calls};source=key.split('::')[0]
        # Prefer static helpers in this translation unit when a name repeats.
        for name,candidates in edges.items():
            local=[p for p in candidates if p.startswith(source+'::')]
            if local:edges[name]=local
        reachable[key]=dict(function_sha256=digest(body),calls=edges,absolute_console_address_literals=sorted(set(x.decode() for x in re.findall(rb'\b0x[aAbB][0-9a-fA-F]{7}\b',body))))
        for name,candidates in edges.items():
            if candidates:queue.extend(candidates)
            else:external.add(name)
    media=C.CDLL(m['DLL']);iso=C.CDLL(b['DLL']);I=C.c_int32;U=C.c_uint32;P=C.c_void_p;S=C.c_size_t
    open_=bind(media,'jp_gd_media_open',I,C.c_wchar_p,U,U,U);request=bind(media,'jp_gd_media_request',I,U,U);wait=bind(media,'jp_gd_media_wait',I,U);view=bind(media,'jp_gd_media_view',I,P,P);release=bind(media,'jp_gd_media_release',I);close=bind(media,'jp_gd_media_close',I,U)
    listing=bind(iso,'peer_list',I,I,I,I,P,S);search=bind(iso,'peer_search',I,C.c_char_p,P,I,I)
    assert open_(str(track),45150,2352,8192)==0
    independent=track.open('rb')
    try:
        def read(fad,sectors):
            assert request(fad,sectors)==0 and wait(5000)==0
            pointer=P();size=S();assert view(C.byref(pointer),C.byref(size))==0 and size.value==sectors*2048
            raw=C.string_at(pointer,size.value);assert release()==0
            independent.seek((fad-45150)*2352);disc=independent.read(sectors*2352)
            assert len(disc)==sectors*2352 and raw==b''.join(disc[j*2352+16:j*2352+2064] for j in range(sectors))
            return raw
        cat=v['catalogues']['SONICADV'];table_bytes=16+1920*44;table=(C.c_ubyte*table_bytes)()
        assert listing(cat['fad'],cat['bytes'],1920,table,table_bytes)==0 and digest(bytes(table))==cat['table_sha256']
        entries=[]
        for j in range(cat['entries']):
            row=bytes(table[16+j*44:16+(j+1)*44]);sector,size,flag,sid=struct.unpack_from('<iiBB',row);name=row[10:42].split(b'\0',1)[0].decode('ascii')
            entries.append(dict(name=name,fad=sector+150,bytes=size,flag=flag,sid=sid))
        driver_entry=next(x for x in entries if x['name']=='MASARU.DRV');ordinal=search(b'masaru.drv',table,1920,0)
        assert ordinal>0 and entries[ordinal-1]==driver_entry
        driver=read(driver_entry['fad'],(driver_entry['bytes']+2047)//2048)[:driver_entry['bytes']]
        assert len(driver)>=32 and driver[:4]==b'SDRV';program_bytes=struct.unpack_from('<i',driver,8)[0]
        assert 0<program_bytes<=len(driver)-32 and driver[4]==1 and driver[5]<=1
        program=driver[32:32+program_bytes];private=d/'private_inputs';private.mkdir()
        (private/'MASARU.DRV').write_bytes(driver);(private/'MASARU.arm.bin').write_bytes(program)
        driver_profile=dict(directory=driver_entry,file_sha256=digest(driver),header_bytes=32,major=driver[4],minor=driver[5],program_bytes=program_bytes,program_sha256=digest(program),trailing_bytes=len(driver)-32-program_bytes,accepted_by_original_header_checks=True,executed=False)
        banks=[]
        for entry in entries:
            if not entry['name'].endswith('.MLT'):continue
            header=read(entry['fad'],1);assert header[:4]==b'SMLT'
            version=header[4];count=struct.unpack_from('<i',header,8)[0]
            at=16 if version<1 and struct.unpack_from('<i',header,16)[0]!=-1 else 32
            assert 0<count<=256 and at+count*32<=entry['bytes']
            sectors=(at+count*32+2047)//2048
            if sectors>1:header=read(entry['fad'],sectors)
            records=[]
            for j in range(count):
                fields=struct.unpack_from('<4s7i',header,at+j*32);kind,number,address,size,offset,length=fields[:6]
                # The actual SDK downloads only positive data_size. Retail
                # allocation-only records use (-1,-1) for offset/data_size.
                assert length<=0 or offset>=0 and offset+length<=entry['bytes']
                assert size<0 or 0<=address and address+size<=2<<20
                records.append(dict(type=kind.decode('ascii'),number=number,sound_memory_offset=address,allocation_bytes=size,data_offset=offset,data_bytes=length,downloads_payload=length>0))
            banks.append(dict(**entry,version=version,bank_records=records))
        assert banks
    finally:
        independent.close();assert close(5000)==0
    assert file_sha(track)==v['media']['data_track_sha256']
    for name,h in originals.items():assert sha(u/name)==h,name
    old=r/'work/sonic-adventure/extracted/files/SONICADV/MASARU.DRV'
    comparison=dict(USA_file_present=old.exists(),USA_driver_sha256=sha(old) if old.exists() else None)
    comparison['JP_USA_driver_bytes_identical']=old.exists() and old.read_bytes()==driver
    report=dict(status='pinned_original_sound_init_call_graph_actual_JP_driver_and_bank_headers_audited_not_executed',upstream_revision=PIN,
        original_source_sha256=originals,reachable_literal_call_graph=reachable,reachable_functions=len(reachable),external_call_tokens_to_review=sorted(external),
        actual_Japanese_driver=driver_profile,JP_USA_comparison=comparison,actual_Japanese_multi_units=banks,multi_unit_files=len(banks),
        media_inputs=v['media'],media_check_sha256=sha(a.media_check/'verification.json'),native_directory_and_payload_readback_matches_independent_raw_IO=True,
        parent_missing_export='sound_system_init',parent_linked_or_executed=False,
        required_next_steps=['Choose and qualify an actual portable ARM7/AICA execution and sound-memory/register provider.',
            'Replace WORK_AREA with owned native staging; retain driver and bank payloads as local inputs.',
            'Adapt original sound SDK work/commands/driver bootstrap with full-width host handles and original console data offsets.',
            'Resolve original final sdDrvInit(block) after sdMemBlkDestroy behavior from reference evidence before adaptation.',
            'Clock driver/bank output into qualified native PCM transport, then qualify real MIDI effects, streaming audio and ordered exit.'],
        original_inputs_modified=False,retail_bytes_in_report=False,driver_emulated=False,game_sound_verified=False,
        auditor_sha256=sha(__file__),overall_completion_percent=10)
    (d/'dependency_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],'functions',len(reachable),'MLT files',len(banks),'driver bytes',len(program),flush=True)

if __name__=='__main__':main()
