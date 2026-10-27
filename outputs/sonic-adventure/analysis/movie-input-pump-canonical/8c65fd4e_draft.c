/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65fd4e(int param_1)

{
  int *piVar1;
  int iVar2;
  byte *pbVar3;
  undefined4 uVar4;
  byte *pbVar5;
  byte bVar6;
  byte bStack_3;
  byte abStack_2 [2];
  
  piVar1 = DAT_8c65fe0c;
  pbVar5 = (byte *)(*DAT_8c65fe0c + 4 + param_1 * 8);
  iVar2 = **(int **)(*DAT_8c65fe0c + (int)DAT_8c65fe04 +
                     (1 - *(int *)((int)DAT_8c65fe02 + *DAT_8c65fe0c)) * 0x60 +
                    *(int *)(PTR_DAT_8c65fe10 + param_1 * 4) * 4);
  if ((iVar2 == -1) || (iVar2 == DAT_8c65fe06)) {
    iVar2 = (int)DAT_8c65fe08;
    *(int *)(pbVar5 + 4) = *(int *)(pbVar5 + 4) + 1;
    if (*(uint *)(pbVar5 + 4) <= *(uint *)(iVar2 + *piVar1)) {
      return 0;
    }
    bVar6 = 0;
    uVar4 = 0xffffffff;
  }
  else {
    if (*(int *)(*DAT_8c65fe0c + (int)DAT_8c65fe0a) == 1) {
      pbVar3 = &bStack_3;
    }
    else {
      pbVar3 = abStack_2;
    }
    bVar6 = *pbVar3;
    uVar4 = 0;
    pbVar5[4] = 0;
    pbVar5[5] = 0;
    pbVar5[6] = 0;
    pbVar5[7] = 0;
  }
  pbVar5[1] = *pbVar5;
  *pbVar5 = bVar6;
  pbVar5[2] = (pbVar5[1] ^ *pbVar5) & *pbVar5;
  pbVar5[3] = (pbVar5[1] ^ *pbVar5) & ~*pbVar5;
  return uVar4;
}

