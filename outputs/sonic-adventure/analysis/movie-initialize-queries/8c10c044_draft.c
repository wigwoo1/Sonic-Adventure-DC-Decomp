/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c10c044(void)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  
  (*(code *)PTR_FUN_8c10c0c4)(0);
  iVar2 = (*(code *)PTR_FUN_8c10c0c8)();
  cVar1 = *(char *)(iVar2 + 0xc);
  if (cVar1 == '1') {
    uVar3 = 1;
  }
  else if (cVar1 == '2') {
    uVar3 = 2;
  }
  else if (cVar1 == '3') {
    uVar3 = 3;
  }
  else {
    uVar3 = 0;
  }
  return uVar3;
}

