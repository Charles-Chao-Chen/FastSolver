#include <assert.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>

#include "direct_solve.h"
#include "lapack_blas.h"
#include "macros.h"

/*
static void
dirct_circulant_solve(double *soln, double *rhs, int rhs_rows, int rhs_cols, int r, double diag) {

  double *U = (double *) malloc(rhs_rows*r*sizeof(double));
  for (int j=0; j<r; j++)
    for (int i=0; i<rhs_rows; i++)
      U[i+j*rhs_rows] = (i+j)%r;

  double *A = (double *) calloc(rhs_rows*rhs_rows, sizeof(double));
  for (int i=0; i<rhs_rows; i++)
    A[i*(rhs_rows+1)] = diag;

  char transa = 'n';
  char transb = 't';
  int  m = rhs_rows;
  int  n = rhs_rows;
  int  k = r;
  int  lda = rhs_rows;
  int  ldb = rhs_rows;
  int  ldc = rhs_rows;
  double alpha = 1.0;
  double beta  = 1.0;
  
  blas::dgemm_(&transa, &transb, &m, &n, &k, &alpha, U, &lda, U, &ldb, &beta, A, &ldc);

  int INFO;
  int IPIV[m];
  lapack::dgesv_(&m, &rhs_cols, A, &lda, IPIV, rhs, &lda, &INFO);
  assert(INFO == 0);

  double diff  = 0;
  double denom = 0;
  for (int j=0; j<rhs_cols; j++)
    for (int i=0; i<rhs_rows; i++) {
      diff += (soln[i+j*rhs_rows] - rhs[i+j*rhs_rows]) *(soln[i+j*rhs_rows] - rhs[i+j*rhs_rows]);
      denom += rhs[i+j*rhs_rows] * rhs[i+j*rhs_rows];
    }

  std::cout << "Err: " << sqrt(diff/denom) << std::endl;
  
  free(U);
  free(A);
}


static void
dirct_circulant_solve(double *soln, int rand_seed, int rhs_rows,
			   int nregions, int rhs_cols, int r, double diag) {

  double *rhs = (double *) malloc(rhs_rows*sizeof(double));
  int block_size = rhs_rows/nregions;
  for (int i=0; i<nregions; i++) {
    srand( rand_seed );
    for (int j=0; j<block_size; j++)
      rhs[i*block_size+j] = frand(0, 1);
  }
  
  double *U = (double *) malloc(rhs_rows*r*sizeof(double));
  for (int j=0; j<r; j++)
    for (int i=0; i<rhs_rows; i++)
      U[i+j*rhs_rows] = (i+j)%r;

  double *A = (double *) calloc(rhs_rows*rhs_rows, sizeof(double));
  for (int i=0; i<rhs_rows; i++)
    A[i*(rhs_rows+1)] = diag;

  char transa = 'n';
  char transb = 't';
  int  m = rhs_rows;
  int  n = rhs_rows;
  int  k = r;
  int  lda = rhs_rows;
  int  ldb = rhs_rows;
  int  ldc = rhs_rows;
  double alpha = 1.0;
  double beta  = 1.0;
  
  blas::dgemm_(&transa, &transb, &m, &n, &k, &alpha, U, &lda, U, &ldb, &beta, A, &ldc);

  int INFO;
  int IPIV[m];
  lapack::dgesv_(&m, &rhs_cols, A, &lda, IPIV, rhs, &lda, &INFO);
  assert(INFO == 0);

  double diff  = 0;
  double denom = 0;
  for (int j=0; j<rhs_cols; j++)
    for (int i=0; i<rhs_rows; i++) {
      diff += (soln[i+j*rhs_rows] - rhs[i+j*rhs_rows]) *(soln[i+j*rhs_rows] - rhs[i+j*rhs_rows]);
      denom += rhs[i+j*rhs_rows] * rhs[i+j*rhs_rows];
    }

  std::cout << "Err: " << sqrt(diff/denom) << std::endl;

  free(rhs);
  free(U);
  free(A);
}
*/

static void writeToFile
(const double* rhs, const int rhs_rows, const int rhs_cols,
 const std::string& file) {

  std::ofstream ofs(file.c_str());
  for (int i=0; i<rhs_rows; i++) {
    for (int j=0; j<rhs_cols; j++)
      ofs << std::setprecision(20)
	  << rhs[i + j*rhs_rows]
	  << '\t';
    ofs << std::endl;
  }
  ofs.close();
}

