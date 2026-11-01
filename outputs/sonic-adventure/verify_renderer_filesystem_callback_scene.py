"""Submit actual texture-backed particle commands through the TA DMA endpoint."""
from pathlib import Path
P=Path(__file__).resolve().parent;builder=P/'verify_renderer_frame_submit.py';source=builder.read_text()
ending="exec(compile(code,str(parent),'exec'),globals())";assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())
code=code.replace('import renderer_frame_submit_fixtures as q','import renderer_filesystem_callback_fixtures as q')
code=code.replace('renderer_frame_submit_verification.json','renderer_filesystem_callback_scene_verification.json').replace('renderer_frame_submit_frontier.json','renderer_filesystem_callback_scene_frontier.json')
marker="  result,path,m,s,f=q.fast.compare(m,s,f,c)";assert code.count(marker)==1
code=code.replace(marker,"  if entry==0x8c6050b0:c['ticks']=list(q.FRAME_TICKS)\n"+marker)
code=code.replace("R/'work/sonic-adventure/build/renderer_context_owner.dll'","R/'work/sonic-adventure/build/renderer_filesystem_callback.dll'")
marker="P/'write_watch_renderer_frame_submit.py'";assert code.count(marker)==1
code=code.replace(marker,marker+",P/'renderer_filesystem_callback_fixtures.py',P/'renderer_filesystem_callback_reference.py',P/'write_watch_renderer_filesystem_callback.py',P/'renderer_frame_ta_dma_reference.py',P/'analysis/renderer_filesystem_callback_connection.json',P/'verify_renderer_frame_submit.py'")
code=code.replace('native_DLL_unchanged=True,','native_DLL_unchanged=False,native_game_C_unchanged=True,native_backend_is_additive_extension=True,')
marker="base[1].r[15]=0x8cfff000;records=[]";assert code.count(marker)==1
code=code.replace(marker,"base[1].r[15]=0x8cfff000;base[1].sr&=~0xf0;base[3]['vbr']=0x8ce50000;q.v.bind_vectors(0x8ce50000);records=[]")
marker=" phase(0x8c602128,[0x8cb00000,0x100000],";assert code.count(marker)==1
code=code.replace(marker," phase(0x8c645de0,[0,0x8ce40000],'actual original vector initialization')\n"+marker)
marker="  if entry==0x8c6050b0:c['ticks']=list(q.FRAME_TICKS)";assert code.count(marker)==1
code=code.replace(marker,"  if entry==0x8c6050b0:c.update(ticks=list(q.FRAME_TICKS),runtime_attached=True,irq_samples=[8]*256)")
marker="  c.update(graphics_in=result['graphics_out']";assert code.count(marker)==1
code=code.replace(marker,"  c['frame_events_seed']=result['frame_events_state']\n  if result['runtime_state'] is not None:c['runtime_seed']=result['runtime_state']\n  for name,value in zip(q.v.EXCEPTION_KEYS,result['exception_state']):c[name]=value\n"+marker)
marker="records.append(dict(label=label,";assert code.count(marker)==1
code=code.replace(marker,"records.append(dict(frame_events_state=result['frame_events_state'],runtime_state=result['runtime_state'],exception_state=result['exception_state'],SDK_watchdog_calls=sum(b['target']=='0x8c650b60' and b['pr'] in ('0x8c651d92','0x8c651eae') for b in result['boundaries']),list_callback_calls=sum(b['target']=='0x8c651f1e' for b in result['boundaries']),IRQ_vector_calls=path.count(0x8ce50600),graphics_handler_calls=path.count(0x8c652210),VBlank_handler_calls=path.count(0x8c651bc0),frame_busy_flag=q.get(m,0x8c8a30c0),label=label,")
code=code.replace('build/renderer_filesystem_callback.dll','build/renderer_filesystem_callback.dll')
exec(compile(code,str(parent),'exec'),globals())
