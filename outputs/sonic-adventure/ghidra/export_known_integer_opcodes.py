"""Reconstruct an integer TSV from retail bytes and existing opcode examples.
Not a function finder. Explicit caller-supplied boundaries, known integer opcode
examples and independently verified CPU execution are required.
"""
import collections,struct,sys,json
from pathlib import Path
root=Path(__file__).resolve().parents[3];out=root/'outputs/sonic-adventure';examples=collections.defaultdict(collections.Counter)
for p in (out/'analysis/instructions').glob('*.tsv'):
 for line in p.read_text().splitlines()[1:]:
  t=line.split('\t')
  if len(t)<3:continue
  try:w=int(t[1],16)
  except ValueError:continue
  examples[w][t[2].lstrip('_')]+=1
for word,instruction in {0x69e2:'mov.l @r14,r9',0x032c:'mov.b @(r0,r2),r3',0x0234:'mov.b r3,@(r0,r2)',0x3344:'div1 r4,r3',0xf647:'fmov fr4,@(r0,r6)',0xf657:'fmov fr5,@(r0,r6)',0x6056:'mov.l @r5+,r0',0x3562:'cmp/hs r6,r5',0x1276:'mov.l r7,@(0x18,r2)'}.items():examples[word][instruction]+=1
b=(root/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();start,end=int(sys.argv[1],16),int(sys.argv[2],16);rows=['address\topcode\tinstruction\treferences'];missing=[]
addresses=range(start,end,2) if len(sys.argv)<5 else json.loads((out/'analysis'/sys.argv[4]).read_text())
for a in addresses:
 w=struct.unpack_from('<H',b,a-0x8c010000)[0]
 if w>>12 in (9,13):
  target=a+4+2*(w&255) if w>>12==9 else ((a+4)&~3)+4*(w&255)
  i=f'mov.{"w" if w>>12==9 else "l"} 0x{target:08x},r{(w>>8)&15}'
 elif w>>12 in (10,11):
  d=w&4095;d=d-4096 if d&2048 else d;i=('bra' if w>>12==10 else 'bsr')+f' 0x{a+4+2*d:08x}'
 elif w>>8 in (0x89,0x8b,0x8d,0x8f):
  d=w&255;d=d-256 if d&128 else d;i={0x89:'bt',0x8b:'bf',0x8d:'bt/s',0x8f:'bf/s'}[w>>8]+f' 0x{a+4+2*d:08x}'
 elif w>>8==0xc7:i=f'mova 0x{((a+4)&~3)+4*(w&255):08x},r0'
 elif w>>12==1:i=f'mov.l r{(w>>4)&15},@(0x{(w&15)*4:x},r{(w>>8)&15})'
 elif w&0xf00f in (0x3002,0x3000,0x300c,0x200b):
  op={0x3002:'cmp/hs',0x3000:'cmp/eq',0x300c:'add',0x200b:'or'}[w&0xf00f];i=f'{op} r{(w>>4)&15},r{(w>>8)&15}'
 elif w&0xf0ff==0x4005:i=f'rotr r{(w>>8)&15}'
 elif w>>12==5:i=f'mov.l @(0x{(w&15)*4:x},r{(w>>4)&15}),r{(w>>8)&15}'
 elif w>>12 in (7,14):
  d=w&255;d=d-256 if d&128 else d;imm=f'-0x{-d:x}' if d<0 else f'0x{d:x}';i=f'{"add" if w>>12==7 else "mov"} #{imm},r{(w>>8)&15}'
 elif w&0xf00f==0x6003:i=f'mov r{(w>>4)&15},r{(w>>8)&15}'
 elif w&0xf0ff==0x4009:i=f'shlr2 r{(w>>8)&15}'
 elif w&0xf00f==0x400c:i=f'shad r{(w>>4)&15},r{(w>>8)&15}'
 elif w&0xf00f==0x400d:i=f'shld r{(w>>4)&15},r{(w>>8)&15}'
 elif w&0xf00f in (0x0007,0x3008,0x2009,0x3003):
  op={0x0007:'mul.l',0x3008:'sub',0x2009:'and',0x3003:'cmp/ge'}[w&0xf00f];i=f'{op} r{(w>>4)&15},r{(w>>8)&15}'
 elif w&0xf00f==0x000e:i=f'mov.l @(r0,r{(w>>4)&15}),r{(w>>8)&15}'
 elif w&0xf00f==0x0005:i=f'mov.w r{(w>>4)&15},@(r0,r{(w>>8)&15})'
 elif w&0xf00f==0x0006:i=f'mov.l r{(w>>4)&15},@(r0,r{(w>>8)&15})'
 elif w&0xf0ff in (0x0093,0x00a3,0x00b3):i={0x0093:'ocbi',0x00a3:'ocbp',0x00b3:'ocbwb'}[w&0xf0ff]+f' @r{(w>>8)&15}'
 elif w>>8==0x88:i=f'cmp/eq #0x{w&255:x},r0'
 elif w&0xf00f==0x3007:i=f'cmp/gt r{(w>>4)&15},r{(w>>8)&15}'
 elif w&0xf00f==0x2002:i=f'mov.l r{(w>>4)&15},@r{(w>>8)&15}'
 elif w&0xf00f==0x6006:i=f'mov.l @r{(w>>4)&15}+,r{(w>>8)&15}'
 elif w&0xf00f==0x600b:i=f'neg r{(w>>4)&15},r{(w>>8)&15}'
 elif w&0xf00f==0x3006:i=f'cmp/hi r{(w>>4)&15},r{(w>>8)&15}'
 elif w>>8==0xc8:i=f'tst #0x{w&255:x},r0'
 elif w&0xf00f==0x6000:i=f'mov.b @r{(w>>4)&15},r{(w>>8)&15}'
 elif w&0xf00f==0x6004:i=f'mov.b @r{(w>>4)&15}+,r{(w>>8)&15}'
 elif w&0xf0ff==0x006a:i=f'sts FPSCR,r{(w>>8)&15}'
 elif w&0xf0ff==0x406a:i=f'lds r{(w>>8)&15},FPSCR'
 elif w&0xf00f==0x2000:i=f'mov.b r{(w>>4)&15},@r{(w>>8)&15}'
 elif w&0xf00f==0x600e:i=f'exts.b r{(w>>4)&15},r{(w>>8)&15}'
 elif w&0xf00f==0x0004:i=f'mov.b r{(w>>4)&15},@(r0,r{(w>>8)&15})'
 elif w&0xf00f in (0x300e,0x2008,0x600c,0x300c):i={0x300e:'addc',0x2008:'tst',0x600c:'extu.b',0x300c:'add'}[w&0xf00f]+f' r{(w>>4)&15},r{(w>>8)&15}'
 elif w&0xf0ff==0x4021:i=f'shar r{(w>>8)&15}'
 elif w>>8==0xc9:i=f'and #0x{w&255:x},r0'
 elif w&0xf00f==0x000c:i=f'mov.b @(r0,r{(w>>4)&15}),r{(w>>8)&15}'
 elif w&0xf00f in (0xf000,0xf001,0xf002,0xf003,0xf004,0xf005):i={0xf000:'fadd',0xf001:'fsub',0xf002:'fmul',0xf003:'fdiv',0xf004:'fcmp/eq',0xf005:'fcmp/gt'}[w&0xf00f]+f' fr{(w>>4)&15},fr{(w>>8)&15}'
 elif w&0xf00f==0xf006:i=f'fmov.s @(r0,r{(w>>4)&15}),fr{(w>>8)&15}'
 elif w&0xf0ff==0xf03d:i=f'ftrc fr{(w>>8)&15},FPUL'
 elif w&0xf0ff==0x005a:i=f'sts FPUL,r{(w>>8)&15}'
 elif w&0xf00f==0xf008:i=f'fmov.s @r{(w>>4)&15},fr{(w>>8)&15}'
 elif w&0xf00f==0x000d:i=f'mov.w @(r0,r{(w>>4)&15}),r{(w>>8)&15}'
 elif w&0xf00f==0x6001:i=f'mov.w @r{(w>>4)&15},r{(w>>8)&15}'
 elif w>>8==0x85:i=f'mov.w @(0x{(w&15)*2:x},r{(w>>4)&15}),r0'
 elif w in examples:i=examples[w].most_common(1)[0][0]
 else:missing.append((hex(a),hex(w)));continue
 rows.append(f'{a:08x}\t{w:x}\t{i}\t')
if missing:raise SystemExit('Missing opcode examples: '+repr(missing))
(out/'analysis/instructions'/sys.argv[3]).write_text('\n'.join(rows)+'\n')
print(len(rows)-1,'instruction examples exported; execution verification required')
