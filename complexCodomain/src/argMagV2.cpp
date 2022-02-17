// Compile this file with: g++ -o bin/<function> src/argMagV2.cpp -Iinclude

#include <cmath>
#include <complex>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "createBitmap.hpp"
using namespace std;

// Please send all complaints about the lazy use of global variables to jlooney314@gmail.com
double ep = .001;
double aCoeff = .000000001;
int pts = 2000;
int mandoIters = 500;

unsigned char* createPicture(int h, int w, map< int , complex<double> > *function, string fn){
	map< int, complex<double> >::iterator fit;
	complex<double> z;
	double ar, mag;

	if(function->size() != h * w) cout << "uh oh";

	Pixel* pixarray = new Pixel[h*w];

	for(fit = function->begin(); fit != function->end(); fit++){
		z = fit->second;
		mag = abs(z);
		ar = arg(z);

		pixarray[fit->first].red = min(5*mag, 255.0);
		pixarray[fit->first].blue = min(40*ar, 255.0);
		pixarray[fit->first].green = min(100*mag, 255.0);

	}

	writeBitmap(h, w, pixarray, fn);

	delete[] pixarray;

	return NULL;
}

// Applies a function to a complex point. Comment out/add whichever function you would like.
complex<double> func(complex<double> z){
	complex<double> output;
	int k;

	output = complex<double>(0,0);
	k=0;
	while(k < mandoIters && abs(output) < 2){
		output = pow(output, 3) + z;
		k++;
	}


	//output = sin(z);
	//output = (((z * z) + complex<double>(-1,0)) * pow((z + complex<double>(-2, -1)), 2))/(z*z + complex<double>(2,2));
	//output = ((z * z * z) + complex<double>(-1, 0));
	//output = z;
	//cout << i << "      " << j << "      " << z << "      " << output << "      " << norm(output) << "                 \r";

	return output;
}

// creates a multimap containing output and input pairs for a function, sorted by magnitude.
multimap< double, pair< complex<double>, complex<double> > > *makeFunction(){
	multimap< double, pair< complex<double>, complex<double> > > *function;
	int i, j, k;
	complex<double> z, inf, output;

	function = new multimap< double, pair< complex<double>, complex<double> > >;

	// starts by taking infinity and adding to function
	//cout << "Size: " << function.size() << endl;

	for( i = -1 * pts; i < pts; i++){
		for( j = -1 * pts; j < pts; j++){
			z = complex<double>(aCoeff * i * i * i, aCoeff * j * j * j);

			output = func(z);
			if(abs(output) > 2) continue; /* If its not in the set, we ain't even want it in the map*/

			function->insert(make_pair(abs(output), make_pair(output, z)));

		}
	}

	cout << "Function map size is: " << function->size() << endl;

	return function;
}


// returns the closest to a  preimage point for a given z in the image
complex<double> preImage(complex<double> z, multimap< double , pair< complex<double>, complex<double> > > *func){
	double mag = abs(z);
	double d1, best;
	map< double, pair< complex<double>, complex<double> > >::const_iterator low, high, fit, bit;
	size_t i = 0;

	cout << "Finding preimage of: " << z << "         \r";

	low = func->lower_bound(mag-(ep * mag));
	high = func->upper_bound(mag+(ep * mag));

	bit = low;
	best = 100000;
	for(fit = low; fit != high; fit++){
		d1 = sqrt(pow(z.real() - fit->second.first.real(), 2) + pow(z.imag() - fit->second.first.imag(), 2));
		if(d1 < best) {
			best = d1;
			bit = fit;
		}

	}

	return bit->second.second;
}

