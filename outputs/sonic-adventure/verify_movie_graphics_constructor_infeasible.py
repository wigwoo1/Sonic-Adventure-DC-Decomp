"""Original constructor option/pool invariants, with explicit delay-slot CFG.

Normal-return proof: finite mapped nonaliasing cleanup lists, private stack,
immutable retail code/literals/strings and no concurrent mutation. No production
branch, descriptor result or helper return is changed or injected.
"""
import csv,hashlib,json
from pathlib import Path
import verify_movie_graphics_root_option as option
import verify_movie_graphics_mode_infeasible as cfg
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def replay_option(arguments):
 # Reuse only the checked verification evaluator, stopping before its root CFG
 # and report writer. Older public source/report bytes remain intact.
 source=Path(option.__file__).read_text();body=source.split('def main():\n',1)[1].split(' root={a:v for a,v in rows.items()',1)[0]
 marker='r=[None]*16;r[4]=0x8c671d98;r[5]=0x8c671da4;r[6]=0;r[15]=STACK';assert body.count(marker)==1
 body=body.replace(marker,'r=[None]*16;r[4]=arguments[0];r[5]=arguments[1];r[6]=0;r[15]=STACK')
 body+=' return dict(result=r[0],steps=len(trace),trace=[hex(a) for a in trace],reads=[[hex(a),n] for a,n in reads],stack_restored=r[15]==STACK)\n'
 namespace=dict(option.__dict__);exec(compile('def replay(arguments):\n'+body,'constructor_original_option_evaluator','exec'),namespace);return namespace['replay'](arguments)
