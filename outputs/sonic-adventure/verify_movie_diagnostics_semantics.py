"""Run the full suite with additional independent output/RAM invariants."""
import hashlib,json
from pathlib import Path
import verify_movie_diagnostics as v
P=v.P

def check(m,a,before,s,r,c):
 if r['status']:return 0
 expected=bytearray(m);entry=c['entry'];checks=0;known=True
 if entry==v.MOVE:
  src=before.r[5];dst=before.r[4];n=before.r[6];v.write(expected,dst,v.sample(m,src,n));assert s.r[0]==dst;checks+=1
 elif entry==v.CHECK:assert s.r[0]==(0 if c.get('format_kind',1)==1 else c.get('file_flags',0)&0x40);checks+=1
 elif entry==v.TERMINATE:
  if c.get('format_kind',1)==1:v.write(expected,v.DEST,b'\0')
  assert s.r[0]==c.get('format_kind',1)&0xffffffff;checks+=1
 elif entry==v.FILE_QUERY:
  wanted=c.get('file_value',0x12345678) if not c.get('null_file') and c.get('file_active',1)&0xffff else 0
  assert s.r[0]==wanted;checks+=1
 elif entry==v.LENGTH:assert s.r[0]==len(c.get('diagnostic_text','ABC'));checks+=1
 elif entry==v.ROUND:
  digits=c.get('digits','099995').encode('ascii');index=c.get('digit_index',5)
  if digits[index]>ord('4'):
   rounded=str(int(digits[:index])+1).zfill(index).encode('ascii');assert len(rounded)==index;v.write(expected,v.DEST,rounded+b'0'+digits[index+1:]+b'\0')
  assert s.r[0]==before.r[0];checks+=1
 elif entry==v.APPEND:
  n=c.get('count',3);kind=c.get('format_kind',1)
  if kind==1:
   v.write(expected,v.DEST,v.sample(m,v.TEXT,n));v.store(expected,v.CONTEXT+4,v.DEST+n);v.store(expected,v.CONTEXT+8,7+n)
  elif c.get('writer',v.CALLBACK)==v.CALLBACK:
   completed=0
   for raw in v.sample(m,v.TEXT,max(0,n)):
    if raw==0:break
    completed+=1;v.store(expected,0x8c67340c,raw if raw<128 else raw-256)
   v.store(expected,v.CONTEXT+8,7+completed)
  else:known=False
 elif entry in (v.OUTPUT,v.LOG):
  callback=c.get('callback',v.CALLBACK);enabled=c.get('enabled',1)==1 and callback!=0
  if enabled and entry==v.LOG:
   text=c.get('expected',c.get('diagnostic_format','hello')).encode('latin1');output=v.load(m,0x8c65a370);v.write(expected,output,text+b'\0')
  if enabled:
   assert callback==v.CALLBACK
   v.store(expected,0x8c67340c,v.TEXT if entry==v.OUTPUT else v.load(m,0x8c65a370))
  assert s.r[0]==0;checks+=1
 elif entry in (v.PRINTF,v.FORMAT) and c.get('format_kind',1)==1:
  text=c.get('expected',c.get('diagnostic_format','hello')).encode('latin1');v.write(expected,v.DEST,text+b'\0');assert s.r[0]==len(text);checks+=1
  if entry==v.FORMAT:
   fmt=c.get('diagnostic_format','hello').replace('%%','');v.store(expected,before.r[15],v.VA+4*fmt.count('%'))
 elif entry==v.INTEGER:
  known=False
  if c.get('number_flags',0)==0 and c.get('number_width',0)==0 and c.get('number_precision',0xffffffff)==0xffffffff:
   raw=c.get('value',42)&0xffffffff;specifier=c.get('specifier',100)
   if specifier in (100,105):text=str(raw if raw<0x80000000 else raw-0x100000000)
   else:text=format(raw,{117:'d',111:'o',120:'x',88:'X'}[specifier])
   assert v.sample(a,v.DEST,len(text)+1)==text.encode()+b'\0';checks+=1
 else:known=False
 if known:
  # The primary differential comparison still checks every stack byte.
  # This second semantic oracle excludes only the declared guest stack window.
  sp=before.r[15]&0xffffff;expected[sp-1024:sp]=a[sp-1024:sp]
  assert a==expected,('independent RAM oracle',hex(entry),c)
  checks+=1
 return checks

def main():
 original=v.compare;checks=0;count=0
 def compare(m,s,f,c):
  nonlocal checks,count
  before=v.effects.State.from_buffer_copy(s);result,path,actual,after,ff=original(m,s,f,c);checks+=check(m,actual,before,after,result,c);count+=1;return result,path,actual,after,ff
 v.compare=compare;v.main()
 path=P/'analysis/movie_diagnostics_verification.json';files=[Path(__file__),Path(v.__file__),path,P/'analysis/movie_diagnostics_integer_case_selection.json']
 report=dict(comparisons=count,independent_semantic_checks=checks,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Fresh original/native comparisons plus independently computed output strings, copy/round/query/termination/appending/callback results. Independent full-RAM oracle excludes only 1024 bytes below the initial guest SP; the primary comparison still checks all stack bytes. Partial/fault paths retain primary comparison but receive no invented semantic result.')
 (P/'analysis/movie_diagnostics_semantics_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Independent diagnostic semantic checks',checks,'across',count,'comparisons')
if __name__=='__main__':main()
