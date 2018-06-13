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


cpdef play(audData, rate, length,windowSize,hopSize):

    #create a memory view, pointer, that can be processed in C
    cdef double[:] audData_view = audData
    #the total length of the correlation arrays is :
    n_samples = int(length)
    print("Total number of samples in Cython %d" % n_samples)
    #create a view for the  double * correlation result pointer of C
    cdef double[:] correlation_result = np.zeros(n_samples)

    print("Analysis with these parameters:")
    print("Length of the signal %d" % length)
    print("Windows length %d" % windowSize)
    print("Hopping size %d" % hopSize)

    stft(&audData_view[0], length, windowSize, hopSize,&correlation_result[0])
    correlation_python = np.asarray(correlation_result)

    print(correlation_python)
    return (correlation_python)