def main():
 retail=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';image=retail.read_bytes()
 files=[Path(__file__),Path(option.__file__),Path(cfg.__file__),retail]
 paths={0x8c663140:P/'analysis/movie-driver-services/8c663140_instructions.tsv',0x8c663080:P/'analysis/movie-driver-registers/8c663080_instructions.tsv',0x8c664ac0:P/'analysis/movie-driver-registers/8c664ac0_instructions.tsv',0x8c665440:P/'analysis/movie-driver-register-lower/8c665440_instructions.tsv'};rows={}
 for entry,path in paths.items():
  body={int(r['address'],16):r for r in csv.DictReader(path.open(),delimiter='\t')};rows[entry]=body;files.append(path)
  for address,row in body.items():assert image[address-0x8c010000:address-0x8c010000+2]==bytes.fromhex(row['bytes'])
 def literal(a):return int.from_bytes(image[a-0x8c010000:a-0x8c010000+4],'little')
 def sequence(body,start,end,expected):assert [cfg.text(body,a) for a in range(start,end,2)]==expected
 ctor,clear,scan,release=(rows[a] for a in paths)
 assert [literal(a) for a in (0x8c663200,0x8c663204,0x8c663208,0x8c6632e4,0x8c6632e8,0x8c6632ec)]==[0x8c672520,0x8c672514,0x8c665a80,0x8c672520,0x8c672514,0x8c665a80]
 replay=replay_option([0x8c672514,0x8c672520]);assert replay['result']==0 and replay['stack_restored']
 sequence(ctor,0x8c663166,0x8c663174,['mov.l 0x8c663208,r1','mov.l 0x8c663204,r4','mov.l 0x8c663200,r5','jsr @r1','mov #0x0,r6','cmp/eq #0x1,r0','bf 0x8c663194'])
 sequence(ctor,0x8c663220,0x8c66322e,['mov.l 0x8c6632ec,r2','mov.l 0x8c6632e8,r4','mov.l 0x8c6632e4,r5','jsr @r2','mov #0x0,r6','cmp/eq #0x1,r0','bf 0x8c66324c'])
 base,size=literal(0x8c663128),literal(0x8c66312c);assert (base,size)==(0x8c8a83e4,0x18000) and (literal(0x8c664b80),literal(0x8c664b84))==(base,size)
 sequence(clear,0x8c6630d0,0x8c6630e2,['mov.l 0x8c66312c,r5','mov #0x0,r14','mov.l 0x8c663128,r6','add r6,r5','mov r6,r4','mov.w r14,@r4','add #0x18,r4','cmp/hs r5,r4','bf 0x8c6630da'])
 count=size//24;assert count==4096 and size%24==0 and base+size<=0xffffffff
 addresses=list(range(base,base+size,24));assert len(addresses)==count and addresses[-1]+24==base+size
 cg,cp=cfg.graph(clear,0x8c663080);cd=cfg.dominators(cg,cp,0x8c663080)
 assert cfg.dominates(cd,cp,0x8c66311c,0x8c6630da)
 # The only post-reset calls free nodes in the separate 76-byte cache pool.
 # Exact checked release body has only a private-stack store and flags write.
 sequence(clear,0x8c6630fe,0x8c663118,['mov.l 0x8c663130,r2','mov.l @r2,r4','tst r4,r4','bt 0x8c663112','mov.l 0x8c663134,r2','jsr @r2','mov.l @(0x4,r4),r13','mov r13,r4','tst r4,r4','bf 0x8c663106','mov.l 0x8c663130,r2','mov #0x0,r0','mov.l r14,@r2'])
 assert literal(0x8c663134)==0x8c665440
 big,big_size=literal(0x8c6654c0),literal(0x8c6654c8);assert (big,big_size)==(0x8c8c03e4,0x13000) and base+size==big and big_size%76==0 and big+big_size<0x8cf00000
 sequence(release,0x8c665440,0x8c665466,['add #-0x4,r15','mov.l 0x8c6654c0,r5','mov.l 0x8c6654c8,r3','mov r5,r1','mov.l 0x8c6654c4,r7','add r3,r1','mov.l r5,@r15','mov r5,r6','cmp/eq r4,r5','bf 0x8c66545a','mov.w @r6,r2','and r7,r2','mov.w r2,@r6','add #0x4c,r5','cmp/hs r1,r5','bf/s 0x8c665450','add #0x4c,r6','rts','add #0x4,r15'])
 sequence(scan,0x8c664ac0,0x8c664aec,['mov.l 0x8c664b84,r7','mov.l 0x8c664b80,r6','add r6,r7','mov r6,r4','cmp/hs r7,r4','mov r6,r5','bt/s 0x8c664ae6','mov #0x1,r6','mov.w @r4,r2','extu.w r2,r2','tst r6,r2','bf 0x8c664ade','mov.w r6,@r5','rts','mov r5,r0','add #0x18,r4','cmp/hs r7,r4','bf/s 0x8c664ad0','add #0x18,r5','mov #0x0,r0','rts','nop'])
 flags=[0]*count;allocated=[]
 for _ in range(2):
  index=next(i for i,value in enumerate(flags) if not value&1);flags[index]=1;allocated.append(base+index*24)
 assert allocated==[base,base+24] and all(allocated)
 # Only payload/metadata stores occur between the two constructor scans.
 sequence(ctor,0x8c66327c,0x8c663296,['mov.l @r9,r2','mov r12,r5','mov.l r2,@(0xc,r4)','mov.l @r13,r3','mov.l r3,@(0x10,r4)','mov.l @(0x8,r5),r2','mov.l r2,@(0x8,r4)','mov.l 0x8c663304,r2','mov.l r4,@(0x8,r5)','mov.l @(0xc,r5),r3','mov.l r3,@(0x4,r4)','jsr @r2','mov.l r4,@(0xc,r5)'])
 assert literal(0x8c66321c)==literal(0x8c663304)==0x8c664ac0 and literal(0x8c6631f0)==0x8c67c2e0
 g,p=cfg.graph(ctor,0x8c663140);dom=cfg.dominators(g,p,0x8c663140)
 for target,source in ((0x8c663172,0x8c66316c),(0x8c66322c,0x8c663226),(0x8c6631b8,0x8c6631ac),(0x8c663272,0x8c663266),(0x8c663292,0x8c663272)):
  assert cfg.dominates(dom,p,target,source)
 for start,expected in ((0x8c6631bc,['mov r0,r4','tst r4,r4','bf 0x8c6631c6']),(0x8c663276,['mov r0,r4','tst r4,r4','bt 0x8c66329c']),(0x8c663296,['mov r0,r4','tst r4,r4','bf 0x8c6632a0'])):sequence(ctor,start,start+6,expected)
 forbidden=[(0x8c663172,0x8c663174),(0x8c6631c0,0x8c6631c2),(0x8c66322c,0x8c66322e),(0x8c66327a,0x8c66329c),(0x8c66329a,0x8c66329c)]
 restricted,payload=cfg.graph(ctor,0x8c663140,forbidden);dead=set(ctor)-set(payload.values());expected_dead=set(range(0x8c663174,0x8c663194,2))|set(range(0x8c66322e,0x8c66324c,2))|{0x8c6631c2,0x8c6631c4,0x8c66329c,0x8c66329e};assert dead==expected_dead and len(dead)==35
 files.extend([P/'analysis/movie_graphics_root_option_verification.json',P/'analysis/movie_graphics_descriptor_verification.json'])
 report=dict(original_instructions_byte_checked=sum(map(len,rows.values())),option_replay=replay,descriptor_pool_base=hex(base),descriptor_pool_bytes=size,descriptor_stride=24,cleared_descriptor_count=count,postclear_cache_pool_disjoint=True,first_two_descriptor_addresses=[hex(a) for a in allocated],constructor_infeasible_rows=[hex(a) for a in sorted(dead)],constructor_infeasible_outcomes=[[hex(a),hex(b)] for a,b in forbidden],constructor_proved_infeasible_rows=35,constructor_proved_infeasible_outcomes=5,native_source_changed=False,assumptions=['Full entry and normal return of actual callees; faults stop before later branch tests','Immutable original instructions/literals/strings and no asynchronous flag/register/memory mutation','Mapped aligned private stack and finite valid cleanup lists,disjoint from smallpool/cachepool/metadata','Normal SH4 callee-saved ABI; output and list storage do not alias descriptor flags'],sha256={str(p):digest(p) for p in files},scope='Checked original constructor option call arguments execute actual verification-only string helper evaluator and return0. Original clear loop writeszero to4096descriptor flags before normal return; only later cache releases touch a disjoint pool. Exact descriptor scanner therefore returnsnonzero first/second records;intervening original stores cannot consume another descriptor. Explicit delay-slotCFG proves35rows/5outcomes infeasible. Runtimecode/branches/helpers unchanged,no result injection. Qualification requires separate fresh constructor comparisons and preservation checks;not physical hardware/gameplay proof.')
 (P/'analysis/movie_graphics_constructor_infeasible_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k not in ('sha256','option_replay')})
if __name__=='__main__':main()
