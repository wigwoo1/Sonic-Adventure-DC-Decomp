/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c64f548(void)

{
  int iVar1;
  undefined8 *puVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  uint uVar5;
  undefined4 uVar6;
  ulonglong uVar7;
  undefined8 in_xd4;
  char in_FPSCR_SZ;
  
  puVar2 = DAT_8c64f5ac;
  *DAT_8c64f5a8 = 0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(puVar2 + 2) = 0x3f800000;
    uVar3 = DAT_8c64f5b0;
    uVar7 = CONCAT44(DAT_8c64f5b0,0x3f800000);
    *(undefined4 *)((int)puVar2 + 0x14) = DAT_8c64f5b0;
    *(undefined4 *)(puVar2 + 3) = uVar3;
    *(undefined4 *)((int)puVar2 + 0x1c) = uVar3;
    uVar6 = 0x3f800000;
    *(undefined4 *)(puVar2 + 4) = 0x3f800000;
    *(undefined4 *)((int)puVar2 + 0x24) = uVar3;
    *(undefined4 *)(puVar2 + 5) = uVar3;
    *(undefined4 *)((int)puVar2 + 0x2c) = uVar3;
  }
  else {
    puVar2[2] = in_xd4;
    uVar6 = DAT_8c64f5b0;
    uVar7 = CONCAT44(_DAT_8c64f5b4,DAT_8c64f5b0);
    *(ulonglong *)((int)puVar2 + 0x14) = uVar7;
    puVar2[3] = uVar7;
    *(ulonglong *)((int)puVar2 + 0x1c) = uVar7;
    puVar2[4] = in_xd4;
    *(ulonglong *)((int)puVar2 + 0x24) = uVar7;
    puVar2[5] = uVar7;
    *(ulonglong *)((int)puVar2 + 0x2c) = uVar7;
  }
  uVar7 = uVar7 & 0xffffffff;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)puVar2 = uVar6;
    *(undefined4 *)((int)puVar2 + 4) = 0;
    *(undefined4 *)(puVar2 + 1) = 0;
    *(undefined4 *)((int)puVar2 + 0xc) = 0;
    iVar1 = DAT_8c64f5b8;
    *(undefined4 *)(puVar2 + 6) = _DAT_8c64f5b4;
  }
  else {
    *puVar2 = in_xd4;
    *(ulonglong *)((int)puVar2 + 4) = uVar7;
    puVar2[1] = uVar7;
    *(ulonglong *)((int)puVar2 + 0xc) = uVar7;
    iVar1 = DAT_8c64f5b8;
    puVar2[6] = CONCAT44(DAT_8c64f5b8,_DAT_8c64f5b4);
  }
  uVar5 = 0;
  *(undefined4 *)(iVar1 + 0x80) = 0;
  do {
    puVar4 = (undefined4 *)(iVar1 + uVar5);
    uVar5 = uVar5 + 4;
    *puVar4 = 0;
  } while (uVar5 < 0x80);
  return;
}

