"""Bounded PRS decoder; format reference: https://github.com/Solybum/PRS ."""
import argparse, hashlib, json, pathlib

def decompress(data, limit=32*1024*1024):
 cursor=0; flags=0; remaining=0; out=bytearray()
 def byte():
  nonlocal cursor
  if cursor>=len(data): raise ValueError('Truncated PRS stream')
  value=data[cursor];cursor+=1;return value
 def bit():
  nonlocal flags,remaining
  if not remaining: flags=byte();remaining=8
  value=flags&1;flags>>=1;remaining-=1;return value
 while True:
  if bit():
   if len(out)>=limit:raise ValueError('Output limit exceeded')
   out.append(byte());continue
  if bit():
   packed=byte()|(byte()<<8)
   if packed==0:return bytes(out),cursor
   distance=(packed>>3)-8192; count=packed&7; count=byte()+1 if not count else count+2
  else:
   count=(bit()<<1)|bit();count+=2;distance=byte()-256
  if len(out)+distance<0:raise ValueError('Back reference before start')
  if len(out)+count>limit:raise ValueError('Output limit exceeded')
  for _ in range(count):out.append(out[len(out)+distance])

def main():
 p=argparse.ArgumentParser();p.add_argument('source',type=pathlib.Path);p.add_argument('output',type=pathlib.Path);p.add_argument('manifest',type=pathlib.Path);a=p.parse_args();records=[]
 for path in sorted(a.source.rglob('*.PRS')):
  raw=path.read_bytes();relative=path.relative_to(a.source);r={'source':relative.as_posix(),'compressed_bytes':len(raw),'compressed_sha256':hashlib.sha256(raw).hexdigest()}
  try:
   decoded,consumed=decompress(raw);target=a.output/relative.with_suffix('.BIN');target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(decoded)
   r.update(decoded_bytes=len(decoded),decoded_sha256=hashlib.sha256(decoded).hexdigest(),consumed_bytes=consumed,trailing_bytes=len(raw)-consumed,header_hex=decoded[:32].hex(),status='decoded')
  except ValueError as e:r.update(status='error',error=str(e))
  records.append(r)
 a.manifest.parent.mkdir(parents=True,exist_ok=True);a.manifest.write_text(json.dumps(records,indent=2),encoding='utf-8'); print('Decoded:',sum(r['status']=='decoded' for r in records),'Errors:',sum(r['status']=='error' for r in records))
if __name__=='__main__':main()
