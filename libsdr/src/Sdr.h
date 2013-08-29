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

void int2byte(int val, char* buf);
void int2byte2(int val, unsigned char* buf);
int byte2int(char* buf);
int byte2int2(unsigned char* buf);
//
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
	string version();
	int wordcount();
	long idxfilesize();
	string bookname();
	string sametypesequence();
	//
	string loadDict();
	void unloadDict();
	string find(string word);
	string find(int beg_idx, int end_idx, string word);
	string dictm(long beg, long size);
	//
	string createEdx(int ecount);
};

} /* namespace sdr */
#endif /* SDR_H_ */
