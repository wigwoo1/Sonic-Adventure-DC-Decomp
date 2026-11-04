/* Diagnostic access to the complete producer. No production frame services.
 * The included bytes are identical to the compiled production model source.
 */
#include JP_MODEL_SOURCE
int event_skipping;
Float port_view_aspect=1.0F;
__declspec(dllexport) int jp_model_probe_project(const NJS_MODEL *m,void *p)
{return project_points(m,p);}
__declspec(dllexport) void jp_model_probe_shade(const NJS_MODEL *m,void *p,Uint32 flags)
{shade_points(m,p,&m->mats[m->meshsets[0].type_matId&MATERIAL_ID],flags);}
__declspec(dllexport) Uint32 *jp_model_probe_offsets(void){return lit_offsets;}
__declspec(dllexport) Uint32 *jp_model_probe_palettes(void){return draw_colors.words;}
__declspec(dllexport) void jp_model_probe_attrs(void){take_constant_attr();}
__declspec(dllexport) int jp_model_probe_cull(const NJS_MODEL *m){return model_in_view(m);}
__declspec(dllexport) void jp_model_probe_alpha(void *p,int count,const NJS_MATERIAL *m,Uint32 flags)
{set_point_alpha(p,count,m,flags);}
__declspec(dllexport) void jp_model_probe_meshes(const NJS_MODEL *m,void *p,int clipped)
{draw_meshes(m,p,clipped);}
