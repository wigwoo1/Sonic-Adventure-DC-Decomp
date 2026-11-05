#ifndef JP_NATIVE_PERIPHERAL_CARDS_H
#define JP_NATIVE_PERIPHERAL_CARDS_H
/* Configure before input startup. The real VMU store/LCD may be initialized
 * afterward; each input poll observes current attachments. Cards remain
 * owned by their existing storage/LCD services, not the controller backend. */
__declspec(dllexport) int jp_input_use_virtual_cards(void);
#endif
