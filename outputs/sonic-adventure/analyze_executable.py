import argparse, collections, hashlib, json, pathlib, re, struct
BASE=0x8c010000
p=argparse.ArgumentParser();p.add_argument('binary',type=pathlib.Path);p.add_argument('output',type=pathlib.Path);a=p.parse_args();b=a.binary.read_bytes();a.output.mkdir(parents=True,exist_ok=True)
strings=[{'offset':m.start(),'address':hex(BASE+m.start()),'text':m.group().decode('ascii')} for m in re.finditer(rb'[ -~]{8,}',b)]
(a.output/'executable_strings.json').write_text(json.dumps(strings,indent=2),encoding='utf-8')
def sx(v,n):return v-(1<<n) if v&(1<<(n-1)) else v
rows=[]
for off in list(range(0,32,2))+list(range(0xa0,0x120,2)):
 op=struct.unpack_from('<H',b,off)[0];pc=BASE+off;n=(op>>8)&15;m=(op>>4)&15;detail={};s=f'.word 0x{op:04x}'
 if op==9:s='nop'
 elif op==0xb:s='rts'
 elif op==0x2b:s='rte'
 elif op>>12==0xd:
  addr=((pc+4)&~3)+(op&255)*4;val=struct.unpack_from('<I',b,addr-BASE)[0];s=f'mov.l @(0x{addr:08x},pc),r{n}';detail={'literal_address':hex(addr),'literal_value':hex(val)}
 elif op>>12==0x9:
  addr=pc+4+(op&255)*2;s=f'mov.w @(0x{addr:08x},pc),r{n}'
 elif op>>12 in (0xa,0xb):s=f'{"bra" if op>>12==0xa else "bsr"} 0x{pc+4+sx(op&4095,12)*2:08x}'
 elif op>>12==0xe:s=f'mov #{sx(op&255,8)},r{n}'
 elif op>>12==7:s=f'add #{sx(op&255,8)},r{n}'
 elif op&0xf0ff==0x402b:s=f'jmp @r{n}'
 elif op&0xf0ff==0x400b:s=f'jsr @r{n}'
 elif op&0xf0ff==0x4010:s=f'dt r{n}'
 elif op&0xff00 in (0x8900,0x8b00,0x8d00,0x8f00):s=f'{ {0x8900:"bt",0x8b00:"bf",0x8d00:"bt/s",0x8f00:"bf/s"}[op&0xff00]} 0x{pc+4+sx(op&255,8)*2:08x}'
 elif op&0xf00f in (0x2009,0x200a,0x200b):s=f'{ {0x2009:"and",0x200a:"xor",0x200b:"or"}[op&0xf00f]} r{m},r{n}'
 elif op&0xf00f==0x6002:s=f'mov.l @r{m},r{n}'
 elif op&0xf00f==0x6003:s=f'mov r{m},r{n}'
 elif op&0xf00f==0x2002:s=f'mov.l r{m},@r{n}'
 elif op&0xf00f==0x2001:s=f'mov.w r{m},@r{n}'
 elif op&0xf00f==0x2000:s=f'mov.b r{m},@r{n}'
 rows.append({'address':hex(pc),'opcode':f'{op:04x}','instruction':s,**detail})
(a.output/'startup_disassembly.json').write_text(json.dumps(rows,indent=2),encoding='utf-8')
(a.output/'startup_disassembly.txt').write_text('\n'.join(f'{r["address"]}: {r["opcode"]}  {r["instruction"]}'+(f' ; {r["literal_value"]}' if 'literal_value' in r else '') for r in rows),encoding='utf-8')
keywords=['ninja','kamui','sofdec','adx','cri','sonic','error','version','malloc']
clues=[s for s in strings if any(k in s['text'].lower() for k in keywords)]
report={'sha256':hashlib.sha256(b).hexdigest(),'bytes':len(b),'assumed_load_address':hex(BASE),'load_address_source':'https://mc.pp.se/dc/ip.bin.html','string_count':len(strings),'subsystem_clues':clues[:250],'limitations':['Startup decoder covers a subset of SH-4 instructions; unknown words are retained verbatim.','Load address follows Dreamcast boot convention; no runtime trace yet.','ASCII string matches are clues, not verified function identifications.']}
(a.output/'executable_analysis.json').write_text(json.dumps(report,indent=2),encoding='utf-8');print('Strings:',len(strings),'Clues:',len(clues));print((a.output/'startup_disassembly.txt').read_text()[:2200])
