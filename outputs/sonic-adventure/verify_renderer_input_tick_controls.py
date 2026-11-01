"""Exercise each error bit through the device that actually owns its register."""
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'verify_renderer_input_tick_mmio.py'
code=parent.read_text()
code=code.replace('renderer_input_tick_mmio_verification.json','renderer_input_tick_controls_verification.json')
marker="  arrays=list(cfg['graphics_in'].arrays)";assert code.count(marker)==1
code=code.replace(marker,"""  if address==0x5f6908:
   # The outer PVR bus owns 6908, including its write-one-to-clear behavior.
   # Seed that declared external-device status, not the shadow startup array.
   state=cfg['graphics_in'];pvr=list(state.pvr);error=list(pvr[-1]);error[0]=value;pvr[-1]=tuple(error)
   cfg['graphics_in']=dataclasses.replace(state,pvr=tuple(pvr));return
"""+marker)
code=code.replace('(0,0xf00)','(0,0x100,0x200,0x400,0x800,0xf00)')
marker="  assert bytes(fp)==before_f;checks+=1";assert code.count(marker)==1
code=code.replace(marker,marker+"""
  if case['kind'].startswith('submission'):
   observed=r['graphics_out'].pvr_error_events
   assert observed and observed[0]==(0xa05f6908,case.get('error',0),1);checks+=1
   assert r['graphics_out'].pvr[-1][0]==0;checks+=1
""")
marker=' files=[Path(__file__),';assert code.count(marker)==1
code=code.replace(marker,' files=[Path(__file__),parent,')
exec(compile(code,str(parent),'exec'),globals())
