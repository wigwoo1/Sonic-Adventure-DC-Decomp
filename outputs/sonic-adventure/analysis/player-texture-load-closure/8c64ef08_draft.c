/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64ef08(int param_1,undefined4 *param_2)

{
  code *pcVar1;
  int iVar2;
  
  if (param_1 == 0) {
    iVar2 = (*(code *)PTR_FUN_8c64f114)(param_2 + 1,*param_2,0);
    pcVar1 = (code *)PTR_FUN_8c64f11c;
  }
  else {
    iVar2 = (*(code *)PTR_FUN_8c64f114)(param_2 + 1,param_1,0);
    pcVar1 = (code *)PTR_FUN_8c64f11c;
  }
  PTR_FUN_8c64f11c = pcVar1;
  if (iVar2 == 0) {
    do {
      iVar2 = (*pcVar1)();
    } while (iVar2 != 0);
    return 1;
  }
  *DAT_8c64f118 = 8;
  return 0xfffffff8;
}

