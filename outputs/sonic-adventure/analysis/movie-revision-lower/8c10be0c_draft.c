/* Ghidra draft only; not buildable/verified source. */

int FUN_8c10be0c(void)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  int local_20;
  int local_1c [2];
  
  bVar1 = false;
  do {
    iVar2 = FUN_8c10be78(local_1c);
    if (iVar2 == 0) {
      bVar1 = true;
    }
  } while (!bVar1);
  if (bVar1) {
    bVar1 = false;
    do {
      iVar2 = 0;
      do {
        iVar3 = FUN_8c10be78(&local_20);
        if ((iVar3 != 0) || (local_1c[0] != local_20)) break;
        iVar2 = iVar2 + 1;
        local_1c[0] = local_20;
      } while (iVar2 < 3);
      if (iVar2 == 3) {
        bVar1 = true;
      }
    } while (!bVar1);
  }
  else {
    local_20 = -1;
  }
  return local_20;
}

