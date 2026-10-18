from pathlib import Path
p=Path(__file__).resolve().parent
for kind in ['allocate','release']:
    name='file_decode_handle_'+kind
    (p/('build_'+name+'.ps1')).write_text((p/'build_sound_source_query.ps1').read_text().replace('sound_source_query',name))
    path=p/'source'/('sa_'+name+'.c');s=path.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a)
    path.write_text(s[:a]+'/* Retail decode handle '+kind+' with explicit SH4 ABI.\n * Verified scope: file_handle_pool_verification.json. */\n'+s[b:])
