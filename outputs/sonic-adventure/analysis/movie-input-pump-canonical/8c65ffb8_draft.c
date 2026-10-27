/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65ffb8(int param_1,byte *param_2,uint *param_3)

{
  int *piVar1;
  byte bVar2;
  int iVar3;
  
  FUN_8c65ff5a(param_2,param_3,0);
  FUN_8c65ff12(param_2,param_3);
  iVar3 = (int)DAT_8c66004c;
  param_2[2] = (param_2[1] ^ *param_2) & *param_2;
  piVar1 = DAT_8c660054;
  param_2[3] = (param_2[1] ^ *param_2) & ~*param_2;
  if (((*(int *)(*piVar1 + iVar3) == 1) && (param_2[8] != DAT_8c66004e)) && (param_2[8] != 0)) {
    (*(code *)PTR_FUN_8c660058)(param_3,(*param_3 & 0xff) + 1);
  }
  if (*param_2 == 0) {
    bVar2 = 1;
  }
  else {
    bVar2 = 9;
  }
  param_2[4] = bVar2;
  if (param_2[2] != 0) {
    *(undefined1 *)(param_1 + 0x10) = 1;
    FUN_8c66018e(param_1,param_2,param_3);
  }
  if (param_2[3] != 0) {
    FUN_8c6602d2(param_1);
  }
  return 0;
}

