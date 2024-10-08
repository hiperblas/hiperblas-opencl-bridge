#include "gtest/gtest.h"
#include "libneblina.h"
#include "clutils.h"
#include "neblina_std.h"
#include "neblina_vector.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>

using namespace std;

class SparseMatrixFixture : public ::testing::Test {
protected:
  cl_int status;
  bridge_manager_t m;
  int idx;
public:

    SparseMatrixFixture() {
        idx = 0;
        load_plugin(&m, "/usr/local/lib64/libneblina-opencl-bridge.so", idx);

        //printf("aqui\n");
        m.bridges[idx].InitEngine_f(0);
    }

    protected:
    static void SetUpTestSuite() {
    }

    static void TearDownTestSuite() {
    }
    
    void SetUp() {
    }

    void TearDown() {
    }

    ~SparseMatrixFixture() {
        m.bridges[idx].StopEngine_f();
    }

};


//TEST_F(SparseMatrixFixture, smatvec_multiply_WithSparseMatrixFloat) {
//
//    int n = 10;
//
//    vector_t * a = vector_new(n, T_FLOAT, 1);
//    smatrix_t * b = smatrix_new(n, n, T_FLOAT);
//    vector_t * out;
//
//    for (int i = 0; i < a->len; i++) {
//        a->value.f[i] = 3.;
//    }
//
//    smatrix_set_real_value(b, 0, 0, 3.);
//    smatrix_set_real_value(b, 0, 1, 3.);
//    smatrix_set_real_value(b, 0, 9, 3.);
//
//    smatrix_set_real_value(b, 1, 1, 3.);
//    smatrix_set_real_value(b, 1, 5, 3.);
//    smatrix_set_real_value(b, 1, 8, 3.);
//
//    smatrix_set_real_value(b, 2, 2, 3.);
//    smatrix_set_real_value(b, 2, 4, 3.);
//    smatrix_set_real_value(b, 2, 7, 3.);
//
//    smatrix_set_real_value(b, 3, 3, 3.);
//    smatrix_set_real_value(b, 3, 1, 3.);
//    smatrix_set_real_value(b, 3, 6, 3.);
//
//    smatrix_pack(b);
//
//    //    for (int i=0; i < b->nrow * b->maxcols; i++) {
//    //        printf("b->idx_col[%d]=%d b->[%d]=%lf \n",i,b->idx_col[i],i,b->m[i]);
//    //    }
//
//    out = (vector_t *) smatvec_multiply(b, a);
//
//    EXPECT_EQ(27., out->value.f[0]);
//    EXPECT_EQ(27., out->value.f[1]);
//    EXPECT_EQ(27., out->value.f[2]);
//    EXPECT_EQ(27., out->value.f[3]);
//    EXPECT_EQ(0., out->value.f[4]);
//    EXPECT_EQ(0., out->value.f[5]);
//    EXPECT_EQ(0., out->value.f[6]);
//    EXPECT_EQ(0., out->value.f[7]);
//    EXPECT_EQ(0., out->value.f[8]);
//    EXPECT_EQ(0., out->value.f[9]);
//    
//    vector_delete(a);
//    smatrix_delete(b);
//    vector_delete(out);
//
//}

TEST_F(SparseMatrixFixture, matvec_mul3_WithSparseMatrixFloat) {

    int n = 10;

    vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 1, NULL);
    smatrix_t * b = m.bridges[idx].smatrix_new(n, n, T_FLOAT);
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 3.;
    }

    m.bridges[idx].smatrix_set_real_value(b, 0, 0, 3.);
    m.bridges[idx].smatrix_set_real_value(b, 0, 1, 3.);
    m.bridges[idx].smatrix_set_real_value(b, 0, 9, 3.);

    m.bridges[idx].smatrix_set_real_value(b, 1, 1, 3.);
    m.bridges[idx].smatrix_set_real_value(b, 1, 5, 3.);
    m.bridges[idx].smatrix_set_real_value(b, 1, 8, 3.);

    m.bridges[idx].smatrix_set_real_value(b, 2, 2, 3.);
    m.bridges[idx].smatrix_set_real_value(b, 2, 4, 3.);
    m.bridges[idx].smatrix_set_real_value(b, 2, 7, 3.);

    m.bridges[idx].smatrix_set_real_value(b, 3, 3, 3.);
    m.bridges[idx].smatrix_set_real_value(b, 3, 1, 3.);
    m.bridges[idx].smatrix_set_real_value(b, 3, 6, 3.);

    m.bridges[idx].smatrix_pack(b);
    object_t ** in = convertToObject4(a, b);

    r = (vector_t *) matvec_mul3(&m, idx, (void **) in, NULL);

    m.bridges[idx].vecreqhost(r);

    EXPECT_EQ(27., r->value.f[0]);
    EXPECT_EQ(27., r->value.f[1]);
    EXPECT_EQ(27., r->value.f[2]);
    EXPECT_EQ(27., r->value.f[3]);
    EXPECT_EQ(0., r->value.f[4]);
    EXPECT_EQ(0., r->value.f[5]);
    EXPECT_EQ(0., r->value.f[6]);
    EXPECT_EQ(0., r->value.f[7]);
    EXPECT_EQ(0., r->value.f[8]);
    EXPECT_EQ(0., r->value.f[9]);
    
    m.bridges[idx].vector_delete(a);
    m.bridges[idx].smatrix_delete(b);
    m.bridges[idx].vector_delete(r);

}

