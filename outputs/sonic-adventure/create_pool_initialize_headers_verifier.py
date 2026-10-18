from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_public_initialize_headers.py').read_text()
s=s.replace("[(0x8c65bb88,'sound_stream_work_update'","[(0x8c65bbd4,'sound_work_pool_update','sa_sound_work_pool_update'),(0x8c65bb88,'sound_stream_work_update'")
for old,new in [('0xa60000','0x8a3910'),('0xa60100','0x8a3a10'),('0xa60001','0x8a3911'),('0xa60004','0x8a3914'),('0x8ca60000','0x8c8a3910')]:s=s.replace(old,new)
s=s.replace(' work_before=bytes('," for slot in range(8):ram[0x8a3910+slot*0x90]=0\n ram[0x8a3910]=1\n pool_before=bytes(ram[0x8a3910:0x8a3d90])\n work_calls=[]\n work_before=bytes(")
s=s.replace('  if a==0x8c644be6:',"  if a==0x8c65bb88:work_calls.append(r[4]);assert r[4]==0x8c8a3910\n  if a==0x8c644be6:")
s=s.replace('s.pc=0x8c65bb88','s.pc=0x8c65bbd4').replace('assert lower[0x8c65bb88](','assert lower[0x8c65bbd4](')
s=s.replace(' assert native[0x8a3910:0x8a3a10]==wanted_work'," assert native[0x8a3910:0x8a3a10]==wanted_work\n wanted_pool=bytearray(pool_before)\n if success:wanted_pool[1]=2\n assert native[0x8a3910:0x8a3d90]==wanted_pool and work_calls==[0x8c8a3910]")
s=s.replace('public_initialize_headers_verification','pool_initialize_headers_verification').replace('public original header initialization comparisons passed','pool original header initialization comparisons passed')
s=s.replace('Public work update65bb88 state1','Original8-slot pool updater65bbd4 selects first active slot8c8a3910, public work update65bb88 state1').replace('No modeled calls.','No modeled calls. Independent exactly one public update call, entire1152-byte pool preserved except state1->2 on success; other slot active flags0.')
(p/'verify_pool_initialize_headers.py').write_text(s)
r=(p/'register_public_initialize_headers.py').read_text().replace('public_initialize_headers','pool_initialize_headers').replace("'sa_sound_stream_work_update'}","'sa_sound_stream_work_update','sa_sound_work_pool_update'}").replace('Original headers through public work initialization:','Original headers through native pool initialization:')
(p/'register_pool_initialize_headers.py').write_text(r)
