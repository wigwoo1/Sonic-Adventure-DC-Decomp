/* Validate original lower-layer records without creating another stream pool. */
int jp_adxstm_handle_live(void *handle)
{
    uintptr_t p=(uintptr_t)handle,a=(uintptr_t)adxstmf_obj;
    return p>=a&&p-a<sizeof adxstmf_obj&&(p-a)%sizeof(ADXSTM_OBJ)==0&&((ADXSTM_OBJ *)handle)->used;
}
int jp_adxstm_handle_pending(void *handle)
{return jp_adxstm_handle_live(handle)&&((ADXSTM_OBJ *)handle)->rqst==1;}
int jp_adxstm_last_native_error(void){return jp_adxstm_native_error;}
