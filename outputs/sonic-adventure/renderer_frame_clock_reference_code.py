"""Extend the unchanged instruction observer with an explicit scheduling domain."""
def extend(code):
 def change(a,b,count=1):
  nonlocal code
  assert code.count(a)==count,(a,code.count(a));code=code.replace(a,b)
 marker=" if flash_reference is not None else None\n def stop("
 change(marker," if flash_reference is not None else None,(queue_reference.frame_clock.snapshot() if getattr(queue_reference,'frame_clock',None) is not None else None)\n def stop(")
 change('if queue_reference.scan_cursor>=len(queue_reference.scan_samples):','if getattr(queue_reference,\'frame_clock\',None) is None and queue_reference.scan_cursor>=len(queue_reference.scan_samples):')
 change('if queue_reference.clock_cursor>=len(queue_reference.ticks):','if getattr(queue_reference,\'frame_clock\',None) is None and queue_reference.clock_cursor>=len(queue_reference.ticks):')
 marker='  if runtime is None or not runtime.enabled or runtime.in_irq:return sr,pc,pr'
 change(marker,"""  clock=getattr(queue_reference,'frame_clock',None)
  if clock is not None and pc==0x8c6050b0 and (runtime is None or not runtime.in_irq):
   rc=clock.next_in(0)
   if rc:stop(pc,regs,sr,rc)
"""+marker
 )
 # This additional PC is an explicit host entry, not a guest polling read.
 change('if not runtime.pending and pc not in (0x8c642060,0x8c6420a0):','if not runtime.pending and pc not in (0x8c6050b0,0x8c642060,0x8c6420a0):')
 change('(0x8c65100a,0x8c642060,0x8c6420a0)','(0x8c6050b0,0x8c65100a,0x8c642060,0x8c6420a0)',2)
 change('pending=queue_reference[0xa05f6900] if pc in (0x8c642060,0x8c6420a0)','pending=queue_reference[0xa05f6900] if pc in (0x8c6050b0,0x8c642060,0x8c6420a0)')
 marker='  if not pending&mask:return sr,pc,pr'
 change(marker,"""  if clock is not None and pc in (0x8c642060,0x8c6420a0) and not pending&mask:
   rc=clock.next_in(1)
   if rc:stop(pc,regs,sr,rc)
   mask=queue_reference.base_read(0xa05f6930)
   pending=queue_reference[0xa05f6900]
"""+marker)
 return code
