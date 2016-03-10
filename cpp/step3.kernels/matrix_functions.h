#pragma once
#include "vector.h"
#include "matrix.h"

void matvec(const matrix& A, const vector& x, const vector &y) {

  unsigned int num_rows=A.num_rows;
  unsigned int *row_offsets=A.row_offsets;
  unsigned int *cols=A.cols;
  double *__restrict Acoefs=A.coefs;
  double *__restrict xcoefs=x.coefs;
  double *__restrict ycoefs=y.coefs;

#pragma acc kernels present(row_offsets,cols,Acoefs,xcoefs,ycoefs)
  {
#pragma acc loop gang worker(32)
    for(int i=0;i<num_rows;i++) {
      double sum=0;
      int row_start=row_offsets[i];
      int row_end=row_offsets[i+1];
#pragma acc loop vector(32)
      for(int j=row_start;j<row_end;j++) {
        unsigned int Acol=cols[j];
        double Acoef=Acoefs[j];
        double xcoef=xcoefs[Acol];
        sum+=Acoef*xcoef;
      }
      ycoefs[i]=sum;
    }
  }
}
