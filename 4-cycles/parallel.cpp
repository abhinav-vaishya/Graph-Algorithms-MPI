#include "bits/stdc++.h"
#include "mpi.h"

using namespace std;
typedef long long ll;

const int N = 1e6 + 5;
vector<int> v1(N, 0);
vector<int>::iterator ls;

ll intersectionCount(vector<ll> a, vector<ll> b) {
	ls = set_intersection(a.begin(), a.end(), b.begin(), b.end(), v1.begin()); 
  	return (ll)(ls - v1.begin());
}

int main( int argc, char **argv ) {
    int rank, num_procs, num_nodes, num_edges;
    vector<vector<ll> > adj(N, vector<ll>(0, 0));

    /*Taking input from the file and storing as adjacency list*/
    FILE* file;
    file = fopen(argv[1], "r+");
    fscanf(file, "%d %d", &num_nodes, &num_edges);
    for(int i=0;i<num_edges;i++){
        ll u, v;
        fscanf(file, "%lld %lld", &u, &v);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    fclose(file);

    for(int i=0;i<num_nodes;i++){
		sort(adj[i].begin(), adj[i].end());
	}

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &num_procs );

    /*synchronize all processes*/
    MPI_Barrier(MPI_COMM_WORLD);
    double tbeg = MPI_Wtime();

    /*calculating the number of nodes for each process, the remaining nodes will be run by process 0*/
    int nodes_per_proc = num_nodes / num_procs;
    int rem_nodes = num_nodes % num_procs;
    int start_ind = rem_nodes + (rank*nodes_per_proc);
    int end_ind = rem_nodes + ((rank+1)*nodes_per_proc);
    if(rank == 0) start_ind = 0;

    /*calculating the number of triangles in each process*/
    ll num_triangles = 0;
    for(int i=start_ind;i<end_ind;i++){
        for(int j:adj[i]){
            for(int k:adj[j]){
		if(k==i) continue;
                num_triangles += (intersectionCount(adj[i], adj[k]) - 1);
            }
        }
    }


    MPI_Barrier( MPI_COMM_WORLD );
    ll global_sum;
    MPI_Reduce(&num_triangles, &global_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    double elapsedTime = MPI_Wtime() - tbeg;
    double maxTime;
    MPI_Reduce( &elapsedTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );
    if ( rank == 0 ) {
        printf("Total number of triangles : %lld\n", global_sum/8);
        printf("Total time (s): %f\n", maxTime );
    }

    /* shut down MPI */
    MPI_Finalize();
    return 0;
}
