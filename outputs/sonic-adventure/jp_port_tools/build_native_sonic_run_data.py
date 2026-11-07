"""Append the genuine player-only full-speed action without replacing toe data."""
import hashlib,inspect
from pathlib import Path
import build_native_toe_models as toe_builder
def main():
 source=inspect.getsource(toe_builder.main)
 old="Path(n).name=='models_native_data.obj'";assert source.count(old)==1;source=source.replace(old,"Path(n).name=='models_with_toes.obj'")
 assert source.count('2793')==2;source=source.replace('2793','2849')
 old="pending=['sonic_toe_'+side+'_z_'+shape for side in ('plus','minus') for shape in ('blend','shape_a','shape_b')]";assert source.count(old)==1;source=source.replace(old,"pending=['sonic_full_speed_action']")
 old="assert all('sonic_toe_'+side+'_z_'+shape in added for side in ('plus','minus') for shape in ('blend','shape_a','shape_b'))";assert source.count(old)==1;source=source.replace(old,"assert 'sonic_full_speed_action' in added")
 source=source.replace('toe_models_','sonic_run_data_').replace('canonical_model_registry_extended_with_genuine_toe_closure_unverified','canonical_model_registry_extended_with_original_full_speed_action_unverified')
 source=source.replace("d.mkdir()","d.mkdir();(d/'private_run_data_builder.py').write_text(derived_source)")
 marker="updates.update(status="
 assert source.count(marker)==1;source=source.replace(marker,"updates['sonic_run_data_original_builder_sha256']=sha(toe_builder.__file__);"+marker)
 namespace=dict(vars(toe_builder),__file__=__file__,toe_builder=toe_builder,derived_source=source);exec(compile(source,__file__,'exec'),namespace);namespace['main']()
if __name__=='__main__':main()
