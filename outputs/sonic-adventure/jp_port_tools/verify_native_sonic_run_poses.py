"""Original full-speed action pose comparison using the qualified executor."""
import inspect
import verify_native_action_poses as pose_suite
def main():
 source=inspect.getsource(pose_suite.main);assert source.count("(0,b'sonic_updraft_action'")==1
 source=source.replace("(0,b'sonic_updraft_action'","(7,b'sonic_full_speed_action'").replace("b'sonic_updraft_action'","b'sonic_full_speed_action'")
 marker="    (d/'verification.json').write_text"
 assert source.count(marker)==1;source=source.replace(marker,"    out.update(action='sonic_full_speed_action',action_bank=7,original_pose_verifier_sha256=sha(pose_suite.__file__))\n"+marker)
 namespace=dict(vars(pose_suite),__file__=__file__,pose_suite=pose_suite);exec(compile(source,__file__,'exec'),namespace);namespace['main']()
if __name__=='__main__':main()
