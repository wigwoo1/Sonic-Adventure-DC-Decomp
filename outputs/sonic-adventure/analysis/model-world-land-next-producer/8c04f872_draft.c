/* Ghidra draft only; not buildable/verified source. */

void FUN_8c04f872(short param_1)

{
  short *psVar1;
  short *psVar2;
  
  psVar2 = DAT_8c04f930;
  psVar1 = DAT_8c04f92c;
  (*(code *)PTR_FUN_8c04f934)((int)*DAT_8c04f92c << 8 | (int)*DAT_8c04f930);
  (*(code *)PTR_FUN_8c04f938)((int)*psVar1,(int)*psVar2);
  (*(code *)PTR_FUN_8c04f93c)();
  *psVar2 = *psVar2 + param_1;
  if (7 < *psVar2) {
    *psVar2 = 0;
  }
  if ((*psVar1 != 0x21) && (*psVar1 != 0x22)) {
    (*(code *)PTR_FUN_8c04f940)();
  }
  (*(code *)PTR_FUN_8c04f944)((int)*psVar1 << 8 | (int)*psVar2);
  (*(code *)PTR_FUN_8c04f948)((int)*psVar1,(int)*psVar2);
  (*(code *)PTR_FUN_8c04f94c)();
  *DAT_8c04f950 = 0;
  return;
}

