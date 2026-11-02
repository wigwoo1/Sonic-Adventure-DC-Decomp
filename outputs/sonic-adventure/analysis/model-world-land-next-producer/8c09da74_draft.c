/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c09da74(void)

{
  int *piVar1;
  int iVar2;
  undefined4 local_c [2];
  
  piVar1 = DAT_8c09db50;
  iVar2 = (*(code *)PTR_FUN_8c09db54)(0);
  *piVar1 = iVar2;
  if (iVar2 == 0) {
    return 0xffffffff;
  }
  iVar2 = (*(code *)PTR_FUN_8c09db58)(local_c);
  if (iVar2 == 0) {
    local_c[0] = 0xffffffff;
  }
  (*(code *)PTR_FUN_8c09db5c)(*piVar1);
  return local_c[0];
}

