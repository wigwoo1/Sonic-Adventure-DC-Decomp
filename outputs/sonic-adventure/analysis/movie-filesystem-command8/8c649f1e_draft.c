/* Ghidra draft only; not buildable/verified source. */

int FUN_8c649f1e(int param_1,undefined4 param_2,undefined1 *param_3)

{
  undefined1 uVar1;
  undefined *puVar2;
  undefined1 *puVar3;
  int iVar4;
  int iVar5;
  undefined1 auStack_38 [48];
  
  puVar2 = PTR_DAT_8c649ff8;
  iVar4 = (int)DAT_8c649fe6;
  iVar5 = param_1 * iVar4 + DAT_8c649ff4;
  *(undefined4 *)(iVar5 + 0x30) = 1;
  *(undefined4 *)(iVar5 + 0x34) = *(undefined4 *)(puVar2 + *(int *)(iVar5 + 4) * 0x50);
  if (*(char *)(iVar5 + 2) != '\0') {
    iVar5 = (*(code *)PTR_FUN_8c649ffc)(param_1);
    if (iVar5 == 0) {
      (*(code *)PTR_FUN_8c64a000)(param_1);
    }
    else {
      iVar5 = (*(code *)PTR_FUN_8c64a004)(param_1);
      if ((iVar5 != 0) && (*(char *)(param_1 * iVar4 + DAT_8c649ff4 + 8) != '\0')) {
        iVar4 = (*(code *)PTR_thunk_FUN_8c64a4b2_8c64a008)(param_1);
        if (iVar4 < 0) {
          return (int)DAT_8c649fea;
        }
        iVar4 = (*(code *)PTR_FUN_8c64a00c)(param_1,param_2);
        if (iVar4 < 0) {
          return (int)DAT_8c649fec;
        }
        iVar4 = FUN_8c64ad56(param_1,param_2,auStack_38);
        if (iVar4 < 0) {
          return (int)DAT_8c649fee;
        }
        iVar5 = FUN_8c64ad56(param_1,param_3,auStack_38);
        if (-1 < iVar5) {
          return (int)DAT_8c649ff0;
        }
        iVar5 = (*(code *)PTR_FUN_8c64a010)(param_1,iVar4);
        for (puVar3 = (undefined1 *)(iVar5 + 4); puVar3 < (undefined1 *)(iVar5 + 0x10);
            puVar3 = puVar3 + 1) {
          uVar1 = *param_3;
          param_3 = param_3 + 1;
          *puVar3 = uVar1;
        }
        (*(code *)PTR_FUN_8c64a100)(param_1,iVar4);
        iVar4 = FUN_8c64ab36(param_1);
        return iVar4;
      }
    }
  }
  return (int)DAT_8c649fe8;
}

