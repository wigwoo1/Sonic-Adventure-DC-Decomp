/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c652e80(uint param_1)

{
  if ((param_1 & 0xfffffffc) == 0) {
    *DAT_8c652f08 = param_1;
    (*(code *)PTR_FUN_8c652f0c)((int)DAT_8c652f00,param_1);
  }
  return 0;
}

