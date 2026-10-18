/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_8c60b3e2(int param_1)

{
  ushort *puVar1;
  int iVar2;
  int iVar3;
  undefined4 in_fr3;
  undefined8 in_xd2;
  char in_FPSCR_SZ;
  
  iVar2 = _DAT_8c60b500;
  puVar1 = DAT_8c60b4fc;
  if (in_FPSCR_SZ == '\0') {
    in_fr3 = *(undefined4 *)(param_1 + 8);
  }
  else {
    in_xd2 = *(undefined8 *)(param_1 + 8);
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((uint)*DAT_8c60b4fc * 0xc + _DAT_8c60b500) = in_fr3;
  }
  else {
    *(undefined8 *)((uint)*DAT_8c60b4fc * 0xc + _DAT_8c60b500) = in_xd2;
  }
  if (in_FPSCR_SZ == '\0') {
    in_fr3 = *(undefined4 *)(param_1 + 0xc);
  }
  else {
    in_xd2 = *(undefined8 *)(param_1 + 0xc);
  }
  iVar3 = ((uint)*puVar1 * 3 + 1) * 4;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(iVar3 + iVar2) = in_fr3;
  }
  else {
    *(undefined8 *)(iVar3 + iVar2) = in_xd2;
  }
  iVar3 = ((uint)*puVar1 * 3 + 2) * 4;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(iVar3 + iVar2) = *(undefined4 *)(param_1 + 0x10);
  }
  else {
    *(undefined8 *)(iVar3 + iVar2) = *(undefined8 *)(param_1 + 0x10);
  }
  return;
}

