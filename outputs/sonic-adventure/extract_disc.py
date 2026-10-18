"""Inventory a Dreamcast raw-sector GDI and extract its boot executable."""
import argparse, hashlib, json, pathlib, shlex

def main():
 ap=argparse.ArgumentParser(); ap.add_argument('source',type=pathlib.Path,help='Directory containing the disc GDI and track files'); ap.add_argument('output',type=pathlib.Path); ap.add_argument('--gdi',type=pathlib.Path,help='Explicit GDI path when the source directory contains more than one image'); ap.add_argument('--all',action='store_true',help='Extract every file and record its SHA-256'); args=ap.parse_args()
 source=args.source.resolve(); output=args.output.resolve()
 if not source.is_dir(): ap.error(f'source directory does not exist: {source}')
 if args.gdi:
  gdi=args.gdi.resolve()
  if not gdi.is_file(): ap.error(f'GDI file does not exist: {gdi}')
 else:
  images=sorted(source.glob('*.gdi'))
  if len(images)!=1: ap.error(f'expected exactly one *.gdi in {source}; found {len(images)} (select one with --gdi)')
  gdi=images[0]
 if gdi.parent!=source: ap.error('GDI and track files must be in the source directory')
 tracks=[shlex.split(x) for x in gdi.read_text(encoding='ascii').splitlines()[1:] if x.strip()]
 t=next(t for t in reversed(tracks) if t[2]=='4'); base=int(t[1]); stride=int(t[3]); start=int(t[5]); dataoffset=16 if stride==2352 else 0
 output.mkdir(parents=True,exist_ok=True)
 with (source/t[4]).open('rb') as f:
  def read(lba,size):
   out=bytearray()
   while size:
    f.seek(start+(lba-base)*stride+dataoffset); n=min(size,2048); b=f.read(n)
    if len(b)!=n: raise ValueError('Truncated sector')
    out.extend(b);size-=n;lba+=1
   return bytes(out)
  boot=read(base,32768); pvd=read(base+16,2048)
  if pvd[:7]!=b'\x01CD001\x01': raise ValueError('Missing ISO9660 PVD')
  records=[]; visited=set()
  def walk(extent,size,parent):
   if extent in visited: return
   visited.add(extent); buf=read(extent,size); pos=0
   while pos<len(buf):
    n=buf[pos]
    if not n: pos=(pos//2048+1)*2048;continue
    r=buf[pos:pos+n];pos+=n
    if len(r)<34: raise ValueError('Invalid directory record')
    name=r[33:33+r[32]]
    if name in (b'\x00',b'\x01'):continue
    name=name.decode('ascii').split(';')[0]; path=parent+'/'+name if parent else name
    lba=int.from_bytes(r[2:6],'little'); length=int.from_bytes(r[10:14],'little'); directory=bool(r[25]&2)
    records.append(dict(path=path,lba=lba,size=length,directory=directory))
    if directory:walk(lba,length,path)
  root=pvd[156:190]; walk(int.from_bytes(root[2:6],'little'),int.from_bytes(root[10:14],'little'),'')
  exe=next(r for r in records if r['path'].upper()=='1ST_READ.BIN'); binary=read(exe['lba'],exe['size'])
  (output/'1ST_READ.BIN').write_bytes(binary); (output/'IP.BIN').write_bytes(boot)
  if args.all:
   for record in records:
    if record['directory']: continue
    relative=pathlib.PurePosixPath(record['path'])
    if relative.is_absolute() or any(x in ('..','.') or ':' in x or '\\' in x for x in relative.parts): raise ValueError('Unsafe ISO path')
    target=output/'files'/pathlib.Path(*relative.parts)
    target.parent.mkdir(parents=True,exist_ok=True)
    payload=read(record['lba'],record['size']); target.write_bytes(payload)
    record['sha256']=hashlib.sha256(payload).hexdigest()
  manifest=dict(source=str(source),gdi=gdi.read_text(encoding='ascii'),volume=pvd[40:72].decode('ascii').strip(),boot_header=boot[:256].decode('ascii',errors='replace'),executable_sha256=hashlib.sha256(binary).hexdigest(),executable_size=len(binary),files=records)
  (output/'disc_inventory.json').write_text(json.dumps(manifest,indent=2),encoding='utf-8')
  print(json.dumps({k:v for k,v in manifest.items() if k not in ('files','gdi','boot_header')},indent=2)); print('Entries:',len(records));print('First executable bytes:',binary[:32].hex())
if __name__=='__main__':main()
