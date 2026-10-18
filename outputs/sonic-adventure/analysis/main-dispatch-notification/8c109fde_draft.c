/* Ghidra draft only; not buildable/verified source. */

void FUN_8c109fde(int param_1,int param_2,int param_3,undefined4 param_4)

{
  undefined *puVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  puVar1 = PTR_DAT_8c10a08c;
  if (*(int *)PTR_DAT_8c10a084 == 0) {
    iVar6 = *(int *)(DAT_8c10a088 + param_1 * 4);
    iVar5 = -1;
    if (iVar6 != -1) {
      iVar6 = iVar6 * 2;
      iVar4 = 0;
      do {
        iVar2 = (*(code *)PTR_thunk_FUN_8c64bed8_8c10a090)
                          (*(undefined4 *)(puVar1 + iVar6 * 4),param_4);
        if (iVar2 != 0) {
          iVar5 = *(int *)(puVar1 + iVar6 * 4);
          break;
        }
        iVar4 = iVar4 + 1;
        iVar6 = iVar6 + 1;
      } while (iVar4 < 2);
    }
    if (iVar5 != -1) {
      if (param_2 < 5) {
        if (param_2 < -4) {
          param_2 = -4;
        }
      }
      else {
        param_2 = 4;
      }
      if (param_2 == 1) {
        param_2 = 2;
      }
      else if (param_2 == -1) {
        param_2 = -2;
      }
      if (param_3 < 0x3c) {
        if (param_3 < 7) {
          param_3 = 7;
        }
      }
      else {
        param_3 = 0x3b;
      }
      (*(code *)PTR_FUN_8c10a100)(0x80);
      uVar3 = (*(code *)PTR_FUN_8c10a100)();
      FUN_8c109e50(iVar5,param_2,param_3,uVar3);
    }
  }
  return;
}

