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

std::map< std::string, std::pair<int,int> > dict;


const int MAXN = 200000;
std::vector<int> start;
std::vector<int> end;
int totalMemory;
int calls;


const int INF = 1000000000;

void build_tree(std::vector<int>& V){
    // размер, доведённый до степени двойки
    int n = (1 << (int)(log(n - 1) + 1));
    V.resize(2 * n, INF);	

    // инициализируем листы
    for (int i = n; i < 2 * n; i++)
        V[i] = V[i - n];

    // и все остальные вершины
    for (int i = n - 1; i > 0; i--)
        V[i] = std::min(V[2 * i], V[2 * i + 1]);
}

int rmq_up(std::vector<int>& T, int l, int r){
    int ans = INF;
    int n = T.size() / 2;
    l += n - 1, r += n - 1;
    while (l <= r)
    {
        // если l - правый сын своего родителя, 
        // учитываем его фундаментальный отрезок
        if (l & 1)
            ans = std::min(ans, T[l]);
        // если r - левый сын своего родителя, 
        // учитываем его фундаментальный отрезок
        if (!(r & 1))
            ans = std::min(ans, T[r]);
        // сдвигаем указатели на уровень выше
        l = (l + 1) / 2, r = (r - 1) / 2;
    }		
    return ans;
}
void update(std::vector<int>& T, int i, int x){
    int n = T.size() / 2;
	i += n - 1;
    T[i] = x;
    while (i /= 2)
        T[i] = std::min(T[2 * i], T[2 * i + 1]);
}

int main(){
	std::cin >> totalMemory, calls;
	
	start.resize(2 * totalMemory, 0);
	end.resize(2 * totalMemory, 0);
	start[0] = totalMemory;
	end[totalMemory - 1] = totalMemory;
	
	
	
	return 0;
}