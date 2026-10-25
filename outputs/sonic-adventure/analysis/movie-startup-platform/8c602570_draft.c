/* Ghidra draft only; not buildable/verified source. */

void FUN_8c602570(void)

{
  undefined2 *puVar1;
  int *piVar2;
  
  puVar1 = DAT_8c6025d8;
  *DAT_8c6025d8 = 0;
  *DAT_8c6025dc = 0;
  *DAT_8c6025e0 = 0;
  puVar1[6] = 0;
  for (piVar2 = (int *)PTR_DAT_8c6025e4; *piVar2 != 0; piVar2 = piVar2 + 2) {
    *(int *)*piVar2 = piVar2[1];
  }
  return;
}

