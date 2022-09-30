#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

const int MaxN = 40000;
const int MaxLogN = 16;
const int MaxM = 100000;

int weight [ 2 * MaxN ];
std::vector<int> adj [ MaxN ];
int start [ MaxN ] , end [ MaxN ];
int order [ 2 * MaxN ];
int par [ MaxN ][ MaxLogN ];
int depth [ MaxN ];

static int timer = 0;

void dfs ( int v , int p = -1 ) {
	start [ v ] = ++timer;
	order [ timer ] = v;

	par [ v ][ 0 ] = p;
	if ( p != -1 ) {
		depth [ v ] = depth [ p ] + 1;
	}
	for ( int i = 1; i < MaxLogN; i++ ) {
		if ( par [ v ][ i - 1 ] != -1 ) {
			par [ v ][ i ] = par [ par [ v ][ i - 1 ] ][ i - 1 ];
		}
	}
	for ( auto u : adj [ v ] ) {
		if ( p != u ) {
			dfs ( u , v );
		}
	}
	end [ v ] = ++timer;
	order [ timer ] = v;
}
int lca ( int v , int u ) {
	if ( v == 1 || u == 1 )
		return 1;
	if ( depth [ v ] < depth [ u ] )
		std::swap ( v , u );
	for ( int i = MaxLogN - 1; i >= 0; i-- ) {
		if ( par [ v ][ i ] + 1 and depth [ par [ v ][ i ] ] >= depth [ u ] ) {
			v = par [ v ][ i ];
		}
	}
	if ( v == u )
		return v;
	for ( int i = MaxLogN - 1; i >= 0; i-- ) {
		if ( par [ v ][ i ] != par [ u ][ i ] ) {
			v = par [ v ][ i ] , u = par [ u ][ i ];
		}
	}
	return par [ v ][ 0 ];
}
int bucket [ MaxM ];
struct node {
	int l , r , id , lca;
	bool operator<( const node &rhs ) const {
		// sort by bucket and then by ending point
		return ( bucket [ l ] == bucket [ rhs.l ] ) ? ( r < rhs.r ) : ( bucket [ l ] < bucket [ rhs.l ] );
	}
}q [ MaxM ];

int buckets;
int ans [ MaxM ];
int res;
int vis [ MaxN ] , cnt [ MaxN ];

inline void update ( int x ) {
	if ( vis [ x ] ) // the second time we visis this node is when we leave it so we need to decrease the counter
		if ( --cnt [ weight [ x ] ] == 0 )
			res--;
	if ( !vis [ x ] ) // the first time we visit this node we need to increment the counter
		if ( cnt [ weight [ x ] ]++ == 0 )
			res++;
	vis [ x ] = !vis [ x ];
}
int main ( ) {
	int n , m;
	scanf ( " %d%d" , &n , &m );
	int tmp_weight [ MaxN ];
	long long mxx = -1;
	for ( int i = 1; i <= n; i++ ) {
		scanf ( " %d" , &weight [ i ] );
		tmp_weight [ i ] = weight [ i ];
		mxx = std::max ( ( long long ) weight [ i ] , mxx );
	}

	if ( mxx >= MaxN ) { // compress the weights, we dont need to care about the actual value.
		std::sort ( tmp_weight + 1 , tmp_weight + n + 1 );
		for ( int i = 1; i <= n; ++i )
			weight [ i ] = std::lower_bound ( tmp_weight + 1 , tmp_weight + n + 1 , weight [ i ] ) - tmp_weight;
	}
	for ( int i = 1; i <= n - 1; i++ ) {
		int u , v;
		scanf ( " %d%d" , &u , &v );
		adj [ u ].push_back ( v );
		adj [ v ].push_back ( u );
	}
	dfs ( 1 );
	for ( int i = 1; i <= m; i++ ) {
		int u , v;
		scanf ( " %d%d" , &u , &v );

		q [ i ].lca = lca ( u , v );
		if ( start [ u ] > start [ v ] ) { // we are gonna find the starting node first.
			std::swap ( u , v );
		}
		if ( q [ i ].lca == u ) {
			q [ i ].l = start [ u ];
			q [ i ].r = start [ v ];
		} else {
			q [ i ].l = end [ u ];
			q [ i ].r = start [ v ];
		}
		q [ i ].id = i;
	}
	// register buckets
	buckets = sqrt ( timer );
	for ( int i = 1; i <= timer; i++ ) {
		bucket [ i ] = ( i - 1 ) / buckets + 1;
	}
	// sort queries based on buckets
	std::sort ( q + 1 , q + m + 1 );

	int curL = 1;
	int curR = 0;
	for ( int i = 1; i <= m; i++ ) {
		while ( curL < q [ i ].l ) update ( order [ curL++ ] ); // move left to the query left ->
		while ( curL > q [ i ].l ) update ( order [ --curL ] ); // move left to the query left <-
		while ( curR < q [ i ].r ) update ( order [ ++curR ] ); // move right to the query right ->
		while ( curR > q [ i ].r ) update ( order [ curR-- ] ); // move right to the query right <-
		int u = order [ curL ]; // retrieve the starting node
		int v = order [ curR ]; // retrieve the ending node

		// FIX FOR : lca appears twice
		if ( q [ i ].lca != u and q [ i ].lca != v )  update ( q [ i ].lca );
		ans [ q [ i ].id ] = res;
		if ( q [ i ].lca != u and q [ i ].lca != v )  update ( q [ i ].lca );
	}

	for ( int i = 1; i <= m; i++ ) {
		printf ( "%d\n" , ans [ i ] );
	}

	return 0;
}
