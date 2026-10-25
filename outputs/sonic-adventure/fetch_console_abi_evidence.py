"""Fetch pinned public ABI sources into private research storage, never source/."""
import hashlib,urllib.request
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];COMMIT='368a2eb9584a7d130a6e77e299468c0b64fa1a9f'
SOURCES=(('kernel/arch/dreamcast/hardware/syscalls.c','23d04411b38a0cfb6b1a2b5b0d42b02e21d7015926e8955a1e4a7b8391f858b3'),('kernel/arch/dreamcast/include/dc/syscalls.h','392031cdd59e3ddf758d9f5cf66497437e14fecef17081023f77b57bf6a43ebd'))
def main():
 out=ROOT/'work/sonic-adventure/evidence/console-abi';out.mkdir(parents=True,exist_ok=True)
 for source,digest in SOURCES:
  path=out/Path(source).name
  if path.exists():assert hashlib.sha256(path.read_bytes()).hexdigest()==digest,path;continue
  url=f'https://raw.githubusercontent.com/KallistiOS/KallistiOS/{COMMIT}/{source}'
  with urllib.request.urlopen(url,timeout=60) as response:raw=response.read()
  assert hashlib.sha256(raw).hexdigest()==digest,url
  with path.open('xb') as stream:stream.write(raw)
 print('Verified two pinned public ABI inputs in private research storage')
if __name__=='__main__':main()
