/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c603f8c(int param_1)

{
  ushort uVar1;
  undefined4 uVar2;
  ushort *puVar3;
  
  if (*(short *)(param_1 + 2) == 0) {
    puVar3 = (ushort *)((*(int *)(param_1 + 0x28) + 1) * 0x40 + *(int *)(param_1 + 0xc));
    thunk_FUN_8c603c2e(puVar3 + 1,*(undefined4 *)(param_1 + 0x30),0x3c);
    if ((*puVar3 == DAT_8c603ffc) ||
       (uVar1 = (*(code *)PTR_FUN_8c604000)(0x3e,puVar3), puVar3[0x1f] == uVar1)) {
      uVar2 = 0;
    }
    else {
      uVar2 = 0xfffffffc;
    }
  }
  else {
    uVar2 = 0xfffffffa;
  }
  return uVar2;
}

