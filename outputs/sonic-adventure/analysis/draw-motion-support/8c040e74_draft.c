/* Ghidra draft only; not buildable/verified source. */

void callback_8c040e74(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 *puVar6;
  undefined8 uVar7;
  undefined8 uVar9;
  char in_FPSCR_SZ;
  undefined4 auStack_1c [2];
  undefined4 uVar8;
  
  iVar5 = (char)((char)*DAT_8c041004 * '\x1c') + DAT_8c04100c;
  iVar4 = *(int *)(iVar5 + 0x14);
  puVar6 = (undefined4 *)((uint)*DAT_8c041004 * 0xc + DAT_8c041018);
  iVar1 = iVar4 * 4;
  iVar3 = *(int *)(*(int *)(iVar5 + 4) + iVar1);
  if (iVar3 == 0) {
    if (in_FPSCR_SZ == '\0') {
      uVar8 = *(undefined4 *)(param_1 + 0x20);
      uVar2 = *(undefined4 *)(param_1 + 0x24);
      puVar6[2] = *(undefined4 *)(param_1 + 0x28);
      puVar6[1] = uVar2;
      *puVar6 = uVar8;
    }
    else {
      uVar7 = *(undefined8 *)(param_1 + 0x20);
      uVar9 = *(undefined8 *)(param_1 + 0x28);
      *(undefined8 *)(puVar6 + 1) = *(undefined8 *)(param_1 + 0x30);
      *(undefined8 *)(puVar6 + -1) = uVar9;
      *(undefined8 *)(puVar6 + -3) = uVar7;
    }
  }
  else {
    uVar2 = *(undefined4 *)(*(int *)(iVar5 + 8) + iVar1);
    FUN_8c040a60(iVar3,uVar2,auStack_1c);
    FUN_8c040c5c(iVar3,auStack_1c[0],uVar2,puVar6);
    *DAT_8c04101c = *DAT_8c04101c | *DAT_8c041004 + 1;
  }
  *(int *)(iVar5 + 0x14) = iVar4 + 1;
  return;
}

