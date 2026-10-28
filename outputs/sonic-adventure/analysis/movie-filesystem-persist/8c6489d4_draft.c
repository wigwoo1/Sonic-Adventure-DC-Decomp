/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6489d4(int param_1,short param_2,int param_3,int param_4)

{
  int *piVar1;
  short sVar2;
  int iVar3;
  undefined4 uVar4;
  
  piVar1 = DAT_8c648abc;
  uVar4 = 0xfffc;
  iVar3 = param_1 * DAT_8c648ab0 + DAT_8c648ab8;
  while( true ) {
    if (param_4 < param_3) {
      return -1;
    }
    sVar2 = (**(code **)(*piVar1 + 0xc))(*(undefined4 *)(iVar3 + 0x24),param_4 << 1,uVar4);
    if (sVar2 == param_2) break;
    param_4 = param_4 + -1;
  }
  return param_4;
}

