/* Ghidra draft only; not buildable/verified source. */

undefined8 FUN_8c10d0f8(void)

{
  undefined8 uVar1;
  undefined4 in_PR;
  char in_FPSCR_SZ;
  undefined8 local_8;
  
  uVar1 = (*(code *)PTR_FUN_8c10d138)();
  local_8 = uVar1;
  if (in_FPSCR_SZ == '\0') {
    local_8 = CONCAT44(in_PR,(int)((ulonglong)uVar1 >> 0x20));
  }
  if (((uint)local_8 & DAT_8c10d13c) == DAT_8c10d13c) {
    if (((uint)local_8 & DAT_8c10d140) == 0) {
      *DAT_8c10d144 = (int)DAT_8c10d134;
    }
    else {
      *DAT_8c10d144 = (int)DAT_8c10d132;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    local_8 = CONCAT44((uint)local_8,(int)uVar1);
  }
  return local_8;
}

