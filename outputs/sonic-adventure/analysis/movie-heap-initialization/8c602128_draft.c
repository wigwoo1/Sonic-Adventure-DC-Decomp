/* Ghidra draft only; not buildable/verified source. */

void FUN_8c602128(int param_1,uint param_2)

{
  uint *puVar1;
  uint *puVar2;
  uint *puVar3;
  int iVar4;
  int *piVar5;
  uint uVar6;
  
  puVar3 = DAT_8c60227c;
  puVar2 = DAT_8c602278;
  puVar1 = DAT_8c602274;
  uVar6 = param_1 + 0x1fU & 0xffffffe0;
  *DAT_8c602274 = uVar6;
  *puVar2 = uVar6;
  piVar5 = DAT_8c602284;
  iVar4 = DAT_8c602280;
  *puVar3 = param_2 & 0xffffffe0;
  *piVar5 = iVar4;
  *(int *)iVar4 = iVar4;
  *(undefined4 *)(iVar4 + 4) = 0;
  uVar6 = *puVar1;
  *(uint *)(uVar6 + 4) = param_2 >> 5;
  FUN_8c602288(uVar6 + 0x20);
  return;
}

