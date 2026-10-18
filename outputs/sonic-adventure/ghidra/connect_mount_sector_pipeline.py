from pathlib import Path
p=Path(__file__).resolve().parents[1]
t=(p/'verify_file_read_sectors_connected.py').read_text().replace('file_read_sectors.dll','file_mount_sector_read.dll').replace('cleanup=lib.sa_file_read_sectors','cleanup=lib.sa_file_mount_sector_read')
t=t.replace("[(0x8c601084", "[(0x8c600d82,'sa_file_reserved_handle_acquire','file_reserved_handle'),(0x8c600f1e,'sa_file_read_sectors','file_read_sectors'),(0x8c601084")
t=t.replace('[0,8,10],[0,4,20]','[0],[0,1,4,20]')
t=t.replace(" struct.pack_into('<H',ram,0xa0004c,active)", " struct.pack_into('<I',ram,0x673414,0x8ca01000);struct.pack_into('<I',ram,0xa01014,0x8ca00000)\n struct.pack_into('<H',ram,0xa0004c,active)")
t=t.replace('s.r[4]=0x8ca00000 if present else 0','s.r[4]=100').replace('s.pc=0x8c600f1e','s.pc=0x8c5ffc98')
t=t.replace('effective=min(request,10-position) if position<10 else 0','effective=request')
t=t.replace('file_read_sectors_connected_verification.json','file_mount_sector_read_connected_verification.json').replace('Native sector-read wrapper,','Native mount sector bridge, reserved-handle acquisition, sector-read wrapper,').replace('EOF, zero requests, clamping, alignment and banks','zero/positive sector counts, alignment and banks').replace('connected sector read comparisons passed','connected mount sector read comparisons passed')
(p/'verify_file_mount_sector_read_connected.py').write_text(t)
