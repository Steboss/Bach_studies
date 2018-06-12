import scipy.io.wavfile
from libc.stdlib cimport free
from cpython cimport PyObject, Py_INCREF

# Import the Python-level symbols of numpy
import numpy as np

# Import the C-level symbols of numpy
cimport numpy as np

# Numpy must be initialized. When using numpy from C or Cython you must
# _always_ do that, or you will have segfaults
np.import_array()

from libc.stdlib cimport free
from cpython cimport PyObject, Py_INCREF

# Import the Python-level symbols of numpy
import numpy as np

# Import the C-level symbols of numpy
cimport numpy as np

# Numpy must be initialized. When using numpy from C or Cython you must
# _always_ do that, or you will have segfaults
np.import_array()

cdef extern from "c_code/stft.c":
    double* stft(double *wav_data, int wav_length, int windowSize, int hop_size, double *correlation_result)


cpdef play():
    #wav_data is the wave file, already opened with scipy
    rate, audData = scipy.io.wavfile.read("wav/canon.wav")
    length = audData.shape[0] #/rate #take only the first 10 seconds
    #channel1 = audData[:,0][6740000:22342656]
    #channel2 = audData[:,1][6740000:22342656]
    channel1 = audData[:,0]
    print(len(channel1))
    #here we could have a float and not a double
    #channel1 = np.float32(channel1)
    channel1 = np.double(channel1)
    print(type(channel1[0]))
    #wind_length = 6745000-6740000
    wind_length = 1024    # 6800000 - 6740000
    hop_size = 1024
    #print(len(channel1))
    cdef double[:] audData_view = channel1
    #audData_view.astype("double")

    #the total length of the correlation arrays is :
    n_samples = int(length)
    print("Total number of samples in Cython %d" % n_samples)
    cdef double[:] correlation_result = np.zeros(n_samples)

    print("Analysis with these parameters:")
    print("Length of the signal %d" % length)
    print("Windows length %d" % wind_length)
    print("Hopping size %d" % hop_size)

    stft(&audData_view[0], length, wind_length, hop_size,&correlation_result[0])
    correlation_python = np.asarray(correlation_result)
    return (correlation_python)