// creates a map over a specified boundary to be passed to create the picture.
// the int of the map represents the evenly spaced codomain as specified from the boundary.
// the attached float is the first found point in the preimiage of that point.
map< int , complex<double> > *makeMap(int h, int w, double realLeft, double realRight,
						  double imagTop, double imagBot, string fn,
						  multimap< double, pair< complex<double>, complex<double> > > *function){

	map< int, complex<double> > *ret;
	int i, j, k;
	complex<double> z, inf, output;

	ret = new map< int, complex<double> >;

	ofstream fout;
	fout.open(fn);

	fout << "Interval: [ " << realLeft << " , " << realRight << " ] real and [ " << imagBot << " , " << imagTop << " ] imaginary." << endl;
	fout << "Points in function map: " << (2 * pts) * (2 * pts) << "  |   Coeffiecent for n^3: " << aCoeff << endl;
	fout << "Iterations: " << mandoIters << "  |  Epsilon on preImage search: " << ep;

	for(i = 0; i < w * h; i++){
		complex<double> z(realLeft + ((i%w) * ((realRight - realLeft)/w)),
					imagTop - ((i/w) * ((imagTop - imagBot)/h)));

		complex<double> pre = preImage(z, function);

		fout << z << " has preimage of " << pre << endl;

		ret->insert(make_pair(i, pre));

	}

	fout.close();

	return ret;
}

int main(){
	multimap< double, pair< complex<double>, complex<double> > > *function, *tmp;
	multimap< double, pair< complex<double>, complex<double> > >::iterator mit;
	map< int , complex<double> > *map;
	double left, right, top, bot;
	unsigned int w, h;
	string fn, fnt, fnb;
	stringstream ss;

	double a;
	complex<double> next;

	/* Use this code for general cases:

	cout << "Define function: " << endl;
	cout << "Enter half the points in each direction (2000 default): ";
	cin >> pts;
	cout << "Enter a coefficient for n^3 (.00000001n^3 defualt): ";
	cin >> aCoeff;
	cout << "Enter number of iterations in mandlebrot function: ";
	cin >> mandoIters;

	cout << "Define interval: " << endl;
	cout << "Left real: "; cin >> left;
	cout << "Right real: "; cin >> right;
	cout << "Top imaginary: "; cin >> top;
	cout << "Bottom imaginary: "; cin >> bot;

	cout << "Define epsilon for preImage search: "; cin >> ep;

	cout << "Define file: " << endl;
	cout << "Width: "; cin >> w;
	cout << "Height: "; cin >> h;
	cout << "File name (without file extension): "; cin >> fn;

	fnt = fn + ".txt";
	fnb = fn + ".bmp";

	function = makeFunction();

	map = makeMap(h, w, left, right, top, bot, fnt, function);
	createPicture(h, w, map, fnb);

	delete function;
	delete map;

	*/

	/* Use this code for a mando/multibrot w/ several iters: */

	left = -.5;
	right = .5;
	top = .5;
	bot = -.5;
	w = 1000;
	h = 1000;


	mandoIters = 729;

	function = makeFunction();


	while(mandoIters < 999){


		string numbers, zeros;
		stringstream ss;

		ss << mandoIters;
		ss >> numbers;

		zeros = "";
		for(int i = 0; i < 4 - numbers.length(); i++) zeros = "0" + zeros;
		numbers = zeros + numbers;

		fnb = "../pics/multis3/" + numbers + "iter.bmp";
		fnt = "../text/multis3/" + numbers + "iter.txt";

		map = makeMap(h, w, left, right, top, bot, fnt, function);
		createPicture(h, w, map, fnb);

		tmp = new multimap< double, pair<complex <double>, complex <double> > >;

		for( mit = function->begin(); mit != function->end(); mit++ ){
			next = pow(mit->second.first, 3) + mit->second.second;

			if (abs(next) > 2) continue;

			tmp->insert(make_pair(abs(next), make_pair(next, mit->second.second)));
		}

		delete function;
		delete map;
		function = tmp;

		cout << "Function map size is: " << function->size() << "           " << endl;

		mandoIters += 1;

	}

	delete function;
	delete map;


	return 0;
}
