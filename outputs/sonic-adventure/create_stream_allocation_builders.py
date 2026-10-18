from pathlib import Path
p=Path(__file__).resolve().parent
for kind in ['create','destroy']:
    name='sound_stream_'+kind
    (p/('build_'+name+'.ps1')).write_text((p/'build_sound_source_query.ps1').read_text().replace('sound_source_query',name))
    path=p/'source'/('sa_'+name+'.c');s=path.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a)
    path.write_text(s[:a]+'/* Retail sound stream '+kind+' with explicit SH4 register/RAM ABI.\n * File handle services remain external. Verification: stream_allocation_verification.json. */\n'+s[b:])
