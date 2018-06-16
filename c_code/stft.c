#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <fftw3.h>
#include <math.h>

#define PI 3.14159265

//typedef double fftw_complex[2];

void hamming(int windowLength, double *buffer){
  for(int i=0; i<windowLength; i++){
    buffer[i] = 0.54 - 0.46*cos((2*PI*i)/(windowLength-1.0));
  }
  //try to normalize it
  double maximum = 0.0 ;
  for (int i =0; i<windowLength; i++){
    if (buffer[i]>maximum)
    {
      maximum = buffer[i];
    }
  }
  for (int i =0; i<windowLength;i++)
  {
    buffer[i] =buffer[i]/maximum;
  }
}

//the main function should compute the  stft of the signal, return the stft
//and the correlation points between a chosen window --for the moment --
//and all the other windows
double* stft(double *wav_data, int wav_length, int windowSize, int hop_size, double *correlation_result,\
             double *magnitude, double *frequencies, int sample_freq)
{
  //double *wav_data should be a 1D array
  //wav_length is the length of the wave data
  //windowSize is the size of the window  for the stft
  //hop_size is the hopping size
  printf("Initialization of parameters...\n");
  //double *hamming_result ; //define the hamming window
  //hamming_result = malloc(sizeof(double));

  //define the total number of samples
  int n_samples,storage_capacity ;
  n_samples = (wav_length/windowSize) ;
  storage_capacity = (wav_length);
  printf("Total number of samples in C %d\n", n_samples);
  //define the fourier variables
  fftw_complex *stft_data, *fft_result, *storage;
  double scale, freq_spacing;
  freq_spacing = 1.0/(sample_freq);
  //allocate the memory for the fftw
  stft_data = (fftw_complex*)(fftw_malloc(sizeof(fftw_complex)*windowSize));
  fft_result= (fftw_complex*)(fftw_malloc(sizeof(fftw_complex)*windowSize));
  storage = (fftw_complex*)(fftw_malloc(sizeof(fftw_complex)*3601263));
  printf("Total length of storage %d\n", (storage_capacity));
  //exit(0);
  //define the fftw plane
  fftw_plan plan_forward, plan_backward;
  plan_forward = fftw_plan_dft_1d(windowSize, stft_data, fft_result, FFTW_FORWARD, FFTW_ESTIMATE);
  //integer indexes
  int i,counter ;
  counter = 0 ;
  //double for the correlation value
  //window to be applied to data
  double hamming_result[windowSize];
  printf("Creation of a hamming window...");
  hamming(windowSize, hamming_result);
  //scaling factor
  double summa = 0 ;
  for (i=0; i<windowSize; i++)
  {
    summa+=hamming_result[i]*hamming_result[i];
    //printf("%.2f\n", hamming_result[i]);
  }

  //scale = sqrt(1.0/(sample_freq*(summa*summa)));
  scale = sqrt(1.0/(summa*summa));
  //create the frequencies
  double val_freq;
  val_freq = 1.0/(windowSize*freq_spacing);
  int N_val ;
  N_val = floor(windowSize/2)+1;
  printf("This is the scaling factor %.8f", scale);
  printf("This is summa %.2f\n", summa);
  printf("This is N %d and this is val %.2f\n", N_val, val_freq);
  for (i=0 ; i<N_val; i++)
  {
    frequencies[i] = i*val_freq;
    //printf("Frequencies %.2f\n", frequencies[i]);
  }

  /*sanity check
  for (i=0; i< windowSize; i++)
  {
    printf("Hamming at %d = %.2f\n", i, hamming_result[i]);
  }
  */
  //now here we need to implement the stft
  int chunkPosition = 0; //actual chunk position
  int readIndex ; //read the index of the wav file

  while (chunkPosition < wav_length ){
    //read the window
    for(i=0; i<windowSize; i++){

      readIndex = chunkPosition + i;
      //printf("Index position %d\n", readIndex);

      if (readIndex < wav_length){
        //stft_data[i][0] = wav_data[readIndex];//*hamming_result[i];
        //stft_data[i][1] = 0.0;
        stft_data[i] = wav_data[readIndex]*hamming_result[i]*_Complex_I  + 0.0*I;
      }
      else{
        //if we are beyond the wav_length
        stft_data[i] = 0.0*_Complex_I + 0.0*I;
        //stft_data[i][0] = 0.0 ;  //this is ok if youdon'twant to use complex.h
        //stft_data[i][1] = 0.0 ; //padding
        break;
      }
    }
    //compute the fft
    fftw_execute(plan_forward);
    //store the stft in a data structure
    for (i=0; i<windowSize/2 + 1;i++)
    {
      //printf("RE: %.2f  IM: %.2f\n", creal(fft_result[i]),cimag(fft_result[i]));
      storage[counter] = fft_result[i];//creal(fft_result[i]) + cimag(fft_result[i]);
      //correlation_result[counter] = fft_result[i]*conj(fft_result[i])*scale;
      counter+=1;
    }

    chunkPosition += hop_size;
    //printf("Chunk Position %d\n", chunkPosition);
    //printf("Counter position %d\n", counter);
    //printf("Fourier transform done\n");

  }

  //printf("This is the storage capacity %d and this the counter %d\n", storage_capacity,counter);
  /*from the fft compute the frequencies */
  for (i=0; i< wav_length; i++)
  {
    magnitude[i] = storage[i]*conj(storage[i])*scale; //cabs(storage[counter]);
    //frequencies[i] =  i*(sample_freq/windowSize);
  }


  /*Correlation among  windows
  //multiply all the elments of one window times the correspictive elments of the other one
  //sum them all
  //divide by number of elmeents (1024?)
  */

  /*
  for ( i=0; i<n_samples ; i++)
  {
    for (j=0; j<windowSize;j++)
    {
      //here I am computing the cross-correlation for windows 100 with all the other windows
      product = storage[3*windowSize]*conj(storage[i*windowSize+j]);
      summa += product;
      //correlation_result[i*windowSize+j] = product;
    }
    correlation_result[i] =  summa/windowSize;
    //printf("Sum for the window %d is:%.2f\n", i, summa);
    //normalize
    summa = 0.0;
  }
  fftw_execute(plan_backward);

  for (i=0; i<windowSize;i++)
  {
    printf("Test %.2f\n", ifft_data[i]);
  }
  */
  //to compute the correlation do :
  //e.g. select element X of the storage
  //multiply this element by all the element in a window sample
  //sum all these values
  // divide each moltiplication by this value

  /*correlation  for (i=0; i<windowSize; i++)
  {
    //conjugate = (complex double)fft_result[i+1];
    //printf("Elements R %.2f, I %.2f\n",creal(fft_result[i]), cimag(fft_result[i]));
    //correlation = fft_result[i]*conj(fft_result[i+1])*scale;
    correlation_result[counter] = correlation ;
    counter+=1;
    //printf("%2.2f, %2.2f\n", fft_result[i][0], fft_result[i][1]);
    //printf("%2.2f\n", fft_result[i][0]);
    //save fft_result in a data structure
    //test teh correlation

    //printf("Re: %.2f  , Im: %.2f \n", fft_result[i][0], fft_result[i][1]);
    //printf("Re: %2.f  , Im: %.2f \n",fft_result[i+1][0], fft_result[i+1][1]);
  }
  */

  //sanity check

  /*printf("These are the wav_data\n");
  for (i=0; i< windowSize; i++)
  {
    printf("%.2f\n", wav_data[i]);
  }
  exit(0);*/

  /*
  for (i=0; i<windowSize; i++)
  {
    stft_data[i][0] = (double)wav_data[i];
  }
  //stft_data[0][1] = 0.0 ;

  //sanity check
  printf("This is the wav_data values\n");
  printf("%.2f\n", wav_data[0]);
  printf("This is the translation into a complex object\n");
  printf("%.2f\n", stft_data[0]);
  //for ( i=0; i<windowSize;i++)
  //{
  //  printf("%.2f\n", stft_data[i]);
  //}
  //perform the fourier transform4

  printf("Fourier transform call...\n");
  fftw_execute(plan_forward);
  printf("Fourier transform done\n");
  printf("%.2f\n", fft_result[0]);
  //for (i=0; i<windowSize/2; i++)
  //{
  //  printf("%.2f\n", fft_result[i]);
  //}
  */
  //clean up the memory

  //return correlation_result ;
  printf("This is the first element of magnitude %.2f\n", magnitude[0]);
  return magnitude, frequencies;

  fftw_destroy_plan(plan_forward);
  fftw_free(stft_data);
  fftw_free(fft_result);
  //fftw_free(ifft_result);//do we need a ifft?
  free(hamming_result);


}
