/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c663c86(int *param_1,int *param_2)

{
  int iVar1;
  ushort *puVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = 0;
  iVar3 = 0;
  for (iVar1 = *(int *)(PTR_DAT_8c663da0 + 8); iVar1 != 0; iVar1 = *(int *)(iVar1 + 8)) {
    if (iVar3 < *(int *)(iVar1 + 0x10)) {
      iVar3 = *(int *)(iVar1 + 0x10);
    }
  }
  for (puVar2 = *(ushort **)PTR_DAT_8c663da0; puVar2 != (ushort *)0x0;
      puVar2 = *(ushort **)(puVar2 + 4)) {
    if (((*puVar2 & 4) == 0) && (iVar3 < *(int *)(puVar2 + 8))) {
      iVar3 = *(int *)(puVar2 + 8);
    }
  }
  for (iVar1 = *(int *)(PTR_DAT_8c663da0 + 0x1c); iVar1 != 0; iVar1 = *(int *)(iVar1 + 8)) {
    if (iVar4 < *(int *)(iVar1 + 0x10)) {
      iVar4 = *(int *)(iVar1 + 0x10);
    }
  }
  for (puVar2 = *(ushort **)(PTR_DAT_8c663da0 + 0x14); puVar2 != (ushort *)0x0;
      puVar2 = *(ushort **)(puVar2 + 4)) {
    if (((*puVar2 & 4) == 0) && (iVar4 < *(int *)(puVar2 + 8))) {
      iVar4 = *(int *)(puVar2 + 8);
    }
  }
  *param_1 = iVar3;
  *param_2 = iVar4;
  return 0;
}

