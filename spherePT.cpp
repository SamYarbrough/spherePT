#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
#include "vec2.h"
#include "vec3.h"
#include "traceroo.h"

int main()
{
	srand( (unsigned)time( NULL ) ); //init random

	std::cout << "DIMENSIONS: " << WIDTH << " X " << HEIGHT << "\n\n";
	std::ofstream fout("outimg.ppm");
	if (fout.fail()) return -1;
	fout << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

	#pragma omp for
	for (int j = HEIGHT - 1; j >= 0; --j)
	{
		std::cerr << "\rRemaining: " << j << " " << std::flush;
		for (int i = 0; i < WIDTH; ++i)
		{
			vec3 dered = clamp(render(vec2((double)i, (double)j)), 0, 1);
			double r = dered.x;
			double g = dered.y;
			double b = dered.z;
			int ir = static_cast<int>(255.999 * r);
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b);

			fout << ir << " " << ig << " " << ib << "\n";
		}
	}

	std::cerr << "\n\nDONE.\n";
	fout.close();
	system("outimg.ppm");
	return 0;
}