static void
dirct_circulant_solve
(const std::string& soln_file, const long seed, const int rhs_rows,
 const int nregions, const int rhs_cols, const int r,
 const double diag) {

  double *rhs = (double *) malloc(rhs_rows*rhs_cols*sizeof(double));
  int block_size = rhs_rows/nregions;
  for (int nr=0; nr<nregions; nr++) {
    struct drand48_data buffer;
    assert( srand48_r( seed, &buffer ) == 0 );
    for (int i=0; i<block_size; i++) {
      for (int j=0; j<rhs_cols; j++) {
	int row_idx = nr*block_size + i;
	int col_idx = j;
	int count = row_idx + col_idx*rhs_rows;
	assert( drand48_r( &buffer, &rhs[count]) == 0 );
      }
    }
  }

#ifdef DEBUG
  std::ofstream ofs("rhs_ref.txt");
  for (int nr=0; nr<nregions; nr++) {
    ofs << seed << std::endl;
    for (int i=0; i<block_size; i++) {
      for (int j=0; j<rhs_cols; j++) {
	int row_idx = nr*block_size + i;
	int col_idx = j;
	int count = row_idx + col_idx*rhs_rows;
	ofs << std::setprecision(20)
	    << rhs[count]
	    << '\t';
      }
      ofs << std::endl;
    }
  }
  ofs.close();
#endif
  
  double *U = (double *) malloc(rhs_rows*r*sizeof(double));
  for (int j=0; j<r; j++)
    for (int i=0; i<rhs_rows; i++)
      U[i+j*rhs_rows] = (i+j)%r;

  double *A = (double *) calloc(rhs_rows*rhs_rows,sizeof(double));
  for (int i=0; i<rhs_rows; i++)
    A[i*(rhs_rows+1)] = diag;

  char transa = 'n';
  char transb = 't';
  int  m = rhs_rows;
  int  n = rhs_rows;
  int  k = r;
  int  lda = rhs_rows;
  int  ldb = rhs_rows;
  int  ldc = rhs_rows;
  double alpha = 1.0;
  double beta  = 1.0;
  
  blas::dgemm_(&transa, &transb, &m, &n, &k,
	       &alpha, U, &lda, U, &ldb, &beta, A, &ldc);

  int INFO;
  int IPIV[m];
  int nRHS = rhs_cols;
  lapack::dgesv_(&m, &nRHS, A, &lda, IPIV, rhs, &lda, &INFO);
  assert(INFO == 0);

  // write the direct output to file
  writeToFile(rhs, rhs_rows, rhs_cols, "soln_ref.txt");
    
  // read solver output from file
  double *soln = (double *) malloc(rhs_rows*rhs_cols*sizeof(double));
  std::ifstream ifs;
  ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    ifs.open(soln_file.c_str());
    for (int i=0; i<rhs_rows; i++)
      for (int j=0; j<rhs_cols; j++)
	ifs >> soln[i + j*rhs_rows];
    ifs.close();
  }
  catch (std::ifstream::failure e) {
    std::cerr << "Exception opening "
	      << soln_file << std::endl;
  }
  
  double diff  = 0;
  double denom = 0;
  for (int j=0; j<rhs_cols; j++) {
    for (int i=0; i<rhs_rows; i++) {
      diff += (soln[i+j*rhs_rows] - rhs[i+j*rhs_rows])
	*(soln[i+j*rhs_rows] - rhs[i+j*rhs_rows]);
      denom += rhs[i+j*rhs_rows] * rhs[i+j*rhs_rows];
    }
  }
  std::cout << "Err: " << sqrt(diff/denom) << std::endl;

  free(rhs);
  free(soln);
  free(U);
  free(A);
}


void compute_L2_error
(const HodlrMatrix &lr_mat, const long rand_seed, const int rhs_rows,
 const int nregions, const int rhs_cols, const int rank,
 const double diag,
 Context ctx, HighLevelRuntime *runtime) {
    
  // write the solution from fast solver
  lr_mat.save_solution(ctx, runtime);
  std::string soln_file = lr_mat.get_file_soln();
  dirct_circulant_solve(soln_file, rand_seed, rhs_rows, nregions,
			rhs_cols, rank, diag); 
}
