#include "mass_storage_service.h"

/* entry point for incoming packets. */
/*report downlink is handled from large data transfer.*/
OBC_returnStateTypedef mass_storage_app(tc_tm_pkt *pkt) {

    ASSERT(pkt != NULL && pkt->data != NULL);
    REQUIRE(pkt->ser_type == TC_MASS_STORAGE_SERVICE);

    MS_sid sid = pkt->data[0];

    REQUIRE(sid < LAST_SID);

    if(pkt->ser_subtype == DISABLE) {

    } else if(pkt->ser_subtype == DELETE) {

        cnv8_32(pkt->data[1], pkt->data[2], pkt->data[3], pkt->data[4], &to);
        mass_storage_delete_api(sid, to);

    } else if(pkt->ser_subtype == REPORT || pkt->ser_subtype == DOWNLINK) {

        large_data_app(pkt);
        
    } else { return R_ERROR; }

    return R_OK; 
}

/*delete handles deletion of mass storage. sid denotes the store id.*/
/*if to is 0: it deletes every file of the sid else it deletes every file which time is lower then the time denoted in to*/
OBC_returnStateTypedef mass_storage_delete_api(MS_sid sid, uint32_t to) {

    DIR dir;
    uint8_t temp_fn[20];
    uint32_t time_temp = 0;
    uint8_t path[MS_MAX_PATH];

    REQUIRE(sid == SU_LOG || sid == EVENT_LOG || sid == FOTOS);

    if(sid == SU_LOG) { strncp(path, MS_SU_LOG, MS_MAX_PATH); }
    else if(sid == EVENT_LOG) { strncp(path, EVENT_LOG, MS_MAX_PATH); }
    else if(sid == FOTOS) { strncp(path, MS_FOTOS, MS_MAX_PATH); }

    if (f_opendir(&dir, path) != FR_OK) { return R_ERROR; } //add more error checking
    for (uint16_t i = 0; i < MS_MAX_FILES; i++) {

        res = f_readdir(&dir, &fno);                   /* Read a directory item */
        if(res != FR_OK) { f_closedir(&dir); return R_ERROR; }  /* Break on error */
        else if(fno.fname[0] == 0) { break; }  /* Break on end of dir */
        if (fno.fname[0] == '.') { continue; }             /* Ignore dot entry */
#if _USE_LFN
        fn = *fno.lfname ? fno.lfname : fno.fname;
#else
        fn = fno.fname;
#endif

        ret = strtol(fn, NULL, 10);
        if(to == ALL || ret <= to) {
            if(f_unlink(fn) != FR_OK) { return R_ERROR; }
        }
    }
    f_closedir(&dir)
    
    return R_OK;
}

/*it downlinks files, it is accessed from large data transfer only*/
/*Higher level, it is used only for differentiatin from different file modes, large files and logs*/
OBC_returnStateTypedef mass_storage_downlink_api(MS_sid sid, MS_mode mode, uint32_t from, uint32_t to, uint8_t *buf, uint16_t *size, uint32_t *part) {

    OBC_returnStateTypedef res; 

    ASSERT(buf != NULL && size != NULL && part != NULL);
    REQUIRE(sid == SU_LOG || sid == EVENT_LOG || sid == FOTOS);

    if(sid == SU_LOG || sid == EVENT_LOG) { res = mass_storage_downlinkLogs(sid, mode, from, to, buf, size, part); }
    else if(sid == FOTOS) { res = mass_storage_downlinkLargeFile(sid, from, buf, size, part); }
    else { return R_ERROR; }

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
OBC_returnStateTypedef mass_storage_downlinkLogs(MS_sid sid, MS_mode mode, uint32_t from, uint32_t to, uint8_t *buf, uint32_t *size, uint16_t *part) {

    uint16_t byteswritten;
    uint8_t path[MS_MAX_PATH];

    ASSERT(buf != NULL && size != NULL && size != NULL && part != NULL);
    ASSERT(*size > 0);
    REQUIRE(sid == SU_LOG || sid == EVENT_LOG);
    REQUIRE(mode < LAST_MODE);

    /*cp dir belonging to sid*/
    if(sid == SU_LOG) { strncp(path, MS_SU_LOG, MS_MAX_PATH); }
    else if(sid == EVENT_LOG) { strncp(path, EVENT_LOG, MS_MAX_PATH); }

    /*Find first file, new search*/
    if(*part == 0) {
        OBC_returnStateTypedef res;

        if(mode == ALL) { res = mass_storage_findLog(sid, part); } 
        else if(mode == TO) { res = mass_storage_findLog(sid, part); }
        else if(mode == BETWEEN) { 
            *part = from
            res = mass_storage_findLog(sid, part); 
        } 
        else if(mode == SPECIFIC ) { res = R_OK; }

        if(res == R_EOT) { return R_EOT; }
        else if(res != R_OK) { return R_ERROR; }
    } 

    ASSERT(*part > 0);

    sprintf(path, "%s//%d", path, part); }

    if(f_open(&fp, path, FA_OPEN_ALWAYS | FA_READ) != FR_OK) { return R_ERROR; }

    uint16_t len = f_size(fp);
    ASSERT(len < (*part) * MS_FILE_SECTOR) { f_close(&fp); return R_ERROR; }

    res = f_read(&fp, buf, *size, (void *)&byteswritten);
    if((byteswritten == 0) || (res != FR_OK)) {  f_close(&fp); return R_ERROR; } 
    *size = byteswritten;

    f_close(&fp);

    if((mode == ALL || mode == TO || mode == BETWEEN) && mass_storage_findLog(sid, part) == R_EOT) { return R_EOT; } 
    if((mode == to || mode == BETWEEN) && *part >= TO) { return R_EOT; }

    return R_OK;
}

