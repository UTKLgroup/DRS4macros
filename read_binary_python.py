#!  /usr/bin/python
#   ==================================================================================================================
#   DRS4 dat file processing using Python.
#   Author: Dung Duc Phan (brianp.dung@utexas.edu).
#   Institution: The University of Texas at Austin.
#   Reference:
#       + https://www.psi.ch/drs/DocumentationEN/manual_rev50.pdf, at page 24-25, for encoding method.
#       + https://docs.python.org/2/library/struct.html for struct.unpack() method.
#
#   ==================================================================================================================
#
#   NOTICE: Following are few places in the code need to be changed before you run the code.
#
filename = "07-14-16-drs4run3.dat"  # Change the following line of code to change the dat file.
nEvents = 20                        # Set the number of events you want to extract from the data file.
nChannels = 3                       # Number of channels of the device.
#
#   ==================================================================================================================

import struct     as st
import numpy      as np
from matplotlib import pyplot as plt

TimeData = np.zeros([nEvents], dtype = np.ndarray)
VoltData = np.zeros([nEvents], dtype = np.ndarray)

with open(filename, "rb") as f:
    print "DATA FILE OPENING: " + filename + "."

    #==================================================================================================================
    # TIME INFORMATION FOR EACH CHANNEL
    word = f.read(4) # TIME HEADER
    word = f.read(4) # BOARD SERIAL NUMBER
    print "\tFOUND BOARD SERIAL " + ''.join(st.unpack('cc', word[0:2])) + str(st.unpack('h', word[2:4])[0]) + "."

    rawTimeBinWidth = np.zeros([nChannels + 1, 1024], dtype = float)
    for iChannel in range(0, nChannels + 1):
        word = f.read(4) # CHANNEL NUMBER HEADER
        if (word[0] != 'C'):
            f.seek(-4,1)
            break
        for iTimeBinWidth in range(0, 1024):
            rawTimeBinWidth[iChannel, iTimeBinWidth] = st.unpack('f', f.read(4))[0] # TIME BIN WIDTH (1024 values in total)

    #==================================================================================================================
    # EVENT INFORMATION FOR EACH CHANNEL
    # Loop over events
    TIME = np.zeros([nChannels + 1, 1024], dtype = float)
    while (word != ''):
        word = f.read(4) # EVENT HEADER
        word = f.read(4) # EVENT SERIAL NUMBER
        evtNumber = st.unpack('i', word)[0] - 1
        if (evtNumber + 1 > nEvents):
            break
        print "\t - Event #" + str(evtNumber + 1) + "\t found."
        f.read(16) # The next 16 bytes are time in YY/MM/DD:HH/MM/SS/MS format. They are not important.
        f.read(4) # BOARD SERIAL NUMBER
        word = f.read(4) # TRIGGER CELL
        triggerCell = st.unpack('h', word[2:4])[0]
        # Loop over each channel
        rawWaveformSet = np.zeros([nChannels + 1, 1024], dtype = float)
        for iChannel in range(0, nChannels + 1):
            word = f.read(4) # CHANNEL NUMBER HEADER (AGAIN)
            if (word[0] != 'C'):
                f.seek(-4,1)
                break
            '''
            # This WORD doesn't exist for the DRS4 rev. 5.
            word = f.read(4) # SCALER FOR THE CURRENT CHANNEL (in Hz)
            '''
            for iTimeBinWidth in range(0, 1024):
                vbv = st.unpack('H', f.read(2)) # VOLTAGE BIN VALUES
                rawWaveformSet[iChannel, iTimeBinWidth] = (float(vbv[0])/65536. - 0.5)
                for jTimeBinWidth in  range(0, iTimeBinWidth):
                    TIME[iChannel, iTimeBinWidth] += rawTimeBinWidth[iChannel, (jTimeBinWidth + triggerCell)%1024];
            # UNCOMMENT THE FOLLOWING LINE IN DEBUG MODE
            #print(rawWaveformSet[iChannel, 2])
        T1 = TIME[0, (1024 - triggerCell)%1024]
        for iChannel in range(1, nChannels + 1):
            T2 = TIME[iChannel, (1024 - triggerCell)%1024]
            DeltaT = T1 - T2
            for iADC in range(0, 1024):
                TIME[iChannel, iADC] += DeltaT

        # Saving information for each event
        TimeData[evtNumber] = np.copy(TIME)
        VoltData[evtNumber] = np.copy(rawWaveformSet)

# =====================================================================================================================
# NOTICE on units
#   + VoltData is in [V]
#   + TimeData is in [ns]
#
# NOTICE on how-to-use the data
#   + VoltData and TimeData are organized event-by-event. Each element is an array of size nC-by-1024, in which, nC is
#     the number of channels available when the data is taken.
# =====================================================================================================================

# Debug mode <VISUALIZATION>
#X = TimeData[0][0,:]
#Y = VoltData[0][0,:]
#plt.plot(X, Y, 'r')
#plt.show
#plt.savefig("test2.png")
