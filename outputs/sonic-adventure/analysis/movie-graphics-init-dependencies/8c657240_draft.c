/* Ghidra draft only; not buildable/verified source. */

void FUN_8c657240(uint param_1)

{
  undefined4 *puVar1;
  undefined *puVar2;
  undefined4 *puVar3;
  uint local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  
  puVar2 = PTR_FUN_8c657448;
  puVar1 = DAT_8c657444;
  local_3c = *(undefined4 *)(PTR_DAT_8c657440 + 4);
  local_40 = *(undefined4 *)PTR_DAT_8c657440;
  local_34 = *(undefined4 *)(PTR_DAT_8c657440 + 0xc);
  local_38 = *(undefined4 *)(PTR_DAT_8c657440 + 8);
  local_30 = *(undefined4 *)(PTR_DAT_8c657440 + 0x10);
  local_2c = *(undefined4 *)(PTR_DAT_8c657440 + 0x14);
  local_24 = *(undefined4 *)(PTR_DAT_8c657440 + 0x1c);
  local_28 = *(undefined4 *)(PTR_DAT_8c657440 + 0x18);
  if (((*DAT_8c65744c & 1) != 0) && ((*DAT_8c657450 == 0 || (*DAT_8c657454 != 0x20)))) {
    local_44 = 0;
    puVar3 = DAT_8c657458;
    if (param_1 != 0) {
      do {
        (*(code *)puVar2)(*puVar3,&local_40,0x20);
        local_44 = local_44 + 1;
        puVar3 = puVar3 + 5;
      } while (local_44 < param_1);
    }
    *puVar1 = 8;
  }
  if (((*DAT_8c65744c & 2) != 0) &&
     ((local_40 = DAT_8c65745c, *DAT_8c657450 == 0 || (*DAT_8c657454 != 0x21)))) {
    local_44 = 0;
    puVar3 = DAT_8c657458;
    if (param_1 != 0) {
      do {
        (*(code *)puVar2)(puVar3[1],&local_40,0x20);
        local_44 = local_44 + 1;
        puVar3 = puVar3 + 5;
      } while (local_44 < param_1);
    }
    puVar1[1] = 8;
  }
  if (((*DAT_8c65744c & 4) != 0) &&
     ((local_40 = DAT_8c657460, *DAT_8c657450 == 0 || (*DAT_8c657454 != 0x22)))) {
    local_44 = 0;
    puVar3 = DAT_8c657458;
    if (param_1 != 0) {
      do {
        (*(code *)puVar2)(puVar3[2],&local_40,0x20);
        local_44 = local_44 + 1;
        puVar3 = puVar3 + 5;
      } while (local_44 < param_1);
    }
    puVar1[2] = 8;
  }
  if (((*DAT_8c65744c & 8) != 0) &&
     ((local_40 = DAT_8c657464, *DAT_8c657450 == 0 || (*DAT_8c657454 != 0x23)))) {
    local_44 = 0;
    puVar3 = DAT_8c657458;
    if (param_1 != 0) {
      do {
        (*(code *)puVar2)(puVar3[3],&local_40,0x20);
        local_44 = local_44 + 1;
        puVar3 = puVar3 + 5;
      } while (local_44 < param_1);
    }
    puVar1[3] = 8;
  }
  if (((*DAT_8c65744c & 0x10) != 0) &&
     ((local_40 = DAT_8c657468, *DAT_8c657450 == 0 || (*DAT_8c657454 != 0x24)))) {
    local_44 = 0;
    puVar3 = DAT_8c657458;
    if (param_1 != 0) {
      do {
        (*(code *)puVar2)(puVar3[4],&local_40,0x20);
        local_44 = local_44 + 1;
        puVar3 = puVar3 + 5;
      } while (local_44 < param_1);
    }
    puVar1[4] = 8;
  }
  return;
}

