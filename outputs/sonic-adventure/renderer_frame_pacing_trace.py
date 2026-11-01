"""Read-only attribution of original RAM stores; no instruction semantics changed."""
FIELDS=(0x8c8a30bc,0x8c8a30c0,0x8c8a2f78,0x8c8a2f7c,0x8c88f7d0,0x8c88f7d8)
def attach(namespace):
 original=namespace['reference'];rows=namespace['ROWS'];access=namespace['dynamic_access'];records=[];pending=[]
 blocks={field&0xfffff0 for field in FIELDS}
 def flush():
  if not pending:return
  memory,pc,a,n,before=pending.pop()
  after=int.from_bytes(memory[a&0xffffff:(a&0xffffff)+n],'little')
  records.append(dict(pc=hex(pc),address=hex(a),width=n,before=before,after=after,instruction=rows[pc]))
 def reference(memory,state,*args,**kwargs):
  flush();trace=kwargs.get('trace')
  if trace is not None:
   def traced(pc,w,r,sr,pr,fpu):
    flush();trace(pc,w,r,sr,pr,fpu)
    instruction=rows.get(pc,'')
    if not instruction.startswith(('mov.','fmov.')):return
    operands=instruction.split(' ',1)[1]
    if operands.split(',')[0].startswith('@'):return
    # The accepted observer computes effective addresses before instruction
    # execution, including predecrement. Only ordinary RAM stores are observed.
    found=access(instruction,r,kwargs['fpu'].fpscr,kwargs.get('special',{}))
    if found:
     a,n=found
     if a&0x1f000000==0x0c000000 and a&0xfffff0 in blocks and any((a&0xffffff)<=(field&0xffffff)<(a&0xffffff)+n or (field&0xffffff)<=(a&0xffffff)<(field&0xffffff)+4 for field in FIELDS):
      pending.append((memory,pc,a,n,int.from_bytes(memory[a&0xffffff:(a&0xffffff)+n],'little')))
   kwargs['trace']=traced
  try:return original(memory,state,*args,**kwargs)
  finally:flush()
 namespace['reference']=reference
 return records
