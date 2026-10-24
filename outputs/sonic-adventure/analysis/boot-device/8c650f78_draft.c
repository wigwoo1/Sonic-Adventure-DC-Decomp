/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c650f78(int param_1)

{
  uint uVar1;
  int *piVar2;
  int *piVar3;
  int *piVar4;
  int *piVar5;
  
  piVar4 = DAT_8c650fe4;
  piVar5 = DAT_8c650fe4;
  do {
    if ((int *)((int)DAT_8c650fe0 + (int)DAT_8c650fe4) <= piVar4) {
      return 1;
    }
    uVar1 = 0;
    piVar2 = piVar4;
    piVar3 = piVar5;
    do {
      if ((*piVar3 == param_1) && ((piVar2[6] & 3U) != 0)) {
        return 0;
      }
      uVar1 = uVar1 + 1;
      piVar2 = piVar2 + 8;
      piVar3 = piVar3 + 8;
    } while (uVar1 < 8);
    piVar4 = (int *)((int)piVar4 + (int)DAT_8c650fe2);
    piVar5 = (int *)((int)piVar5 + (int)DAT_8c650fe2);
  } while( true );
}

