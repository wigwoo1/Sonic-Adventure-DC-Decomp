from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_magic_lifecycle_default_callbacks.py').read_text()
s=s.replace("input_default=extra('sound_stream_default_input.dll','sa_sound_stream_default_input')","input_default=extra('sound_stream_default_input.dll','sa_sound_stream_default_input')\nsource_set=extra('sound_stream_source_set.dll','sa_sound_stream_source_set')")
s=s.replace('    ram[0xa7000e]=2','    ram[0xa7000e]=2;ram[0xa7000f]=18')
s=s.replace('(0x54,16)','(0x54,0x55555555)')
s=s.replace('in [(0x8c66768c,input_set,','in [(0x8c66771c,source_set,0x8ca10000,len(payload)),(0x8c66768c,input_set,')
s=s.replace("    assert struct.unpack_from('<I',native,0xa70004)[0]==1", "    assert struct.unpack_from('<2I',native,0xa70050)==(0x8ca10000,16)\n    assert all(struct.unpack_from('<I',native,0xa70000+offset)[0]==0 for offset in [0x84,0xa0,0xa4])\n    assert struct.unpack_from('<I',native,0xa70004)[0]==1")
s=s.replace('magic_lifecycle_default_callbacks_verification','magic_lifecycle_native_source_verification').replace('Native default input667384 and output6673a2,callback registration','Native source setter66771c derives16 units from288 encoded bytes and resets84/a0/a4 (independent assertions),default input667384 and output6673a2,callback registration').replace('default-callback original-audio lifecycle','native-source original-audio lifecycle')
(p/'verify_magic_lifecycle_native_source.py').write_text(s)
r=(p/'register_magic_lifecycle_default_callbacks.py').read_text().replace('magic_lifecycle_default_callbacks','magic_lifecycle_native_source').replace("'sa_sound_stream_update_dispatch'}","'sa_sound_stream_update_dispatch','sa_sound_stream_source_set'}").replace('Default callbacks original-audio stream lifecycle:','Native source setup original-audio lifecycle:')
(p/'register_magic_lifecycle_native_source.py').write_text(r)