/*it downlinks fotos, it is accessed from large data transfer only*/
/*valid sid is only stores that have fotos.*/
/*file: is the file to be downlinked*/
/*size: its the size of the buffer, in return it stores the actual bytes written in the buffer */
OBC_returnStateTypedef mass_storage_downlinkLargeFile(MS_sid sid, uint32_t file, uint8_t *buf, uint16_t *size, uint32_t *part) {

    FIL fp;
    uint16_t byteswritten;
    uint8_t path[MS_MAX_PATH];

    ASSERT(buf != NULL && size != NULL && part != NULL);
    REQUIRE(sid == FOTOS);

    sprintf(path,"%s//%d" MS_FOTOS, file);

    if(f_open(&fp, path, FA_OPEN_ALWAYS | FA_READ) != FR_OK) { return R_ERROR; }

    uint16_t len = f_size(fp);
    ASSERT(len < (*part) * MS_FILE_SECTOR) { f_close(&fp); return R_ERROR; }
    
    res = f_lseek(fp, part* MS_FILE_SECTOR);

    res = f_read(&fp, buf, *size, (void *)&byteswritten);
    if((byteswritten == 0) || (res != FR_OK)) { f_close(&fp); return R_ERROR; } 

    f_close(&fp);

    if(*size > byteswritten) { *size = byteswritten; return R_EOT; } 
    *size = byteswritten;

    return R_OK;
}

/*it stores files, it is accessed from large data transfer for su scripts & fotos and from for logs*/
/*Higher level, it is used only for differentiatin from different file modes, large files and logs*/
OBC_returnStateTypedef mass_storage_store_api(MS_sid sid, MS_mode mode, uint8_t *buf, uint16_t *size, uint32_t part) {

    OBC_returnStateTypedef res; 

    ASSERT(buf != NULL && && size != NULL && part != NULL);
    REQUIRE(sid == SU_LOG || sid == EVENT_LOG || sid == FOTOS);

    if(sid == SU_LOG || sid == EVENT_LOG) { res = mass_storage_storeLogs_api(sid, buf, size); }
    else if(sid == FOTOS || sid =< SU_SCRIPT_7); { res = mass_storage_storeLargeFile_api(sid, mode, buf, size, part); }
    else { return R_ERROR; }

    return res;

}


