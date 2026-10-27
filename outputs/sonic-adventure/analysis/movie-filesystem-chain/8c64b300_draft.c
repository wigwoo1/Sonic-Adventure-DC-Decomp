/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64b300(int param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  undefined1 auStack_40 [12];
  uint local_34;
  int local_18;
  
  iVar3 = param_1 * DAT_8c64b3cc + DAT_8c64b3d4;
  iVar1 = FUN_8c64ad56(auStack_40);
  if (iVar1 < 0) {
    iVar1 = (int)DAT_8c64b3ce;
  }
  else {
    FUN_8c64b274(param_1);
    if ((int)local_34 < *(int *)(*(int *)(iVar3 + 0x1c) + 0x30)) {
      uVar4 = local_34;
      for (iVar1 = 0; iVar1 < local_18; iVar1 = iVar1 + 1) {
        uVar2 = FUN_8c64b1c0(param_1,uVar4);
        uVar2 = uVar2 & 0xffff;
        if (iVar1 == local_18 + -1) {
          if (uVar2 != DAT_8c64b3d8) goto LAB_8c64b370;
        }
        else if ((((uVar2 == uVar4) || (uVar2 == DAT_8c64b3d8)) || (uVar2 == DAT_8c64b3dc)) ||
                ((uVar2 == DAT_8c64b3e0 || (*(int *)(*(int *)(iVar3 + 0x1c) + 0x30) <= (int)uVar2)))
                ) goto LAB_8c64b370;
        uVar4 = uVar2;
      }
      iVar1 = 0;
    }
    else {
LAB_8c64b370:
      iVar1 = (int)DAT_8c64b3d0;
    }
  }
  return iVar1;
}

