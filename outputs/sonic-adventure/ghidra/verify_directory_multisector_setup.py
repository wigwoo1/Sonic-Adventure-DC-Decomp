from pathlib import Path
p=Path(__file__).resolve().parents[1]
t=(p/'verify_file_directory_load.py').read_text()
a=t.index('sector=');b=t.index('cases=0',a)
t=t[:a]+'''sectors=[bytearray(2048),bytearray(2048)]
filenames=[b'FIRST.MLT',b'SECOND.MLT',b'THIRD.MLT',b'FOURTH.MLT']
for i,name in enumerate(filenames):
 data=sectors[i//2];o=i%2*64;name+=b';1';data[o]=64;struct.pack_into('<I',data,o+2,100+i);struct.pack_into('<I',data,o+10,4096);data[o+32]=len(name);data[o+33:o+33+len(name)]=name
''' +t[b:]
t=t.replace('for capacity,cached,error,bank in itertools.product([1,2,3],[0,1],[0,0xffffffef],[0,0x200000]):','for capacity,cached,fail_at,bank in itertools.product([1,2,3,4,5],[0,1],[0,1,2],[0,0x200000]):')
a=t.index(' def service(');b=t.index(' def hook(',a)
t=t[:a]+''' def service(m,regs,log):
  log.append(tuple(regs[i] for i in range(4,8)));index=regs[4]-123
  assert index in [0,1] and regs[5]==1 and regs[6]==0x8ca00000
  failed=fail_at==index+1
  if not failed:
   for i,v in enumerate(sectors[index]):m[0xa00000+i]=v
  regs[0]=0xffffffef if failed else 0
''' +t[b:]
t=t.replace('s.r[7]=2048','s.r[7]=4096')
a=t.index(' result=');b=t.index(' cases+=1',a)
t=t[:a]+''' read_count=0 if cached else 1 if fail_at==1 or capacity<=2 else 2
 read_failed=not cached and ((fail_at==1) or (fail_at==2 and read_count==2))
 result=0 if cached else 0xffffffef if read_failed else 0xfffffff4 if capacity<4 else 0
 assert r[0]==result and len(actual)==read_count
 assert [e[0] for e in actual]==list(range(123,123+read_count))
 assert struct.unpack_from('<I',native,0xb00008)[0]==(0 if result else 123)
 parsed=0 if cached or fail_at==1 else min(capacity,2 if read_failed else 4)
 assert struct.unpack_from('<I',native,0xb00000)[0]==(99 if cached or read_failed else parsed)
 for i,name in enumerate(filenames[:parsed]):
  o=0xb00010+i*44;assert struct.unpack_from('<II',native,o)==(100+i,4096);assert native[o+10:o+11+len(name)]==name+b'\\0'
''' +t[b:]
a=t.index('report=');b=t.index('\n(root/',a)
t=t[:a]+"report={'cases_passed':cases,'scope':'Native directory loader/parser/copy/checksum/uppercase against full retail RAM/CPU/FPU/SR/PR, two modeled sector reads. Capacities1..5, cached reuse, successful load, first/second read failures and banks. Independent sector progression, partial records, count, result and cache invalidation. Actual disc/device read remains modeled.'}"+t[b:]
t=t.replace('file_directory_load_verification.json','file_directory_load_multisector_verification.json').replace('directory loader comparisons passed','multi-sector directory loader comparisons passed')
(p/'verify_file_directory_load_multisector.py').write_text(t)
