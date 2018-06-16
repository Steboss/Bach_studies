import scipy.io.wavfile
import stft
import matplotlib.pyplot  as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy  as np
import sys,os
import seaborn as sbn
sbn.set_style("whitegrid")

#read the input file
rate, audData = scipy.io.wavfile.read("wav/partita2-1.wav")
print(rate)
#extract the info
length = audData.shape[0] # this is the number of samples,
#if you divide length by the rateyou get the length in seconds /rate
channel1 = audData[:,0]#[0:length]
#convert in double format
channel1 = np.double(channel1)

#channel2 = audData[:,1]
windowSize = 8192 #length of the window to analyse with STFT
hopSize = 8192  #hopsize between windows

#UNCOMMENT if you want to analyse just a piece of the entire wav

#save_wav = audData[:,0][0:length]
#scipy.io.wavfile.write("study.wav",rate,save_wav)
#window_start = 3*12288
#window_end = window_start + 12288
#save_wav = audData[:,0][window_start:window_end]
#scipy.io.wavfile.write("100th.wav",rate,save_wav)


#compute the STFT
magnitude, frequencies = stft.play(channel1, rate, length , windowSize, hopSize)
print("This is the first element in python %.2f\n"% magnitude[0])
print("This is the maximum value %.2f\n" % max(magnitude))
#PLOT
#x-axis for  the data
x_axis_ref = np.linspace(0, len(magnitude),len(magnitude))
#colors andfigure
colors= sbn.color_palette()
col_ref = sbn.color_palette("cubehelix", 8)
fig = plt.figure(figsize=[15,10])
ax = fig.add_subplot(111)
#axis plot
ax.plot(x_axis_ref,magnitude,color="blue",linewidth=2.5,zorder=10)
ax.xaxis.set_tick_params(labelsize=30)
ax.yaxis.set_tick_params(labelsize=30)
ax.set_ylabel(r"Freq",fontsize=30)
ax.set_xlabel(r"ticks",fontsize=30)
#lgd = ax.legend(loc="best", fontsize=30)#ax.legend(loc='upper center', bbox_to_anchor=(0.5, 1.1),
          #fancybox=True, shadow=True, ncol=3,fontsize=30)

plt.tight_layout()
plt.savefig("power_spectrum.pdf")#,bbox_extra_artists=(lgd,), bbox_inches='tight')


#plt.savefig("newCB8.png",dpi=300,bbox_extra_artists=(lgd,), bbox_inches='tight')
'''
rate, audData = scipy.io.wavfile.read("wav/toccata_fugue_dmin.wav")
length = audData.shape[0]/rate #take only the first 10 seconds
#fugue 2.30 = 190 sec
#the fugue is 6700000:22342656
channel1 = audData[:,0][6740000:22342656]
#length for one single note: 6740000:6745000 --A
wind_length = 6745000-6740000
print("window length :%d" % wind_length)
#scipy.io.wavfile.write("test.wav",rate,channel1)

#sample_length = len(audData[:,0][0:1500000])
#create various fourier spectogram
print("Computing  Custom window...")
#this must be done with stft
#stft.play(channel1)
#f256,t256,Zxx256 = stft(channel1, fs=rate,window="hann",nperseg=wind_length)

print("Computing  256 window...")
f256,t256,Zxx256 = stft(channel1, fs=rate,window="hann",nperseg=256)
print("Computing 1024 window...")
f1024,t1024,Zxx1024 = stft(channel1, fs=rate,window="hann",nperseg=1024)
print("Computing 4096 window...")
f4096,t4096,Zxx4096 = stft(channel1, fs=rate,window="hann",nperseg=4096)
print("Computing 8192 window...")
f8192,t8192,Zxx8192 = stft(channel1, fs=rate,window="hann",nperseg=8192)
'''
