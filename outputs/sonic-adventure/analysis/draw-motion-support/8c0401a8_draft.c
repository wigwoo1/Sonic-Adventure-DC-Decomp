/* Ghidra draft only; not buildable/verified source. */

void callback_8c0401a8(int param_1)

{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  undefined4 uStack_24;
  undefined4 auStack_20 [4];
  
  iVar1 = DAT_8c040384;
  iVar6 = *(int *)(DAT_8c040384 + 0x14);
  iVar3 = iVar6 * 4;
  iVar5 = *(int *)(*(int *)(DAT_8c040384 + 4) + iVar3);
  puVar2 = (undefined4 *)(param_1 + 0x14);
  if (iVar5 != 0) {
    uVar4 = *(undefined4 *)(*(int *)(DAT_8c040384 + 8) + iVar3);
    FUN_8c03feb8(iVar5,uVar4,&uStack_24);
    FUN_8c03ff90(iVar5,uStack_24,uVar4,auStack_20);
    puVar2 = auStack_20;
  }
  (*(code *)PTR_FUN_8c040394)(0,*puVar2,puVar2[1],puVar2[2]);
  *(int *)(iVar1 + 0x14) = iVar6 + 1;
  return;
}

