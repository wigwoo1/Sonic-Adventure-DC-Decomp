/* Ghidra draft only; not buildable/verified source. */

void FUN_8c601bf0(int param_1,int param_2,int param_3)

{
  uint uVar1;
  ushort uVar2;
  short sVar3;
  char cVar4;
  int iVar5;
  uint uVar6;
  byte *pbVar7;
  byte *pbVar8;
  char *pcVar9;
  
  iVar5 = *(int *)(param_2 + 8);
  pbVar8 = (byte *)(iVar5 + 0xb);
  uVar6 = *(ushort *)(iVar5 + 8) | DAT_8c601c64;
  pcVar9 = (char *)(DAT_8c601c48 + param_3);
  if (*pbVar8 < *DAT_8c601c58) {
    if (*DAT_8c601d54 < *pbVar8) {
      if (*pcVar9 != '\0') {
        uVar6 = uVar6 & DAT_8c601d5c;
      }
    }
    else {
      uVar6 = uVar6 | DAT_8c601d58;
      *pcVar9 = '\0';
    }
  }
  else {
    uVar6 = uVar6 & DAT_8c601c68;
    *pcVar9 = '\x01';
  }
  pbVar7 = (byte *)(iVar5 + 10);
  if (*pbVar7 < *DAT_8c601d64) {
    if (*DAT_8c601d54 < *pbVar7) {
      if (*(char *)(DAT_8c601d60 + param_3) != '\0') {
        uVar6 = uVar6 & DAT_8c601d68;
      }
      goto LAB_8c601cc2;
    }
    cVar4 = '\0';
    uVar6 = uVar6 | DAT_8c601d6c;
  }
  else {
    uVar6 = uVar6 & DAT_8c601d68;
    cVar4 = '\x01';
  }
  *(char *)(DAT_8c601d60 + param_3) = cVar4;
LAB_8c601cc2:
  uVar1 = DAT_8c601d6c;
  if (*DAT_8c601d70 == 0) {
    uVar6 = uVar6 ^ 0xffffffff;
  }
  *(uint *)(param_1 + 8) = uVar6;
  if ((*(uint *)(param_1 + 4) & uVar1) == 0) {
    uVar2 = 0;
  }
  else {
    uVar2 = (ushort)*pbVar7;
  }
  *(ushort *)(param_1 + 0x18) = uVar2;
  if ((*(uint *)(param_1 + 4) & DAT_8c601d58) == 0) {
    uVar2 = 0;
  }
  else {
    uVar2 = (ushort)*pbVar8;
  }
  *(ushort *)(param_1 + 0x1a) = uVar2;
  if ((*(uint *)(param_1 + 4) & DAT_8c601d74) == 0) {
    sVar3 = 0;
  }
  else {
    sVar3 = *(byte *)(iVar5 + 0xc) - 0x80;
  }
  *(short *)(param_1 + 0x1c) = sVar3;
  uVar6 = DAT_8c601d7c;
  if ((*(uint *)(param_1 + 4) & DAT_8c601d78) == 0) {
    sVar3 = 0;
  }
  else {
    sVar3 = *(byte *)(iVar5 + 0xd) - 0x80;
  }
  *(short *)(param_1 + 0x1e) = sVar3;
  uVar1 = DAT_8c601d80;
  if ((*(uint *)(param_1 + 4) & uVar6) == 0) {
    sVar3 = 0;
  }
  else {
    sVar3 = *(byte *)(iVar5 + 0xe) - 0x80;
  }
  *(short *)(param_1 + 0x20) = sVar3;
  if ((*(uint *)(param_1 + 4) & uVar1) == 0) {
    sVar3 = 0;
  }
  else {
    sVar3 = *(byte *)(iVar5 + 0xf) - 0x80;
  }
  *(short *)(param_1 + 0x22) = sVar3;
  return;
}

