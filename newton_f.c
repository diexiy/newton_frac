#include <stdio.h>

#define W 800
#define H 800

typedef struct
{
    double re; // realdel??
    double im; // im
} complex;

complex add(complex a, complex b)
{
    double res;
    return (complex){a.re + b.re, a.im + b.im}; 
}

complex sub(complex a, complex b)
{
    return (complex){a.re - b.re, a.im - b.im};
}

complex mult(complex a, complex b)
{
    return (complex){
        a.re * b.re - a.im * b.im,
        a.re * b.im + a.im * b.re};
}

complex divC(complex a, complex b)
{
    double d = b.re * b.re + b.im * b.im;
    return (complex){
        (a.re * b.re + a.im * b.im) / d,
        (a.im * b.re - a.re * b.im) / d};
}

complex function(complex z)
{
    return sub(mult(mult(z, z), z), (complex){1, 0});
}

complex Dfunction(complex z)
{
    return mult((complex){3, 0}, mult(z, z));
}

double dist2(complex a, complex b)
{
    double dx = a.re - b.re;
    double dy = a.im - b.im;
    return dx * dx + dy * dy;
}

double f(double x)
{
    return x * x - 1;
}

double df(double x)
{
    return 2 * x;
}

int main()
{
    FILE *fout = fopen("newton_xcom80040.ppm", "w");
    if (!fout)
    {
        return 1;
    }

    const int MAX_ITERATIONS = 40;
    fprintf(fout, "P3\n%d %d\n255\n", W, H);

    complex root1 = {1.0, 0.0};
    complex root2 = {-0.5, 0.866};
    complex root3 = {-0.5, -0.866};

    for (int y = 0; y < H; y++)
    {
        for (int x = 0; x < W; x++)
        {

            complex z = {
                -2.0 + 4.0 * x / W,
                -2.0 + 4.0 * y / H};

            for (int i = 0; i < MAX_ITERATIONS; i++)
            {
                complex dz = divC(function(z), Dfunction(z));
                z = sub(z, dz);
            }

            double d1 = dist2(z, root1);
            double d2 = dist2(z, root2);
            double d3 = dist2(z, root3);

            if (d1 < d2 && d1 < d3)
            {
                fprintf(fout, "160 190 220 ");
            }
            else if (d2 < d3)
            {
                fprintf(fout, "120 150 200 ");
            }
            else
            {
                fprintf(fout, "200 200 200 ");
            }
        }
        fprintf(fout, "\n");
    }

    fclose(fout);
    return 0;
}
