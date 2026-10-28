"""Check paired-FMOV frame aliases; this exposes an unclosed dynamic call target."""
import hashlib,json
from pathlib import Path
from prove_collision_wrapper_scalar_returns import P,ROOT,tables,digest

def frame_aliases(rows):
 def expect(a,text):assert rows[f'{a:08x}']==text
 pairs={0x8c10e5ac:'fmov.s fr15,@-r15',0x8c10e5ae:'sts.l PR,@-r15',0x8c10e5b0:'add #-0x8,r15',0x8c10e5b4:'fmov fr4,fr5',0x8c10e5b6:'fmov fr5,@(r0,r15)',0x8c10ef90:'sts.l PR,@-r15',0x8c10ef92:'add #-0xc,r15',0x8c10ef96:'fmov fr4,fr5',0x8c10ef98:'fmov fr5,@(r0,r15)',0x8c10e6a0:'sts.l PR,@-r15',0x8c10e6a2:'add #-0x8,r15',0x8c10e6a4:'mov r15,r3',0x8c10e6a6:'add #0x4,r3',0x8c10e6aa:'fmov.s fr4,@r3',0x8c10e7b8:'mov.l r14,@-r15',0x8c10e7ba:'add #-0x4,r15',0x8c10e7bc:'mov r15,r6',0x8c10e7c6:'fmov.s fr4,@r6',0x8c10f05e:'mov.l 0x8c10f140,r14',0x8c10f09c:'jsr @r3',0x8c10f0ba:'jsr @r3',0x8c10f0f2:'jsr @r14'}
 for a,t in pairs.items():expect(a,t)
 # Symbolic words are caller register inputs, not injected runtime results/frames.
 banks=[[f'active_FR{i}' for i in range(16)],[f'inactive_FR{i}' for i in range(16)]]
 def copy_pair(n,s):
  nb=n&1;sb=s&1;n&=~1;s&=~1
  values=banks[sb][s:s+2];banks[nb][n:n+2]=values
 def pair(reg):return list(banks[reg&1][(reg&~1):(reg&~1)+2])
 copy_pair(5,4);assert pair(5)==['active_FR4','active_FR5']
 aliases=[]
 for owner,slot,offset,reg in [('asin',8,4,5),('atan',12,8,5),('quadrant',8,4,4),('scale',4,0,4)]:
  memory={slot:'saved_PR' if owner!='scale' else 'saved_R14'}
  words=pair(reg)
  for i,w in enumerate(words):memory[offset+4*i]=w
  assert memory[slot]=='active_FR5'
  aliases.append(dict(body=owner,saved_word_offset=slot,store_start_offset=offset,store_bytes=8,overwritten_saved_word=memory[slot]))
 # The scale callee restores its saved R14 from a slot overwritten by FR5.
 # Its caller later uses R14 as an indirect function target. Thus treating the
 # scalar call graph as fixed throughout SZ1 execution is unsound.
 return dict(paired_fmov_aliases=aliases,scale_can_change_caller_R14=True,caller_uses_R14_as_indirect_target='0x8c10f0f2',whole_paired_domain_closed=False,scope='Local symbolic transfer/stack-offset certificate only. Does not establish a reachable infinity return,global unreachable arm,valid return frame,or qualified nonlocal route.')

def main():
 binary,rows,files=tables();r=frame_aliases(rows);r.update(overall_completion_percent=10,new_routine_credit=0,sha256={str(p):digest(p) for p in files+[Path(__file__),P/'prove_collision_wrapper_scalar_returns.py']})
 (P/'analysis/collision_wrapper_paired_frame_aliases.json').write_text(json.dumps(r,indent=2)+'\n');print('PAIRED FRAME ALIASES CHECKED;dynamic call target remains OPEN;NO routine credit')
if __name__=='__main__':main()
