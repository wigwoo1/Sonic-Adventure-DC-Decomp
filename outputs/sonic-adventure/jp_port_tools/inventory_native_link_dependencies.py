"""Map actual native-link diagnostics to pinned source inventory providers."""
import argparse,hashlib,json,re
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--workspace',type=Path,required=True);a=ap.parse_args();r=a.workspace.resolve();w=r/'work/sonic-adventure';p=r/'outputs/sonic-adventure'
 build_path=p/'analysis/jp_native_heap_build_v2_verification.json';build=json.loads(build_path.read_text());assert [c['exit_code'] for c in build['commands']]==[0,0,1]
 log=Path(build['commands'][-1]['log']);assert sha(log)==build['commands'][-1]['log_sha256']
 names=sorted(set(re.findall(r'undefined symbol: ([^\r\n]+)',log.read_text())))
 inventory_path=w/'japanese_source_entry_inventory.json';inventory=json.loads(inventory_path.read_text());assert inventory['upstream_commit']==build['upstream_commit']
 up=Path(build['upstream_source']).parents[2];symbol_table=up/'config/1st_read/symbols.txt'
 symbols={m[2]:m[1] for line in symbol_table.read_text().splitlines() if (m:=re.fullmatch(r'(0x[0-9a-fA-F]+)\s+(\S+)',line))}
 results=[]
 for name in names:
  providers=[]
  for entry in inventory['entries']:
   if entry['image']=='1st_read' and name in entry['names']:
    source=up/entry['source'];assert sha(source)==inventory['source_sha256'][entry['source']]
    providers.append(dict(source=entry['source'],source_sha256=sha(source),Japanese_address=hex(entry['address']),declarations=entry['declarations'],assembly_source=source.suffix=='.src'))
  results.append(dict(symbol=name,original_symbol_table_address=symbols.get(name),source_named_entry_providers=providers,provider_behavior_on_native_PC_verified=False))
 report=dict(status='verified_native_object_compilation_and_unresolved_translation_unit_import_inventory',upstream_commit=build['upstream_commit'],native_target='x86_64-pc-windows-msvc',original_task_source_compiles=True,authored_heap_bridge_compiles=True,compiler_diagnostics_for_both_objects_empty=all(c['log_bytes']==0 for c in build['commands'][:2]),native_DLL_linked=False,behavior_tests_possible=False,unresolved_symbols=len(results),providers=results,
 limitations=['These are the whole task translation unit imports reported by the linker, not a claim that each heap function calls Ninja.','Source/provider correspondence is not native ABI or behavior qualification.','The size_t host adapter fixes one compiler ABI conflict; disc layouts and all other pointer assumptions remain unaudited.','No dummy collision/event/vector implementations were linked. No native game or heap behavior was executed.'],
 next_native_integration_task='Adapt real shared math/Ninja and collision/event lifecycle providers as a coherent dependency group, using the upstream GCC C replacements and portable host interfaces. Establish explicit 32-bit data versus native pointer layouts before linking gameplay.',
 builder_sha256=sha(p/'jp_port_tools/build_native_heap_probe_v2.py'),native_build_report_sha256=sha(build_path),link_log_sha256=sha(log),source_inventory_sha256=sha(inventory_path),symbol_table_sha256=sha(symbol_table),script_sha256=sha(Path(__file__)),overall_completion_percent=10,goal_paused=False,playable=False)
 target=p/'analysis/jp_native_link_dependency_inventory.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
 print('Mapped',len(results),'native-link imports;',sum(bool(x['source_named_entry_providers']) for x in results),'have source-named code providers. Runtime remains unverified.',flush=True)
if __name__=='__main__':main()
