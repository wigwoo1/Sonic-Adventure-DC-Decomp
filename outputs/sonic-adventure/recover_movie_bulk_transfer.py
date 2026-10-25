"""Extend two existing byte-checked transfer adapters for the explicit RAM domain."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__)];routines=[];outputs=[]
 raw_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=raw_path.read_bytes();files.append(raw_path)
 for address,stage,suffix in (('0x8c10d460','rtc','read'),('0x8c10d770','hardware','write')):
  source=P/f'source/sa_movie_{stage}.c';mp=P/f'analysis/movie_{stage}_translation.json';text=source.read_text();header=text[:text.index('SA_STAGE_EXPORT int ')]
  guard=' if(valid_ram(a,n))return 1;';assert header.count(guard)==1
  addition='\n if((n==1u||n==2u||n==4u)&&!(a&(n-1u))&&(a&0x1fffffffu)>=0x00800000u&&(a&0x1fffffffu)<=0x00a00000u-n)return 1;'
  header=header.replace(guard,guard+addition);parts=re.split(r'(?=SA_STAGE_EXPORT int )',text)[1:]
  r=next(r for r in json.loads(mp.read_text())['routines'] if r['address']==address);body=next(p for p in parts if p.startswith('SA_STAGE_EXPORT int '+r['name']+'('))
  for row in csv.DictReader(Path(r['listing']).open(),delimiter='\t'):
   a=int(row['address'],16);assert raw[a-0x8c010000:a-0x8c010000+int(row['length'])]==bytes.fromhex(row['bytes'])
  target=P/f'source/sa_movie_bulk_transfer_{suffix}.c';target.write_text(header+body.replace(f'sa_movie_{stage}_','sa_movie_bulk_transfer_'))
  outputs.append(target.relative_to(P).as_posix());routines.append(r|dict(extended_source=target.relative_to(P).as_posix(),original_source=source.relative_to(P).as_posix()));files.extend((source,mp,Path(r['listing']),target))
 report=dict(sources=outputs,routines=routines,newly_registered_entries=0,scope='Existing original transfer-provider instruction bodies retained exactly except export names. Only the shared address guard adds aligned 1/2/4-byte accesses within the explicit 2-MiB sound-RAM window. An external bus must provide actual storage. No callee return, copy count or guest loop is replaced; canonical helpers get zero duplicate credit.',sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_bulk_transfer_translation.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated two extended original providers; zero new routine credit')
if __name__=='__main__':main()
