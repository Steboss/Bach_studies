import scipy.io.wavfile
from libc.stdlib cimport free
from cpython cimport PyObject, Py_INCREF

# Import the Python-level symbols of numpy
import numpy as np
import sys

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
    double* stft(double *wav_data, int samples, int windowSize, int hop_size,\
                 double *magnitude, int sample_freq)

cpdef play(audData, rate,windowSize,hopSize):

    #create a memory view, pointer, that can be processed in C
    cdef double[:] audData_view = audData
    #the total number of elements to retrieve from the C code is:
    n_elements = len(audData)
    #Create a view for the magnitude
    cdef double[:] magnitude = np.zeros(n_elements)
    print("Analysis with these parameters:")
    print("Total number of samples %d" % n_elements)
    print("Windows length %d" % windowSize)
    print("Hopping size %d" % hopSize)

    #compute the stft
    stft(&audData_view[0], n_elements, windowSize, hopSize,&magnitude[0], rate)

    #translate the pointer to a numpy array
    magnitude = np.asarray(magnitude)
    #frequencies = np.asarray(frequencies)

    return (magnitude)
