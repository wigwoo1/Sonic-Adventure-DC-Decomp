from pathlib import Path
p=Path(__file__).resolve().parent
for kind in ['enter','exit']:
    name='sound_source_lock_'+kind
    (p/('build_'+name+'.ps1')).write_text((p/'build_sound_source_query.ps1').read_text().replace('sound_source_query',name))
    src=p/'source'/('sa_'+name+'.c');s=src.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a)
    src.write_text(s[:a]+'/* Retail sound source interrupt mask '+kind+'. Explicit SH4 register ABI.\n * Scope documented in sound_source_locks_verification.json. */\n'+s[b:])
