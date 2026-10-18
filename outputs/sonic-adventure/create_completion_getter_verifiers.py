from pathlib import Path
root=Path(__file__).resolve().parent
items=[('sound_stream_total_samples_get','8c6676f6','8c6676fa','18'),('sound_stream_decoded_bytes_get','8c6677b6','8c6677bc','a4'),('sound_stream_decoded_samples_get','8c6677d8','8c6677de','a0')]
for name,start,end,offset in items:
    text=(root/'verify_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',name).replace('0x8c65e798','0x'+start).replace('0xa2001c','0xa200'+offset).replace('0xa20040','0xa20100').replace('*64','*256').replace('all64','all256').replace('two-instruction source buffer getter8c65e798','stream completion getter'+start)
    (root/f'verify_{name}.py').write_text(text)
    text=(root/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',name)
    (root/f'build_{name}.ps1').write_text(text)
    text=(root/'register_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',name).replace('0x8c65e798','0x'+start).replace('0x8c65e79c','0x'+end).replace('0x64e798','0x'+format(int(start,16)-0x8c010000,'x')).replace('0x64e79c','0x'+format(int(end,16)-0x8c010000,'x'))
    (root/f'register_{name}.py').write_text(text)
    path=root/f'source/sa_{name}.c'
    text=path.read_text().replace('retail Tikal helper','retail stream completion getter')
    path.write_text(text)
