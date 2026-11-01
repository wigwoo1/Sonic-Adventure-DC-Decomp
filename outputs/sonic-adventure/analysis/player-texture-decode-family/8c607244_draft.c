/* Ghidra draft only; not buildable/verified source. */

int FUN_8c607244(int *param_1,uint param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = 0;
  do {
    iVar2 = iVar1;
    iVar1 = iVar2 + 1;
  } while ((*(uint *)(iVar2 * 0xc + *param_1 + 4) & 0xffff) != param_2);
  return iVar2;
}

