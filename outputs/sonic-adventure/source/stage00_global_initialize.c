/* Direct C recovery of STG00 0x0c900e00..0x0c900e25.
 * Literal 0x0c900f20 resolves to the global record at 0x8c799788.
 * Field names remain offsets because the record's game meaning is unknown.
 */
#include "stage00_global_initialize.h"

SA_STAGE00_GLOBAL_EXPORT void sa_stage00_global_initialize_0c900e00(
 struct sa_stage00_global_record *record) {
 record->word_00=0;
 record->word_04=0x1f4;
 record->word_08=0;
 record->word_0c=0x64;
 record->word_10=0;
 record->word_14=0xffffff38u;
}
