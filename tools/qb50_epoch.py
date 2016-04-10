import datetime
import sys


tsecs = [0 for x in range(32)] 

print "const uint32_t cnv_QB50_YM[MAX_YEAR][13] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },"

for yr in range(1, 21):
    for mt in range(1, 13):

        t = datetime.datetime(2000 + yr,mt,1)
        secs = (t-datetime.datetime(2000,1,1)).total_seconds()
        tsecs[mt] = secs

    print "                                  { 0, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d }," % (tsecs[1], tsecs[2], tsecs[3], tsecs[4], tsecs[5], tsecs[6], tsecs[7], tsecs[8], tsecs[9], tsecs[10], tsecs[11], tsecs[12])

print "                                  }"

print "const uint32_t cnv_QB50_D[32] = { 0,"

for d in range(1, 32):
    print "                                  %d," % (d*24*60*60)

print "                                }"

print "const uint32_t cnv_QB50_H[25] = { 0,"

for d in range(1, 25):
    print "                                  %d," % (d*60*60)

print "                                }" 