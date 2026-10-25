/* Ghidra draft only; not buildable/verified source. */

int FUN_8c10bd96(int param_1)

{
  bool bVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  int *piVar5;
  int local_24 [3];
  
  piVar3 = local_24;
  piVar4 = piVar3;
  for (piVar5 = piVar3; piVar5 < local_24 + 2; piVar5 = piVar5 + 1) {
    iVar2 = FUN_8c10be78(piVar5);
    if (iVar2 != 0) {
      *piVar4 = param_1;
    }
    piVar4 = piVar4 + 1;
  }
  bVar1 = false;
  piVar4 = local_24 + 1;
  do {
    if (local_24 + 1 <= piVar3) {
LAB_8c10bdf0:
      if (!bVar1) {
        param_1 = local_24[0];
      }
      return param_1;
    }
    if (*piVar3 != *piVar4) {
      bVar1 = true;
      goto LAB_8c10bdf0;
    }
    piVar3 = piVar3 + 1;
    piVar4 = piVar4 + 1;
  } while( true );
}

