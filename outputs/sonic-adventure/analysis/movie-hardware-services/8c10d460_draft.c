/* Ghidra draft only; not buildable/verified source. */

undefined4
FUN_8c10d460(undefined4 *param_1,undefined4 *param_2,int param_3,int param_4,int param_5,int param_6
            )

{
  undefined1 uVar1;
  undefined2 uVar2;
  undefined4 uVar3;
  int iVar4;
  undefined1 *puVar5;
  int unaff_r13;
  
  puVar5 = &DAT_8c88f644;
  if (param_5 == 0) {
    puVar5 = (undefined1 *)0x0;
  }
  else if (param_5 == 1) {
    puVar5 = (undefined1 *)0x1;
  }
  else if (param_5 == 2) {
    puVar5 = (undefined1 *)0xffffffff;
  }
  if (param_6 == 0) {
    unaff_r13 = 0;
  }
  else if (param_6 == 1) {
    unaff_r13 = 1;
  }
  else if (param_6 == 2) {
    unaff_r13 = -1;
  }
  (*(code *)PTR_FUN_8c10d53c)(&DAT_8c88f644);
  (*(code *)PTR_FUN_8c10d540)();
  if (param_3 == 1) {
    iVar4 = 0;
    if (0 < param_4) {
      do {
        uVar1 = *(undefined1 *)param_1;
        iVar4 = iVar4 + 1;
        param_1 = (undefined4 *)((int)param_1 + (int)puVar5);
        *(undefined1 *)param_2 = uVar1;
        param_2 = (undefined4 *)((int)param_2 + unaff_r13);
      } while (iVar4 < param_4);
    }
  }
  else if (param_3 == 2) {
    iVar4 = 0;
    if (0 < param_4) {
      do {
        uVar2 = *(undefined2 *)param_1;
        iVar4 = iVar4 + 1;
        param_1 = (undefined4 *)((int)param_1 + (int)puVar5 * 2);
        *(undefined2 *)param_2 = uVar2;
        param_2 = (undefined4 *)((int)param_2 + unaff_r13 * 2);
      } while (iVar4 < param_4);
    }
  }
  else if ((param_3 == 4) && (iVar4 = 0, 0 < param_4)) {
    do {
      uVar3 = *param_1;
      iVar4 = iVar4 + 1;
      param_1 = param_1 + (int)puVar5;
      *param_2 = uVar3;
      param_2 = param_2 + unaff_r13;
    } while (iVar4 < param_4);
  }
  (*(code *)PTR_FUN_8c10d5a0)();
  return 0;
}

