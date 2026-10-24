/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c665f42(uint param_1,uint param_2,int param_3)

{
  uint uVar1;
  uint *puVar2;
  undefined8 in_xd4;
  char in_FPSCR_SZ;
  
  uVar1 = DAT_8c665ff8;
  puVar2 = (uint *)(DAT_8c665ff0 | param_2);
  *puVar2 = (param_2 - param_3 >> 2) + 2 | DAT_8c665ff4;
  puVar2[1] = uVar1;
  uVar1 = DAT_8c666000;
  puVar2[2] = DAT_8c665ffc;
  puVar2[3] = uVar1;
  puVar2[4] = 0;
  if (in_FPSCR_SZ == '\0') {
    puVar2[5] = 0;
    puVar2[6] = 0;
    uVar1 = DAT_8c666008;
    param_1 = DAT_8c666004;
    puVar2[7] = DAT_8c666004;
  }
  else {
    *(ulonglong *)(puVar2 + 5) = (ulonglong)param_1;
    *(ulonglong *)(puVar2 + 6) = (ulonglong)param_1;
    uVar1 = DAT_8c666008;
    in_xd4 = CONCAT44(DAT_8c666008,DAT_8c666004);
    *(undefined8 *)(puVar2 + 7) = in_xd4;
  }
  puVar2[8] = uVar1;
  if (in_FPSCR_SZ == '\0') {
    puVar2[9] = 0;
    uVar1 = DAT_8c666010;
    puVar2[10] = DAT_8c66600c;
    puVar2[0xb] = param_1;
  }
  else {
    *(ulonglong *)(puVar2 + 9) = (ulonglong)param_1;
    uVar1 = DAT_8c666010;
    *(ulonglong *)(puVar2 + 10) = CONCAT44(DAT_8c666010,DAT_8c66600c);
    *(undefined8 *)(puVar2 + 0xb) = in_xd4;
  }
  puVar2[0xc] = uVar1;
  uVar1 = DAT_8c666018;
  if (in_FPSCR_SZ == '\0') {
    puVar2[0xd] = DAT_8c666014;
    puVar2[0xe] = 0;
    puVar2[0xf] = param_1;
  }
  else {
    *(ulonglong *)(puVar2 + 0xd) = CONCAT44(DAT_8c666018,DAT_8c666014);
    *(ulonglong *)(puVar2 + 0xe) = (ulonglong)param_1;
    *(undefined8 *)(puVar2 + 0xf) = in_xd4;
  }
  puVar2[0x10] = uVar1;
  return param_2;
}

