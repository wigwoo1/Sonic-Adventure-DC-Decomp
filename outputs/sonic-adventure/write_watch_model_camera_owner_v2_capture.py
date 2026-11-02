"""Read-only evidence of the actual sprite command buffer; comparisons unchanged."""
import ctypes
import write_watch_model_camera_owner_v2_state as previous
code=previous.code
marker="  boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15]),arguments=list(r[4:8]),sr=sr[0]))";assert code.count(marker)==1
code=code.replace(marker,marker+'''
  if target==0x8c6065b0:
   def read32(a):
    assert a&0x1f000000==0x0c000000 and a&0xffffff<=0x1000000-4
    return int.from_bytes(ctypes.string_at(ctypes.addressof(mem.contents)+(a&0xffffff),4),'little')
   words=[read32(0x8c890104+4*i) for i in range(8)]
   boundaries[-1]['sprite_header_input']=dict(record_address='0x8c890104',words=words,material_words=[read32(r[4]+4*i) for i in range(4)],qacr=list(memory.vertex.qacr))
  if target==0x8c6069a4:
   boundaries[-1]['sprite_end_qacr']=list(memory.vertex.qacr)
''')
marker=' metrics=journal.finish()';assert code.count(marker)==1
code=code.replace(marker,'''
 sprite_buffers=[]
 starts=[b for b in boundaries if b['target']=='0x8c6065b0']
 ends=[b for b in boundaries if b['target']=='0x8c6069a4']
 for begin,end in zip(starts,ends):
  info=begin['sprite_header_input'];sq=0xe0000000|(info['words'][5]&0x3ffffff);last=end['arguments'][0]
  assert sq<=last<=sq+0x10000
  address=((end['sprite_end_qacr'][(sq>>5)&1]&0x1c)<<24)|(sq&0x3ffffe0)
  assert address&0x1f000000==0x0c000000 and address&0xffffff<=0x1000000-(last-sq)
  raw=bytes(actual[address&0xffffff:(address&0xffffff)+(last-sq)])
  sprite_buffers.append(dict(ram_address=hex(0x80000000|address),store_queue_start=hex(sq),store_queue_end=hex(last),bytes=len(raw),words=[int.from_bytes(raw[i:i+4],'little') for i in range(0,len(raw),4)]))
'''+marker)
marker=' return dict(';assert code.count(marker)==1
code=code.replace(marker,' return dict(sprite_command_buffers=sprite_buffers,vertex_events=[list(x) for x in trace],vertex_queue=list(memory.vertex.queue),vertex_counts=[memory.vertex.invalidates,memory.vertex.prefetches,memory.vertex.flushes],')
namespace=dict(previous.namespace,ctypes=ctypes)
exec(compile(code,'read_only_sprite_command_capture','exec'),namespace,local:={})
compare=local['compare']
