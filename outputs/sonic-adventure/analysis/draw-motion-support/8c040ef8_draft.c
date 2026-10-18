/* Ghidra draft only; not buildable/verified source. */

void callback_8c040ef8(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 *puVar5;
  int iVar6;
  undefined4 auStack_1c [2];
  
  iVar6 = (char)((char)*DAT_8c041004 * '\x1c') + DAT_8c04100c;
  iVar2 = *(int *)(iVar6 + 0x14);
  puVar5 = (undefined4 *)((uint)*DAT_8c041004 * 0xc + DAT_8c041020);
  iVar1 = iVar2 * 4;
  iVar4 = *(int *)(*(int *)(iVar6 + 4) + iVar1);
  if (iVar4 == 0) {
    *puVar5 = *(undefined4 *)(param_1 + 0x14);
    puVar5[1] = *(undefined4 *)(param_1 + 0x18);
    puVar5[2] = *(undefined4 *)(param_1 + 0x1c);
  }
  else {
    uVar3 = *(undefined4 *)(*(int *)(iVar6 + 8) + iVar1);
    FUN_8c040a60(iVar4,uVar3,auStack_1c);
    FUN_8c040d60(iVar4,auStack_1c[0],uVar3,puVar5);
    *DAT_8c041024 = *DAT_8c041024 | *DAT_8c041004 + 1;
  }
  *(int *)(iVar6 + 0x14) = iVar2 + 1;
  return;
}

