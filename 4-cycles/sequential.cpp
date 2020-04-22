#include "bits/stdc++.h"
using namespace std;

const int N = 1e6 + 5;
vector<int> adj[N];
vector<int> v1(N, 0);
int num_4cycles = 0;
vector<int>::iterator ls;

int intersectionCount(vector<int> a, vector<int> b) {
	ls = set_intersection(a.begin(), a.end(), b.begin(), b.end(), v1.begin());
  	return (int)(ls - v1.begin());
}

int main(){
	int n,m,x,y;
	cin>>n>>m;

	clock_t start = clock();

	for(int i=0;i<m;i++){
		cin>>x>>y;
		adj[x].push_back(y);
		adj[y].push_back(x);
	}

	for(int i=0;i<=n;i++)
		sort(adj[i].begin(), adj[i].end());

	for(int i=0;i<n;i++){
		cout<<i<<endl;
		for(int j:adj[i]){
			for(int k:adj[j]){
				if(i == k) continue;
				num_4cycles += (intersectionCount(adj[i], adj[k]) - 1);
			}
		}
	}
	cout<<"Number of 4-cycles -> "<<num_4cycles/8<<endl;

	clock_t end = clock();
    printf("Time %f\n", ((float)(end - start))/CLOCKS_PER_SEC);

}
