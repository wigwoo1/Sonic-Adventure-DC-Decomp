/* Ghidra draft only; not buildable/verified source. */

void callback_8c0414f0(undefined4 param_1,int param_2)

{
  undefined4 *puVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 in_xd4;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    uVar4 = CONCAT44(*(undefined4 *)(param_2 + 0x20),param_1);
    puVar2 = (undefined8 *)(param_2 + 0x24);
  }
  else {
    uVar4 = *(undefined8 *)(param_2 + 0x20);
    in_xd4 = *(undefined8 *)(param_2 + 0x28);
    puVar2 = (undefined8 *)(param_2 + 0x30);
  }
  uVar3 = *puVar2;
  puVar1 = (undefined4 *)((uint)*DAT_8c0415c4 * 0xc + DAT_8c0415e4);
  if (in_FPSCR_SZ == '\0') {
    puVar1[2] = (int)((ulonglong)uVar3 >> 0x20);
    puVar1[1] = (int)uVar3;
    *puVar1 = (int)((ulonglong)uVar4 >> 0x20);
  }
  else {
    *(undefined8 *)(puVar1 + 1) = uVar3;
    *(undefined8 *)(puVar1 + -1) = in_xd4;
    *(undefined8 *)(puVar1 + -3) = uVar4;
  }
  return;
}

