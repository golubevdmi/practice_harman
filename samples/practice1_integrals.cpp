#include <iostream>
#include <cmath>
#include <cassert>


#define M_PI_2          1.57079632679489661923


// Calculate definite integral with rectangles rule
double rectangleRule(double(*func)(double), double a, double b, int N);
double rectangleRuleL(double(*func)(double), double a, double b, int N);
double rectangleRuleR(double(*func)(double), double a, double b, int N);
// Calculate definite integral with trapezoidal rule
double trapezoidalRule(double(*func)(double), double a, double b, int N);
// Calculate definite integral with Simpon rule
double simpsonsRule(double(*func)(double), double a, double b, int N);


double g_a = 1., g_b = 1.;

// Calculate ellipse eccentricity
double eccentricity()
{
	assert(g_a != 0.0);
	if (fabs(g_a) < fabs(g_b)) std::swap(g_a, g_b);
	return sqrt(1.0 - (g_b * g_b) / (g_a * g_a));
}
// Ellipse function
double completeEllipticIntegral2kind(double fi)
{
	return sqrt(1. - eccentricity() * eccentricity() * std::sin(fi) * std::sin(fi));
}

int main()
{
	printf("Ellipse Length Calculation. \n\n");

	int N = 10;
	printf("Calculate ellipse length: a = %1.2lf, b = %1.2lf\n", g_a, g_b);
	// Rectangles rules
	printf("Rectangle method: l: %lf, m: %lf, r: %lf\n",
		4 * g_a * rectangleRuleL(completeEllipticIntegral2kind, 0, M_PI_2, N),
		4 * g_a * rectangleRule(completeEllipticIntegral2kind, 0, M_PI_2, N),
		4 * g_a * rectangleRuleR(completeEllipticIntegral2kind, 0, M_PI_2, N));
	// Trapezoidal rule
	printf("Trapezoidal rule: %lf\n", 4 * g_a * trapezoidalRule(completeEllipticIntegral2kind, 0, M_PI_2, N));
	// Simpson's rule (for 2N)
	if (!(N % 2))
		printf("Simpson's rule: %lf\n", 4 * g_a * simpsonsRule(completeEllipticIntegral2kind, 0, M_PI_2, N));
	return 0;
}


double rectangleRule(double(*func)(double), double a, double b, int N)
{
	assert(N > 0);
	double h = (b - a) / (double)N;
	double sum = 0.0;
	for (size_t i = 0; i < N; i++)
		sum += func(a + h * i + h / 2);
	return sum * h;
}


double rectangleRuleL(double(*func)(double), double a, double b, int N)
{
	assert(N > 0);
	double h = (b - a) / (double)N;
	double sum = 0.0;
	for (size_t i = 0; i < N; i++)
		sum += func(a + h * i);
	return sum * fabs(h);
}


double rectangleRuleR(double(*func)(double), double a, double b, int N)
{
	return rectangleRuleL(func, b, a, N);
}


double trapezoidalRule(double(*func)(double), double a, double b, int N)
{
	assert(N > 0);
	double h = (b - a) / (double)N;
	double sum = (func(a) + func(b)) / 2.0;
	for (size_t i = 1; i < N; i++)
		sum += func(a + h * i);
	return sum * h;
}


double simpsonsRule(double(*func)(double), double a, double b, int N)
{
	assert(N > 0 && !(N % 2));
	double h = (b - a) / (double)N;
	double sum = func(a) + func(b);
	for (size_t i = 1; i < N; i++)
	{
		if (i % 2)
			sum += 4 * func(a + h * i);
		else
			sum += 2 * func(a + h * i);
	}
	return sum * h / 3.0;
}