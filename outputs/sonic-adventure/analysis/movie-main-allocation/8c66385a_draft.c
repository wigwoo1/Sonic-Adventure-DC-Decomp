/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c66385a(int param_1)

{
  ushort *puVar1;
  undefined4 uVar2;
  ushort *puVar3;
  int iVar4;
  short local_14 [2];
  short local_10 [4];
  
  puVar1 = (ushort *)(*(code *)PTR_FUN_8c663908)(local_14);
  if (puVar1 == (ushort *)0x0) {
    uVar2 = 7;
  }
  else if ((*puVar1 & 2) == 0) {
    uVar2 = (*(code *)PTR_FUN_8c663910)((int)local_14[0],puVar1);
  }
  else {
    iVar4 = *(int *)PTR_DAT_8c66390c;
    if (local_14[0] != 0) {
      iVar4 = -iVar4;
    }
    puVar3 = (ushort *)(*(code *)PTR_FUN_8c663908)(iVar4 + param_1,local_10);
    if (puVar3 == (ushort *)0x0) {
      (*(code *)PTR_FUN_8c663910)((int)local_14[0],puVar1);
      uVar2 = 0;
    }
    else if ((*puVar3 & 4) == 0) {
      (*(code *)PTR_FUN_8c663910)((int)local_14[0],puVar1);
      uVar2 = (*(code *)PTR_FUN_8c663910)((int)local_10[0],puVar3);
    }
    else {
      *puVar1 = *puVar1 & (ushort)DAT_8c663914;
      uVar2 = 0;
    }
  }
  return uVar2;
}

