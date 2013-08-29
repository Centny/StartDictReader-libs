//============================================================================
// Name        : libsdr.cpp
// Author      : Centny
// Version     :
// Copyright   : Your copyright notice
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Sdr.h"
using namespace std;
//中
int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
//	string ss = "s";
//	int val = ss.c_str()[1];
//	cout << "ss" << ss.c_str()[1] << "jj" << val << endl;
//	cout << strncmp("s", "s", 2) << endl;
//	cout << strncmp("s", "sa", 2) << endl;
	sdr::Sdr ist("/tmp/stardict-langdao-ec-gb-2.4.2", "langdao-ec-gb");
//	cout << ist.createEdx(4) << endl;
	cout << ist.loadDict() << endl;
	sdr::SdrRes res = ist.find("B k");
	cout << res.matched << endl;
	cout << res.content << endl;
	cout << res.msg << endl;
	return 0;
}
