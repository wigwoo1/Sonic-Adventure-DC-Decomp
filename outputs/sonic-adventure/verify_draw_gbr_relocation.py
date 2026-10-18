"""Check live GBR state at aliases, distinct RAM records and boundary faults."""
import collections,json
from pathlib import Path
import verify_draw_gbr_geometry as base
from verify_overlay_selector_initializer import store
PROJECT=base.PROJECT

def main():
 proof=PROJECT/'analysis/draw_gbr_geometry_verification.json';body=json.loads(proof.read_text())
 for p,h in body['sha256'].items():assert base.digest(p)==h,p
 ram=bytearray(16<<20)
 for path,offset in ((base.SEED.state.gate.support.morph.MAIN,0x10000),(base.SEED.state.gate.support.morph.MODULE,0x900000)):
  data=path.read_bytes();ram[offset:offset+len(data)]=data
 ram=base.caller.prepare(ram,dict(draw_flags=0x10,semantic=True))[0];records=[];boundaries=0
 cases=[dict(entry=entry,mode=bank*4,renderer_record=record,semantic=entry==base.INIT or record&0xffffff==0x8ffe00) for entry in (base.INIT,base.CULL) for bank in range(2) for record in (0x0c8ffe00,0x8c8ffe00,0xac8ffe00,0x8ce10000,0x8ce18000)]
 cases.extend(dict(entry=base.INIT,mode=bank*4,renderer_record=record) for bank in range(2) for record in (0x8cffffa8,0x8cfffffc,0))
 for c in cases:
  m=bytearray(ram)
  # Change a data literal in this isolated guest-RAM fixture, never the original
  # game file/opcodes. LDC and subsequent accesses must use the loaded pointer.
  store(m,base.ORIGINAL_GBR_LITERAL,c['renderer_record'])
  record,path,b=base.compare(*base.prepare(m,c),c);records.append(record);boundaries+=b
  if c['entry']==base.CULL:assert c['gbr']==c['renderer_record'],(c,'actual initializer result supplied as live GBR')
 files=[Path(__file__),PROJECT/'verify_draw_gbr_geometry.py',proof]
 report=dict(cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),boundary_comparisons=boundaries,distinct_physical_renderer_records=['0x8ffe00','0xe10000','0xe18000'],unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records if r['status']==-2)),external_game_service_fixtures=0,dll_sha256=base.digest(base.DLL),sha256={str(p):base.digest(p) for p in files},scope='Original/native live-GBR initialization, segment aliases and distinct RAM records; bad/boundary pointer literals preserve exact reached partial state. Original binary/opcodes remain unchanged; the initializer pointer data literal is patched only in isolated guest-RAM fixtures. Full machine/GBR/RAM, ordered entry and queue state compared. Alternate-record predicates retain the original fixed configuration pointer rather than assuming it aliases relocated GBR.',cases=records)
 assert not report['unconnected_targets'] and report['cases_passed']==26
 (PROJECT/'analysis/draw_gbr_relocation_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','independent_cases','independent_checks','boundary_comparisons')},indent=2))
if __name__=='__main__':main()
