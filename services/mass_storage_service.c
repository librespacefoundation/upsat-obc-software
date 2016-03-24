#include "mass_storage_service.h"

struct _MS_data MS_data;

/* entry point for incoming packets. */
/*report downlink is handled from large data transfer.*/
SAT_returnState mass_storage_app(tc_tm_pkt *pkt) {

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return SATR_ERROR; }
    if(!C_ASSERT(pkt->ser_type == TC_MASS_STORAGE_SERVICE) == true) { return SATR_ERROR; }
    if(!C_ASSERT(pkt->ser_subtype == DISABLE || pkt->ser_subtype == TC_MS_DELETE || pkt->ser_subtype == TC_MS_REPORT || pkt->ser_subtype == TC_MS_DOWNLINK) == true) { return SATR_ERROR; }

    MS_sid sid = (MS_sid)pkt->data[0];

    if(!C_ASSERT(sid < LAST_SID) == true) { return SATR_ERROR; }

    if(pkt->ser_subtype == DISABLE) {

    } else if(pkt->ser_subtype == TC_MS_DELETE) {

        uint32_t to;

        cnv8_32(&pkt->data[1], &to);
        mass_storage_delete_api(sid, to);

    } else if(pkt->ser_subtype == TC_MS_REPORT || pkt->ser_subtype == TC_MS_DOWNLINK) {

        large_data_app(pkt);
        
    } else { return SATR_ERROR; }

    return SATR_OK; 
}

/*delete handles deletion of mass storage. sid denotes the store id.*/
/*if to is 0: it deletes every file of the sid else it deletes every file which time is lower then the time denoted in to*/
SAT_returnState mass_storage_delete_api(MS_sid sid, uint32_t to) {

    FRESULT res;
    FILINFO fno;
    DIR dir;
    uint8_t *fn;
    uint8_t path[MS_MAX_PATH];
    uint16_t i;

    if(!C_ASSERT(sid == SU_LOG || sid == EVENT_LOG || sid == FOTOS) == true) { return SATR_ERROR; }

    if(sid == SU_LOG)           { strncpy((char*)path, MS_SU_LOG, MS_MAX_PATH); }
    else if(sid == EVENT_LOG)   { strncpy((char*)path, MS_EVENT_LOG, MS_MAX_PATH); }
    else if(sid == FOTOS)       { strncpy((char*)path, MS_FOTOS, MS_MAX_PATH); }

    if (f_opendir(&dir, (char*)path) != FR_OK) { return SATR_ERROR; } //add more error checking
    for (i = 0; i < MS_MAX_FILES; i++) {

        res = f_readdir(&dir, &fno);                   /* Read a directory item */
        if(res != FR_OK) { f_closedir(&dir); return SATR_ERROR; }  /* Break on error */
        else if(fno.fname[0] == 0) { break; }  /* Break on end of dir */
        if (fno.fname[0] == '.') { continue; }             /* Ignore dot entry */
#if _USE_LFN
        fn = *fno.lfname ? fno.lfname : fno.fname;
#else
        fn = (uint8_t*)fno.fname;
#endif

        uint32_t ret = strtol((char*)fn, NULL, 10);
        if(to == ALL || ret <= to) {

            if(f_stat((char*)fn, &fno) != FR_OK) { f_closedir(&dir); return SATR_ERROR; } 

            if(f_unlink((char*)fn) != FR_OK) { return SATR_ERROR; }

            MS_data.stores_fcount[sid-SU_SCRIPT_7]--;
            MS_data.stores_fsize[sid-SU_SCRIPT_7] -= fno.fsize;

        }
    }
    f_closedir(&dir);

    if(i == MS_MAX_FILES - 1) { return SATR_MS_MAX_FILES; }
    
    return SATR_OK;
}

