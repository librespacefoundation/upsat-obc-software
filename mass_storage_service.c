#include "mass_storage_service.h"

OBC_returnStateTypedef mass_storage_app(tc_tm_pkt *pkt) {

    sid = pkt->data[0];

    if(pkt->ser_type == TC_MASS_STORAGE_SERVICE &&  pkt->ser_subtype == DISABLE) {

    } else if(pkt->ser_type == TC_MASS_STORAGE_SERVICE && pkt->ser_subtype == DELETE) {

        to = pkt->data[1];
        to = pkt->data[2];
        to = pkt->data[3];
        to = pkt->data[4];

        mass_storage_delete_api(sid, to,);

    } else if(pkt->ser_type == TC_MASS_STORAGE_SERVICE && pkt->ser_subtype == REPORT) {

        large_data_startReport_api(sid, pkt);

    } else if(pkt->ser_type == TC_MASS_STORAGE_SERVICE && pkt->ser_subtype == DOWNLINK) {

        mode = pkt->data[1];

        from = pkt->data[2];
        from = pkt->data[3];
        from = pkt->data[4];
        from = pkt->data[5];

        to = pkt->data[6];
        to = pkt->data[7];
        to = pkt->data[8];
        to = pkt->data[9];

        large_data_start_api(sid, mode, from, to, pkt);

    } else {
        return R_ERROR;
    }

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

OBC_returnStateTypedef mass_storage_downlinkLogs_api(uint8_t sid, uint8_t mode, uint32_t from, uint32_t to, uint8_t *buf, uint16_t *size, uint16_t *part) {

    if(sid == LOGS) { 
        if(*part == 0) {}
        else {}


    } else if(sid == FOTO || sid == SU_SCRIPT) {

    }

    if(mode == ALL) {

    } else if(mode == BETWEEN) {

    } else if(mode == TO) {

    } else if(mode == SPECIFIC) {

    }

    if(f_open(&fp, file, FA_OPEN_ALWAYS | FA_READ) != FR_OK) { return R_ERROR; }

    res = f_read(&fp, buf, *size, (void *)&byteswritten);
    if((byteswritten == 0) || (res != FR_OK)) { return R_ERROR; } 

    if(sid == FOTO || sid == SU_SCRIPT) {
        if(len < (*part) * FILE_SIZE) { return R_ERROR; }
        res = f_lseek(fp, part* FILE_SIZE);
    }


    f_close(&fp);
    return R_OK;
}

OBC_returnStateTypedef mass_storage_downlinkLargeFile_api(uint8_t sid, uint8_t mode, uint32_t from, uint32_t to, uint8_t *buf, uint16_t *size, uint16_t *part) {

    if(sid == LOGS) { 
        if(*part == 0) {}
        else {}


    } else if(sid == FOTO || sid == SU_SCRIPT) {

    }

    if(mode == ALL) {

    } else if(mode == BETWEEN) {

    } else if(mode == TO) {

    } else if(mode == SPECIFIC) {

    }

    if(f_open(&fp, file, FA_OPEN_ALWAYS | FA_READ) != FR_OK) { return R_ERROR; }

    res = f_read(&fp, buf, *size, (void *)&byteswritten);
    if((byteswritten == 0) || (res != FR_OK)) { return R_ERROR; } 

    if(sid == FOTO || sid == SU_SCRIPT) {
        if(len < (*part) * FILE_SIZE) { return R_ERROR; }
        res = f_lseek(fp, part* FILE_SIZE);
    }


    f_close(&fp);
    return R_OK;
}

OBC_returnStateTypedef mass_storage_getFileContents(uint8_t sid, uint32_t file, uint8_t *buf, uint16_t *size, uint16_t *part) {

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

        if(*mode == LAST_PART) { if(f_rename(temp_fn, file) != FR_OK) { return R_ERROR; } }
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

OBC_returnStateTypedef mass_storage_report_api(uint8_t sid, uint8_t *buf, uint16_t *size, uint8_t *fn) {

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
        buf((*size)++) = ret;
        if(cnt++ >= MAX) { 
            strncp(MS_data.report_fn, fn, MAX); 
            f_closedir(&dir)
            return R_EOT; 
        }
    }
    f_closedir(&dir)
    
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

