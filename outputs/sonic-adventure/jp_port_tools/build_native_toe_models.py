"""Append genuine player-only toe data to the existing canonical model owner."""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_motion_data import definitions,records,exact_floats,LAYOUTS
from build_native_read_queue import PIN
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
 ap=argparse.ArgumentParser()
 for n in ('workspace','upstream','inputs','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
 a=ap.parse_args();r,u,inputs,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','inputs','baseline','output'));p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
 for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
 pinned=read(inputs/'inputs.json')
 for key,row in pinned['files'].items():bank,ext=key.split('/');assert sha(inputs/bank/('content.'+ext))==row['sha256']
 defs=definitions((inputs/'1st_read/content.c').read_text());meta=records(inputs/'1st_read/content.txt');extra=inputs/'model_sources-v3';cpp=read(extra/'inputs.json')
 for name,row in cpp.items():
  assert sha(extra/(name+'.i'))==row['preprocessed_sha256'] and sha(extra/(name+'.txt'))==row['body_table_sha256']
  candidate=definitions((extra/(name+'.i')).read_text());table=records(extra/(name+'.txt'))
  for key,value in table.items():
   if key in candidate:assert key not in defs;defs[key]=candidate[key];meta[key]=value
 source=(u/'src/player/sonic.c').read_bytes();assert source==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':src/player/sonic.c'])
 symbols={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(u/'config/1st_read/symbols.txt').read_text(),re.M)}
 for side in ('plus','minus'):
  name='sonic_toe_'+side+'_z_face_normals';declaration='NJS_VECTOR '+name+'[32];';assert declaration.encode() in source and name not in defs
  defs[name]=('NJS_VECTOR '+name+'[32]',declaration+'\n');meta[name]=dict(name=name,address=symbols[name],bytes=384,type='NJS_VECTOR')
 link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_motion_data.dll'));obj=Path(next(n for n in link if Path(n).name=='models_native_data.obj'));oldsource=obj.with_suffix('.c');old=oldsource.read_text();old_names=re.findall(r'^\{"([^"\n]+)",',old,re.M);assert len(old_names)==2793 and len(set(old_names))==2793
 selected=set(old_names);pending=['sonic_toe_'+side+'_z_'+shape for side in ('plus','minus') for shape in ('blend','shape_a','shape_b')]
 while pending:
  name=pending.pop()
  if name in selected:continue
  assert name in defs,name;selected.add(name);body=defs[name][1];pending.extend(set(re.findall(r'\b[A-Za-z_]\w*\b',body))&defs.keys()-selected)
 added=[n for n in defs if n in selected and n not in old_names];assert all('sonic_toe_'+side+'_z_'+shape in added for side in ('plus','minus') for shape in ('blend','shape_a','shape_b'))
 aliases=''.join('#define '+n+' jp_models_'+n+'\n' for n in added);forward=''.join('extern '+defs[n][0]+';\n' for n in added);body,stats=exact_floats('\n'.join(defs[n][1] for n in added));anchor='struct motion_item {';assert old.count(anchor)==1;before,after=old.split(anchor)
 rows=[];registry=[]
 for index,n in enumerate(added,len(old_names)):
  head=defs[n][0];ctype=re.sub(r'\s+'+re.escape(n)+r'(?:\[.*\])?$','',head).strip();ctype=re.sub(r'^(?:static|const)\s+','',ctype);kind=list(LAYOUTS).index(ctype)+1 if ctype in LAYOUTS else 0;element=LAYOUTS[ctype][0] if kind else 1;row=meta[n];assert row['bytes']%element==0
  rows.append('{"'+n+'",&'+n+',sizeof('+n+'),0x%08xu,%d,%d,%d,'%(row['address'],row['bytes'],kind,element)+('sizeof('+ctype+')' if kind else '1')+'},\n');registry.append(dict(row,index=index,kind=kind,original_element=element,ctype=ctype))
 tail='};\n__declspec(dllexport) unsigned jp_motion_models_count';assert after.count(tail)==1;after=after.replace(tail,''.join(rows)+tail)
 generated=aliases+before+forward+body+'\n'+anchor+after;d.mkdir()
 for path in base.iterdir():
  if path.is_file() and path.suffix in ('.dll','.lib','.c','.h','.obj'):shutil.copyfile(path,d/path.name)
  elif path.is_dir() and path.name in ('include','history_include','game_init_imports','motion_view'):shutil.copytree(path,d/path.name)
 unit=d/'models_with_toes.c';unit.write_text(generated);compiler=b['commands'][0]['command'][0];newobj=d/'models_with_toes.obj';host=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-Wno-ignored-qualifiers','-Wno-missing-braces','-include',str(p/'native/jp_host_types.h'),'-I'+str(u/'sdk/include')];link[link.index(str(obj))]=str(newobj);link[-1]=str(d/'jp_native_motion_data.dll');commands=[]
 for i,cmd in enumerate((host+['-c',str(unit),'-o',str(newobj)],link)):
  run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);commands.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));assert not run.returncode and not log.stat().st_size,log.read_text()[-6000:]
 updates={}
 for k,v in b.items():
  if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
 updates.update(status='canonical_model_registry_extended_with_genuine_toe_closure_unverified',toe_models_baseline_sha256=sha(base/'build_verification.json'),toe_models_old_source_sha256=sha(oldsource),toe_models_old_record_names=old_names,toe_models_added_registry=registry,toe_models_original_sonic_source_sha256=sha(u/'src/player/sonic.c'),toe_models_generated_source_sha256=sha(unit),toe_models_exact_float_adaptation=stats,commands=commands+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{Path(__file__).resolve().relative_to(r).as_posix():sha(__file__)}),playable=False,overall_completion_percent=10)
 (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print('Canonical model owner extended:',len(added),'records appended; two clean commands.',flush=True)
if __name__=='__main__':main()
