/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c019996) */

void FUN_8c01995e(void)

{
  char cVar1;
  char cVar2;
  undefined *puVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  char *pcVar8;
  
  if (((*(int *)PTR_DAT_8c019a68 != 0) &&
      (FUN_8c019adc(&DAT_8c6b706c), puVar3 = PTR_DAT_8c019a44,
      *(char *)(*(int *)PTR_DAT_8c019a44 + 6) != '@')) &&
     (uVar5 = (*(code *)PTR_FUN_8c019a6c)(),
     *(int *)(PTR_DAT_8c019a70 + (uVar5 & 0xff) * 4 + ((int)(uVar5 & 0xffff) >> 8) * 0x20) != 0)) {
LAB_8c0199b2:
    piVar4 = DAT_8c019a7c;
    cVar1 = *(char *)(*(int *)puVar3 + 8);
    iVar7 = *(int *)*DAT_8c019a74 + -1;
    pcVar8 = (char *)((*(int *)*DAT_8c019a74 + -1) * 0x40 + *DAT_8c019a78);
    if (0 < (short)iVar7) {
      do {
        if (*(char *)(*(int *)puVar3 + 8) <= *(char *)(*pcVar8 * 0x10 + *piVar4 + 4)) {
          cVar2 = *(char *)(*(int *)puVar3 + 8);
          iVar6 = *piVar4;
          if (cVar2 == '\0') {
            if (*(char *)(*pcVar8 * 0x10 + iVar6 + 4) != '\x04') {
              FUN_8c019be6(iVar7,pcVar8);
            }
          }
          else if ((cVar2 == '\x01') || (cVar2 == '\x03')) {
            if (*(char *)(*pcVar8 * 0x10 + iVar6 + 4) != '\x04') {
              FUN_8c019c50(iVar7,pcVar8);
            }
          }
          else if (cVar2 == '\x02') {
            FUN_8c019dc4(iVar7,pcVar8);
          }
          if (*(char *)(*(int *)puVar3 + 8) < cVar1) goto LAB_8c0199b2;
        }
        iVar7 = iVar7 + -1;
        pcVar8 = pcVar8 + -0x40;
        if ((short)iVar7 < 1) {
          return;
        }
      } while( true );
    }
  }
  return;
}

