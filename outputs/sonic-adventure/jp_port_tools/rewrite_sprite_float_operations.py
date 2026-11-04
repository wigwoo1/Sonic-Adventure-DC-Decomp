"""Rewrite typed float expressions from a Clang AST, preserving source order.

Only named functions defined in the flattened external sprite source are
eligible. Integer math and exact UV_SCALE constant expressions stay unchanged.
The resulting LLVM is separately checked for residual native FP arithmetic.
"""
import collections

OPS={'+':'add','-':'subtract','*':'multiply','/':'divide','==':'eq','!=':'ne','<':'lt','<=':'le','>':'gt','>=':'ge'}

def floating(node):
    t=node.get('type',{});return t.get('desugaredQualType',t.get('qualType')) in ('float','const float')

def span(node):
    r=node.get('range',{});b,e=r.get('begin',{}),r.get('end',{})
    b=b.get('expansionLoc',b);e=e.get('expansionLoc',e)
    if 'offset' not in b or 'offset' not in e:return None
    return b['offset'],e['offset']+e['tokLen']

def rewrite(raw,tree,function_names):
    candidates={};counts=collections.Counter();constant_macros=0
    def visit(node,low,high):
        nonlocal constant_macros
        kind=node.get('kind');op=node.get('opcode');children=node.get('inner',[]);s=span(node);helper=None
        if kind in ('BinaryOperator','CompoundAssignOperator') and len(children)==2:
            if op in OPS and (floating(node) or op in ['==','!=','<','<=','>','>='] and any(floating(c) for c in children)):
                helper=OPS[op]
            elif kind=='CompoundAssignOperator' and op in ('+=','-=','*=','/=') and floating(node):helper=OPS[op[0]]+'_assign'
        elif kind=='UnaryOperator' and op=='-' and floating(node) and len(children)==1:helper='negate'
        if helper:
            begin=node.get('range',{}).get('begin',{});end=node.get('range',{}).get('end',{})
            pure_macro='expansionLoc' in begin and 'expansionLoc' in end and begin['expansionLoc']['offset']==end['expansionLoc']['offset']
            if pure_macro:
                # UV_SCALE is the only float-operator macro in pinned inputs.
                # Its exact 1/256 is representable under either rounding mode.
                token=begin.get('expansionLoc',{})
                off=token.get('offset',-1)
                assert raw[off:off+8]==b'UV_SCALE',('unsupported floating macro',node.get('range'))
                constant_macros+=1
            else:
                assert s is not None,('unresolved expression span',helper,node.get('range'))
                assert low<=s[0]<s[1]<=high,(helper,s,low,high)
                operands=[span(c) for c in children];assert all(v is not None and s[0]<=v[0]<v[1]<=s[1] for v in operands),(helper,operands)
                record=(helper,operands);assert s not in candidates or candidates[s]==record
                candidates[s]=record
        for c in children:visit(c,low,high)
    found=set()
    for node in tree['inner']:
        if node.get('kind')!='FunctionDecl' or node.get('name') not in function_names:continue
        bodies=[c for c in node.get('inner',[]) if c.get('kind')=='CompoundStmt']
        if not bodies:continue
        assert node['name'] not in found;found.add(node['name']);s=span(bodies[0]);assert s
        visit(bodies[0],*s)
    assert found==set(function_names),(found,set(function_names))
    def roots(low,high,exclude=None):
        choices=sorted((s for s in candidates if s!=exclude and low<=s[0] and s[1]<=high),key=lambda s:(s[0],-s[1]))
        out=[];end=low
        for s in choices:
            if s[0]<end:continue
            out.append(s);end=s[1]
        return out
    def emit(s):
        helper,operands=candidates[s];args=[render(*v,exclude=s) for v in operands]
        if helper.endswith('_assign'):args[0]=b'&('+args[0]+b')'
        counts[helper]+=1
        return b'jp_sprite_'+helper.encode()+b'('+b','.join(args)+b')'
    def render(low,high,exclude=None):
        result=[];at=low
        for s in roots(low,high,exclude):result.extend([raw[at:s[0]],emit(s)]);at=s[1]
        result.append(raw[at:high]);return b''.join(result)
    result=render(0,len(raw));assert sum(counts.values())==len(candidates)
    return result,dict(operators_rewritten=dict(counts),total_operators_rewritten=len(candidates),exact_constant_macro_nodes=constant_macros,functions=sorted(found))
