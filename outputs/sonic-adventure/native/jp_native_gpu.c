/* Authored D3D11 backend. No SDK/game/emulator bodies are copied or linked here.
 * Inputs come from the qualified packet and encoded-surface consumers.
 */
#define COBJMACROS
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "jp_native_gpu.h"
#include "jp_native_texels.h"
extern const void *jp_frame_surface(uint64_t frame,uint32_t word,size_t *bytes);
struct jp_gpu {
    ID3D11Device *device;ID3D11DeviceContext *context;
    ID3D11VertexShader *vs;ID3D11PixelShader *ps,*off_ps;
    ID3D11InputLayout *layout;D3D_FEATURE_LEVEL level;
    uint32_t texture_bindings[2],sampler_bindings[2];
};
struct gpu_vertex { float x,y,q,depth,u,v,base[4],offset[4]; };
struct constants { float logical[2];uint32_t textured,shade,alpha,ignore_alpha,offset,punch;float threshold,padding[3]; };
struct draw_batch {
    ID3D11ShaderResourceView *texture;ID3D11SamplerState *sampler;
    ID3D11DepthStencilState *depth;ID3D11BlendState *blend;
    UINT first,count;
};
static const char shader[]=
    "cbuffer Params:register(b0){float2 logicalSize;uint textured;uint shade;uint useAlpha;uint ignoreAlpha;uint useOffset;uint punch;float threshold;float3 pad;}\n"
    "struct V{float2 xy:POSITION;float q:TEXCOORD0;float depth:TEXCOORD1;float2 uv:TEXCOORD2;float4 base:COLOR0;float4 offset:COLOR1;};\n"
    "struct O{float4 pos:SV_POSITION;float3 uvq:TEXCOORD0;float depth:TEXCOORD1;float4 base:COLOR0;float4 offset:COLOR1;};\n"
    "O vertexMain(V v){O o;o.pos=float4(v.xy.x*2/logicalSize.x-1,1-v.xy.y*2/logicalSize.y,0.5,1);o.uvq=float3(v.uv*v.q,v.q);o.depth=v.depth;o.base=v.base*v.q;o.offset=v.offset*v.q;return o;}\n"
    "struct Pixel{float4 color:SV_TARGET;float depth:SV_Depth;};\n"
    "#ifndef TEXTURES_OFF\nTexture2D surface:register(t0);SamplerState filtering:register(s0);\n#endif\n"
    "Pixel pixelMain(O i){float4 b=saturate(i.base/i.uvq.z);float4 o=saturate(i.offset/i.uvq.z);if(!useAlpha)b.a=1;float4 c=b;\n"
    "#ifndef TEXTURES_OFF\nif(textured){float4 t=surface.Sample(filtering,i.uvq.xy/i.uvq.z);if(ignoreAlpha)t.a=1;if(shade==0)c=t;else if(shade==1)c=float4(b.rgb*t.rgb,t.a);else if(shade==2)c=float4(lerp(b.rgb,t.rgb,t.a),b.a);else c=b*t;}\n#endif\n"
    "if(useOffset)c.rgb+=o.rgb;if(punch && c.a<threshold)discard;Pixel p;p.color=saturate(c);p.depth=i.depth;return p;}\n";
