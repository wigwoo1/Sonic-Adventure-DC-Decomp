from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'register_sound_stream_sample_bits_get.py').read_text()
s=s.replace('sound_stream_sample_bits_get','sound_work_reset').replace('0x8c6676b2','0x8c65b606').replace('0x8c6676ee','0x8c65b624').replace('0x6576b2:0x6576ee','0x64b606:0x64b624').replace('Odd decode flag verification failed','Sound work reset verification failed')
(p/'register_sound_work_reset.py').write_text(s)
