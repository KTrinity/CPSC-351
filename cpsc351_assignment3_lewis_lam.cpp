//cpsc351_assignment3_lewis_lam.cpp : Synchronization
//Copyright (c) Lewis Lam 10/24/2019

#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <string>

using namespace std;

void writeThread(string thread[]);
mutex mtx;

int main(int argc, const char* argv[]) {
	string a[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26" };
	string b[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K" ,"L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };

	thread threadA(writeThread, a);
	thread threadB(writeThread, b);

	threadA.join();
	threadB.join();

	return 0;
}

void writeThread(string thread[]) {
	mtx.lock();
	ofstream file;
	file.open("synch.txt", fstream::app);
	for (size_t i = 0; i < 20; i++) {
		for (size_t j = 0; j < 26; j++) {
			file << thread[j];
			file << " ";
		}
		file << endl;
	}
	file.close();
	mtx.unlock();
}