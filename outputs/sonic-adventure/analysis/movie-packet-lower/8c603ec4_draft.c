/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c603ec4(int param_1)

{
  undefined *puVar1;
  undefined2 uVar2;
  undefined4 uVar3;
  int iVar4;
  undefined2 *puVar5;
  
  if (*(short *)(param_1 + 2) == 0) {
    puVar5 = (undefined2 *)(param_1 + 0x34);
    *puVar5 = (short)*(undefined4 *)(param_1 + 0x28);
    thunk_FUN_8c603c2e(*(undefined4 *)(param_1 + 0x30),param_1 + 0x36,0x3c);
    uVar2 = (*(code *)PTR_FUN_8c604000)(0x3e,puVar5);
    *(undefined2 *)(param_1 + 0x72) = uVar2;
    iVar4 = (*(int *)(param_1 + 0x28) + 1) * 0x40;
    FUN_8c603c08(puVar5,*(int *)(param_1 + 0xc) + iVar4,0x40,iVar4);
    if (*(int *)(param_1 + 0x10) < *(int *)(param_1 + 0x14)) {
      iVar4 = (*(int *)(param_1 + 0x14) + 1) * 0x40;
      FUN_8c603c84(*(int *)(param_1 + 0x10) + 1,*(int *)(param_1 + 0xc) + iVar4,
                   *(undefined4 *)(param_1 + 0x18));
      iVar4 = (*(code *)PTR_FUN_8c604004)
                        (*(int *)(param_1 + 4) + iVar4,*(int *)(param_1 + 0xc) + iVar4,
                         *(int *)(param_1 + 0x18) << 6);
      puVar1 = PTR_FUN_8c604004;
      if (iVar4 == *(int *)(param_1 + 0x18) << 6) {
        iVar4 = *(int *)(param_1 + 0x10);
        *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
        iVar4 = (*(code *)puVar1)(*(int *)(param_1 + 4) + (iVar4 + 1) * 0x40,puVar5,0x40);
        if (iVar4 == 0x40) {
          return 0;
        }
      }
      uVar3 = 0xfffffffe;
    }
    else {
      uVar3 = 0xfffffff6;
    }
  }
  else {
    uVar3 = 0xfffffffa;
  }
  return uVar3;
}

