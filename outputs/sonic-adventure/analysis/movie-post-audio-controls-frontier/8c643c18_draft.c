/* Ghidra draft only; not buildable/verified source. */

int FUN_8c643c18(int param_1,char *param_2,byte param_3)

{
  char cVar1;
  byte bVar2;
  undefined4 *puVar3;
  uint uVar4;
  undefined1 *puVar5;
  undefined1 *puVar6;
  undefined4 uVar7;
  char cVar8;
  uint uVar9;
  int iVar10;
  
  iVar10 = DAT_8c643d1c;
  if ((*(int *)PTR_DAT_8c643d18 != 0) && (iVar10 = DAT_8c643d20, param_1 != 0)) {
    cVar1 = *(char *)(param_1 + 1);
    uVar9 = (uint)DAT_8c643d10;
    uVar4 = (uint)param_3;
    if ((uVar4 & uVar9) == uVar9) {
      bVar2 = 0;
      *param_2 = '\0';
      param_2[1] = '\0';
      param_2[2] = '\0';
    }
    else {
      if (((uint)&stack0x00000001 & 3) == 0) {
        puVar5 = &stack0x00000001;
      }
      else {
        puVar5 = &stack0x00000004;
      }
      puVar6 = puVar5 + 4;
      if (((uint)puVar6 & 3) != 0) {
        puVar6 = puVar5 + 8;
      }
      puVar3 = (undefined4 *)(puVar6 + -4);
      if (((uint)puVar3 & 3) != 0) {
        puVar3 = (undefined4 *)(puVar6 + -8);
      }
      uVar7 = *puVar3;
      if (((uVar4 & uVar9) == (int)DAT_8c643d12) || ((uVar4 & uVar9) == (int)DAT_8c643d14)) {
        cVar8 = '\0';
      }
      else {
        puVar5 = puVar6 + 4;
        if (((uint)puVar5 & 3) != 0) {
          puVar5 = puVar6 + 8;
        }
        puVar3 = (undefined4 *)(puVar5 + -4);
        if (((uint)puVar3 & 3) != 0) {
          puVar3 = (undefined4 *)(puVar5 + -8);
        }
        cVar8 = (char)*puVar3;
      }
      (*(code *)PTR_FUN_8c643d28)
                (PTR_s_MIDI_Mes___02X__02X__02X_8c643d24,uVar4,(int)(char)uVar7,(int)cVar8,0x10);
      *param_2 = cVar8;
      param_2[1] = (char)uVar7;
      param_2[2] = param_3 & 0xf | cVar1 << 4;
      bVar2 = param_3 >> 4 & 7;
    }
    param_2[3] = bVar2;
    (*(code *)PTR_FUN_8c643d7c)
              (PTR_s_Pack_Data__Ptr__08XH___02X__02X___8c643d78,param_2,*param_2,param_2[1],
               param_2[2],param_2[3]);
    iVar10 = 0;
  }
  if (iVar10 != 0) {
    (*(code *)PTR_FUN_8c643d84)(PTR_FUN_8c643d80,iVar10);
  }
  return iVar10;
}

