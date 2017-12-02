/*******************************************************************************/
/* optfft.c                                                                    */
/*                                                                             */
/* An optimized version of the fft function using only 16-bit integer math.    */
/*                                                                             */
/* Optimized by Brent Plump                                                    */
/* Based heavily on code by Jinhun Joung                                       */
/*                                                                             */
/* - Works only for input arrays of 256 length.                                */
/* - Requires two arrays of 16-bit ints.  The first contains the samples, the  */
/*   second contains all zeros.  The samples range from -31 to 32              */
/* - Returns the index of the peak frequency                                   */
/*******************************************************************************/
#include "utils/optfft.h"

#define ABS(x)  (((x)<0)?(-(x)):(x))
#define CEILING(x) (((x)>511)?511:(x))

signed int optfft(signed int real[256], signed int imag[256]) {

  signed int i, i1, j, l, l1, l2, t1, t2, u;

#include "tables.c"

  /* Bit reversal. */
  /*Do the bit reversal */
  l2 = 128;
  i=0;
  for(l=0;l<255;l++) {
    if(l < i) {
      j=real[l];real[l]=real[i];real[i]=j;
    }
    l1 = l2;
    while (l1 <= i){
      i -= l1;
      l1 >>= 1;
    }
    i += l1;
  }

  /* Compute the FFT */
  u = 0;
  l2 = 1;
  for(l=0;l<8;l++){
    l1 = l2;
    l2 <<= 1;
    for(j=0;j<l1;j++){
      for(i=j;i<256;i+=l2){
        i1 = i + l1;
        t1 = (u1[u]*real[i1] - u2[u]*imag[i1])/32;
        t2 = (u1[u]*imag[i1] + u2[u]*real[i1])/32;
        real[i1] = real[i]-t1;
        imag[i1] = imag[i]-t2;
        real[i] += t1;
        imag[i] += t2;
      }
      u++;
    }
  }

  /* Find the highest amplitude value */
  /* start at index 1 because 0 can hold high values */
  j=1;
  l=0;
  for ( i=1; i<(128); i++ ) {
    l1 = square[CEILING(ABS(real[i]))]+square[CEILING(ABS(imag[i]))];
    if (l1 > l) {
      j = i;
      l = l1;
    }
  }
  return (j);
}