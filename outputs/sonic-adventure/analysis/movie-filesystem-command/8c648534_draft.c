/* Ghidra draft only; not buildable/verified source. */

int FUN_8c648534(int param_1,undefined4 param_2,int *param_3)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined1 auStack_38 [4];
  int local_34;
  undefined2 local_30;
  undefined1 uStack_2e;
  undefined1 auStack_18 [8];
  int local_10;
  
  iVar3 = (int)DAT_8c6485ae;
  iVar4 = param_1 * iVar3 + DAT_8c6485c8;
  if (*(char *)(iVar4 + 2) != '\0') {
    iVar2 = (*(code *)PTR_FUN_8c6485cc)(param_1);
    if (iVar2 == 0) {
      FUN_8c64850a(param_1);
    }
    else {
      iVar2 = (*(code *)PTR_FUN_8c6485d0)(param_1);
      if ((iVar2 != 0) && (*(char *)(param_1 * iVar3 + DAT_8c6485c8 + 8) != '\0')) {
        iVar3 = (*(code *)PTR_thunk_FUN_8c64a4b2_8c6485d4)(param_1);
        if (iVar3 < 0) {
          return (int)DAT_8c6485b2;
        }
        iVar3 = (*(code *)PTR_FUN_8c6485d8)(param_1,param_2,auStack_38);
        if (iVar3 < 0) {
          return (int)DAT_8c6485b4;
        }
        iVar3 = (int)DAT_8c6486fa;
        *param_3 = local_10 * (*(int *)(*(int *)(iVar4 + 0x1c) + 0x2c) + 1);
        *(short *)(param_3 + 1) = (short)local_10;
        *(char *)((int)param_3 + 6) = (char)local_34;
        *(undefined1 *)((int)param_3 + 7) = uStack_2e;
        *(undefined2 *)(param_3 + 2) = local_30;
        puVar1 = PTR_FUN_8c648704;
        *(bool *)((int)param_3 + 6) = local_34 == iVar3;
        (*(code *)puVar1)((int)param_3 + 10,auStack_18);
        return 0;
      }
    }
  }
  return (int)DAT_8c6485b0;
}

