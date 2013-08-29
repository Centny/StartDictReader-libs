/*
 * Sdr.cpp
 *
 *  Created on: Aug 28, 2013
 *      Author: cny
 */

#include "Sdr.h"
namespace sdr {
void int2byte(int val, char* buf) {
	buf[0] = val;
	buf[1] = val >> 8;
	buf[2] = val >> 16;
	buf[3] = val >> 24;
}
int byte2int(char* buf) {
	int val = 0;
	val += buf[3];
	val = val << 8;
	val += buf[2];
	val = val << 8;
	val += buf[1];
	val = val << 8;
	val += buf[0];
	return val;
}
void offset2buf(string& chars, int offset, char* buf, int count) {
	memcpy(buf, chars.c_str(), count);
	int2byte(offset, buf + count);
}
bool fexist(const char* path) {
	fstream fs_(path, ios::in);
	bool res = fs_.is_open();
	fs_.close();
	return res;
}
int ccmp_c(const char* a, const char* b, int count) {
	int dis = 'a' - 'A';
	char ta[count], tb[count];
	for (int i = 0; i < 4; i++) {
		if (a[i] >= 'A' && a[i] <= 'Z') {
			ta[i] = a[i] + dis;
		} else {
			ta[i] = a[i];
		}
		if (b[i] >= 'A' && b[i] <= 'Z') {
			tb[i] = b[i] + dis;
		} else {
			tb[i] = b[i];
		}
	}
	return strncmp(ta, tb, count);
}
Sdr::Sdr(string rpath, string name) {
	this->rpath = rpath;
	this->name = name;
	this->idx = this->dict = this->edx = 0;
	this->edxCount = this->edxSize = this->entryCount = 0;
}

Sdr::~Sdr() {

}
string Sdr::idxpath() {
	return this->rpath + "/" + this->name + ".idx";
}
string Sdr::edxpath() {
	return this->rpath + "/" + this->name + ".edx";
}
string Sdr::dictpath() {
	return this->rpath + "/" + this->name + ".dict";
}
string Sdr::infopath() {
	return this->rpath + "/" + this->name + ".ifo";
}
string Sdr::info(string key) {
	return this->infoes[key];
}
//
string Sdr::loadDict() {
	string fpath = this->infopath();
	string epath = this->edxpath();
	string ipath = this->idxpath();
	string dpath = this->dictpath();
	fstream info(fpath.c_str(), ios::in | ios::binary);
	if (!info.is_open()) {
		return "open .ifo error:" + fpath;
	}
	string sbuf;
	getline(info, sbuf);
	while (!info.eof()) {
		sbuf.clear();
		getline(info, sbuf);
		size_t p = sbuf.find("=");
		if (p < 0) {
			continue;
		}
//		cout << sbuf.substr(0, p) << ":" << sbuf.substr(p + 1) << endl;
		this->infoes[sbuf.substr(0, p)] = sbuf.substr(p + 1);
	}
	info.close();
	this->edx = new fstream(epath.c_str(), ios::in);
	if (!this->edx->is_open()) {
		this->unloadDict();
		return "open .edx error:" + epath;
	}
	this->edx->read(buf, EDX_HSIZE);
	this->edxCount = byte2int(buf);
	this->entryCount = this->edxCount + 4;
	this->edxSize = byte2int(buf + 8);
	this->idx = new fstream(epath.c_str(), ios::in);
	if (!this->idx->is_open()) {
		this->unloadDict();
		return "open .idx error:" + ipath;
	}
	this->dict = new fstream(dpath.c_str(), ios::in);
	if (!this->dict->is_open()) {
		this->unloadDict();
		return "open .dict error:" + dpath;
	}
	return "";
}
void Sdr::unloadDict() {
	if (this->dict) {
		this->dict->close();
		delete this->dict;
		this->dict = 0;
	}
	if (this->edx) {
		this->edx->close();
		delete this->edx;
		this->edx = 0;
	}
	if (this->idx) {
		this->idx->close();
		delete this->idx;
		this->idx = 0;
	}
}
//
string Sdr::find(string word) {
	const char* tword = word.c_str();
	int wsize = word.length();
	int beg = 0, end = this->edxSize;
	char bufa[this->entryCount], bufb[this->entryCount];
	int cmp = wsize > this->edxCount ? this->edxCount : word.size();
	//check begin
	this->edx->seekg(EDX_HSIZE);
	this->edx->read(bufa, this->entryCount);
	if (ccmp_c(bufa, tword, cmp) == 0) {
		this->edx->read(bufb, this->entryCount);
		return this->find(byte2int(bufa + this->edxCount),
				byte2int(bufb + this->edxCount), word);
	}
	this->edx->seekg(
			this->edxSize * this->entryCount + EDX_HSIZE - this->entryCount);
	this->edx->read(bufb, this->entryCount);
	if (ccmp_c(bufb, tword, cmp) == 0) {
		return this->find(byte2int(bufb + this->edxCount), -1, word);
	}
	int center, cres;
	while (beg < end) {
		center = (end + beg) / 2;
		int oft = center * this->entryCount + EDX_HSIZE;
		this->edx->seekg(oft);
		this->edx->read(bufa, this->entryCount);
//		cout << oft << ":" << bufa << endl;
		cres = ccmp_c(bufa, tword, cmp);
		if (cres == 0) {
			this->edx->read(bufb, this->entryCount);
			return this->find(byte2int(bufa + this->edxCount),
					byte2int(bufb + this->edxCount), word);
		} else if (cres < 0) {
			beg = center;
//			cout << center << ",beg:" << string(bufa, this->edxCount) << endl;
		} else {
			end = center;
//			cout << center << ",end:" << string(bufa, this->edxCount) << endl;
		}
	}
	return "";
}
string Sdr::find(int beg_idx, int end_idx, string word) {
	cout << beg_idx << "---" << end_idx << endl;
	return "";
}
//
string Sdr::createEdx(int ecount) {
	string epath = this->edxpath();
	string ipath = this->idxpath();
	if (fexist(epath.c_str())) {
		return "edx file already exist";
	}
	fstream ifs(ipath.c_str(), ios::in);
	if (!ifs.is_open()) {
		ifs.close();
		return "open .idx file error:" + ipath;
	}
	fstream efs(epath.c_str(), ios::out);
	if (!efs.is_open()) {
		efs.close();
		return "open edx file error:" + epath;
	}
	string sbuf;
	int cbuf_len = ecount + 4;
	char cbuf[cbuf_len];
	char tbuf[256];
	int offset = 0;
	int esize = 0;
	//
	efs.seekp(12);
	//
	getline(ifs, sbuf, '\0');
	ifs.read(tbuf, 8);
	offset2buf(sbuf, offset, cbuf, ecount);
	efs.write(cbuf, cbuf_len);
	offset = ifs.tellg();
	esize++;
	while (!ifs.eof()) {
		sbuf.clear();
		getline(ifs, sbuf, '\0');
		ifs.read(tbuf, 8);
		if (ccmp_c(cbuf, sbuf.c_str(), ecount) == 0) {
			continue;
		}
		offset2buf(sbuf, offset, cbuf, ecount);
		efs.write(cbuf, cbuf_len);
		offset = ifs.tellg();
		esize++;
	}
	int2byte(ecount, tbuf);
	int2byte(4, tbuf + 4);
	int2byte(esize, tbuf + 8);
	efs.seekp(0);
	efs.write(tbuf, 12);
	efs.close();
	ifs.close();
	return "";
}
} /* namespace sdr */