static int fail(struct jp_gpu_failure *f,int code,size_t batch,HRESULT hr)
{if(f){f->code=code;f->batch=batch>UINT32_MAX?UINT32_MAX:(uint32_t)batch;f->hresult=(int32_t)hr;}return code;}
static float clamp(float f){return f<0?0:f>1?1:f;}
static int finite(float f){return isfinite(f);}
static HRESULT compile(const char *entry,const char *target,int off,ID3DBlob **out)
{
    ID3DBlob *error=NULL;D3D_SHADER_MACRO macros[]={{"TEXTURES_OFF","1"},{NULL,NULL}};
    HRESULT hr=D3DCompile(shader,sizeof shader-1,"authored_native_frame_shader",off?macros:NULL,NULL,entry,target,
        D3DCOMPILE_ENABLE_STRICTNESS|D3DCOMPILE_WARNINGS_ARE_ERRORS,0,out,&error);
    if(error)ID3D10Blob_Release(error);return hr;
}
static HRESULT inventory(ID3DBlob *blob,uint32_t *textures,uint32_t *samplers)
{
    ID3D11ShaderReflection *reflection=NULL;D3D11_SHADER_DESC desc;
    HRESULT hr=D3DReflect(ID3D10Blob_GetBufferPointer(blob),ID3D10Blob_GetBufferSize(blob),&IID_ID3D11ShaderReflection,(void **)&reflection);
    if(FAILED(hr))return hr;
    *textures=*samplers=0;hr=reflection->lpVtbl->GetDesc(reflection,&desc);
    if(SUCCEEDED(hr))for(UINT i=0;i<desc.BoundResources;i++){
        D3D11_SHADER_INPUT_BIND_DESC binding;hr=reflection->lpVtbl->GetResourceBindingDesc(reflection,i,&binding);if(FAILED(hr))break;
        if(binding.Type==D3D_SIT_TEXTURE)*textures+=binding.BindCount;
        if(binding.Type==D3D_SIT_SAMPLER)*samplers+=binding.BindCount;
    }
    reflection->lpVtbl->Release(reflection);return hr;
}
int jp_gpu_open(struct jp_gpu **out,struct jp_gpu_failure *failure)
{
    if(!out)return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    struct jp_gpu *g=calloc(1,sizeof *g);if(!g)return fail(failure,JP_GPU_ALLOCATION,0,E_OUTOFMEMORY);
    D3D_FEATURE_LEVEL wanted=D3D_FEATURE_LEVEL_11_0;ID3DBlob *v=NULL,*p=NULL,*off=NULL;
    HRESULT hr=D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,0,&wanted,1,D3D11_SDK_VERSION,&g->device,&g->level,&g->context);
    int code=JP_GPU_DEVICE;if(FAILED(hr))goto error;
    code=JP_GPU_SHADER;hr=compile("vertexMain","vs_5_0",0,&v);if(FAILED(hr))goto error;
    hr=compile("pixelMain","ps_5_0",0,&p);if(FAILED(hr))goto error;
    hr=compile("pixelMain","ps_5_0",1,&off);if(FAILED(hr))goto error;
    hr=inventory(p,&g->texture_bindings[1],&g->sampler_bindings[1]);if(FAILED(hr))goto error;
    hr=inventory(off,&g->texture_bindings[0],&g->sampler_bindings[0]);if(FAILED(hr))goto error;
    if(g->texture_bindings[0]||g->sampler_bindings[0]){hr=E_FAIL;goto error;}
    code=JP_GPU_API;hr=ID3D11Device_CreateVertexShader(g->device,ID3D10Blob_GetBufferPointer(v),ID3D10Blob_GetBufferSize(v),NULL,&g->vs);if(FAILED(hr))goto error;
    hr=ID3D11Device_CreatePixelShader(g->device,ID3D10Blob_GetBufferPointer(p),ID3D10Blob_GetBufferSize(p),NULL,&g->ps);if(FAILED(hr))goto error;
    hr=ID3D11Device_CreatePixelShader(g->device,ID3D10Blob_GetBufferPointer(off),ID3D10Blob_GetBufferSize(off),NULL,&g->off_ps);if(FAILED(hr))goto error;
    D3D11_INPUT_ELEMENT_DESC elements[]={
        {"POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"TEXCOORD",0,DXGI_FORMAT_R32_FLOAT,0,8,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"TEXCOORD",1,DXGI_FORMAT_R32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"TEXCOORD",2,DXGI_FORMAT_R32G32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"COLOR",1,DXGI_FORMAT_R32G32B32A32_FLOAT,0,40,D3D11_INPUT_PER_VERTEX_DATA,0}};
    hr=ID3D11Device_CreateInputLayout(g->device,elements,6,ID3D10Blob_GetBufferPointer(v),ID3D10Blob_GetBufferSize(v),&g->layout);if(FAILED(hr))goto error;
    ID3D10Blob_Release(v);ID3D10Blob_Release(p);ID3D10Blob_Release(off);*out=g;return fail(failure,JP_GPU_OK,0,S_OK);
error:
    if(v)ID3D10Blob_Release(v);if(p)ID3D10Blob_Release(p);if(off)ID3D10Blob_Release(off);
    jp_gpu_close(g);return fail(failure,code,0,hr);
}
void jp_gpu_close(struct jp_gpu *g)
{
    if(!g)return;if(g->context)ID3D11DeviceContext_ClearState(g->context);
    if(g->layout)ID3D11InputLayout_Release(g->layout);if(g->vs)ID3D11VertexShader_Release(g->vs);
    if(g->ps)ID3D11PixelShader_Release(g->ps);if(g->off_ps)ID3D11PixelShader_Release(g->off_ps);
    if(g->context)ID3D11DeviceContext_Release(g->context);if(g->device)ID3D11Device_Release(g->device);free(g);
}
static int account(size_t *used,size_t count,size_t width,size_t budget)
{if(*used>budget || count>(budget-*used)/width)return 0;*used+=count*width;return 1;}
static D3D11_BLEND factor(unsigned n,int source,int alpha)
{
    static const D3D11_BLEND common[8]={D3D11_BLEND_ZERO,D3D11_BLEND_ONE,D3D11_BLEND_ZERO,D3D11_BLEND_ZERO,D3D11_BLEND_SRC_ALPHA,D3D11_BLEND_INV_SRC_ALPHA,D3D11_BLEND_DEST_ALPHA,D3D11_BLEND_INV_DEST_ALPHA};
    if(n==2)return alpha?(source?D3D11_BLEND_DEST_ALPHA:D3D11_BLEND_SRC_ALPHA):(source?D3D11_BLEND_DEST_COLOR:D3D11_BLEND_SRC_COLOR);
    if(n==3)return alpha?(source?D3D11_BLEND_INV_DEST_ALPHA:D3D11_BLEND_INV_SRC_ALPHA):(source?D3D11_BLEND_INV_DEST_COLOR:D3D11_BLEND_INV_SRC_COLOR);
    return common[n];
}
static D3D11_TEXTURE_ADDRESS_MODE address_mode(unsigned clamp_bit,unsigned flip_bit)
{return clamp_bit?D3D11_TEXTURE_ADDRESS_CLAMP:flip_bit?D3D11_TEXTURE_ADDRESS_MIRROR:D3D11_TEXTURE_ADDRESS_WRAP;}
static HRESULT texture(struct jp_gpu *g,uint64_t frame,const struct jp_packet_batch *b,
    const struct jp_gpu_config *cfg,struct draw_batch *out,size_t *cost,struct jp_gpu_report *report,int *code)
{
    uint32_t tsp=b->header[2],tcw=b->header[3];size_t bytes=0;const void *src=jp_frame_surface(frame,tcw&0x1fffff,&bytes);
    if(!src){*code=JP_GPU_TEXTURE;return E_INVALIDARG;}
    struct jp_texel_level levels[11];unsigned count=1;size_t total=0;
    for(unsigned i=0;i<count;i++){
        if(jp_texel_describe(tsp,tcw,i,cfg->stride_pixels,&levels[i]) || levels[i].required_source_bytes>bytes){*code=JP_GPU_TEXTURE;return E_INVALIDARG;}
        count=levels[0].mip_count;total+=levels[i].rgba_bytes;
    }
    if(!account(cost,total,2,cfg->budget)){*code=JP_GPU_BUDGET;return E_OUTOFMEMORY;}
    unsigned char *pixels=malloc(total);if(!pixels){*code=JP_GPU_ALLOCATION;return E_OUTOFMEMORY;}
    size_t at=0;D3D11_SUBRESOURCE_DATA data[11];HRESULT hr=E_INVALIDARG;ID3D11Texture2D *tex=NULL;
    for(unsigned i=0;i<count;i++){
        struct jp_texel_level info;
        if(jp_texel_decode(tsp,tcw,i,cfg->stride_pixels,src,bytes,cfg->palette,cfg->palette_entries,pixels+at,levels[i].rgba_bytes,&info)){*code=JP_GPU_TEXTURE;goto done;}
        data[i].pSysMem=pixels+at;data[i].SysMemPitch=levels[i].width*4;data[i].SysMemSlicePitch=0;at+=levels[i].rgba_bytes;
    }
    D3D11_TEXTURE2D_DESC desc={0};desc.Width=levels[0].width;desc.Height=levels[0].height;desc.MipLevels=count;desc.ArraySize=1;
    desc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;desc.SampleDesc.Count=1;desc.Usage=D3D11_USAGE_IMMUTABLE;desc.BindFlags=D3D11_BIND_SHADER_RESOURCE;
    *code=JP_GPU_API;hr=ID3D11Device_CreateTexture2D(g->device,&desc,data,&tex);if(FAILED(hr))goto done;
    hr=ID3D11Device_CreateShaderResourceView(g->device,(ID3D11Resource *)tex,NULL,&out->texture);if(FAILED(hr))goto done;
    D3D11_SAMPLER_DESC sampler={0};sampler.Filter=((tsp>>13)&3)?D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT:D3D11_FILTER_MIN_MAG_MIP_POINT;
    sampler.AddressU=address_mode(tsp&0x10000,tsp&0x40000);sampler.AddressV=address_mode(tsp&0x8000,tsp&0x20000);sampler.AddressW=D3D11_TEXTURE_ADDRESS_CLAMP;
    sampler.MaxLOD=(float)(count-1);sampler.MaxAnisotropy=1;sampler.ComparisonFunc=D3D11_COMPARISON_NEVER;
    hr=ID3D11Device_CreateSamplerState(g->device,&sampler,&out->sampler);
    if(SUCCEEDED(hr)){++report->texture_uploads;report->decoded_texels+=total/4;}
done:
    if(tex)ID3D11Texture2D_Release(tex);free(pixels);return hr;
}
int jp_gpu_draw_frame(struct jp_gpu *g,uint64_t frame,const struct jp_gpu_config *cfg,
    void *rgba,size_t bytes,struct jp_gpu_report *report,struct jp_gpu_failure *failure)
{
    if(!g||!cfg||!rgba||!report||!cfg->width||!cfg->height||cfg->width>8192||cfg->height>8192||
       !finite(cfg->logical_width)||!finite(cfg->logical_height)||cfg->logical_width<1||cfg->logical_height<1||
       cfg->textures>1||(cfg->flat_color_vertex!=0 && cfg->flat_color_vertex!=2)||!finite(cfg->punch_alpha)||cfg->punch_alpha<0||cfg->punch_alpha>1||
       !finite(cfg->cull_small_area)||cfg->cull_small_area<0)return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    for(unsigned i=0;i<4;i++)if(!finite(cfg->clear_rgba[i])||cfg->clear_rgba[i]<0||cfg->clear_rgba[i]>1)return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    size_t pixels=(size_t)cfg->width*cfg->height;if(bytes<pixels*4)return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    struct jp_packet_plan *plan=NULL;struct jp_packet_failure pf;size_t cost=0;
    if(jp_packets_decode_frame(frame,cfg->budget,&plan,&pf))return fail(failure,JP_GPU_PACKETS,pf.list,E_INVALIDARG);
    struct jp_gpu_report result={0};struct draw_batch *batches=NULL;struct gpu_vertex *vertices=NULL;size_t bi=0;int code=JP_GPU_BUDGET;HRESULT hr=E_OUTOFMEMORY;
    ID3D11Texture2D *target=NULL,*staging=NULL,*zbuffer=NULL;ID3D11RenderTargetView *rtv=NULL;ID3D11DepthStencilView *dsv=NULL;
    ID3D11Buffer *vb=NULL,*cb=NULL;ID3D11RasterizerState *raster=NULL;
    if(!account(&cost,plan->allocation_bytes,1,cfg->budget)||!account(&cost,pixels,12,cfg->budget)||
       !account(&cost,plan->batch_count?plan->batch_count:1,sizeof *batches,cfg->budget)||
       !account(&cost,plan->triangle_count?plan->triangle_count:1,6*sizeof *vertices,cfg->budget)||
       !account(&cost,1,sizeof(struct constants),cfg->budget))goto done;
    if(plan->triangle_count>UINT_MAX/(3*sizeof *vertices)){code=JP_GPU_GEOMETRY;hr=E_INVALIDARG;goto done;}
    batches=calloc(plan->batch_count?plan->batch_count:1,sizeof *batches);vertices=malloc(plan->triangle_count?plan->triangle_count*3*sizeof *vertices:1);
    if(!batches||!vertices){code=JP_GPU_ALLOCATION;goto done;}
    float maximum=1;
    for(size_t i=0;i<plan->vertex_count;i++){
        const struct jp_packet_vertex *v=&plan->vertices[i];
        if(v->inv_w<0 || v->inv_w>1e20f || (v->inv_w>0 && v->inv_w<1e-20f)){code=JP_GPU_GEOMETRY;hr=E_INVALIDARG;goto done;}
        if(v->inv_w>maximum)maximum=v->inv_w;
    }
    int exponent;frexpf(maximum,&exponent);float scale=ldexpf(1,-exponent);size_t emitted=0;
    for(bi=0;bi<plan->batch_count;bi++){
        const struct jp_packet_batch *b=&plan->batches[bi];uint32_t pcw=b->header[0],isp=b->header[1],tsp=b->header[2];
        /* No silent hardware-state substitution. Ordinary color clamp means
         * the native RGBA8 range here; custom hardware clamp registers remain
         * a frame-state integration gate. No table fog or secondary buffers. */
        if((pcw&0x000300c0)||((tsp>>22)&3)!=2||(tsp&0x03000000)||
           (cfg->textures&&(pcw&8)&&((tsp&0x1000) || ((b->header[3]>>31) && (((tsp>>13)&3)>1 || ((tsp>>8)&15)!=4))))){code=JP_GPU_STATE;hr=E_INVALIDARG;goto done;}
        batches[bi].first=(UINT)emitted;
        for(size_t ti=b->first_triangle;ti<b->first_triangle+b->triangle_count;ti++){
            const struct jp_packet_triangle *t=&plan->triangles[ti];const struct jp_packet_vertex *v[3];for(unsigned i=0;i<3;i++)v[i]=&plan->vertices[t->indices[i]];
            double area=((double)v[1]->x-v[0]->x)*((double)v[2]->y-v[0]->y)-((double)v[1]->y-v[0]->y)*((double)v[2]->x-v[0]->x);
            unsigned cull=(isp>>27)&3;if((cull==1 && fabs(area)<cfg->cull_small_area)||(cull==2 && area<0)||(cull==3 && area>0))continue;
            for(unsigned i=0;i<3;i++){
                const struct jp_packet_vertex *color=(pcw&2)?v[i]:v[cfg->flat_color_vertex];struct gpu_vertex *out=&vertices[emitted++];
                out->x=v[i]->x;out->y=v[i]->y;out->q=v[i]->inv_w>0?v[i]->inv_w*scale:scale;out->depth=v[i]->inv_w*scale;out->u=v[i]->u;out->v=v[i]->v;
                for(unsigned ch=0;ch<4;ch++){
                    out->base[ch]=clamp(color->base[ch])*(ch<3?clamp(color->base_intensity):1);
                    out->offset[ch]=clamp(color->offset[ch])*(ch<3?clamp(color->offset_intensity):1);
                }
            }
        }
        batches[bi].count=(UINT)(emitted-batches[bi].first);if(!batches[bi].count)continue;
        if(cfg->textures&&(pcw&8)){hr=texture(g,frame,b,cfg,&batches[bi],&cost,&result,&code);if(FAILED(hr))goto done;}
        code=JP_GPU_API;D3D11_DEPTH_STENCIL_DESC depth={0};depth.DepthEnable=TRUE;depth.DepthWriteMask=(isp&0x04000000)?D3D11_DEPTH_WRITE_MASK_ZERO:D3D11_DEPTH_WRITE_MASK_ALL;
        depth.DepthFunc=(D3D11_COMPARISON_FUNC)(((isp>>29)&7)+1);hr=ID3D11Device_CreateDepthStencilState(g->device,&depth,&batches[bi].depth);if(FAILED(hr))goto done;
        D3D11_BLEND_DESC blend={0};D3D11_RENDER_TARGET_BLEND_DESC *rt=&blend.RenderTarget[0];rt->BlendEnable=TRUE;rt->RenderTargetWriteMask=D3D11_COLOR_WRITE_ENABLE_ALL;
        unsigned sf=(tsp>>29)&7,df=(tsp>>26)&7;rt->SrcBlend=factor(sf,1,0);rt->DestBlend=factor(df,0,0);rt->BlendOp=D3D11_BLEND_OP_ADD;
        rt->SrcBlendAlpha=factor(sf,1,1);rt->DestBlendAlpha=factor(df,0,1);rt->BlendOpAlpha=D3D11_BLEND_OP_ADD;
        hr=ID3D11Device_CreateBlendState(g->device,&blend,&batches[bi].blend);if(FAILED(hr))goto done;
    }
    code=JP_GPU_API;D3D11_TEXTURE2D_DESC desc={0};desc.Width=cfg->width;desc.Height=cfg->height;desc.MipLevels=desc.ArraySize=1;desc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count=1;desc.Usage=D3D11_USAGE_DEFAULT;desc.BindFlags=D3D11_BIND_RENDER_TARGET;
    hr=ID3D11Device_CreateTexture2D(g->device,&desc,NULL,&target);if(FAILED(hr))goto done;
    hr=ID3D11Device_CreateRenderTargetView(g->device,(ID3D11Resource *)target,NULL,&rtv);if(FAILED(hr))goto done;
    desc.Format=DXGI_FORMAT_D32_FLOAT;desc.BindFlags=D3D11_BIND_DEPTH_STENCIL;
    hr=ID3D11Device_CreateTexture2D(g->device,&desc,NULL,&zbuffer);if(FAILED(hr))goto done;
    hr=ID3D11Device_CreateDepthStencilView(g->device,(ID3D11Resource *)zbuffer,NULL,&dsv);if(FAILED(hr))goto done;
    desc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;desc.BindFlags=0;desc.Usage=D3D11_USAGE_STAGING;desc.CPUAccessFlags=D3D11_CPU_ACCESS_READ;
    hr=ID3D11Device_CreateTexture2D(g->device,&desc,NULL,&staging);if(FAILED(hr))goto done;
    D3D11_BUFFER_DESC bd={0};bd.ByteWidth=sizeof(struct constants);bd.Usage=D3D11_USAGE_DEFAULT;bd.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
    hr=ID3D11Device_CreateBuffer(g->device,&bd,NULL,&cb);if(FAILED(hr))goto done;
    if(emitted){bd.ByteWidth=(UINT)(emitted*sizeof *vertices);bd.Usage=D3D11_USAGE_IMMUTABLE;bd.BindFlags=D3D11_BIND_VERTEX_BUFFER;D3D11_SUBRESOURCE_DATA data={vertices,0,0};
        hr=ID3D11Device_CreateBuffer(g->device,&bd,&data,&vb);if(FAILED(hr))goto done;}
    D3D11_RASTERIZER_DESC rd={0};rd.FillMode=D3D11_FILL_SOLID;rd.CullMode=D3D11_CULL_NONE;rd.DepthClipEnable=TRUE;
    hr=ID3D11Device_CreateRasterizerState(g->device,&rd,&raster);if(FAILED(hr))goto done;
    ID3D11DeviceContext_OMSetRenderTargets(g->context,1,&rtv,dsv);ID3D11DeviceContext_ClearRenderTargetView(g->context,rtv,cfg->clear_rgba);
    ID3D11DeviceContext_ClearDepthStencilView(g->context,dsv,D3D11_CLEAR_DEPTH,0,0);
    D3D11_VIEWPORT viewport={0,0,(FLOAT)cfg->width,(FLOAT)cfg->height,0,1};ID3D11DeviceContext_RSSetViewports(g->context,1,&viewport);ID3D11DeviceContext_RSSetState(g->context,raster);
    ID3D11DeviceContext_IASetInputLayout(g->context,g->layout);ID3D11DeviceContext_IASetPrimitiveTopology(g->context,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    UINT stride=sizeof *vertices,offset=0;ID3D11DeviceContext_IASetVertexBuffers(g->context,0,1,&vb,&stride,&offset);
    ID3D11DeviceContext_VSSetShader(g->context,g->vs,NULL,0);ID3D11DeviceContext_PSSetShader(g->context,cfg->textures?g->ps:g->off_ps,NULL,0);
    ID3D11DeviceContext_VSSetConstantBuffers(g->context,0,1,&cb);ID3D11DeviceContext_PSSetConstantBuffers(g->context,0,1,&cb);
    /* Explicit presorted/submission order. Actual hardware autosort state is
     * not captured yet; no claim of intersecting translucent equivalence. */
    static const unsigned order[3]={0,2,1};
    for(unsigned lane=0;lane<3;lane++)for(bi=0;bi<plan->batch_count;bi++){
        const struct jp_packet_batch *b=&plan->batches[bi];struct draw_batch *draw=&batches[bi];if(b->list!=order[lane]||!draw->count)continue;
        uint32_t pcw=b->header[0],tsp=b->header[2];struct constants c={{cfg->logical_width,cfg->logical_height},cfg->textures&&!!(pcw&8),(tsp>>6)&3,!!(tsp&0x100000),!!(tsp&0x80000),!!(pcw&4),b->list==2,cfg->punch_alpha,{0}};
        ID3D11DeviceContext_UpdateSubresource(g->context,(ID3D11Resource *)cb,0,NULL,&c,0,0);
        ID3D11DeviceContext_PSSetShaderResources(g->context,0,1,&draw->texture);ID3D11DeviceContext_PSSetSamplers(g->context,0,1,&draw->sampler);
        ID3D11DeviceContext_OMSetDepthStencilState(g->context,draw->depth,0);ID3D11DeviceContext_OMSetBlendState(g->context,draw->blend,NULL,UINT_MAX);
        ID3D11DeviceContext_Draw(g->context,draw->count,draw->first);++result.draw_calls;
    }
    ID3D11DeviceContext_CopyResource(g->context,(ID3D11Resource *)staging,(ID3D11Resource *)target);D3D11_MAPPED_SUBRESOURCE mapped={0};
    hr=ID3D11DeviceContext_Map(g->context,(ID3D11Resource *)staging,0,D3D11_MAP_READ,0,&mapped);if(FAILED(hr))goto done;
    for(unsigned y=0;y<cfg->height;y++)memcpy((unsigned char *)rgba+(size_t)y*cfg->width*4,(const unsigned char *)mapped.pData+(size_t)y*mapped.RowPitch,(size_t)cfg->width*4);
    ID3D11DeviceContext_Unmap(g->context,(ID3D11Resource *)staging,0);
    result.batches=plan->batch_count;result.triangles=emitted/3;result.width=cfg->width;result.height=cfg->height;result.feature_level=g->level;
    result.textures_enabled=cfg->textures;result.accounted_payload_bytes=cost;*report=result;code=JP_GPU_OK;hr=S_OK;
    report->shader_texture_bindings=g->texture_bindings[cfg->textures];report->shader_sampler_bindings=g->sampler_bindings[cfg->textures];
done:
    ID3D11DeviceContext_ClearState(g->context);
    if(batches)for(size_t i=0;i<plan->batch_count;i++){
        if(batches[i].texture)ID3D11ShaderResourceView_Release(batches[i].texture);if(batches[i].sampler)ID3D11SamplerState_Release(batches[i].sampler);
        if(batches[i].depth)ID3D11DepthStencilState_Release(batches[i].depth);if(batches[i].blend)ID3D11BlendState_Release(batches[i].blend);
    }
    if(vb)ID3D11Buffer_Release(vb);if(cb)ID3D11Buffer_Release(cb);if(raster)ID3D11RasterizerState_Release(raster);
    if(rtv)ID3D11RenderTargetView_Release(rtv);if(dsv)ID3D11DepthStencilView_Release(dsv);
    if(target)ID3D11Texture2D_Release(target);if(staging)ID3D11Texture2D_Release(staging);if(zbuffer)ID3D11Texture2D_Release(zbuffer);
    free(vertices);free(batches);jp_packets_destroy(plan);return fail(failure,code,bi,hr);
}
