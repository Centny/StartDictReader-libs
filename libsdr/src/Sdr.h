/*
 * Sdr.h
 *
 *  Created on: Aug 28, 2013
 *      Author: cny
 */

#ifndef SDR_H_
#define SDR_H_
#include<string>
#include<fstream>
#include<iostream>
#include<map>
namespace sdr {
using namespace std;
#define BSIZE 1024
#define EDX_HSIZE 12
class Sdr {
private:
	string rpath;
	string name;
	fstream *idx;
	fstream *dict;
	fstream *edx;
	int edxCount;
	int edxSize;
	int entryCount;
	char buf[BSIZE];
	map<string, string> infoes;
public:
	Sdr(string rpath, string name);
	virtual ~Sdr();
	string idxpath();
	string edxpath();
	string dictpath();
	string infopath();
	//
	string info(string key);
	string loadDict();
	void unloadDict();
	string find(string word);
	string find(int beg_idx, int end_idx, string word);
	//
	string createEdx(int ecount);
};

} /* namespace sdr */
#endif /* SDR_H_ */
