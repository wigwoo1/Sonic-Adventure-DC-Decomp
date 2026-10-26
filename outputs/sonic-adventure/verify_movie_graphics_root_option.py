"""Fixed-input original string-helper proof for the root's immutable option call.

This is verification-only execution, separate from production C and the existing
SH4 reference. Unknown initial registers are rejected on read. All instruction
bytes, literals and string reads are checked against the local retail image.
"""
import csv,hashlib,json,re
from pathlib import Path
import verify_movie_graphics_mode_infeasible as cfg
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
BASE=0x8c010000;ENTRY=0x8c665a80;RETURN=0x8cfed000;STACK=0x8cf00000

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 retail=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';image=retail.read_bytes()
 listings=[P/'analysis/movie-render-config-dependencies/8c665a80_instructions.tsv',P/'analysis/movie-render-config-lower/8c10d3c8_instructions.tsv',P/'analysis/movie-render-config-lower/8c10db1c_instructions.tsv',P/'analysis/movie-command-handoff-next/8c656940_instructions.tsv']
 rows={}
 for listing in listings:
  for row in csv.DictReader(listing.open(),delimiter='\t'):
   a=int(row['address'],16);assert image[a-BASE:a-BASE+2]==bytes.fromhex(row['bytes']);rows[a]=row
 def word(a):return int.from_bytes(image[a-BASE:a-BASE+4],'little')
 assert [word(a) for a in (0x8c656f98,0x8c656f94,0x8c656f9c,0x8c665ac0,0x8c665ac4,0x8c665ac8,0x8c10d3d8)]==[0x8c671d98,0x8c671da4,ENTRY,0x8c672534,0x8c10d3c8,0x8c672540,0x8c10db1c]
 def string(a):return image[a-BASE:].split(b'\0',1)[0]
 assert string(0x8c671d98)==string(0x8c672534)==b'HARDWARE'
 assert string(0x8c671da4)==string(0x8c672540)==b'EnableTexture16MB'
 r=[None]*16;r[4]=0x8c671d98;r[5]=0x8c671da4;r[6]=0;r[15]=STACK
 pr=RETURN;t=None;pc=ENTRY;pending=None;memory={};trace=[];reads=[];calls=[]
 def reg(token):
  index=int(token[1:]);assert r[index] is not None,('Unknown live-in',token,hex(pc));return r[index]
 def setreg(token,value):r[int(token[1:])]=value&0xffffffff
 def read(a,n):
  reads.append([a,n]);result=0
  for i in range(n):
   address=a+i
   if address in memory:value=memory[address]
   else:assert BASE<=address<BASE+len(image),hex(address);value=image[address-BASE]
   result|=value<<(8*i)
  return result
 def write(a,value):
  assert STACK-256<=a<=STACK-4,'All writes must be private stack, disjoint from retail bytes'
  for i in range(4):memory[a+i]=(value>>(8*i))&255
 while pc!=RETURN:
  assert len(trace)<2048 and pc in rows;trace.append(pc);ins=rows[pc]['instruction'].lstrip('_');op,_,args=ins.partition(' ');target=None;delayed=False
  if op=='nop':pass
  elif ins=='sts.l PR,@-r15':r[15]-=4;write(r[15],pr)
  elif ins=='lds.l @r15+,PR':pr=read(r[15],4);r[15]+=4
  elif op=='jsr':target=reg(args[1:]);pr=pc+4;delayed=True;calls.append(target)
  elif op=='rts':target=pr;delayed=True
  elif op in ('bt','bf','bra'):
   assert op=='bra' or t is not None
   if op=='bra' or t==(op=='bt'):target=int(args,16);delayed=op=='bra'
  elif op=='add':a,b=args.split(',');setreg(b,reg(b)+int(a[1:],0))
  elif op=='or':a,b=args.split(',');setreg(b,reg(a)|reg(b))
  elif op=='sub':a,b=args.split(',');setreg(b,reg(b)-reg(a))
  elif op=='tst':a,b=args.split(',');t=((int(a[1:],0) if a.startswith('#') else reg(a))&reg(b))==0
  elif op in ('cmp/eq','cmp/str'):
   a,b=args.split(',');x=int(a[1:],0) if a.startswith('#') else reg(a);y=reg(b)
   t=x==y if op=='cmp/eq' else any((x>>(8*i)&255)==(y>>(8*i)&255) for i in range(4))
  elif op=='mov':a,b=args.split(',');setreg(b,int(a[1:],0) if a.startswith('#') else reg(a))
  elif op in ('mov.l','mov.b'):
   width=4 if op=='mov.l' else 1
   literal=re.fullmatch(r'(0x[0-9a-f]+),(r\d+)',args)
   post=re.fullmatch(r'@(r\d+)\+,(r\d+)',args)
   load=re.fullmatch(r'@(r\d+),(r\d+)',args)
   slotload=re.fullmatch(r'@\((0x[0-9a-f]+),(r\d+)\),(r\d+)',args)
   slotstore=re.fullmatch(r'(r\d+),@\((0x[0-9a-f]+),(r\d+)\)',args)
   store=re.fullmatch(r'(r\d+),@(r\d+)',args)
   if literal:a,b=literal.groups();setreg(b,read(int(a,16),width))
   elif post or load:
    a,b=(post or load).groups();value=read(reg(a),width)
    if post:setreg(a,reg(a)+width)
    setreg(b,value-256 if width==1 and value&128 else value)
   elif slotload:off,a,b=slotload.groups();setreg(b,read(reg(a)+int(off,16),width))
   elif slotstore:a,off,b=slotstore.groups();write(reg(b)+int(off,16),reg(a))
   elif store:a,b=store.groups();write(reg(b),reg(a))
   else:raise AssertionError(ins)
  else:raise AssertionError(ins)
  if pending is not None:
   assert target is None,'Branch in delay slot';pc=pending;pending=None
  elif delayed:assert target is not None;pending=target;pc+=2
  else:pc=target if target is not None else pc+2
 assert r[0]==0 and r[15]==STACK and pr==RETURN
 assert calls==[0x8c10d3c8,0x8c10db1c,0x8c10d3c8,0x8c10db1c]
 root={a:v for a,v in rows.items() if 0x8c656940<=a<0x8c656f50}
 expected=['mov.l 0x8c656f98,r4','sub r1,r3','mov.l @(r0,r13),r1','mov #0x58,r0','mov.l 0x8c656f94,r5','shll r1','sub r1,r3','shlr r3','mov.l r3,@(r0,r13)','mov.l 0x8c656f9c,r3','jsr @r3','mov #0x0,r6','cmp/eq #0x1,r0','bf 0x8c656e62']
 assert [cfg.text(root,a) for a in range(0x8c656e38,0x8c656e54,2)]==expected
 g,p=cfg.graph(root,0x8c656940);dom=cfg.dominators(g,p,0x8c656940)
 assert cfg.dominates(dom,p,0x8c656e52,0x8c656e4c)
 forbidden=[(0x8c656e52,0x8c656e54)];restricted,payload=cfg.graph(root,0x8c656940,forbidden)
 dead=set(root)-set(payload.values());assert dead==set(range(0x8c656e54,0x8c656e62,2))
 report=dict(original_instructions_byte_checked=len(rows),original_helper_steps=len(trace),helper_result=0,unknown_register_reads=0,stack_restored=True,helper_calls=[hex(x) for x in calls],helper_trace=[hex(x) for x in trace],memory_reads=[[hex(a),n] for a,n in reads],infeasible_edges=[[hex(a),hex(b)] for a,b in forbidden+[(0x8c656e5c,0x8c656e5e),(0x8c656e5c,0x8c656e6e)]],infeasible_rows=[hex(a) for a in sorted(dead)],native_source_changed=False,additional_recovery_credit=0,assumptions=['Full entry, fixed immutable original code/literals/strings','Valid mapped aligned stack disjoint from code, literals and strings','No asynchronous mutation or stack/data aliases that overwrite immutable inputs','Actual root call arguments as checked; helpers execute original instructions'],sha256={str(p):digest(p) for p in [Path(__file__),Path(cfg.__file__),retail]+listings},scope='Verification-only fixed-input execution and delay-slot CFG proof; production helpers/branches unchanged. Unknown initial registers rejected on read; no helper return injected. Proves seven root rows and three conditional outcomes infeasible within stated domain. Root remains partial and receives no registration credit; not full startup or physical hardware proof.')
 (P/'analysis/movie_graphics_root_option_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k not in ('sha256','helper_trace','memory_reads')})
if __name__=='__main__':main()
