/* Ghidra draft only; not buildable/verified source. */

int FUN_8c669088(uint param_1,int param_2,uint param_3,code *param_4,undefined4 param_5)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = 0;
  iVar3 = DAT_8c6690f4;
  if (((param_1 & 3) == 0) && ((param_3 & 3) == 0)) {
    if ((int)param_3 < 0) {
      param_3 = param_3 + 3;
    }
    uVar1 = (*(code *)PTR_FUN_8c6690fc)(param_2 + DAT_8c6690f8,4,(int)param_3 >> 2,1,1);
    *DAT_8c669100 = uVar1;
    iVar3 = iVar2;
    if ((param_4 != (code *)0x0) && (param_4 != (code *)0xffffffff)) {
      (*param_4)(param_5);
    }
  }
  if (iVar3 != 0) {
    (*(code *)PTR_FUN_8c669108)(PTR_FUN_8c669104,iVar3);
  }
  return iVar3;
}

