/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c667188(uint param_1,undefined1 *param_2)

{
  undefined1 auStack_8 [8];
  
  if ((int)param_1 < DAT_8c6671bc) {
    (*(code *)PTR_FUN_8c6671c0)(param_1 & 0xfffffffc,auStack_8);
    *param_2 = auStack_8[param_1 & 3];
  }
  return 0;
}

