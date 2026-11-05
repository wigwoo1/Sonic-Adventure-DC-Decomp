/* Authored D3D11 fog implementation from Sega System Architecture 3.4.8.
 * Native float interpolation/RGBA8 policy; console fixed-point precision,
 * dithering and cycle timing are not claimed. No emulator shader is copied.
 * The table is a constant buffer, never a sampled texture: Texture Off still
 * has zero texture/sampler bindings and retains vertex/material colors.
 */
#ifndef JP_NATIVE_GPU_FOG_SHADER_H
#define JP_NATIVE_GPU_FOG_SHADER_H
struct constants {
    float logical[2];uint32_t textured,shade,alpha,ignore_alpha,offset,punch;
    float threshold;uint32_t fog_mode;float fog_mantissa;int32_t fog_exponent;
    float fog_color[4],vertex_fog_color[4];uint32_t fog_table[128];
};
_Static_assert(sizeof(struct constants)==592,"D3D constant buffer packing");
static void jp_fog_bind_constants(struct constants *c,const struct jp_video_snapshot *s)
{
    if(!s)return;
    c->fog_mantissa=(float)((s->fog_density>>8)&255)/128.0f;
    c->fog_exponent=(int32_t)(s->fog_density&255);
    if(c->fog_exponent&128)c->fog_exponent-=256;
    for(unsigned i=0;i<4;i++){
        unsigned shift=i==3?24:16-i*8;
        c->fog_color[i]=(float)((s->fog_color>>shift)&255)/255.0f;
        c->vertex_fog_color[i]=(float)((s->vertex_fog_color>>shift)&255)/255.0f;
    }
    memcpy(c->fog_table,s->fog_table,sizeof c->fog_table);
}
static const char shader[]=
    "cbuffer Params:register(b0){float2 logicalSize;uint textured;uint shade;uint useAlpha;uint ignoreAlpha;uint useOffset;uint punch;float threshold;uint fogMode;float fogMantissa;int fogExponent;float4 fogColor;float4 vertexFogColor;uint4 fogTable[32];}\n"
    "struct V{float2 xy:POSITION;float q:TEXCOORD0;float depth:TEXCOORD1;float2 uv:TEXCOORD2;float4 base:COLOR0;float4 offset:COLOR1;float fogQ:TEXCOORD3;};\n"
    "struct O{float4 pos:SV_POSITION;float3 uvq:TEXCOORD0;float depth:TEXCOORD1;float4 base:COLOR0;float4 offset:COLOR1;float fogQ:TEXCOORD2;};\n"
    "O vertexMain(V v){O o;o.pos=float4(v.xy.x*2/logicalSize.x-1,1-v.xy.y*2/logicalSize.y,0.5,1);o.uvq=float3(v.uv*v.q,v.q);o.depth=v.depth;o.base=v.base*v.q;o.offset=v.offset*v.q;o.fogQ=v.fogQ;return o;}\n"
    "float tableFog(float q){uint word=fogTable[0][0];if(!(q>0)||!(fogMantissa>0))return float((word>>8)&255)/255;\n"
    /* Decompose before applying the signed density exponent, avoiding a
     * potentially overflowing/underflowing q*ldexp(mantissa,exponent).
     * Vertex reciprocal depths retain the qualified geometry domain.
     */
    "uint z=asuint(q);int e=int((z>>23)&255)-127;float m=asfloat((z&0x7fffff)|0x3f800000);uint s=asuint(m*fogMantissa);e+=fogExponent+int((s>>23)&255)-127;\n"
    "if(e<0)return float((word>>8)&255)/255;if(e>7)return float(fogTable[31][3]&255)/255;uint index=uint(e)*16+((s>>19)&15);word=fogTable[index>>2][index&3];float f=float(s&0x7ffff)/524288;return lerp(float((word>>8)&255),float(word&255),f)/255;}\n"
    "struct Pixel{float4 color:SV_TARGET;float depth:SV_Depth;};\n"
    "#ifndef TEXTURES_OFF\nTexture2D surface:register(t0);SamplerState filtering:register(s0);\n#endif\n"
    "Pixel pixelMain(O i){float4 b=saturate(i.base/i.uvq.z);float4 o=saturate(i.offset/i.uvq.z);if(!useAlpha)b.a=1;float4 c=b;\n"
    "#ifndef TEXTURES_OFF\nif(textured){float4 t=surface.Sample(filtering,i.uvq.xy/i.uvq.z);if(ignoreAlpha)t.a=1;if(shade==0)c=t;else if(shade==1)c=float4(b.rgb*t.rgb,t.a);else if(shade==2)c=float4(lerp(b.rgb,t.rgb,t.a),b.a);else c=b*t;}\n#endif\n"
    "if(useOffset)c.rgb+=o.rgb;if(fogMode==0)c.rgb=lerp(c.rgb,fogColor.rgb,tableFog(i.fogQ));else if(fogMode==1&&useOffset)c.rgb=lerp(c.rgb,vertexFogColor.rgb,o.a);else if(fogMode==3)c=float4(fogColor.rgb,tableFog(i.fogQ));\n"
    "if(punch && c.a<threshold)discard;Pixel p;p.color=saturate(c);p.depth=i.depth;return p;}\n";
#endif