TEST_F(SparseMatrixFixture, matvec_mul3_WithSparseMatrixComplex) {

    int n = 7000;

    vector_t * a = m.bridges[idx].vector_new(n, T_COMPLEX, 1, NULL);
    smatrix_t * b = m.bridges[idx].smatrix_new(n, n, T_COMPLEX);
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[2 * i] = 3.;
        a->value.f[2 * i + 1] = 3.;
    }

    m.bridges[idx].smatrix_set_complex_value(b, 0, 0, 3., 3.);
    m.bridges[idx].smatrix_set_complex_value(b, 0, 1, 3., 3.);
    m.bridges[idx].smatrix_set_complex_value(b, 0, 9, 3., 3.);

    m.bridges[idx].smatrix_set_complex_value(b, 1, 1, 3., 3.);
    m.bridges[idx].smatrix_set_complex_value(b, 1, 5, 3., 3.);
    m.bridges[idx].smatrix_set_complex_value(b, 1, 8, 3., 3.);

    m.bridges[idx].smatrix_set_complex_value(b, 2, 2, 3., 3.);
    m.bridges[idx].smatrix_set_complex_value(b, 2, 4, 3., 3.);
    m.bridges[idx].smatrix_set_complex_value(b, 2, 7, 3., 3.);

    m.bridges[idx].smatrix_set_complex_value(b, 3, 3, 3., 3.);
    m.bridges[idx].smatrix_set_complex_value(b, 3, 1, 3., 3.);
    m.bridges[idx].smatrix_set_complex_value(b, 3, 6, 3., 3.);
    
    for (int i = 0; i < a->len; i++) {
        m.bridges[idx].smatrix_set_complex_value(b, 10, i, 3., 3.);
    }

    m.bridges[idx].smatrix_pack_complex(b);

    object_t ** in = convertToObject4(a, b);

    r = (vector_t *) matvec_mul3(&m, idx, (void **) in, NULL);

    m.bridges[idx].vecreqhost(r);

    EXPECT_EQ(0., r->value.f[0]);
    EXPECT_EQ(54., r->value.f[1]);
    EXPECT_EQ(0., r->value.f[2]);
    EXPECT_EQ(54., r->value.f[3]);
    EXPECT_EQ(0., r->value.f[4]);
    EXPECT_EQ(54., r->value.f[5]);
    EXPECT_EQ(0., r->value.f[6]);
    EXPECT_EQ(54., r->value.f[7]);
    EXPECT_EQ(0., r->value.f[8]);
    EXPECT_EQ(0., r->value.f[9]);
    EXPECT_EQ(0., r->value.f[10]);
    EXPECT_EQ(0., r->value.f[11]);
    EXPECT_EQ(0., r->value.f[12]);
    EXPECT_EQ(0., r->value.f[13]);
    EXPECT_EQ(0., r->value.f[14]);
    EXPECT_EQ(0., r->value.f[15]);
    EXPECT_EQ(0., r->value.f[16]);
    EXPECT_EQ(0., r->value.f[17]);
    EXPECT_EQ(0., r->value.f[18]);
    EXPECT_EQ(0., r->value.f[19]);
    
    m.bridges[idx].vector_delete(a);
    m.bridges[idx].smatrix_delete(b);
    m.bridges[idx].vector_delete(r);

}

TEST_F(SparseMatrixFixture, matvec_mul3_WithLargeSparseMatrixFloat) {

    int n = 1000;

    vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 1, NULL );
    smatrix_t * b = m.bridges[idx].smatrix_new(n, n, T_FLOAT);
    matrix_t * b_dense = m.bridges[idx].matrix_new(n, n, T_FLOAT, 1, NULL);
    vector_t * r;
    vector_t * r_dense;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = i * 1.;
    }

    for (int i = 0; i < b->ncol; i++) {
        for (int j = 0; j < b->nrow; j++) {
            m.bridges[idx].smatrix_set_real_value(b, i, j, (i + j) * 1.);
            b_dense->value.f[i * b->ncol + j] = (i + j) * 1.;
        }
    }

    m.bridges[idx].smatrix_pack(b);

    object_t ** in = convertToObject4(a, b);
    
    object_t ** in_dense = convertToObject3(a, b_dense);

    r_dense = (vector_t *) matvec_mul3(&m, idx, (void **) in_dense, NULL);

    m.bridges[idx].vecreqhost(r_dense);

    printf("will call opencl\n");
    r = (vector_t *) matvec_mul3(&m, idx, (void **) in, NULL);
    printf("returned from opencl call\n");
    m.bridges[idx].vecreqhost(r);

    for (int i = 0; i < r->len; i++) {
        EXPECT_EQ(r_dense->value.f[i], r->value.f[i]);
    
    }

    delete_object_array(in, 2);
    delete_object_array(in_dense, 2);

    m.bridges[idx].vector_delete(a);

    m.bridges[idx].smatrix_delete(b);

    m.bridges[idx].vector_delete(r);

    m.bridges[idx].matrix_delete(b_dense);
    m.bridges[idx].vector_delete(r_dense);


}