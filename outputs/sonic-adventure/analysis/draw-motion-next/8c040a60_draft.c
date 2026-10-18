/* Ghidra draft only; not buildable/verified source. */

void FUN_8c040a60(int param_1,uint param_2,uint *param_3)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  float in_fr3;
  undefined4 in_fr2;
  uint uVar4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  uVar3 = 0;
  if (in_FPSCR_SZ == '\0') {
    in_fr3 = *(float *)(DAT_8c040ce8 + (char)(*DAT_8c040ce4 * '\x1c'));
  }
  if (in_FPSCR_PR == '\0') {
    uVar4 = (uint)in_fr3;
  }
  else {
    uVar4 = (uint)(double)CONCAT44(in_fr2,in_fr3);
  }
  while (uVar1 = param_2, uVar1 - uVar3 != 1) {
    uVar2 = uVar1 + uVar3 >> 1;
    param_2 = uVar2;
    if (*(uint *)(uVar2 * 0x10 + param_1) <= uVar4) {
      param_2 = uVar1;
      uVar3 = uVar2;
    }
  }
  *param_3 = uVar3;
  return;
}

