/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c667240(void)

{
  undefined *puVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  
  puVar1 = PTR_DAT_8c667358;
  uVar4 = (uint)DAT_8c667354;
  uVar3 = 0;
  do {
    *(undefined1 *)(*(int *)puVar1 + uVar3 + 1) = 0;
    iVar2 = *(int *)puVar1 + uVar3;
    uVar3 = uVar3 + 8;
    *(undefined4 *)(iVar2 + 4) = 0;
  } while (uVar3 < uVar4);
  uVar3 = 0;
  do {
    uVar4 = uVar3 + 1;
    *(char *)(*(int *)puVar1 + uVar3 * 8 + 9) = (char)uVar3;
    *(undefined1 *)(uVar3 * 8 + *(int *)puVar1 + 8) = 1;
    uVar3 = uVar4;
  } while (uVar4 < 8);
  uVar3 = 0;
  do {
    uVar4 = uVar3 + 1;
    *(char *)(*(int *)puVar1 + uVar3 * 8 + 0x49) = (char)uVar3;
    *(undefined1 *)(uVar3 * 8 + *(int *)puVar1 + 0x48) = 2;
    uVar3 = uVar4;
  } while (uVar4 < 8);
  uVar3 = 0;
  do {
    iVar5 = uVar3 * 8;
    iVar2 = (int)DAT_8c667356;
    *(char *)((int)DAT_8c667356 + *(int *)puVar1 + iVar5 + 1) = (char)uVar3;
    uVar3 = uVar3 + 1;
    *(undefined1 *)(iVar5 + iVar2 + *(int *)puVar1) = 3;
  } while (uVar3 < 4);
  *(undefined1 *)(*(int *)puVar1 + 1) = 0;
  **(undefined1 **)puVar1 = 9;
  return 0;
}