OBC_returnStateTypedef mass_storage_storeLargeFile(MS_sid sid, MS_mode mode, uint8_t *buf, uint16_t *size, uint32_t part) {

    FILINFO fno;
    FIL fp;
    BYTE flags;
    uint16_t byteswritten;
    uint8_t path[MS_MAX_PATH];

    ASSERT(buf != NULL && && size != NULL);
    ASSERT(*size > 0);
    REQUIRE(sid == FOTOS || sid =< SU_SCRIPT_7);

    if(sid == FOTOS) { strncp(path, MS_TMP_FOTOS, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_1) { strncp(path, MS_TMP_SU_SCRIPT_1, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_2) { strncp(path, MS_TMP_SU_SCRIPT_2, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_3) { strncp(path, MS_TMP_SU_SCRIPT_3, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_4) { strncp(path, MS_TMP_SU_SCRIPT_4, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_5) { strncp(path, MS_TMP_SU_SCRIPT_5, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_6) { strncp(path, MS_TMP_SU_SCRIPT_6, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_7) { strncp(path, MS_TMP_SU_SCRIPT_7, MS_MAX_PATH); }

    if(*part == 0) { flags = (FA_CREATE_ALWAYS | FA_WRITE); } 
    else { flags = (FA_CREATE_ALWAYS | FA_WRITE); }

    if(f_open(&fp, path, flags) != FR_OK) { return R_ERROR; }

    uint16_t len = f_size(fp);
    ASSERT(len < (part) * FILE_SIZE) { f_close(&fp); return R_ERROR; }
    res = f_lseek(fp, len);

    res = f_write(&fp, buf, *size, (void *)&byteswritten);
    if((byteswritten == 0) || (res != FR_OK)) { f_close(&fp); return R_ERROR; }

    f_close(&fp);

    if(mode == LAST_PART) {

        if(sid == FOTOS) {

            mass_storage_getFileName(path);
            if(f_rename(path, MS_TMP_FOTOS) != FR_OK) { return R_ERROR; } 
        } else if(sid == SU_SCRIPT_1) { 
            if(mass_storage_su_checksum_api(TMP_SU_SCRIPT_1) != R_OK) { return R_ERROR; } 
            if(f_rename(MS_SU_SCRIPT_1, MS_TMP_SU_SCRIPT_1) != FR_OK) { return R_ERROR; }
        } else if(sid == SU_SCRIPT_2) { 
            if(mass_storage_su_checksum_api(TMP_SU_SCRIPT_2) != R_OK) { return R_ERROR; } 
            if(f_rename(MS_SU_SCRIPT_2, MS_TMP_SU_SCRIPT_2) != FR_OK) { return R_ERROR; }
        } else if(sid == SU_SCRIPT_3) { 
            if(mass_storage_su_checksum_api(TMP_SU_SCRIPT_3) != R_OK) { return R_ERROR; } 
            if(f_rename(MS_SU_SCRIPT_3, MS_TMP_SU_SCRIPT_3) != FR_OK) { return R_ERROR; }
        }  else if(sid == SU_SCRIPT_4) { 
            if(mass_storage_su_checksum_api(TMP_SU_SCRIPT_4) != R_OK) { return R_ERROR; } 
            if(f_rename(MS_SU_SCRIPT_4, MS_TMP_SU_SCRIPT_4) != FR_OK) { return R_ERROR; }
        }  else if(sid == SU_SCRIPT_5) { 
            if(mass_storage_su_checksum_api(TMP_SU_SCRIPT_5) != R_OK) { return R_ERROR; } 
            if(f_rename(MS_SU_SCRIPT_5, MS_TMP_SU_SCRIPT_5) != FR_OK) { return R_ERROR; }
        }  else if(sid == SU_SCRIPT_6) { 
            if(mass_storage_su_checksum_api(TMP_SU_SCRIPT_6) != R_OK) { return R_ERROR; } 
            if(f_rename(MS_SU_SCRIPT_6, MS_TMP_SU_SCRIPT_6) != FR_OK) { return R_ERROR; }
        }  else if(sid == SU_SCRIPT_7) { 
            if(mass_storage_su_checksum_api(TMP_SU_SCRIPT_7) != R_OK) { return R_ERROR; } 
            if(f_rename(MS_SU_SCRIPT_7, MS_TMP_SU_SCRIPT_7) != FR_OK) { return R_ERROR; }
        }  
    }

    return R_OK;
}

OBC_returnStateTypedef mass_storage_storeLogs(MS_sid sid, uint8_t *buf, uint16_t *size) {

    FILINFO fno;
    FIL fp;
    OBC_returnStateTypedef res; 

    uint16_t byteswritten;
    uint8_t path[MS_MAX_PATH];

    ASSERT(buf != NULL && && size != NULL);
    ASSERT(*size > 0);

    if(mass_storage_getLog(sid, path) != R_OK) { return R_ERROR; }

    if(f_open(&fp, path, FA_OPEN_ALWAYS | FA_WRITE) != FR_OK) { return R_ERROR; }

    /*appending*/
    res = f_lseek(fp, f_size(fp));
    /* Write data to the text file */
    res = f_write(&fp, buf, *size, (void *)&byteswritten);
    if((byteswritten == 0) || (res != FR_OK)) {  f_close(&fp); return R_ERROR; } 
    /* Close the open text file */
    f_close(&fp);

    return R_OK;
}


OBC_returnStateTypedef mass_storage_report_api(MS_sid sid, uint8_t *buf, uint16_t *size, uint32_t *iter) {

    DIR dir;
    uint8_t temp_fn[20];
    uint32_t time_temp = 0;
    uint8_t start_flag = 0;
    uint8_t path[MS_MAX_PATH];

    ASSERT(buf != NULL && size != NULL && iter != NULL);
    REQUIRE(*size == 0);
    REQUIRE(sid == SU_LOG || sid == EVENT_LOG || sid == FOTOS);

    if(sid == SU_LOG) { strncp(path, MS_SU_LOG, MS_MAX_PATH); }
    else if(sid == EVENT_LOG) { strncp(path, EVENT_LOG, MS_MAX_PATH); }
    else if(sid == FOTOS) { strncp(path, MS_FOTOS, MS_MAX_PATH); }

    if(*iter == 0) { start_flag = 1; }
    else { start_flag = 0; }

    if (f_opendir(&dir, path) != FR_OK) { return R_ERROR; }
    for (uint16_t i = 0; i < MS_MAX_FILES; i++) {

        res = f_readdir(&dir, &fno);                   /* Read a directory item */
        if(res != FR_OK) { f_closedir(&dir); return R_ERROR; }  /* Break on error */
        else if(fno.fname[0] == 0) { f_closedir(&dir); return R_EOT;}  /* Break on end of dir */
        if (fno.fname[0] == '.') { continue; }             /* Ignore dot entry */
#if _USE_LFN
        temp_fn = *fno.lfname ? fno.lfname : fno.fname;
#else
        temp_fn = fno.fname;
#endif

        ret = strtol(temp_fn, NULL, 10);
        if(start_flag = 0 && *iter == ret) { start_flag = 1; }
        if(start_flag == 1) {

            cnv32_8(ret, buf[(*size)], buf[(*size)+1], buf[(*size)+2], buf[(*size)+3]);
            *size += sizeof(uint32_t);

            //if(f_stat(fn, &fno) != FR_OK) { f_closedir(&dir) return R_ERROR; } 

            //(*fcount)++;
            //*fsize += fno.fsize;    

            if(*size >= MS_MAX_LOG_FILE_SIZE) {
                *iter = ret;
                f_closedir(&dir)
                return R_OK; 
            }
        } 

    }
    f_closedir(&dir)
 
    REQUIRE(*size != 0) { return R_ERROR; }

    return R_OK;
}

OBC_returnStateTypedef mass_storage_su_checksum_api(MS_sid sid) {

    uint8_t path[MS_MAX_PATH];
    uint16_t byteswritten;

    uint16_t sum1 = 0;
    uint16_t sum2 = 0;

    uint8_t c = 0;

    REQUIRE(sid < LAST_SID;

    if(sid == SU_SCRIPT_1) { strncp(path, MS_SU_SCRIPT_1, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_2) { strncp(path, MS_SU_SCRIPT_2, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_3) { strncp(path, MS_SU_SCRIPT_3, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_4) { strncp(path, MS_SU_SCRIPT_4, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_5) { strncp(path, MS_SU_SCRIPT_5, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_6) { strncp(path, MS_SU_SCRIPT_6, MS_MAX_PATH); }
    else if(sid == SU_SCRIPT_7) { strncp(path, MS_SU_SCRIPT_7, MS_MAX_PATH); }
    else if(sid == TMP_SU_SCRIPT_1) { strncp(path, MS_TMP_SU_SCRIPT_1, MS_MAX_PATH); }
    else if(sid == TMP_SU_SCRIPT_2) { strncp(path, MS_TMP_SU_SCRIPT_2, MS_MAX_PATH); }
    else if(sid == TMP_SU_SCRIPT_3) { strncp(path, MS_TMP_SU_SCRIPT_3, MS_MAX_PATH); }
    else if(sid == TMP_SU_SCRIPT_4) { strncp(path, MS_TMP_SU_SCRIPT_4, MS_MAX_PATH); }
    else if(sid == TMP_SU_SCRIPT_5) { strncp(path, MS_TMP_SU_SCRIPT_5, MS_MAX_PATH); }
    else if(sid == TMP_SU_SCRIPT_6) { strncp(path, MS_TMP_SU_SCRIPT_6, MS_MAX_PATH); }
    else if(sid == TMP_SU_SCRIPT_7) { strncp(path, MS_TMP_SU_SCRIPT_7, MS_MAX_PATH); }
    else { return R_ERROR; }

    if(f_open(&fp, path, FA_OPEN_ALWAYS | FA_READ) != FR_OK) { return R_ERROR; }

    for(uint16_t i = 0; i < MS_MAX_SU_FILE_SIZE; i++) {
        
        res = f_read(&fp, &c, 1, (void *)&byteswritten);
        if(res != FR_OK) { f_close(&fp); return R_ERROR; } 
        if((byteswritten == 0) {
            
            if(((sum2 << 8) | sum1) == 0)  { return R_OK; }
            else {  return R_CRC_ERROR; }
        } 

        sum1 = (sum1 + c) % 255; 
        sum2 = (sum2 + sum1) % 255;
    }

    f_close(&fp);

}

OBC_returnStateTypedef mass_storage_getLog(MS_sid sid, uint8_t *fn) {

    ASSERT(fn != NULL);
    REQUIRE(sid == SU_LOG || sid == EVENT_LOG);


    if(sid == SU_LOG) { sprintf(fn, "%s//%d", MS_SU_LOG, time.now()); return R_OK; }

    if(MS_data.ev_temp_log == 0) { 
        MS_data.ev_temp_log = time.now(); 
        sprintf(fn, "%s//%d", MS_EVENT_LOG, MS_data.ev_temp_log); 
        return R_OK; 
    }

    sprintf(fn, "%s//%d", MS_EVENT_LOG, MS_data.ev_temp_log); 

    if(f_stat(fn, &fno) != FR_OK) { return R_ERROR; } 

    if(fno.fsize >= MS_MAX_FILE_SIZE) { 
        MS_data.ev_temp_log = time.now(); 
        sprintf(fn, "%s//%d", MS_EVENT_LOG, MS_data.ev_temp_log); 
        return R_OK; 
    }

    return R_OK;
}

OBC_returnStateTypedef mass_storage_findLog(MS_sid sid, uint32_t *fn) {

    DIR dir;
    uint8_t temp_fn[20];
    uint32_t time_temp = 0;
    uint8_t start_flag = 0;
    uint32_t min = 0;

    REQUIRE(sid == SU_LOG || sid == EVENT_LOG);
    ASSERT(fn != NULL);

    if(sid == SU_LOG) { strncp(path, MS_SU_LOG, MS_MAX_PATH); }
    else if(sid == EVENT_LOG) { strncp(path, MS_EVENT_LOG, MS_MAX_PATH); }

    ASSERT(*size != 0) { return R_ERROR; }

    if (f_opendir(&dir, path) != FR_OK) { return R_ERROR; }
    for (uint16_t i = 0; i < MS_MAX_FILES; i++) {

        res = f_readdir(&dir, &fno);                   /* Read a directory item */
        if(res != FR_OK) { f_closedir(&dir); return R_ERROR; }  /* Break on error */
        else if(fno.fname[0] == 0) { break; }  /* Break on end of dir */
         if (fno.fname[0] == '.') continue;             /* Ignore dot entry */
#if _USE_LFN
        temp_fn = *fno.lfname ? fno.lfname : fno.fname;
#else
        temp_fn = fno.fname;
#endif

        ret = strtol(temp_fn, NULL, 10);
        if(fn == 0 && min == 0) { min = ret; }
        else if(fn == 0 && ret < min) { min = ret; } 
        else if(fn != 0 && fn < ret && min == 0) { min = ret; }
        else if(fn != 0 && fn < ret && ret < min) { min = ret; } 

    }
    f_closedir(&dir);
 
    if(min == 0) { return R_EOT; }

    *fn = min;

    return R_OK;
}

OBC_returnStateTypedef mass_storage_init() {

    MS_data.ev_temp_log = 0;
    if(f_mount(&test, SD_Path, 0) != FR_OK) { return R_ERROR; }
    return R_OK;
}

OBC_returnStateTypedef mass_storage_getFileName(uint8_t *fn) {

    sprintf(fn,"%d", time.now());
    return R_OK;
}
