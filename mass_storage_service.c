#include "mass_storage_service.h"

OBC_returnStateTypedef mass_storage_app(tc_tm_pkt *pkt) {

    sid = pkt->data[0];

    if(pkt->ser_subtype == DISABLE) {

    } else if(pkt->ser_subtype == DELETE) {

        to = pkt->data[1];
        to = pkt->data[2];
        to = pkt->data[3];
        to = pkt->data[4];

        mass_storage_delete_api(sid, to);

    } else if(pkt->ser_subtype == REPORT) {

        large_data_startReport_api(sid, pkt);

    } else if(pkt->ser_subtype == DOWNLINK) {

        mode = pkt->data[1];

        from = pkt->data[2];
        from = pkt->data[3];
        from = pkt->data[4];
        from = pkt->data[5];

        to = pkt->data[6];
        to = pkt->data[7];
        to = pkt->data[8];
        to = pkt->data[9];

        large_data_startDownlink_api(sid, mode, from, to, pkt);

    } else { return R_ERROR; }

    return R_OK;
}

OBC_returnStateTypedef mass_storage_delete_api(uint8_t sid, uint32_t to) {

    DIR dir;
    uint8_t temp_fn[20];
    uint32_t time_temp = 0;

    if(sid == SU_LOG) {
        path = temp;
    }

    if (f_opendir(&dir, path) != FR_OK) { return R_ERROR; }
    i = strlen(path);
    for (;;) {

        res = f_readdir(&dir, &fno);                   /* Read a directory item */
        if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
        if (fno.fname[0] == '.') continue;             /* Ignore dot entry */
#if _USE_LFN
        fn = *fno.lfname ? fno.lfname : fno.fname;
#else
        fn = fno.fname;
#endif

        ret = strtol(fn, NULL, 10);
        if(to == DELETE_ALL || ret <= to) {
            sprintf(temp_fn, "%s%s", path, fn);
            if(f_unlink(temp_fn) != FR_OK) { return R_ERROR; }
        }
    }
    f_closedir(&dir)
    
    return R_OK;
}

OBC_returnStateTypedef mass_storage_downlinkLogs_api(uint8_t sid, uint8_t mode, uint32_t from, uint32_t to, uint8_t *buf, uint16_t *size, uint8_t *fn) {

    if(sid != LOGS) { return R_ERROR; }

    /*cp dir belonging to sid*/
    if(sid ==) { strncp(file,path, MAX_PATH); }

    /*Find first file, new search*/
    if(fn == NULL) {
        if(mode == ALL) { mass_storage_findLog(sid, fn); } 
        else if(mode == TO) { mass_storage_findLog(sid, fn); }
        else if(mode == BETWEEN) { 
            strncp(fn, from, MAX_FNAME); 
            mass_storage_findLog(sid, fn); 
        } 
        else if(mode == SPECIFIC ) { strncp(file, from, MAX_FNAME); }
    }

    if(f_open(&fp, file, FA_OPEN_ALWAYS | FA_READ) != FR_OK) { return R_ERROR; }

    if(len < (*part) * FILE_SIZE) { 
        f_close(&fp);
        return R_ERROR; 
    }

    res = f_read(&fp, buf, *size, (void *)&byteswritten);
    if((byteswritten == 0) || (res != FR_OK)) { return R_ERROR; } 
    *size = byteswritten;

    f_close(&fp);

    if((mode == ALL || mode == TO || mode == BETWEEN) && mass_storage_findLog(sid, fn) == R_EOT) { return R_EOT; } 
    if(mode == to || mode == BETWEEN) { 
        ret = strtol(fn, NULL, 10);
        if(ret >= TO) { return R_EOT; }
    }

    return R_OK;
}

OBC_returnStateTypedef mass_storage_downlinkLargeFile_api(uint8_t sid, uint8_t mode, uint32_t file, uint8_t *buf, uint16_t *size, uint16_t *part) {

    if(sid != FOTO) { return R_ERROR; }

    path + file

    if(f_open(&fp, file, FA_OPEN_ALWAYS | FA_READ) != FR_OK) { return R_ERROR; }

    if(len < (*part) * FILE_SIZE) { return R_ERROR; }
    
    res = f_lseek(fp, part* FILE_SIZE);

    res = f_read(&fp, buf, *size, (void *)&byteswritten);
    if((byteswritten == 0) || (res != FR_OK)) { return R_ERROR; } 
    *size = byteswritten;

    if eof return R_EOT

    f_close(&fp);
    return R_OK;
}

