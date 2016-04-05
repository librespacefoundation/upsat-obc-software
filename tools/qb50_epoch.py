import datetime
import sys


tsecs = [0 for x in range(13)] 

print "const uint32_t cnv_QB50[MAX_YEAR][MONTHS] = {"

for yr in range(1, 21):
    for mt in range(1, 13):

        t = datetime.datetime(2000 + yr,mt,1)
        secs = (t-datetime.datetime(2000,1,1)).total_seconds()
        tsecs[mt] = secs

    print "                                  { %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d }," % (tsecs[1], tsecs[2], tsecs[3], tsecs[4], tsecs[5], tsecs[6], tsecs[7], tsecs[8], tsecs[9], tsecs[10], tsecs[11], tsecs[12])

print "                                  }"