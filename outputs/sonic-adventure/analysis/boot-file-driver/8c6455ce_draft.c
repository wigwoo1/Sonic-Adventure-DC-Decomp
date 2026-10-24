/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6455ce(undefined4 param_1,int *param_2,int param_3)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  
  piVar5 = param_2 + 4;
  iVar3 = *param_2;
  cVar1 = FUN_8c645442(param_1,&DAT_8c88e970);
  iVar4 = 0;
  if (0 < iVar3) {
    do {
      if (*(char *)((int)piVar5 + 9) == cVar1) {
        if (param_3 == 1) {
          iVar2 = thunk_FUN_8c6453cc(param_1,(int)piVar5 + 10);
        }
        else {
          iVar2 = FUN_8c6453e2(param_1,(int)piVar5 + 10);
        }
        if (iVar2 == 1) {
          return iVar4 + 1;
        }
      }
      iVar4 = iVar4 + 1;
      piVar5 = piVar5 + 0xb;
    } while (iVar4 < iVar3);
  }
  return 0;
}

