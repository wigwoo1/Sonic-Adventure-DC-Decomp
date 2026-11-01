/* Ghidra draft only; not buildable/verified source. */

int FUN_8c04c5e0(char *param_1,byte *param_2)

{
  byte bVar1;
  int iVar2;
  byte *pbVar3;
  byte *pbVar4;
  byte *pbVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  
  uVar11 = (uint)DAT_8c04c5f0;
  uVar10 = (uint)DAT_8c04c5f2;
  uVar6 = (uint)*param_1;
  pbVar3 = (byte *)(param_1 + 1);
  iVar9 = 9;
  pbVar5 = param_2;
  do {
    while( true ) {
      iVar9 = iVar9 + -1;
      pbVar4 = pbVar3;
      uVar7 = uVar6;
      if (iVar9 == 0) {
        uVar7 = (uint)(char)*pbVar3;
        pbVar4 = pbVar3 + 1;
        iVar9 = 8;
      }
      uVar6 = uVar7 >> 1;
      if ((uVar7 & 1) == 0) break;
      pbVar3 = pbVar4 + 1;
      *pbVar5 = *pbVar4;
      pbVar5 = pbVar5 + 1;
    }
    iVar9 = iVar9 + -1;
    uVar7 = uVar7 >> 2;
    uVar8 = uVar6;
    if (iVar9 == 0) {
      uVar8 = (uint)(char)*pbVar4;
      pbVar4 = pbVar4 + 1;
      iVar9 = 8;
      uVar7 = uVar8 >> 1;
    }
    uVar6 = uVar7;
    if ((uVar8 & 1) == 0) {
      iVar9 = iVar9 + -1;
      if (iVar9 == 0) {
        uVar6 = (uint)(char)*pbVar4;
        pbVar4 = pbVar4 + 1;
        iVar9 = 8;
      }
      uVar7 = uVar6 & 1;
      uVar8 = uVar6 >> 1;
      iVar9 = iVar9 + -1;
      uVar6 = uVar6 >> 2;
      if (iVar9 == 0) {
        uVar8 = (uint)(char)*pbVar4;
        pbVar4 = pbVar4 + 1;
        iVar9 = 8;
        uVar6 = uVar8 >> 1;
      }
      pbVar3 = pbVar4 + 1;
      uVar7 = (uint)(uVar7 != 0) << 1 | (uint)((uVar8 & 1) != 0);
      uVar8 = (int)(char)*pbVar4 | uVar11;
LAB_8c04c638:
      iVar2 = uVar7 + 2;
      pbVar4 = pbVar5 + uVar8;
    }
    else {
      pbVar3 = pbVar4 + 2;
      if ((int)*(short *)pbVar4 == 0) {
        return (int)pbVar5 - (int)param_2;
      }
      uVar7 = (int)(char)*pbVar4 & 7;
      uVar8 = (uint)(int)*(short *)pbVar4 >> 3 | uVar10;
      if (uVar7 != 0) goto LAB_8c04c638;
      bVar1 = *pbVar3;
      pbVar3 = pbVar4 + 3;
      pbVar4 = pbVar5 + uVar8;
      iVar2 = bVar1 + 1;
    }
    do {
      bVar1 = *pbVar4;
      pbVar4 = pbVar4 + 1;
      iVar2 = iVar2 + -1;
      *pbVar5 = bVar1;
      pbVar5 = pbVar5 + 1;
    } while (iVar2 != 0);
  } while( true );
}

