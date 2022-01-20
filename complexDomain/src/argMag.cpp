#include <cmath>
#include <complex>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "createBitmap.hpp"

using namespace std;


unsigned char* createPicture(int h, int w, map< int , complex<double> > &function, string fn){
	map< int, complex<double> >::iterator fit;
	complex<double> z;
	double ar, mag;

	if(function.size() != h * w) cout << "uh oh fucky wucky!";

	Pixel* pixarray = new Pixel[h*w];

	for(fit = function.begin(); fit != function.end(); fit++){
		z = fit->second;
		mag = abs(z);
		ar = arg(z);

		//cout << fit->first << " " << fit->second << endl;

		pixarray[fit->first].red = min(5*mag, 255.0);
		pixarray[fit->first].blue = min(40*ar, 255.0);
		pixarray[fit->first].green = min(100*mag, 255.0);

	}

	writeBitmap(h, w, pixarray, fn);

	delete[] pixarray;

	return NULL;
}

// creates a map over a specified boundary to be passed to create the picture.
// the int of the map represents the evenly spaced codomain as specified from the boundary.
// the attached float is the first found point in the preimiage of that point.
map< int , complex<double> > makeMap(int h, int w, double realLeft, double realRight,
						  double imagTop, double imagBot){

	map< int, complex<double> > ret;
	int i, j, k;
	complex<double> z, output;

	ofstream fout;
	fout.open("test.txt");
	for(i = 0; i < w * h; i++){
		complex<double> z(realLeft + ((i%w) * ((realRight - realLeft)/w)),
					imagTop - ((i/w) * ((imagTop - imagBot)/h)));

		/*output = complex<double>(0,0);
		k=0;
		while(k < 500 && abs(output) < 2){
			output = pow(output, 2) + z;
			k++;
		}
		if(abs(output) > 2) output = complex<double>(INFINITY, 0);*/
		//output = sin(z);
		//output = (((z * z) + complex<double>(-1,0)) * pow((z + complex<double>(-2, -1)), 2))/(z*z + complex<double>(2,2));
		//output = ((z * z * z) + complex<double>(-1, 0));
		output = z;
		//cout << i << "      " << j << "      " << z << "      " << output << "      " << norm(output) << "                 \r";

		fout << z << " has image of " << output << endl;

		ret.insert(make_pair(i, output));

	}

	fout.close();

	return ret;
}

int main(){
	map< int , complex<double> > map;


	map = makeMap(1000, 1000, -2, 2, 2, -2);
	createPicture(1000, 1000, map, "../pics/f(z)=z_D.bmp");


	/*map = makeMap(1000, 1000, -2.0, 2.0, 2.0, -2.0,function);
	cout << "Map size: " << map.size();
	createPicture(1000, 1000, map, "mando.bmp");

	map = makeMap(1000, 1000, -1.0, 1.0, 1.0, -1.0,function);
	cout << "Map size: " << map.size();
	createPicture(1000, 1000, map, "mando1.bmp");

	map = makeMap(1000, 1000, -.5, .5, .5, -.5, function);
	cout << "Map size: " << map.size();
	createPicture(1000, 1000, map, "mando2.bmp");


	map = makeMap(1000, 1000, -.25, .25, .25, -.25, function);
	cout << "Map size: " << map.size();
	createPicture(1000, 1000, map, "mando3.bmp");

	map = makeMap(1000, 1000, -.125, .125, .125, -.125, function);
	cout << "Map size: " << map.size();
	createPicture(1000, 1000, map, "mando4.bmp");
	*/
	return 0;
}
