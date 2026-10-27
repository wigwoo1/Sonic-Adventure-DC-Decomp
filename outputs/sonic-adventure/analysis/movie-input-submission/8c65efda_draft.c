/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65efda(void)

{
  byte bVar1;
  int *piVar2;
  undefined *puVar3;
  undefined4 uVar4;
  int iVar5;
  int extraout_r2;
  int iVar6;
  char *pcVar7;
  
  piVar2 = DAT_8c65f024;
  if (*(int *)(*DAT_8c65f024 + (int)DAT_8c65f00c) == 1) {
    if ((*(uint *)(*(int *)((int)DAT_8c65f00e + *DAT_8c65f024) + 0x18) & 1) == 0) {
      bVar1 = **(byte **)PTR_PTR_8c65f154;
      **(byte **)PTR_PTR_8c65f154 = bVar1 | 0x80;
      if (bVar1 == 0) {
        if (*(int *)(DAT_8c65f13a + 4 + *piVar2) != *(int *)((int)DAT_8c65f13a + *piVar2)) {
          iVar5 = (int)DAT_8c65f13a;
          *(undefined4 *)(iVar5 + 4 + *piVar2) = *(undefined4 *)(iVar5 + *piVar2);
          if (*(int *)(iVar5 + *piVar2) == 0) {
            *(undefined4 *)((int)DAT_8c65f13e + *piVar2) = DAT_8c65f164;
          }
          else {
            iVar5 = (*(code *)PTR_FUN_8c65f160)(&DAT_a05f6908);
            *(int *)(DAT_8c65f13e + extraout_r2) = iVar5 << 0x10;
          }
          iVar5 = (int)DAT_8c65f140;
          *(undefined4 *)(*(int *)(iVar5 + *piVar2) + 0x14) = 0;
          iVar6 = *piVar2;
          *(uint *)((int)DAT_8c65f142 + *(int *)(iVar5 + iVar6)) =
               *(uint *)(iVar5 + -0x34 + iVar6) | *(uint *)(iVar5 + -0x24 + iVar6);
          *(undefined4 *)(*(int *)((int)DAT_8c65f140 + *piVar2) + 0x14) = 1;
        }
        FUN_8c65eeda();
        puVar3 = PTR_FUN_8c65f168;
        iVar5 = *piVar2;
        *(undefined4 *)(DAT_8c65f148 + iVar5) =
             *(undefined4 *)(*(int *)(DAT_8c65f144 + iVar5) * 4 + DAT_8c65f146 + iVar5);
        iVar5 = *piVar2;
        iVar6 = *(int *)(DAT_8c65f140 + iVar5);
        uVar4 = (*(code *)puVar3)(*(undefined4 *)
                                   (*(int *)(DAT_8c65f144 + iVar5) * 4 + DAT_8c65f146 + iVar5));
        *(undefined4 *)(iVar6 + 4) = uVar4;
        *(undefined4 *)(*(int *)((int)DAT_8c65f140 + *piVar2) + 0x18) = 1;
        iVar5 = (int)DAT_8c65f144;
        *(undefined4 *)((int)DAT_8c65f14a + *piVar2) = 0;
        *(uint *)((int)DAT_8c65f144 + *piVar2) = *(uint *)((int)DAT_8c65f144 + *piVar2) ^ 1;
        iVar6 = *piVar2;
        *(undefined4 *)(DAT_8c65f14c + iVar6) =
             *(undefined4 *)(*(int *)(iVar5 + iVar6) * 4 + DAT_8c65f146 + iVar6);
        *(uint *)((int)DAT_8c65f14e + *piVar2) = *(uint *)((int)DAT_8c65f14e + *piVar2) ^ 1;
        *(undefined4 *)((int)DAT_8c65f150 + *piVar2) = 0;
        iVar5 = *piVar2 + 0x28;
        iVar6 = 0;
        while (puVar3 = PTR_PTR_8c65f28c, iVar6 < 0x18) {
          pcVar7 = (char *)((int)DAT_8c65f284 + *piVar2 + iVar6);
          iVar6 = iVar6 + 1;
          if (*pcVar7 != '\0') {
            *(undefined1 *)(iVar5 + 7) = *(undefined1 *)(iVar5 + 6);
            *(undefined1 *)(iVar5 + 6) = 0;
            *(undefined4 *)(iVar5 + 0x1c) = *(undefined4 *)(iVar5 + 0x18);
            *(undefined4 *)(iVar5 + 0x18) = 0;
          }
          iVar5 = iVar5 + 0x2c;
        }
        *(undefined4 *)((int)DAT_8c65f286 + *piVar2) = 0;
        uVar4 = 0;
        **(undefined1 **)puVar3 = 0;
      }
      else {
        FUN_8c65f348(PTR_s____FATAL_ERROR_Cannot_get_semaph_8c65f158);
        uVar4 = 0xfffffffe;
      }
    }
    else {
      uVar4 = 0xffffffff;
    }
  }
  else {
    uVar4 = 0xfffffffd;
  }
  return uVar4;
}

