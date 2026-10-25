/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c5fc626) */

void callback_8c5fc614(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int unaff_r12;
  uint uVar4;
  int unaff_r13;
  uint uVar5;
  uint unaff_r14;
  uint uStack00000004;
  
  uStack00000004 = *DAT_8c5fc644 >> 7 & 1;
  uVar4 = (unaff_r12 + unaff_r13) - 1U & DAT_8c5fc640;
  uVar5 = *DAT_8c5fc644 >> 5 & 1;
  uVar3 = DAT_8c5fc680 & unaff_r14;
  for (uVar1 = DAT_8c5fc668; uVar1 < DAT_8c5fc678; uVar1 = uVar1 + 0x20) {
    uVar2 = *(uint *)(uVar1 | DAT_8c5fc66c) & DAT_8c5fc670 | uVar1;
    if (((unaff_r14 & ~uVar3) <= uVar2) && (uVar2 <= uVar4)) {
      CacheBlockWriteBack(uVar2 | uVar3);
    }
    if ((uVar5 != 0) && (DAT_8c5fc67c == uVar1)) break;
  }
  if (uStack00000004 == 0) {
                    /* WARNING: Could not recover jumptable at 0x8c5fc6b8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)PTR_LAB_8c5fc6c8)(uVar3);
    return;
  }
  uVar3 = DAT_8c5fc708 & unaff_r14;
  for (uVar1 = DAT_8c5fc6f0; uVar1 < DAT_8c5fc700; uVar1 = uVar1 + 0x20) {
    uVar2 = *(uint *)(uVar1 | DAT_8c5fc6f4) & DAT_8c5fc6f8 | uVar1 | uVar1 & DAT_8c5fc6fc | uVar4;
    if (((unaff_r14 & ~uVar3) <= uVar2) && (uVar2 <= uVar4)) {
      CacheBlockWriteBack(uVar2 | uVar3);
    }
    if ((uVar5 != 0) && (DAT_8c5fc704 == uVar1)) break;
  }
                    /* WARNING: Could not recover jumptable at 0x8c5fc742. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_8c5fc750)(uVar3);
  return;
}

