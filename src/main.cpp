#include <cstdio>

int enzyme_dup;
int enzyme_dupnoneed;
int enzyme_out;
int enzyme_const;

template <typename return_type, typename... T>
return_type __enzyme_fwddiff(void *, T...);

template <typename return_type, typename... T>
return_type __enzyme_autodiff(void *, T...);

double sumAndMul(double* array, size_t size, double mul) {
  double sum = 0;
  for(int i=0; i<size; i++) {
    sum += array[i];
  }
  return sum * mul;
}


int main () {

size_t size = 4;
double mul = 1.5;

double array[4] = {1.0, 2.0, 3.0, 4.0};
double d_array[4] = {1.0, 2.0, 3.0, 4.0};

double d_mul = __enzyme_autodiff<double>((void*)sumAndMul,
                     enzyme_dup, array, d_array,
                     enzyme_const, size,
                     enzyme_out, mul);

printf("d(output)/darray[0] = %f\n", d_array[0]);
printf("d(output)/darray[1] = %f\n", d_array[1]);
printf("d(output)/darray[2] = %f\n", d_array[2]);
printf("d(output)/darray[3] = %f\n", d_array[3]);

printf("mul = %f\n", mul);
printf("d_mul = %f\n", d_mul);

return 0;

}