/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c093e90) */

void FUN_8c093e60(void)

{
  undefined4 uVar1;
  int iVar2;
  int local_10;
  int local_c [2];
  
  local_10 = 0;
  uVar1 = (*(code *)PTR_FUN_8c094044)(PTR_s_masaru_drv_8c094040,0);
  (*(code *)PTR_FUN_8c094048)(uVar1,local_c);
  (*(code *)PTR_FUN_8c094050)(uVar1,local_c[0] + 0x7ff >> 0xb,DAT_8c09404c);
  (*(code *)PTR_FUN_8c094054)(uVar1);
  iVar2 = (*(code *)PTR_FUN_8c094058)(0,0);
  if (iVar2 == 0) {
    (*(code *)PTR_FUN_8c09405c)(1);
    iVar2 = (*(code *)PTR_FUN_8c094060)(&local_10);
    if ((iVar2 == 0) &&
       (iVar2 = (*(code *)PTR_FUN_8c094064)(local_10,DAT_8c09404c,0,0xffffffff,0), iVar2 == 0)) {
      (*(code *)PTR_FUN_8c094068)(local_10);
    }
  }
  if (local_10 != 0) {
    (*(code *)PTR_FUN_8c09406c)(local_10);
  }
  (*(code *)PTR_FUN_8c094068)(local_10);
  (*(code *)PTR_FUN_8c094074)(PTR_FUN_8c094070);
  return;
}

