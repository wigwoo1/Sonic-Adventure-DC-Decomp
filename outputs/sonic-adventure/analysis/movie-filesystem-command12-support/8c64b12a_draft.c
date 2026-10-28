/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c64b12a(int param_1,uint param_2)

{
  int *piVar1;
  byte *pbVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  
  piVar1 = DAT_8c64b1ec;
  iVar5 = 0;
  uVar6 = 0xfffa;
  iVar4 = param_1 * DAT_8c64b1e0 + DAT_8c64b1e8;
  while( true ) {
    if (*(int *)(*(int *)(iVar4 + 0x1c) + 0x30) <= iVar5) {
      return param_2 & 0xffff;
    }
    pbVar2 = (byte *)FUN_8c64b274(param_1,iVar5,uVar6);
    if (((*pbVar2 == 0x33) || (*pbVar2 == DAT_8c64b1e2)) &&
       (uVar3 = (**(code **)(*piVar1 + 0xc))(2), (param_2 & 0xffff) == (uVar3 & 0xffff))) break;
    iVar5 = iVar5 + 1;
  }
  return 0xffffffff;
}

