"""Add untextured 32-byte floating ARGB support; preserve earlier backends."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;S=P/'source';files=[]
def write(name,text):
 path=P/name;assert not path.exists();path.write_text(text);files.append(path);return path
old=S/'frame_events_bus_eol.c';text=old.read_text();marker='(kind==4u&&(word&0xd0u))';assert text.count(marker)==1
text=text.replace(marker,'(kind==4u&&(word&0xd0u)&&((word&0xfdu)!=0x10u))')
text='/* Extend the existing event profile with untextured floating ARGB32 vertices.\n * The previous packed/intensity profile and unsupported-format guards remain. */\n'+text
write('source/frame_events_bus_float.c',text);files.append(old)
old=S/'pvr_pixels.c';text=old.read_text();marker='(format!=0u&&format!=2u)';assert text.count(marker)==1
text=text.replace(marker,'(format==3u)||(format==1u&&(pcw&8u))')
marker=' }else v->color=w[6];';assert text.count(marker)==1
text=text.replace(marker,''' }else if(((h->pcw>>4)&3u)==1u){
  v->color=0;
  for(unsigned i=0;i<4;i++){
   double component=number(w[4u+i]);
   if(!finite(component)||component<0||component>1)return -3;
   v->color|=(uint32_t)quantize(component,255.0)<<(24u-8u*i);
  }
 }else v->color=w[6];''')
text='/* Add the original untextured floating ARGB32 vertex layout.\n * This bounded host profile rounds finite [0,1] channels to nearest 8-bit values;\n * it does not claim hardware colour conversion or precision. */\n'+text
write('source/pvr_pixels_float.c',text);files.append(old)
old=P/'build_model_view_producer.ps1';write('build_model_float_color.ps1',old.read_text().replace('frame_events_bus_eol.c','frame_events_bus_float.c').replace('model_view_producer.dll','model_float_color.dll'));files.append(old)
old=P/'build_banked_pixel_jobs.ps1';write('build_float_color_pixels.ps1',old.read_text().replace('source/pvr_pixels.c','source/pvr_pixels_float.c').replace('banked_pixel_jobs.dll','float_color_pixels.dll'));files.append(old)
old=P/'frame_events_eol_reference.py';text=old.read_text();marker='(tag==4 and words[0]&0xd0)';assert text.count(marker)==1
text=text.replace(marker,'(tag==4 and words[0]&0xd0 and words[0]%256 not in (16,18))')
write('frame_events_float_reference.py',text);files.append(old)
old=P/'pvr_pixels_reference.py';text=old.read_text();marker='color not in (0,2)';assert text.count(marker)==1
text=text.replace(marker,'color not in (0,1,2) or (color==1 and pcw&8)')
marker='    else:col=w[6]';assert text.count(marker)==1
text=text.replace(marker,'''    elif h['color']==1:
     components=[float_value(bits) for bits in w[4:8]]
     if any(v<0 or v>1 for v in components):raise ValueError(-3)
     col=sum(rounded(v*255)*(256**(3-i)) for i,v in enumerate(components))
    else:col=w[6]''')
write('pvr_pixels_float_reference.py',text);files.append(old)
old=P/'pixel_banked_native.py';write('float_color_pixels_native.py',old.read_text().replace('banked_pixel_jobs.dll','float_color_pixels.dll'));files.append(old)
report=dict(status='additive_untextured_float_ARGB32_platform_support_behavior_proofs_separate',new_original_routine_credit=0,previous_backends_preserved=True,supported_new_profile='32-byte polygon header and 32-byte untextured ARGB float vertices, non-volume/non-shadow, no offset, existing flat/Gouraud shading',unsupported_new_profiles=['textured floating colour64-byte vertices','offset/specular floating colour','modifier/shadow','nonfinite or out-of-range vertex channels'],hardware_precision_qualified=False,primary_layout_sources=['https://raw.githubusercontent.com/KallistiOS/KallistiOS/master/kernel/arch/dreamcast/include/dc/pvr/pvr_header.h','https://raw.githubusercontent.com/flyinghead/flycast/master/core/hw/pvr/ta_structs.h'],third_party_implementation_copied=False,sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files+[Path(__file__)]})
write('analysis/model_float_color_extension.json',json.dumps(report,indent=2)+'\n')
