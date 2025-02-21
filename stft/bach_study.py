import scipy.io.wavfile
import stft
import matplotlib.pyplot  as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy  as np
import sys,os
import seaborn as sbn
sbn.set_style("whitegrid")


def padding(x, windowSize):
    #iinitialy add the boundaries of the signal,
    #so create an array of windowSize/2 zeroes that will be added ad the top and
    #bottom of the signal
    #final singal [ 0,0,0,0,0,.....  x ..... 0,0,0,0,0,0] as many 0 as windowSize/2
    #on both sides
    zeros = np.zeros(int(windowSize/2), dtype=x.dtype)
    x = np.concatenate((zeros, x, zeros), axis=-1)
    #then
    # Pad to integer number of windowed segments
    # I.e make x.shape[-1] = nperseg + (nseg-1)*nstep, with integer nseg
    nadd = (-(x.shape[-1]-windowSize) % int(windowSize/2)) % int(windowSize)
    zeros_shape = list(x.shape[:-1]) + [int(nadd)]
    x = np.concatenate((x, np.zeros(zeros_shape)), axis=-1)
    return x


#MAIN#
#read the input file
rate, audData = scipy.io.wavfile.read("../wav/partita2-1.wav")
#extract the info
length = int(len(audData)/10)#audData.shape[0] # this is the number of samples,
#if you divide length by the rateyou get the length in seconds /rate
channel1 = audData[:,0][0:length]
print("Total number of elements in the signarl %d"%length);
#convert in double format
channel1 = np.double(channel1)
#channel2 = audData[:,1]
windowSize = 1024#8192 #length of the window to analyse with STFT
hopSize = 1024#8192  #hopsize between windows

#pad the signal based on the windowSize
print("Padding signal...")
signal = padding(channel1, windowSize)
print("Dimension of the signal after boundary and padding %d\n"% len(signal))

#UNCOMMENT if you want to analyse just a piece of the entire wav
#save_wav = audData[:,0][0:length]
#scipy.io.wavfile.write("study.wav",rate,save_wav)
#window_start = 3*12288
#window_end = window_start + 12288
#save_wav = audData[:,0][window_start:window_end]
#scipy.io.wavfile.write("100th.wav",rate,save_wav)

#compute the STFT
print("STFT...")
#magnitude,frequencies = stft.play(signal, rate , windowSize, hopSize)
#print(type(frequencies))
#print(frequencies.shape)

#signal = [1,2,3,4,5,6,7,8,9,10]
#signal = np.asarray(signal)
#signal = padding(signal,8)
#print(signal)
#rate = 1
#windowSize = 8
#hopSize = 8
magnitude, frequencies = stft.play(signal, rate , windowSize, hopSize)
#print(magnitude.shape)
print("Roll magnitude values")
magnitude = np.rollaxis(magnitude, -1, -2)
#print(magnitude.shape)
start_time = int(windowSize/2)
stop_time  = int(magnitude.shape[-1] - windowSize/2 +1)

time = np.arange(windowSize/2, signal.shape[-1] - windowSize/2 + 1,
                 windowSize - (windowSize/2))/float(rate)

time -= (windowSize/2)/ rate
#for i in range(0,10):#
#    print(magnitude[i])
#PLOT
#x-axis for  the data
'''
x_axis_ref = np.linspace(0, len(magnitude),len(magnitude))
#colors andfigure
colors= sbn.color_palette()
col_ref = sbn.color_palette("cubehelix", 8)
fig = plt.figure(figsize=[15,10])
ax = fig.add_subplot(111)
#axis plot
ax.plot(x_axis_ref,np.abs(magnitude))
ax.xaxis.set_tick_params(labelsize=30)
ax.yaxis.set_tick_params(labelsize=30)
ax.set_ylabel(r"Freq",fontsize=30)
ax.set_xlabel(r"ticks",fontsize=30)
#lgd = ax.legend(loc="best", fontsize=30)#ax.legend(loc='upper center', bbox_to_anchor=(0.5, 1.1),
          #fancybox=True, shadow=True, ncol=3,fontsize=30)

plt.tight_layout()
plt.savefig("power_spectrum.pdf")#,bbox_extra_artists=(lgd,), bbox_inches='tight')
plt.clf()
'''
#plot frequenceis
print("plotting frequencies...")
fig = plt.figure(figsize=[15,10])
ax = fig.add_subplot(111)
#ax.imshow(np.abs(magnitude))
print("Computing magnitude in dB...")
magnitude = 20*np.log10(np.abs(magnitude))
magnitude = np.clip(magnitude, -40, 200)
print("pcolormesh call")
print(type(magnitude))
print(magnitude.shape)
ax.pcolormesh(time, frequencies, magnitude, vmin=0, vmax=0.5,cmap="bwr")
ax.set_ylabel(r"Frequency / Hz",fontsize=20)
ax.set_xlabel(r"Time / s",fontsize=20)
plt.tight_layout()
plt.savefig("frequencies.png")
