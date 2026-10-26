/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c663a60(int *param_1,int *param_2,int *param_3,int *param_4)

{
  uint uVar1;
  ushort *puVar2;
  int iVar3;
  ushort *puVar4;
  int iVar5;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  
  local_1c = 0;
  local_28 = 0;
  local_20 = 0;
  local_24 = 0;
  local_30 = 0;
  local_34 = 0;
  local_2c = 0;
  for (iVar3 = *(int *)(PTR_DAT_8c663b68 + 8); iVar3 != 0; iVar3 = *(int *)(iVar3 + 8)) {
    if (local_34 < *(int *)(iVar3 + 0x10)) {
      local_34 = *(int *)(iVar3 + 0x10);
    }
    local_24 = local_24 + *(int *)(iVar3 + 0x10);
  }
  for (puVar4 = *(ushort **)PTR_DAT_8c663b68; puVar4 != (ushort *)0x0;
      puVar4 = *(ushort **)(puVar4 + 4)) {
    if ((*puVar4 & 4) == 0) {
      if (local_34 < *(int *)(puVar4 + 8)) {
        local_34 = *(int *)(puVar4 + 8);
      }
      local_28 = local_28 + *(int *)(puVar4 + 8);
    }
  }
  for (iVar3 = *(int *)(PTR_DAT_8c663b68 + 0x1c); iVar3 != 0; iVar3 = *(int *)(iVar3 + 8)) {
    if (local_30 < *(int *)(iVar3 + 0x10)) {
      local_30 = *(int *)(iVar3 + 0x10);
    }
    local_20 = local_20 + *(int *)(iVar3 + 0x10);
  }
  for (puVar4 = *(ushort **)(PTR_DAT_8c663b68 + 0x14); puVar4 != (ushort *)0x0;
      puVar4 = *(ushort **)(puVar4 + 4)) {
    if ((*puVar4 & 4) == 0) {
      if (local_30 < *(int *)(puVar4 + 8)) {
        local_30 = *(int *)(puVar4 + 8);
      }
      local_1c = local_1c + *(int *)(puVar4 + 8);
    }
  }
  puVar4 = *(ushort **)PTR_DAT_8c663b6c;
  do {
    if (puVar4 == (ushort *)0x0) {
      *param_1 = local_24 + local_28;
      *param_2 = local_20 + local_1c;
      *param_3 = local_2c;
      if (local_34 < local_30) {
        local_34 = local_30;
      }
      *param_4 = local_34;
      return 0;
    }
    uVar1 = *puVar4 & DAT_8c663b70;
    if (uVar1 == (int)DAT_8c663b60) {
      iVar3 = (int)DAT_8c663bc6;
LAB_8c663bb2:
      iVar5 = 0x10;
    }
    else {
      if (uVar1 == (int)DAT_8c663b62) {
        iVar3 = (int)DAT_8c663bc8;
        goto LAB_8c663bb2;
      }
      if (uVar1 == (int)DAT_8c663b64) {
        iVar3 = (int)DAT_8c663bca;
        goto LAB_8c663bb2;
      }
      if (uVar1 == (int)DAT_8c663b66) {
        iVar3 = (int)DAT_8c663bcc;
        goto LAB_8c663bb2;
      }
      if (uVar1 == (int)DAT_8c663bc0) {
        iVar3 = (int)DAT_8c663c00;
        iVar5 = 8;
      }
      else if (uVar1 == (int)DAT_8c663bc2) {
        iVar3 = (int)DAT_8c663c02;
        iVar5 = 4;
      }
      else if (uVar1 == (int)DAT_8c663bc4) {
        iVar3 = (int)DAT_8c663c04;
        iVar5 = 2;
      }
      else if (uVar1 == DAT_8c663bd0) {
        iVar3 = (int)DAT_8c663c06;
        iVar5 = 1;
      }
      else {
        iVar5 = 0;
        iVar3 = 0;
      }
    }
    local_38 = 0;
    puVar2 = puVar4 + 6;
    if (iVar5 != 0) {
      do {
        if (*(int *)puVar2 == 0) {
          local_2c = local_2c + iVar3;
        }
        local_38 = local_38 + 1;
        puVar2 = puVar2 + 2;
      } while (local_38 < iVar5);
    }
    puVar4 = *(ushort **)(puVar4 + 2);
  } while( true );
}

