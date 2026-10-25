/* Ghidra draft only; not buildable/verified source. */

void startup_8c0100d8(void)

{
  undefined1 uVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  
  puVar2 = PTR_DAT_8c010154;
  for (puVar4 = *(undefined1 **)PTR_DAT_8c010158; puVar3 = PTR_PTR_8c01015c,
      puVar4 < *(undefined1 **)puVar2; puVar4 = puVar4 + 1) {
    *puVar4 = 0;
  }
  puVar5 = *(undefined1 **)PTR_PTR_8c010164;
  for (puVar4 = *(undefined1 **)PTR_PTR_8c010160; puVar2 = PTR_DAT_8c010168,
      puVar4 < *(undefined1 **)puVar3; puVar4 = puVar4 + 1) {
    uVar1 = *puVar5;
    puVar5 = puVar5 + 1;
    *puVar4 = uVar1;
  }
  for (puVar4 = *(undefined1 **)PTR_DAT_8c01016c; puVar3 = PTR_DAT_8c010170,
      puVar4 < *(undefined1 **)puVar2; puVar4 = puVar4 + 1) {
    *puVar4 = 0;
  }
  puVar5 = *(undefined1 **)PTR_PTR_8c010178;
  for (puVar4 = *(undefined1 **)PTR_DAT_8c010174; puVar4 < *(undefined1 **)puVar3;
      puVar4 = puVar4 + 1) {
    uVar1 = *puVar5;
    puVar5 = puVar5 + 1;
    *puVar4 = uVar1;
  }
  return;
}