/*it downlinks files, it is accessed from large data transfer only*/
/*Higher level, it is used only for differentiatin from different file modes, large files and logs*/
SAT_returnState mass_storage_downlink_api(MS_sid sid, MS_mode mode, uint32_t from, uint32_t to, uint8_t *buf, uint16_t *size, uint32_t *part) {

    SAT_returnState res; 

    if(!C_ASSERT(buf != NULL && size != NULL && part != NULL) == true)       { return SATR_ERROR; }
    if(!C_ASSERT(sid == SU_LOG || sid == EVENT_LOG || sid == FOTOS) == true) { return SATR_ERROR; }

    if(sid == SU_LOG || sid == EVENT_LOG) { res = mass_storage_downlinkLogs(sid, mode, from, to, buf, size, part); }
    else if(sid == FOTOS) { res = mass_storage_downlinkLargeFile(sid, from, buf, size, part); }
    else { return SATR_ERROR; }

    return res;
}

/*it downlinks logs, it is accessed from large data transfer only*/
/*valid sid is only stores that have logs, SU and event.*/
/*mode: */
/*from: depanding on the mode its the */
/*to: depanding on the mode its the */
/*buf: the buffer that the data are copied*/
/*size: its the size of the buffer, in return it stores the actual bytes written in the buffer */
/*part: part stores the next file log to be downlinked. after the cp to the buffer it iterates to the next file. in a new search should be 0. */
SAT_returnState mass_storage_downlinkLogs(MS_sid sid, MS_mode mode, uint32_t from, uint32_t to, uint8_t *buf, uint16_t *size, uint32_t *part) {

    FIL fp;
    FRESULT res;
    uint16_t byteswritten;
    uint8_t path[MS_MAX_PATH];

    if(!C_ASSERT(buf != NULL && size != NULL && size != NULL && part != NULL) == true)  { return SATR_ERROR; }
    if(!C_ASSERT(*size > 0) == true)                                                    { return SATR_ERROR; }
    if(!C_ASSERT(sid == SU_LOG || sid == EVENT_LOG) == true)                            { return SATR_ERROR; }
    if(!C_ASSERT(mode < LAST_MODE) == true)                                             { return SATR_ERROR; }

    /*cp dir belonging to sid*/
    if(sid == SU_LOG)           { strncpy((char*)path, MS_SU_LOG, MS_MAX_PATH); }
    else if(sid == EVENT_LOG)   { strncpy((char*)path, MS_EVENT_LOG, MS_MAX_PATH); }

    /*Find first file, new search*/
    if(*part == 0) {
        SAT_returnState res;

        if(mode == ALL) { res = mass_storage_findLog(sid, part); } 
        else if(mode == TO) { res = mass_storage_findLog(sid, part); }
        else if(mode == BETWEEN) { 
            *part = from;
            res = mass_storage_findLog(sid, part); 
        } 
        else if(mode == SPECIFIC ) { res = SATR_OK; }

        if(res == SATR_EOT) { return SATR_EOT; }
        else if(res != SATR_OK) { return SATR_ERROR; }
    } 

    if(!C_ASSERT(*part > 0) == true) { return SATR_ERROR; }

    snprintf((char*)path, MS_MAX_PATH, "%s//%d", path, *part);

    if(f_open(&fp, (char*)path, FA_OPEN_ALWAYS | FA_READ) != FR_OK) { return SATR_ERROR; }

    uint16_t len = f_size(&fp);
    if(!C_ASSERT(len < (*part) * MS_FILE_SECTOR) == true){ f_close(&fp); return SATR_ERROR; }

    res = f_read(&fp, buf, *size, (void *)&byteswritten);
    if((byteswritten == 0) || (res != FR_OK)) {  f_close(&fp); return SATR_ERROR; } 
    *size = byteswritten;

    f_close(&fp);

    if((mode == ALL || mode == TO || mode == BETWEEN) && mass_storage_findLog(sid, part) == SATR_EOT) { return SATR_EOT; } 
    if((mode == to || mode == BETWEEN) && *part >= TO) { return SATR_EOT; }

    return SATR_OK;
}

