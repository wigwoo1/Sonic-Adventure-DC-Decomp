/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c10e804) */
/* WARNING: Removing unreachable block (ram,0x8c10e844) */

ulonglong FUN_8c10e7b8(ulonglong param_1,uint param_2,int param_3)

{
  uint uVar1;
  int iVar2;
  uint in_fr1;
  ulonglong uVar3;
  ulonglong in_xd4;
  ulonglong in_xd6;
  char in_FPSCR_SZ;
  uint local_8;
  
  _local_8 = param_1;
  if (in_FPSCR_SZ == '\0') {
    _local_8 = param_1 >> 0x20;
  }
  if (in_FPSCR_SZ == '\0') {
    param_2 = *(uint *)PTR_DAT_8c10e860;
  }
  else {
    in_xd6 = *(ulonglong *)PTR_DAT_8c10e860;
  }
  uVar1 = local_8 & DAT_8c10e858;
  if (in_FPSCR_SZ == '\0') {
    param_1 = (ulonglong)*(uint *)PTR_DAT_8c10e864;
  }
  else {
    in_xd4 = *(ulonglong *)PTR_DAT_8c10e864;
  }
  if (uVar1 != DAT_8c10e858) {
    if (in_FPSCR_SZ == '\0') {
      param_1 = param_1 & 0xffffffff;
      uVar3 = (ulonglong)param_2;
    }
    else {
      param_1 = *(ulonglong *)PTR_DAT_8c10e870;
      uVar3 = *(ulonglong *)PTR_DAT_8c10e874;
    }
    if (0 < (int)uVar1) {
      iVar2 = ((int)uVar1 >> 0x17) + param_3;
      if ((0 < iVar2) && (iVar2 < DAT_8c10e856)) {
        uVar1 = DAT_8c10e85c & local_8 | iVar2 * 0x800000 | DAT_8c10e868 & local_8;
        _local_8 = (ulonglong)uVar1;
        if (in_FPSCR_SZ == '\0') {
          _local_8 = CONCAT44(uVar1,in_fr1);
        }
        return _local_8;
      }
      if (-1 < param_3) goto joined_r0x8c10e7de;
    }
    if ((DAT_8c10e85c & local_8) != 0) {
      if (in_FPSCR_SZ == '\0') {
        uVar3 = (ulonglong)in_fr1;
      }
      return uVar3;
    }
    if (in_FPSCR_SZ == '\0') {
      param_1 = (ulonglong)in_fr1;
    }
    return param_1;
  }
  if ((DAT_8c10e868 & local_8) != 0) {
    if (in_FPSCR_SZ == '\0') {
      uVar3 = (ulonglong)in_fr1;
    }
    else {
      uVar3 = *(ulonglong *)PTR_DAT_8c10e86c;
    }
    return uVar3;
  }
joined_r0x8c10e7de:
  if ((DAT_8c10e85c & local_8) != 0) {
    if (in_FPSCR_SZ == '\0') {
      in_xd4 = (ulonglong)in_fr1;
    }
    return in_xd4;
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd6 = (ulonglong)in_fr1;
  }
  return in_xd6;
}

