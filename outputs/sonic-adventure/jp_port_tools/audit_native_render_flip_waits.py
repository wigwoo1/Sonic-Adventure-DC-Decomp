"""Record pinned game wait/statistics contracts without copying source bodies."""
import argparse,hashlib,json,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();assert not a.report.exists()
    head=subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'rev-parse','HEAD'],text=True).strip();assert head=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    paths=['src/system/frame.c','src/system/frame.h','gcc/src/system/present.c','sdk/ninja/kmsurfac_kamui_lib.c','sdk/ninja/interupt_kamui_lib.c','sdk/include/kamui.h','src/system/timer.c']
    sources={n:(u/n).read_text() for n in paths};frame=sources[paths[0]];present=sources[paths[2]]
    expectations={
        paths[0]:['while (frame_countdown > 0)','while (frame_countdown > left)','frame_countdown--;','njSetVSyncFunction(frame_vsync);','buExecServer();','loader_poll_drive();','sdSysServer();'],
        paths[2]:['while (frame_countdown == frame_interval)','~kmiIntStatisticData.render_start','kmiDeviceExtension.render_busy != 0','kmiIntStatisticData.ta_time','kmRender();','kmFlipFrameBuffer();'],
        paths[3]:['KMSTATUS kmRender(void)','KMSTATUS kmFlipFrameBuffer(void)','kmiIntStatisticData.list_max[i]','kmiDeviceExtension.list_limit[i]','kmiDeviceExtension.flip_pending != 0'],
        paths[4]:['kmiIntStatisticData.render_start = (unsigned int)TCNT0;','kmiIntStatisticData.ta_time = kmiIntStatisticData.ta_start - TCNT0;'],
    }
    locations={}
    for n,needles in expectations.items():
        locations[n]={}
        for needle in needles:
            hits=[i+1 for i,line in enumerate(sources[n].splitlines()) if needle in line];assert hits,(n,needle);locations[n][needle]=hits
    calls=[]
    for path in sorted((u/'src').rglob('*.c')):
        for line,text in enumerate(path.read_text().splitlines(),1):
            if 'frame_pacing_init(' in text and not text.startswith('void '):calls.append(dict(path=path.relative_to(u).as_posix(),line=line,call=text.strip()))
    assert len(calls)==7
    clock=r/'outputs/sonic-adventure/native/jp_native_clock.c'
    assert '#define WALL_RATE UINT64_C(781250)' in clock.read_text() and '#define TIMER1_RATE UINT64_C(12500000)' in clock.read_text()
    result=dict(status='native_render_flip_wait_contracts_traced_not_implemented',upstream_commit=head,
        external_source_sha256={n:sha(u/n) for n in paths},contract_source_lines=locations,frame_pacing_call_sites=calls,
        wait_loops_requiring_same_game_thread_pump=['frame_wait: countdown > 0','frame_wait_step: countdown > left','present_hold: countdown == interval'],
        verified_source_facts=[
            'Default initialization uses interval 2 / steps 2; events also use 2 / 1; demos use 1 / 1.',
            'frame_wait waits before kmRender/kmFlipFrameBuffer. Present-only callbacks cannot release this wait.',
            'The GCC hold path waits for a callback without issuing a flip. A render-driven callback alone would hang it.',
            'frame_vsync changes stage fog and runs sound, backup and loader services. It must be serialized with game state, not invoked on an unguarded timer thread.',
            'The GCC pvr_fits decision consumes render_busy and render_start/render_time/ta_time. Filling these with zeros would change split-frame policy.',
            'The native wall clock exposes 781250 counts per second separately from pausable timer 1. render_start requires complemented 32-bit wall counts.',
            'Kamui closes five lists in four slots, tracks list maxima and limits, and flip waits for progress with timeout behavior. Direct rendering alone supplies none of this.',
            'The upstream split presentation adapts 30 FPS steps to two pictures. It does not establish 120/240 FPS support.'
        ],
        next_required_work=[
            'Define explicit native vertical tick scheduling independent of window refresh and game step rate; establish source VSYNC_COUNTS rate.',
            'Adapt all three busy waits to a game-thread wait/message pump with bounded close/error propagation.',
            'Bind owned frame submission to kmRender and safe ready/present/retire transitions to kmFlipFrameBuffer.',
            'Measure actual CPU preparation and GPU completion; supply typed statistics and real busy/completion states, never synthetic zero costs.',
            'Verify the actual frame/present source with controlled clocks and real hardware rendering before claiming a game loop.'
        ],native_clock_source_sha256=sha(clock),verifier_sha256=sha(Path(__file__)),implemented=False,playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print('Three native waits and seven pacing initialization calls traced.')
if __name__=='__main__':main()
