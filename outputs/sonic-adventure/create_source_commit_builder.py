from pathlib import Path
p=Path(__file__).resolve().parent
(p/'build_sound_source_commit.ps1').write_text((p/'build_sound_source_query.ps1').read_text().replace('sound_source_query','sound_source_commit'))
r=(p/'register_sound_source_query.py').read_text().replace('sound_source_query','sound_source_commit').replace('==288','==192').replace('0x8c65e4b8','0x8c65e58a').replace('0x8c65e58a\'', '0x8c65e644\'').replace('0x64e4b8:0x64e58a','0x64e58a:0x64e644')
# Keep the entry separate from the exclusive end after replacing the old entry.
r=r.replace("address='0x8c65e644'","address='0x8c65e58a'")
(p/'register_sound_source_commit.py').write_text(r)
path=p/'source/sa_sound_source_commit.c';s=path.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a)
path.write_text(s[:a]+'/* Retail source commit65e58a..65e644, explicit SH4 ABI.\n * Ring mirror/wrap copies and per-mode counters. Scope: sound_source_commit_verification.json. */\n'+s[b:])
