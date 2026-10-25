/* Ghidra draft only; not buildable/verified source. */

undefined4 * FUN_8c6021d2(int param_1)

{
  int *piVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 *puVar5;
  uint uVar6;
  undefined4 *puVar7;
  
  piVar1 = DAT_8c602284;
  puVar3 = (undefined4 *)*DAT_8c602284;
  uVar6 = (param_1 + 0x1fU >> 5) + 1;
  puVar2 = (undefined4 *)*puVar3;
  puVar7 = puVar3;
  while( true ) {
    puVar5 = puVar2;
    if (uVar6 <= (uint)puVar5[1]) {
      if (puVar5[1] == uVar6) {
        *puVar7 = *puVar5;
      }
      else {
        iVar4 = puVar5[1] - uVar6;
        puVar5[1] = iVar4;
        puVar5 = puVar5 + iVar4 * 8;
        puVar5[1] = uVar6;
      }
      *piVar1 = (int)puVar7;
      return puVar5 + 8;
    }
    if (puVar5 == puVar3) break;
    puVar2 = (undefined4 *)*puVar5;
    puVar7 = puVar5;
  }
  return (undefined4 *)0x0;
}

