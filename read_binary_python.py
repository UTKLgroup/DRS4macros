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
nEvents = 10                        # Set the number of events you want to extract from the data file.
nChannels = 4                       # Number of channels of the device.
#
#   ==================================================================================================================

import struct as st
import numpy  as np

# RAW DATA ARRAYS
rawTimeBinWidth = np.empty([nEvents, nChannels + 1, 1024], dtype = float)
rawWaveformSet  = np.empty([nEvents, nChannels + 1, 1024], dtype = float)

with open(filename, "rb") as f:
    print "DATA FILE OPENING: " + filename + "."

    #==================================================================================================================
    # TIME INFORMATION FOR EACH CHANNEL
    word = f.read(4) # TIME HEADER
    word = f.read(4) # BOARD SERIAL NUMBER
    print "\tFOUND BOARD SERIAL " + ''.join(st.unpack('cc', word[0:2])) + str(st.unpack('h', word[2:4])[0]) + "."

    for iChannel in range(0, nChannels + 1):
        word = f.read(4) # CHANNEL NUMBER HEADER
        if (word[0] != 'C'):
            f.seek(-4,1)
            break
        for iTimeBinWidth in range(0, 1024):
            rawTimeBinWidth[:, iChannel, iTimeBinWidth] = st.unpack('f', f.read(4))[0] # TIME BIN WIDTH (1024 values in total)

    #==================================================================================================================
    # EVENT INFORMATION FOR EACH CHANNEL
    while (word != ''):
        word = f.read(4) # EVENT HEADER
        word = f.read(4) # EVENT SERIAL NUMBER
        evtNumber = st.unpack('i', word)[0] - 1
        print "\t\t- Event #" + str(evtNumber + 1) + "\t found."
        if (evtNumber + 1 >= nEvents):
            break
        f.read(16) # The next 16 bytes are time in YY/MM/DD:HH/MM/SS/MS format. They are not important.
        f.read(4) # BOARD SERIAL NUMBER
        f.read(4) # TRIGGER CELL
        for iChannel in range(0, nChannels + 1):
            word = f.read(4) # CHANNEL NUMBER HEADER (AGAIN)
            if (word[0] != 'C'):
                f.seek(-4,1)
                break
            word = f.read(4) # SCALER FOR THE CURRENT CHANNEL (in Hz)
            '''
            # Debug this shit!
            for iTimeBinWidth in range(0, 1024):
                vbv = st.unpack('h', f.read(2)) # VOLTAGE BIN VALUES
                rawWaveformSet[evtNumber, iChannel, iTimeBinWidth] = (vbv[0]/65536. - 0.5)
            '''
            for iTimeBinWidth in range(0, 511):
                vbv = st.unpack('h', f.read(2)) # VOLTAGE BIN VALUES
                rawWaveformSet[evtNumber, iChannel, 2*iTimeBinWidth] = (vbv[0]/65536. - 0.5)
                vbv = st.unpack('h', f.read(2)) # VOLTAGE BIN VALUES
                rawWaveformSet[evtNumber, iChannel, 2*iTimeBinWidth+1] = (vbv[0]/65536. - 0.5)

        # This piece of code is enough for the PSD. Good for now!
        # Fix time bug here, later!

# Debug mode
# print(rawTimeBinWidth)
# print(rawWaveformSet)
