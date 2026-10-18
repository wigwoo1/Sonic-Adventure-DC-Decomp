from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_wave_retail_pipeline.py').read_text()
s=s.replace("('sound_header_detectors',[(0x8c66ab2a,'sa_sound_wave_header_detect')", "('sound_stream_format_initialize',[(0x8c667596,'sa_sound_stream_format_initialize')]),('sound_header_detectors',[(0x8c66a61a,'sa_sound_spsd_header_detect'),(0x8c66ab2a,'sa_sound_wave_header_detect')")
s=s.replace('(0x8c66a714,[0x8ca20000,0x8ca10000,len(data)])','(0x8c667596,[0x8ca20000,0x8ca10000,len(data)])').replace('if address==0x8c66a714:','if address==0x8c667596:')
s=s.replace('analysis/wave_retail_pipeline_verification.json','analysis/wave_format_pipeline_verification.json').replace('native detector->initializer->transfer dispatcher','native detector->format selector->initializer->transfer dispatcher').replace('Native comparator,metadata/division/fill','Native SPSD/WAVE detectors,comparator,format selector,metadata/division/fill').replace('original WAVE pipeline comparisons passed','original WAVE format pipeline comparisons passed')
(p/'verify_wave_format_pipeline.py').write_text(s)