OBC_returnStateTypedef mass_storage_storeLargeFile_api(uint8_t sid, uint32_t *mode, uint8_t *buf, uint16_t *size, uint16_t *part) {

    FILINFO fno;
    FIL fp;

    if(sid == FOTO || sid == SU_SCRIPT) {

        BYTE flags;

        if(*part == 0) { flags = (FA_CREATE_ALWAYS | FA_WRITE); } 
        else { flags = (FA_CREATE_ALWAYS | FA_WRITE); }

        if(f_open(&fp, file, flags) != FR_OK) { return R_ERROR; }

        len = f_size(fp);
        if(len < (*part) * FILE_SIZE) { return R_ERROR; }
        res = f_lseek(fp, len);

        res = f_write(&fp, buf, *size, (void *)&byteswritten);
        if((byteswritten == 0) || (res != FR_OK)) { return R_ERROR; }

        f_close(&fp);

        if(*mode == LAST_PART) 
        { 
            if(sid == SU_SCRIPT) { 
                check checksum
            }
            if(f_rename(temp_fn, file) != FR_OK) { return R_ERROR; } 
        }
    }
    return R_OK;
}

OBC_returnStateTypedef mass_storage_storeLog_api(uint8_t sid, uint32_t *mode, uint8_t *buf, uint16_t *size, uint16_t *part) {

    FILINFO fno;
    FIL fp;

    if(sid == LOGS) {

        if(f_stat(SU_LOG_TEMP, &fno) != FR_OK) { return R_ERROR; } 

        if(fno.fsize >= max_file_size) {

            uint8_t temp_fn[MAX_NAME_LEN];

            mass_storage_getFileName(temp_fn);
            if(f_rename(temp_fn, file) != FR_OK) { return R_ERROR; } 
        }

        if(f_open(&fp, file, FA_OPEN_ALWAYS | FA_WRITE) != FR_OK) { return R_ERROR; }

        res = f_lseek(fp, f_size(fp));
        /* Write data to the text file */
        res = f_write(&fp, buf, *size, (void *)&byteswritten);
        if((byteswritten == 0) || (res != FR_OK)) { return R_ERROR; } 
        /* Close the open text file */
        f_close(&fp);

    }
    return R_OK;
}
//add temp file
OBC_returnStateTypedef mass_storage_report_api(uint8_t sid, uint8_t *buf, uint16_t *size, uint8_t *fn) {

    DIR dir;
    uint8_t temp_fn[20];
    uint32_t time_temp = 0;
    uint8_t start_flag = 0;

    if(sid == SU_LOG) {
        path = temp;
    }

    if(*size != 0) { return R_ERROR; }

    if(*fn == NULL) { start_flag = 1; }
    else { start_flag = 0; }

    if (f_opendir(&dir, path) != FR_OK) { return R_ERROR; }
    i = strlen(path);
    for (;;) {

        res = f_readdir(&dir, &fno);                   /* Read a directory item */
        if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
        if (fno.fname[0] == '.') continue;             /* Ignore dot entry */
#if _USE_LFN
        temp_fn = *fno.lfname ? fno.lfname : fno.fname;
#else
        temp_fn = fno.fname;
#endif

        if(start_flag == 1) {
            ret = strtol(temp_fn, NULL, 10);
            buf((*size)++) = ret;            

            if(*size >= MAX) { 
                strncp(temp_fn, fn, MAX_FNAME);
                f_closedir(&dir)
                return R_EOT; 
            }

        } else if(strncmp(temp_fn,fn, MAX_FNAME) == 0) {
            start_flag = 1;
        }

    }
    f_closedir(&dir)
 
    if(*size == 0) { return R_ERROR; }

    return R_OK;
}

OBC_returnStateTypedef mass_storage_findLog(sid, fn) {

    DIR dir;
    uint8_t temp_fn[20];
    uint32_t time_temp = 0;
    uint8_t start_flag = 0;
    uint32_t min = 0;

    if(sid == SU_LOG) {
        path = temp;
    }

    ASSERT(*size != 0) { return R_ERROR; }

    for (;;) {

        res = f_readdir(&dir, &fno);                   /* Read a directory item */
        if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
        if (fno.fname[0] == '.') continue;             /* Ignore dot entry */
#if _USE_LFN
        temp_fn = *fno.lfname ? fno.lfname : fno.fname;
#else
        temp_fn = fno.fname;
#endif

        ret = strtol(temp_fn, NULL, 10);
        if(fn == NULL && min == 0) { min = ret; }
        else if(fn == NULL && ret < min) { min = ret; } 
        else if(fn != NULL && fn < ret && min == 0) { min = ret; }
        else if(fn != NULL && fn < ret && ret < min) { min = ret; } 

    }
    f_closedir(&dir)
 
    *fn = min;

    ASSERT(min != 0);

    return R_OK;
}

OBC_returnStateTypedef mass_storage_init() {

    if(f_mount(&test, SD_Path, 0) != FR_OK) { return R_ERROR; }
    return R_OK;
}

OBC_returnStateTypedef mass_storage_getFileName(uint8_t *fn) {

    sprintf(fn,"%d", time.now());
    return R_OK;
}
