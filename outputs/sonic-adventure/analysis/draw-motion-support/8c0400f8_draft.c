/* Ghidra draft only; not buildable/verified source. */

void callback_8c0400f8(undefined4 param_1,int param_2)

{
  int iVar1;
  undefined8 *puVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  char in_FPSCR_SZ;
  undefined4 uStack_24;
  undefined8 auStack_20 [2];
  
  iVar1 = DAT_8c040384;
  iVar6 = *(int *)(DAT_8c040384 + 0x14);
  iVar3 = iVar6 * 4;
  iVar5 = *(int *)(*(int *)(DAT_8c040384 + 4) + iVar3);
  puVar2 = (undefined8 *)(param_2 + 0x20);
  if (iVar5 != 0) {
    uVar4 = *(undefined4 *)(*(int *)(DAT_8c040384 + 8) + iVar3);
    FUN_8c03feb8(iVar5,uVar4,&uStack_24);
    FUN_8c03ff2c(iVar5,uStack_24,uVar4,auStack_20);
    puVar2 = auStack_20;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar7 = CONCAT44(*(undefined4 *)puVar2,*(undefined4 *)((int)puVar2 + 4));
    uVar8 = CONCAT44(*(undefined4 *)(puVar2 + 1),param_1);
  }
  else {
    uVar7 = *puVar2;
    uVar8 = puVar2[2];
  }
  (*(code *)PTR_FUN_8c04038c)(uVar7,uVar8,0);
  *(int *)(iVar1 + 0x14) = iVar6 + 1;
  return;
}

