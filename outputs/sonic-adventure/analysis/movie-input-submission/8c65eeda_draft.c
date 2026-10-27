/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65eeda(void)

{
  char cVar1;
  undefined *puVar2;
  int *piVar3;
  uint uVar4;
  uint *puVar5;
  int iVar6;
  int iVar7;
  
  piVar3 = DAT_8c65ef2c;
  iVar7 = *DAT_8c65ef2c;
  iVar6 = iVar7 + 0x28;
  puVar5 = (uint *)(DAT_8c65ef18 + iVar7);
  if (0 < *(int *)(DAT_8c65ef1a + iVar7)) {
    (*(code *)PTR_FUN_8c65ef30)(*(undefined4 *)(DAT_8c65ef1a + iVar7),1);
  }
  puVar2 = PTR_FUN_8c65ef20;
  for (iVar7 = 0; iVar7 < 0x18; iVar7 = iVar7 + 1) {
    if ((*(char *)((int)DAT_8c65f008 + *piVar3 + iVar7) != '\0') && (*(char *)(iVar6 + 6) == '\0'))
    {
      cVar1 = *(char *)(iVar6 + 4);
      if (cVar1 == '\x01') {
        (*(code *)puVar2)(iVar7);
      }
      else if (cVar1 == '\x02') {
        (*(code *)PTR_FUN_8c65f010)(iVar7);
      }
      else if (cVar1 == '\x03') {
        (*(code *)PTR_FUN_8c65f014)(iVar7);
      }
      else if (cVar1 == '\x04') {
        (*(code *)PTR_FUN_8c65f018)(iVar7);
      }
      else if (cVar1 == '\t') {
        uVar4 = *puVar5;
        if ((uVar4 & 1) != 0) {
          uVar4 = 1;
        }
        (*(code *)PTR_FUN_8c65f01c)(iVar7,uVar4);
      }
    }
    puVar5 = puVar5 + 0x1e;
    iVar6 = iVar6 + 0x2c;
  }
  (*(code *)PTR_FUN_8c65f020)();
  return 0;
}

