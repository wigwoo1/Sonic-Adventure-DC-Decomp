"""Preserve original register state across explicitly routed call continuations."""
import csv,re

ARGUMENTS='m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr'

def body_end(text,start):
 opening=text.index('{',start);depth=0
 for i in range(opening,len(text)):
  depth+=int(text[i]=='{')-int(text[i]=='}')
  if depth==0:return i+1
 raise AssertionError('Unbalanced generated C body')

def continuation_pcs(record):
 rows=list(csv.DictReader(open(record['listing']),delimiter='\t'));addresses={int(r['address'],16) for r in rows}
 pcs=sorted({int(r['address'],16)+4 for r in rows if r['instruction'].lstrip('_').startswith(('jsr ','bsr '))})
 assert set(pcs)<=addresses
 return pcs

def expand_function(function,record,prefix):
 pcs=continuation_pcs(record)
 if not pcs:return function,[]
 name=record['name'];signature=function[:function.index('{')].strip();assert signature.startswith('SA_STAGE_EXPORT int '+name+'(')
 run_name=f'{prefix}_run_{int(record["address"],16):08x}'
 run_signature=signature.replace('SA_STAGE_EXPORT int '+name,'static int '+run_name).replace('const uint32_t *vbr)','const uint32_t *vbr,uint32_t resume_pc)')
 assert run_signature!=signature and run_signature.endswith('resume_pc)')
 body=function[function.index('{')+1:];marker=f' goto L{int(record["address"],16):x};';assert body.count(marker)==1
 body=body.replace(marker,' '+''.join(f'if(resume_pc==0x{pc:x}u)goto L{pc:x};' for pc in pcs)+marker.lstrip(),1)
 output=[];rts_count=0
 for line in body.splitlines():
  if '/* rts */' in line:
   before='if(*pr!=entry_pr)return -3;';after='return 0; /* rts */';assert line.count(before)==line.count(after)==1
   line=line.replace(before,'{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;')
   line=line.replace(after,'if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */');rts_count+=1
  output.append(line)
 source=run_signature+'{\n'+'\n'.join(output)+'\n'+signature+'{return '+run_name+'('+ARGUMENTS+',0);}\n';routes=[]
 for pc in pcs:
  export=f'{prefix}_resume_{pc:08x}';source+=signature.replace(name,export)+'{return '+run_name+'('+ARGUMENTS+f',0x{pc:x}u);}}\n';routes.append(dict(pc=hex(pc),name=export,parent=record['address']))
 record.update(native_call_continuations=list(map(hex,pcs)),native_resumed_rts_count=rts_count)
 return source,routes

def adapt_decoder(text,records,prefix):
 routes=[]
 for record in records:
  marker='SA_STAGE_EXPORT int '+record['name']+'(';assert text.count(marker)==1;start=text.index(marker);end=body_end(text,start)
  extended,added=expand_function(text[start:end],dict(record),prefix);text=text[:start]+extended+text[end:];routes.extend(added)
 marker='typedef int (*sa_shared_service)'
 assert text.count(marker)==1;text=text.replace(marker,'int sa_movie_channel_io_return_allowed(void *,uint32_t);\n'+marker)
 return text,routes

def adapt_worker(text,record,prefix):
 pcs=continuation_pcs(record);old=' if(resume_pc==0x8c647982u)goto L8c647982;if(resume_pc==0x8c647b74u)goto L8c647b74;goto L8c647980;';assert text.count(old)==1
 text=text.replace(old,' '+''.join(f'if(resume_pc==0x{pc:x}u)goto L{pc:x};' for pc in sorted(set(pcs)|{0x8c647982}))+ 'goto L8c647980;')
 name=record['name'];marker='SA_STAGE_EXPORT int '+name+'(';start=text.index(marker);signature=text[start:text.index('{',start)].strip();routes=[]
 for pc in pcs:
  if pc==0x8c647b74:continue
  export=f'{prefix}_resume_{pc:08x}';text+='\n'+signature.replace(name,export)+'{return channel_worker_run('+ARGUMENTS+f',0x{pc:x}u);}}\n';routes.append(dict(pc=hex(pc),name=export,parent=record['address']))
 return text,routes
