#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<string>
#include<cstring>
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cassert>

std::map< std::string, std::pair<int, int> > dict;


int totalMemory;
int calls;
const int MAXN = 16;// 262144;
// const int MAXN = 262144;// 262144;
const int TREMAXN = MAXN * 2;
int start[TREMAXN];
int end[TREMAXN];


void build(int* t, int v, int tl, int tr){
	//std::cout << "build " << tl << ", " << tr << std::endl;
	if (tl == tr){
		//t[v] = a[tl];
		//std::cout << "node = " << v << std::endl;
	} else {
		int tm = (tl + tr) / 2;
		build (t, v*2, tl, tm);
		build (t, v*2+1, tm+1, tr);
		t[v] = std::max(t[v*2], t[v*2+1]);
	}
}

int sum(int* t, int v, int tl, int tr, int l, int r){
	if (l > r)
		return 0;
	if (l == tl && r == tr)
		return t[v];
	int tm = (tl + tr) / 2;
	return std::max(sum (t, v*2, tl, tm, l, std::min(r,tm)), sum (t, v*2+1, tm+1, tr, std::max(l,tm+1), r));
}

void update(int* T, int i, int x){
	T[i] = x;
	while (i /= 2){
		T[i] = std::max(T[2 * i], T[2 * i + 1]);
	}
}

void cantAllocate(){
	std::cout << "-1" << std::endl;
}

void allocate(std::string processName, int lengthAlloc){
	//std::cout << "allocate " << lengthAlloc << " for " << processName << std::endl;
	int v = 1;
	while(v < MAXN){
		if(start[2 * v] >= lengthAlloc){
			v = 2 * v;
		} else {
			v = 2 * v + 1;
		}
	}
	// now v is right place to allocate from
	int lengthFree = start[v];
	int rest = lengthFree - lengthAlloc;
	if(rest > 0){
		update(start, v, 0);
		update(start, v + lengthAlloc, rest);
		update(end, v + lengthFree - 1, rest);
	} else {
		update(start, v, 0);
		update(end, v + lengthFree - 1, 0);
	}
	dict.insert(std::pair< std::string, std::pair<int, int> >(processName, std::pair<int, int>(v, lengthAlloc)));
	std::cout << v - MAXN << std::endl;
}

void free(std::string processName){
	int firstTakenPlace = (*(dict.find(processName))).second.first;
	int len = (*(dict.find(processName))).second.second;
	int lastTakenPlace = firstTakenPlace + len - 1;
	int freeToTheLeft = 0;
	int freeToTheRight = 0;
	
	if(firstTakenPlace > MAXN){
		freeToTheLeft = end[firstTakenPlace - 1];
	}
	if(lastTakenPlace < MAXN + totalMemory){
		freeToTheRight = start[lastTakenPlace + 1];
	}
	
	int newLength = len + freeToTheLeft + freeToTheRight;
	
	update(start, firstTakenPlace - freeToTheLeft, newLength);
	if(freeToTheLeft){
		update(end, firstTakenPlace - 1, 0);
	}
	update(end, firstTakenPlace + len + freeToTheRight - 1, newLength);
	if(freeToTheRight){
		update(start, firstTakenPlace + len, 0);
	}
	dict.erase(processName);
	
}

void printArrays(){
	std::cout << "start : [";
	for(int i = MAXN; i < TREMAXN; i++){
		std::cout << start[i] << " ";
	}
	std::cout << "]" << std::endl;
	std::cout << "end   : [";
	for(int i = MAXN; i < TREMAXN; i++){
		std::cout << end[i] << " ";
	}
	std::cout << "]" << std::endl;
	std::cout << std::endl << std::endl << std::endl;
}

int main(){
	freopen("G_alloc.in", "rt", stdin);
	
	std::fill_n (start, 2 * MAXN, 0);
	std::fill_n (end, 2 * MAXN, 0);
	
	std::cin >> totalMemory >> calls;
	//std::cout << "totalMemory = " << totalMemory << std::endl;
	//std::cout << "calls = " << calls << std::endl;
	
	
	
	start[MAXN] = totalMemory;
	end[MAXN + totalMemory - 1] = totalMemory;
	
	build(start, 1, 0, MAXN - 1);
	build(end, 1, 0, MAXN - 1);
	
	printArrays();
	
	std::string action;
	std::string processName;
	int amount;
	for(int i = 0; i < calls; i++){
		std::cin >> action >> processName;
		//std::cout << action << " -- " << processName <<std::endl;;
		if(action == "ALLOCATE"){
			std::cin >> amount;
			if(start[1] < amount){
				cantAllocate();
			} else {
				allocate(processName, amount);
			}
		} else {
			free(processName);
		}
		printArrays();
	}
	
	
	return 0;
}