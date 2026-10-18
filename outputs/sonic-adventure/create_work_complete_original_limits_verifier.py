from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_work_complete_native_getters.py').read_text()
old='for total,consumed,channels,limit,callback,bank in itertools.product([5,32,100],[0,4],[0,1,2],[0xffffffff,0,50],[0,0x8c010300],[0,1]):'
assert old in s
s=s.replace(old,"inventory=json.loads((p/'analysis/magic_audio_endings_inventory.json').read_text())\nassets=[a for a in inventory['assets'] if a['first_word_after_end']=='0x8001']\nassert len(assets)==108\nfor asset,bank in itertools.product(assets,[0,1]):\n    header=(ROOT/asset['path']).read_bytes()[:64]\n    total=int.from_bytes(header[12:16],'big');assert total==asset['total_samples']\n    valid=total%32 or 32;assert valid==asset['final_pair_valid_samples']\n    consumed=total-valid;channels=2;limit=0xffffffff;callback=0")
a=s.index("scope='");z=s.index("'),indent=2)",a)
s=s[:a]+"scope='216 original-header final sample limit comparisons: 108 assets with confirmed 0x8001 ending, both FPU banks. Native completion/getters/range splitting/source commit/return/locks/remainder/acknowledgment; no function models invoked. Totals read directly from original headers; consumed fixture equals total minus final valid sample count. Independent counters, source objects, returned reservations and unchanged buffers checked against original SH4 instructions. 104 assets have partial last sample pairs. This tests header-derived trimming and bookkeeping with controlled decoded32/bytes36/reservations, not preceding full-file decode history or actual final sample values. SSRACING header exceeds file and is excluded; full playback/fullgame unverified."+s[z:]
s=s.replace('work_complete_native_getters_verification','work_complete_original_limits_verification').replace('native getters work completion comparisons','original header limits work completion comparisons')
(p/'verify_work_complete_original_limits.py').write_text(s)
r=(p/'register_work_complete_native_getters.py').read_text().replace('work_complete_native_getters','work_complete_original_limits').replace('Native completion getters in stream work completion:','Original header final limits in stream work completion:')
(p/'register_work_complete_original_limits.py').write_text(r)
