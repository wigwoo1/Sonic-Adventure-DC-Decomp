"""Append the full genuine Sonic weld model/action closure to its same owner.

Reuses the qualified model-data builder; new static definitions retain their
storage class. All generated game data remains private.
"""
import inspect
from pathlib import Path
import build_native_toe_models as original
def main():
 source=inspect.getsource(original.main)
 def rep(old,new):
  nonlocal source
  assert source.count(old)==1,(old,source.count(old));source=source.replace(old,new)
 rep(" a=ap.parse_args();", " ap.add_argument('--module-inputs',type=Path,required=True);ap.add_argument('--binary',type=Path,required=True)\n a=ap.parse_args();")
 rep("Path(n).name=='models_native_data.obj'","Path(n).name=='models_with_toes.obj'")
 assert source.count('2793')==2;source=source.replace('2793','2876')
 anchor=" source=(u/'src/player/sonic.c').read_bytes();"
 extra=""" module=a.module_inputs.resolve();module_inputs=read(module/'inputs.json');assert module_inputs['actual_supplied_JP_disc_module_verified']
 for name,row in module_inputs['files'].items():assert sha(module/name)==row['sha256']
 for cpp_name,table_name in (('super.i','super.txt'),('generated_content.c','generated_content.txt')):
  candidates=definitions((module/cpp_name).read_text());table=records(module/table_name)
  for key,row in table.items():
   if key in candidates:assert key not in defs,key;defs[key]=candidates[key];meta[key]=row
 binary=a.binary.resolve();assert sha(binary)==read(p/'jp_port_tools/jp_view_stack_external_inputs.json')['original_binary_sha256'];import struct
 original_binary=binary.read_bytes();object_pointers=set();weld_rows=0
 for index in range(256):
  words=struct.unpack_from('<IIIBbBBII',original_binary,0x1dc28c-0x10000+index*24)
  if not words[0]:break
  weld_rows+=1;object_pointers.update(x for x in words[:3] if x)
 else:raise AssertionError('unterminated original Sonic weld table')
 assert weld_rows==32
"""
 rep(anchor,extra+anchor)
 old=" selected=set(old_names);pending=['sonic_toe_'+side+'_z_'+shape for side in ('plus','minus') for shape in ('blend','shape_a','shape_b')]"
 new=""" selected=set(old_names)
 old_object_names=[n for n in old_names if n in meta and n in defs and re.match(r'^(?:static\\s+)?NJS_OBJECT\\s',defs[n][0])]
 wanted={ptr for ptr in object_pointers if not any(meta[n]['address']<=ptr<meta[n]['address']+meta[n]['bytes'] and (ptr-meta[n]['address'])%52==0 for n in old_object_names)}
 roots=[n for n,row in meta.items() if n in defs and row['address'] in wanted and re.match(r'^(?:static\\s+)?NJS_OBJECT\\s',defs[n][0])]
 assert len(roots)==len(wanted)==26
 pending=list(roots)+['sonic_stand_action','super_sonic_stand_action']"""
 rep(old,new)
 rep("assert all('sonic_toe_'+side+'_z_'+shape in added for side in ('plus','minus') for shape in ('blend','shape_a','shape_b'))","assert set(roots+['sonic_stand_action','super_sonic_stand_action'])<=set(added)")
 rep(r"forward=''.join('extern '+defs[n][0]+';\n' for n in added)",r"forward=''.join((defs[n][0] if defs[n][0].startswith('static ') else 'extern '+defs[n][0])+';\n' for n in added)")
 source=source.replace('toe_models_','sonic_weld_data_').replace('canonical_model_registry_extended_with_genuine_toe_closure_unverified','canonical_model_registry_extended_with_genuine_full_Sonic_weld_closure_unverified')
 rep('d.mkdir()',"d.mkdir();(d/'private_weld_closure_builder.py').write_text(derived_source)")
 rep("updates.update(status=","updates['sonic_weld_data_module_inputs_sha256']=sha(module/'inputs.json');updates['sonic_weld_data_original_binary_sha256']=sha(binary);updates['sonic_weld_data_weld_table_rows']=weld_rows;updates['sonic_weld_data_object_roots_added']=roots;updates['sonic_weld_data_parent_builder_sha256']=sha(original.__file__);updates.update(status=")
 namespace=dict(vars(original),__file__=__file__,derived_source=source,original=original);exec(compile(source,__file__,'exec'),namespace);namespace['main']()
if __name__=='__main__':main()