/*it downlinks fotos, it is accessed from large data transfer only*/
/*valid sid is only stores that have fotos.*/
/*file: is the file to be downlinked*/
/*size: its the size of the buffer, in return it stores the actual bytes written in the buffer */
SAT_returnState mass_storage_downlinkLargeFile(MS_sid sid, uint32_t file, uint8_t *buf, uint16_t *size, uint32_t *part) {

    FIL fp;
    FRESULT res;
    uint16_t byteswritten;
    uint8_t path[MS_MAX_PATH];

    if(!C_ASSERT(buf != NULL && size != NULL && part != NULL) == true)  { return SATR_ERROR; }
    if(!C_ASSERT(sid == FOTOS) == true)                                 { return SATR_ERROR; }

    snprintf((char*)path, MS_MAX_PATH, "%s//%d", MS_FOTOS, file);

    if(f_open(&fp, (char*)path, FA_OPEN_ALWAYS | FA_READ) != FR_OK) { return SATR_ERROR; }

    uint16_t len = f_size(&fp);
    if(!C_ASSERT(len < (*part) * MS_FILE_SECTOR) == true) { f_close(&fp); return SATR_ERROR; }
    
    res = f_lseek(&fp, ((*part)*MS_FILE_SECTOR));

    res = f_read(&fp, buf, *size, (void *)&byteswritten);
    if((byteswritten == 0) || (res != FR_OK)) { f_close(&fp); return SATR_ERROR; } 

    f_close(&fp);

    if(*size > byteswritten) { *size = byteswritten; return SATR_EOT; } 
    *size = byteswritten;

    return SATR_OK;
}

/*it stores files, it is accessed from large data transfer for su scripts & fotos and from for logs*/
/*Higher level, it is used only for differentiatin from different file modes, large files and logs*/
SAT_returnState mass_storage_store_api(MS_sid sid, MS_mode mode, uint8_t *buf, uint16_t *size, uint32_t part) {

    SAT_returnState res; 

    if(!C_ASSERT(buf != NULL && size != NULL && part != NULL) == true)       { return SATR_ERROR; }
    if(!C_ASSERT(sid == SU_LOG || sid == EVENT_LOG || sid == FOTOS) == true)    { return SATR_ERROR; }

    if(sid == SU_LOG || sid == EVENT_LOG) { res = mass_storage_storeLogs(sid, buf, size); }
    else if(sid == FOTOS || sid <= SU_SCRIPT_7) { res = mass_storage_storeLargeFile(sid, mode, buf, size, part); }
    else { return SATR_ERROR; }

    return res;

}


