"""Structural ARM export address comparison; not semantic/completeness coverage."""
import csv,hashlib,json,re
from collections import defaultdict
from pathlib import Path
P=Path(__file__).resolve().parent
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
builder=P/'build_arm_sound_boot_native.ps1'
names=re.findall(r"'(sa_arm_sound[^']+\.c)'",builder.read_text());drivers=[];paths=[Path(__file__),builder]
exception_translation=P/'analysis/arm_sound_exception_translation.json';exceptions=json.loads(exception_translation.read_text());paths.append(exception_translation)
input_translation=P/'analysis/arm_sound_input_translation.json';standalone=json.loads(input_translation.read_text());paths.append(input_translation)
for driver,name in enumerate(('MANATEE.DRV','MASARU.DRV')):
    known=set()
    for source in names:
        path=P/'source'/source;paths.append(path);text=path.read_text()
        if 'if(driver==0){switch(r[15]){' in text:text=text.split('if(driver==0){switch(r[15]){',1)[1].split('}}else{switch(r[15]){',1)[driver]
        known.update(int(a,16) for a in re.findall(r'case (0x[0-9a-f]+)u:',text))
    listing=P/'analysis/arm-sequencer-enrichment'/f'{name}.arm.bin/instructions.tsv';paths.append(listing)
    rows=list(csv.DictReader(listing.open(),delimiter='\t'));missing=[r for r in rows if int(r['address'],16) not in known]
    extra={int(r['address'],16) for r in standalone['drivers'][driver]}
    remaining=[r for r in missing if int(r['address'],16) not in extra]
    exception_addresses={int(r['address'],16) for r in exceptions['drivers'][driver]}
    final_remaining=[r for r in remaining if int(r['address'],16) not in exception_addresses]
    groups=defaultdict(list)
    for row in remaining:groups[row['function_entry'] or 'unassigned'].append(row['address'])
    drivers.append({'driver':name,'exported_instruction_rows':len(rows),'rows_without_main_native_case':len(missing),'standalone_input_sites_not_in_main':sum(int(r['address'],16) in extra for r in missing),'remaining_after_standalone_input':len(remaining),'standalone_exception_sites_not_in_main':sum(int(r['address'],16) in exception_addresses for r in remaining),'remaining_after_both_standalone_modules':len(final_remaining),'remaining_groups':dict(groups),'missing_main_rows':missing})
report={'drivers':drivers,'sha256':{str(p):sha(p) for p in paths},'scope':'Address-case presence in main build sources versus enriched Ghidra export snapshots, plus separately recovered input and exception translations. Structural only: case presence does not prove correct semantics, reachability, execution mode, completeness of original code discovery or full decompilation. Snapshot contains analysis warnings and is not a whole-game routine denominator. No milestone credit.'}
(P/'analysis/arm_sound_native_site_audit.json').write_text(json.dumps(report,indent=2)+'\n')
for d in drivers:print(d['driver'],d['rows_without_main_native_case'],'exported sites absent from main cases;',d['standalone_input_sites_not_in_main'],'standalone input sites;',d['remaining_after_standalone_input'],'other sites;',d['remaining_after_both_standalone_modules'],'without any recovered case in snapshot')
