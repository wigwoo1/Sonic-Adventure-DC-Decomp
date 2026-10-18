from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_magic_stream_lifecycle.py').read_text()
needle="start_stream=extra('sound_stream_start.dll','sa_sound_stream_start')"
s=s.replace(needle,needle+'''
input_provider=extra('sound_stream_input_provider.dll','sa_sound_stream_input_provider')
channel_get=extra('sound_stream_channel_count_get.dll','sa_sound_stream_channel_count_get')
position_get=extra('sound_stream_position_get.dll','sa_sound_stream_position_get')
buffer_get=extra('sound_source_buffer_get.dll','sa_sound_source_buffer_get')
query=extra('sound_source_query.dll','sa_sound_source_query')
lock_enter=extra('sound_source_lock_enter.dll','sa_sound_source_lock_enter')
lock_exit=extra('sound_source_lock_exit.dll','sa_sound_source_lock_exit')''')
needle='    stream_before=bytes(ram[0xa70000:0xa70100]);actual=[];expected=[]'
s=s.replace(needle,'''    ram[0xa7000e]=2
    ram[0xa90000:0xa90090]=bytes([0x55])*144
    struct.pack_into('<I',ram,0xa90004,0x8ca70000)
    struct.pack_into('<2I',ram,0xa9000c,0x8ca80000,0x8ca80100)
    struct.pack_into('<3I',ram,0xa90038,256,256,0)
    for address in [0xa80000,0xa80100]:
        ram[address:address+48]=bytes([0x77])*48
        struct.pack_into('<I',ram,address,0x8caa0000)
        struct.pack_into('<8I',ram,address+0xc,0,512,0,0,0x8ca10000,1024,0,0)
    struct.pack_into('<I',ram,0xaa0018,0x8c65e4b8)
    source_before=[bytes(ram[a:a+48]) for a in [0xa80000,0xa80100]]
    work_before=bytes(ram[0xa90000:0xa90090])
'''+needle)
start=s.index('        if address==0x8c010100:')
end=s.index('    def hook(address):',start)
s=s[:start]+"        assert address==0x8c010200 and args[:3]==(0x87654321,288,256)\n"+s[end:]
s=s.replace('if address in [0x8c010100,0x8c010200]:','if address==0x8c010200:')
s=s.replace('0x8c667900:sample_copy}', '0x8c667900:sample_copy,0x8c65b88c:input_provider,0x8c6676ae:channel_get,0x8c6676a0:position_get,0x8c65e798:buffer_get,0x8c65e4b8:query,0x8c65e368:lock_enter,0x8c65e380:lock_exit}')
s=s.replace('input_set,0x8c010100,0x12345678','input_set,0x8c65b88c,0x8ca90000')
s=s.replace('(0x8c010100,0x12345678,0x8c010200,0x87654321)','(0x8c65b88c,0x8ca90000,0x8c010200,0x87654321)')
s=s.replace('[0x8c010100,0x8c010200]','[0x8c010200]')
needle='    assert native[0xa70000:0xa70100]==wanted_stream'
s=s.replace(needle,needle+'''
    for index,address in enumerate([0xa80000,0xa80100]):
        wanted_source=bytearray(source_before[index])
        struct.pack_into('<2I',wanted_source,0x10,0,512)
        assert native[address:address+48]==wanted_source
    wanted_work=bytearray(work_before)
    for index in range(2):struct.pack_into('<2I',wanted_work,0x1c+index*8,0x8ca10000,512)
    assert native[0xa90000:0xa90090]==wanted_work''')
s=s.replace('magic_stream_lifecycle_verification','magic_lifecycle_native_input_verification')
s=s.replace('only input/output callbacks modeled, all internal calls native.','native input provider65b88c, both native source queries65e4b8, getters, interrupt-mask save/restore and signed remainder; only output callback modeled, all input/internal calls native. Independent two source objects consume512 bytes and advance ring positions to512; input-provider work queries and guards checked.')
s=s.replace('native original-audio stream lifecycle comparisons','native-input original-audio lifecycle comparisons')
(p/'verify_magic_lifecycle_native_input.py').write_text(s)
r=(p/'register_magic_stream_lifecycle.py').read_text().replace('magic_stream_lifecycle','magic_lifecycle_native_input').replace("'sa_sound_stream_start'}","'sa_sound_stream_start','sa_sound_stream_input_provider','sa_sound_stream_channel_count_get','sa_sound_stream_position_get','sa_sound_source_buffer_get','sa_sound_source_query','sa_sound_source_lock_enter','sa_sound_source_lock_exit'}").replace('Native original-audio stream lifecycle:','Native input original-audio stream lifecycle:').replace('File-dispatch stereo pipeline','Native input audio lifecycle')
(p/'register_magic_lifecycle_native_input.py').write_text(r)