SAT_returnState mass_storage_storeLargeFile(MS_sid sid, MS_mode mode, uint8_t *buf, uint16_t *size, uint32_t part) {

    FILINFO fno;
    FRESULT res;
    FIL fp;
    BYTE flags;
    uint16_t byteswritten;
    uint8_t path[MS_MAX_PATH];

    if(!C_ASSERT(buf != NULL && size != NULL) == true)          { return SATR_ERROR; }
    if(!C_ASSERT(*size > 0) == true)                            { return SATR_ERROR; }
    if(!C_ASSERT(sid == FOTOS || sid <= SU_SCRIPT_7) == true)   { return SATR_ERROR; }

    if(sid == FOTOS)            { strncpy((char*)path, MS_TMP_FOTOS, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_1) { strncpy((char*)path, MS_TMP_SU_SCRIPT_1, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_2) { strncpy((char*)path, MS_TMP_SU_SCRIPT_2, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_3) { strncpy((char*)path, MS_TMP_SU_SCRIPT_3, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_4) { strncpy((char*)path, MS_TMP_SU_SCRIPT_4, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_5) { strncpy((char*)path, MS_TMP_SU_SCRIPT_5, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_6) { strncpy((char*)path, MS_TMP_SU_SCRIPT_6, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_7) { strncpy((char*)path, MS_TMP_SU_SCRIPT_7, MS_MAX_PATH); }

    if(part == 0) { flags = (FA_CREATE_ALWAYS | FA_WRITE); } 
    else { flags = (FA_CREATE_ALWAYS | FA_WRITE); }

    if(f_open(&fp, (char*)path, flags) != FR_OK) { return SATR_ERROR; }

    uint16_t len = f_size(&fp);
    if(!C_ASSERT(len < (part * MS_FILE_SECTOR)) == true) { f_close(&fp); return SATR_ERROR; }
    res = f_lseek(&fp, len);

    res = f_write(&fp, buf, *size, (void *)&byteswritten);
    if((byteswritten == 0) || (res != FR_OK)) { f_close(&fp); return SATR_ERROR; }

    f_close(&fp);

    if(mode == LAST_PART) {

        if(sid == FOTOS) {

            mass_storage_getFileName(path);
            if(f_rename((char*)path, MS_TMP_FOTOS) != FR_OK) { return SATR_ERROR; }

            if(f_stat((char*)path, &fno) != FR_OK) { return SATR_ERROR; } 

            if(sid == FOTOS) {
	            MS_data.stores_fcount[sid-SU_SCRIPT_7]++;
	            MS_data.stores_fsize[sid-SU_SCRIPT_7] += fno.fsize;
	        }

        } else if(sid == SU_SCRIPT_1) { 
            if(mass_storage_su_checksum_api(TMP_SU_SCRIPT_1) != SATR_OK) { return SATR_ERROR; } 
            if(f_rename(MS_SU_SCRIPT_1, MS_TMP_SU_SCRIPT_1) != FR_OK) { return SATR_ERROR; }
        } else if(sid == SU_SCRIPT_2) { 
            if(mass_storage_su_checksum_api(TMP_SU_SCRIPT_2) != SATR_OK) { return SATR_ERROR; } 
            if(f_rename(MS_SU_SCRIPT_2, MS_TMP_SU_SCRIPT_2) != FR_OK) { return SATR_ERROR; }
        } else if(sid == SU_SCRIPT_3) { 
            if(mass_storage_su_checksum_api(TMP_SU_SCRIPT_3) != SATR_OK) { return SATR_ERROR; } 
            if(f_rename(MS_SU_SCRIPT_3, MS_TMP_SU_SCRIPT_3) != FR_OK) { return SATR_ERROR; }
        }  else if(sid == SU_SCRIPT_4) { 
            if(mass_storage_su_checksum_api(TMP_SU_SCRIPT_4) != SATR_OK) { return SATR_ERROR; } 
            if(f_rename(MS_SU_SCRIPT_4, MS_TMP_SU_SCRIPT_4) != FR_OK) { return SATR_ERROR; }
        }  else if(sid == SU_SCRIPT_5) { 
            if(mass_storage_su_checksum_api(TMP_SU_SCRIPT_5) != SATR_OK) { return SATR_ERROR; } 
            if(f_rename(MS_SU_SCRIPT_5, MS_TMP_SU_SCRIPT_5) != FR_OK) { return SATR_ERROR; }
        }  else if(sid == SU_SCRIPT_6) { 
            if(mass_storage_su_checksum_api(TMP_SU_SCRIPT_6) != SATR_OK) { return SATR_ERROR; } 
            if(f_rename(MS_SU_SCRIPT_6, MS_TMP_SU_SCRIPT_6) != FR_OK) { return SATR_ERROR; }
        }  else if(sid == SU_SCRIPT_7) { 
            if(mass_storage_su_checksum_api(TMP_SU_SCRIPT_7) != SATR_OK) { return SATR_ERROR; } 
            if(f_rename(MS_SU_SCRIPT_7, MS_TMP_SU_SCRIPT_7) != FR_OK) { return SATR_ERROR; }
        }  
    }

    return SATR_OK;
}

SAT_returnState mass_storage_storeLogs(MS_sid sid, uint8_t *buf, uint16_t *size) {

    FIL fp;
    FRESULT res; 

    uint16_t byteswritten;
    uint8_t path[MS_MAX_PATH];
    uint32_t fsize;

    if(!C_ASSERT(buf != NULL && size != NULL) == true)      { return SATR_ERROR; }
    if(!C_ASSERT(*size > 0) == true)                        { return SATR_ERROR; }

    if(mass_storage_getLog(sid, path) != SATR_OK) { return SATR_ERROR; }

    if(f_open(&fp, (char*)path, FA_OPEN_ALWAYS | FA_WRITE) != FR_OK) { return SATR_ERROR; }

    /*appending*/
    //fsize = f_size(&fp)
    //if(!C_ASSERT(fsize > ) && fsize < MS_MAX_LOG_FILE_SIZE) == true) { f_close(&fp); return SATR_ERROR; } //check this equation

	//should be always zero
    //res = f_lseek(&fp, fsize);
    /* Write data to the text file */
    res = f_write(&fp, buf, *size, (void *)&byteswritten);
    if((byteswritten == 0) || (res != FR_OK)) { f_close(&fp); return SATR_ERROR; } 
    /* Close the open text file */
    f_close(&fp);

    //if(fsize == 0) { MS_data.stores_fcount[sid-SU_SCRIPT_7]++; } //check this
    MS_data.stores_fcount[sid-SU_SCRIPT_7]++;
    MS_data.stores_fsize[sid-SU_SCRIPT_7] += byteswritten;

    return SATR_OK;
}


SAT_returnState mass_storage_report_api(MS_sid sid, uint8_t *buf, uint16_t *size, uint32_t *iter) {

    DIR dir;
    FILINFO fno;
    FRESULT res;
    uint32_t ret;
    uint8_t *fn;
    uint8_t start_flag = 0;
    uint8_t path[MS_MAX_PATH];
    uint16_t i;

    if(!C_ASSERT(buf != NULL && size != NULL && iter != NULL) == true)          { return SATR_ERROR; }
    if(!C_ASSERT(*size == 0) == true)                                           { return SATR_ERROR; }
    if(!C_ASSERT(sid == SU_LOG || sid == EVENT_LOG || sid == FOTOS) == true)    { return SATR_ERROR; }

    if(sid == SU_LOG)           { strncpy((char*)path, MS_SU_LOG, MS_MAX_PATH); }
    else if(sid == EVENT_LOG)   { strncpy((char*)path, MS_EVENT_LOG, MS_MAX_PATH); }
    else if(sid == FOTOS)       { strncpy((char*)path, MS_FOTOS, MS_MAX_PATH); }

    if(*iter == 0) { start_flag = 1; }
    else { start_flag = 0; }

    if (f_opendir(&dir, (char*)path) != FR_OK) { return SATR_ERROR; }
    for (i = 0; i < MS_MAX_FILES; i++) {

        res = f_readdir(&dir, &fno);                   /* Read a directory item */
        if(res != FR_OK) { f_closedir(&dir); return SATR_ERROR; }  /* Break on error */
        else if(fno.fname[0] == 0) { f_closedir(&dir); return SATR_EOT;}  /* Break on end of dir */
        if (fno.fname[0] == '.') { continue; }             /* Ignore dot entry */
#if _USE_LFN
        fn = *fno.lfname ? fno.lfname : fno.fname;
#else
        fn = (uint8_t*)fno.fname;
#endif

        ret = strtol((char*)fn, NULL, 10);
        if(start_flag = 0 && *iter == ret) { start_flag = 1; }
        if(start_flag == 1) {

            cnv32_8(ret, &buf[(*size)]);
            *size += sizeof(uint32_t);

            //if(f_stat(fn, &fno) != FSATR_OK) { f_closedir(&dir) return SATR_ERROR; } 

            //(*fcount)++;
            //*fsize += fno.fsize;    

            if(*size >= MS_MAX_LOG_FILE_SIZE) {
                *iter = ret;
                f_closedir(&dir);
                return SATR_OK; 
            }
        } 

    }
    f_closedir(&dir);
 
    if(!C_ASSERT(*size != 0) == true) { return SATR_ERROR; }
    if(i == MS_MAX_FILES - 1) { return SATR_MS_MAX_FILES; }

    return SATR_OK;
}

SAT_returnState mass_storage_su_checksum_api(MS_sid sid) {

    FIL fp;
    FRESULT res;
    uint8_t path[MS_MAX_PATH];
    uint16_t byteswritten;
    uint16_t i;

    uint16_t sum1 = 0;
    uint16_t sum2 = 0;

    uint8_t c = 0;

    if(!C_ASSERT(sid < LAST_SID) == true) { return SATR_ERROR; }

    if(sid == SU_SCRIPT_1)          { strncpy((char*)path, MS_SU_SCRIPT_1, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_2)     { strncpy((char*)path, MS_SU_SCRIPT_2, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_3)     { strncpy((char*)path, MS_SU_SCRIPT_3, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_4)     { strncpy((char*)path, MS_SU_SCRIPT_4, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_5)     { strncpy((char*)path, MS_SU_SCRIPT_5, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_6)     { strncpy((char*)path, MS_SU_SCRIPT_6, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_7)     { strncpy((char*)path, MS_SU_SCRIPT_7, MS_MAX_PATH); }
    else if(sid == TMP_SU_SCRIPT_1) { strncpy((char*)path, MS_TMP_SU_SCRIPT_1, MS_MAX_PATH); }
    else if(sid == TMP_SU_SCRIPT_2) { strncpy((char*)path, MS_TMP_SU_SCRIPT_2, MS_MAX_PATH); }
    else if(sid == TMP_SU_SCRIPT_3) { strncpy((char*)path, MS_TMP_SU_SCRIPT_3, MS_MAX_PATH); }
    else if(sid == TMP_SU_SCRIPT_4) { strncpy((char*)path, MS_TMP_SU_SCRIPT_4, MS_MAX_PATH); }
    else if(sid == TMP_SU_SCRIPT_5) { strncpy((char*)path, MS_TMP_SU_SCRIPT_5, MS_MAX_PATH); }
    else if(sid == TMP_SU_SCRIPT_6) { strncpy((char*)path, MS_TMP_SU_SCRIPT_6, MS_MAX_PATH); }
    else if(sid == TMP_SU_SCRIPT_7) { strncpy((char*)path, MS_TMP_SU_SCRIPT_7, MS_MAX_PATH); }
    else { return SATR_ERROR; }

    if(f_open(&fp, (char*)path, FA_OPEN_ALWAYS | FA_READ) != FR_OK) { return SATR_ERROR; }

    for(i = 0; i < MS_MAX_SU_FILE_SIZE; i++) {
        
        res = f_read(&fp, &c, 1, (void *)&byteswritten);
        if(res != FR_OK) { f_close(&fp); return SATR_ERROR; } 
        if(byteswritten == 0) {
            
            if(((sum2 << 8) | sum1) == 0)  { return SATR_OK; }
            else {  return SATR_CRC_ERROR; }
        } 

        sum1 = (sum1 + c) % 255; 
        sum2 = (sum2 + sum1) % 255;
    }

    f_close(&fp);

	if(i == MS_MAX_FILES - 1) { return SATR_MS_MAX_FILES; }

    return SATR_OK;
}
//add assertions for file size?
SAT_returnState mass_storage_getLog(MS_sid sid, uint8_t *fn) {

    FILINFO fno;

    if(!C_ASSERT(fn != NULL) == true)                           { return SATR_ERROR; }
    if(!C_ASSERT(sid == SU_LOG || sid == EVENT_LOG) == true)    { return SATR_ERROR; }

    if(sid == SU_LOG) { snprintf((char*)fn, MS_MAX_PATH, "%s//%d", MS_SU_LOG, time_now()); }
    else if(sid == EVENT_LOG) { snprintf((char*)fn, MS_MAX_PATH, "%s//%d", MS_EVENT_LOG, time_now()); }
    return SATR_OK; 

    //if(sid == SU_LOG) { 

    //    snprintf((char*)fn, MS_MAX_PATH, "%s//%d", MS_SU_LOG, time_now()); 
    //    return SATR_OK; 
    //}

    // if(MS_data.ev_temp_log == 0) { 
    //     MS_data.ev_temp_log = time_now(); 
    //     snprintf((char*)fn, MS_MAX_PATH, "%s//%d", MS_EVENT_LOG, MS_data.ev_temp_log); 
    //     return SATR_OK; 
    // }

    // snprintf((char*)fn, MS_MAX_PATH, "%s//%d", MS_EVENT_LOG, MS_data.ev_temp_log); 

    // if(f_stat((char*)fn, &fno) != FR_OK) { return SATR_ERROR; } 

    // if(fno.fsize >= MS_MAX_LOG_FILE_SIZE) {

    //     MS_data.ev_temp_log = time_now(); 
    //     snprintf((char*)fn, MS_MAX_PATH, "%s//%d", MS_EVENT_LOG, MS_data.ev_temp_log); 
    //     return SATR_OK; 
    // }

    // return SATR_OK;
}

SAT_returnState mass_storage_findLog(MS_sid sid, uint32_t *fn) {

    DIR dir;
    FRESULT res;
    FILINFO fno;
    uint32_t ret;
    uint8_t path[MS_MAX_PATH];
    uint8_t *temp_fn;
    uint32_t min = 0;
    uint16_t i;

    if(!C_ASSERT(sid == SU_LOG || sid == EVENT_LOG) == true)    { return SATR_ERROR; }
    if(!C_ASSERT(fn != NULL) == true)                           { return SATR_ERROR; }

    if(sid == SU_LOG)           { strncpy((char*)path, MS_SU_LOG, MS_MAX_PATH); }
    else if(sid == EVENT_LOG)   { strncpy((char*)path, MS_EVENT_LOG, MS_MAX_PATH); }

    if (f_opendir(&dir,(char*) path) != FR_OK) { return SATR_ERROR; }
    for (i = 0; i < MS_MAX_FILES; i++) {

        res = f_readdir(&dir, &fno);                   /* Read a directory item */
        if(res != FR_OK) { f_closedir(&dir); return SATR_ERROR; }  /* Break on error */
        else if(fno.fname[0] == 0) { break; }  /* Break on end of dir */
         if (fno.fname[0] == '.') continue;             /* Ignore dot entry */
#if _USE_LFN
        temp_fn = *fno.lfname ? fno.lfname : fno.fname;
#else
        temp_fn = (uint8_t*)fno.fname;
#endif

        ret = strtol((char*)temp_fn, NULL, 10);
        if(*fn == 0 && min == 0) { min = ret; }
        else if(*fn == 0 && ret < min) { min = ret; } 
        else if(*fn != 0 && *fn < ret && min == 0) { min = ret; }
        else if(*fn != 0 && *fn < ret && ret < min) { min = ret; } 

    }
    f_closedir(&dir);
 
    if(min == 0) { return SATR_EOT; }

    *fn = min;

	if(i == MS_MAX_FILES - 1) { return SATR_MS_MAX_FILES; }

    return SATR_OK;
}


SAT_returnState mass_storage_getFileSizeCount(MS_sid sid) {

    DIR dir;
    FRESULT res;
    FILINFO fno;
    uint8_t path[MS_MAX_PATH];
    uint16_t i;

    if(!C_ASSERT(sid == SU_LOG || sid == EVENT_LOG || sid == FOTOS) == true) { return SATR_ERROR; }

    if(sid == SU_LOG)           { strncpy((char*)path, MS_SU_LOG, MS_MAX_PATH); }
    else if(sid == EVENT_LOG)   { strncpy((char*)path, MS_EVENT_LOG, MS_MAX_PATH); }
    else if(sid == FOTOS)       { strncpy((char*)path, MS_FOTOS, MS_MAX_PATH); }

    if (f_opendir(&dir, (char*)path) != FR_OK) { return SATR_ERROR; }
    for (i = 0; i < MS_MAX_FILES; i++) {

        res = f_readdir(&dir, &fno);                   /* Read a directory item */
        if(res != FR_OK) { f_closedir(&dir); return SATR_ERROR; }  /* Break on error */
        else if(fno.fname[0] == 0) { break; }  /* Break on end of dir */
        if (fno.fname[0] == '.') continue;             /* Ignore dot entry */

        MS_data.stores_fcount[sid-SU_SCRIPT_7]++;
        MS_data.stores_fsize[sid-SU_SCRIPT_7] += fno.fsize;

    }
    f_closedir(&dir);
 
	if(i == MS_MAX_FILES - 1) { return SATR_MS_MAX_FILES; }

    return SATR_OK;
}

SAT_returnState mass_storage_init() {

    //MS_data.ev_temp_log = 0;
    if(f_mount(&MS_data.test, MS_SD_PATH, 0) != FR_OK) { return SATR_ERROR; }

    mass_storage_getFileSizeCount(FOTOS);
    mass_storage_getFileSizeCount(SU_LOG);
    mass_storage_getFileSizeCount(EVENT_LOG);

    return SATR_OK;
}

SAT_returnState mass_storage_getFileName(uint8_t *fn) {

    snprintf((char*)fn, MS_MAX_FNAME, "%d", time_now());
    return SATR_OK;
}
