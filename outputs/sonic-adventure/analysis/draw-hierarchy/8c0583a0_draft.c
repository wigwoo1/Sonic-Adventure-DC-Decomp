/* Ghidra draft only; not buildable/verified source. */

undefined4
FUN_8c0583a0(undefined4 param_1,undefined4 param_2,int param_3,undefined4 *param_4,int param_5,
            int param_6)

{
  uint *puVar1;
  char in_FPSCR_SZ;
  
  puVar1 = DAT_8c058484;
  if (in_FPSCR_SZ == '\0') {
    param_2 = param_1;
  }
  *DAT_8c058480 = 0;
  *puVar1 = param_5 + 1U;
  if (0x80 < param_5 + 1U) {
    *puVar1 = 0x80;
  }
  (*(code *)PTR_FUN_8c058488)(0,param_2);
  if (param_3 != 0) {
    (*(code *)PTR_FUN_8c05848c)(0,param_3);
  }
  FUN_8c057bc0(*param_4,&stack0xffffffe4);
  if (param_6 != 0) {
    (*(code *)PTR_FUN_8c05848c)(param_6,param_5 * 0x40 + DAT_8c058490);
  }
  (*(code *)PTR_FUN_8c058494)(1);
  return 1;
}

