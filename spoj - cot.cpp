#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <climits>
#include <cstring>
#include <map>
#include <cassert>
#include <vector>
#include <cmath>
 
#define MAXN			100'500
 
int N ;
int nWeight [ MAXN ] = { 0 } ;
int nWeightIdx [ MAXN ] = { 0 } ;
int rnWeightIdx [ MAXN ] = { 0 } ;
int nLevel [ MAXN ] = { 0 } ;
int expParent [ MAXN ][ 20 ] = { 0 } ;
std::vector<int> adj [ MAXN ] ;
 
int Q ;
 
int MAXK ;
 
int cmp ( const void *a , const void *b ) {
	return nWeight [ *( const int * ) a ] - nWeight [ *( const int * ) b ] ;
}
 
void compress_weights ( ) {
	for ( int i = 1 ; i <= N ; i++ )
		nWeightIdx [ i ] = i ;
	qsort ( nWeightIdx + 1 , N , sizeof ( int ) , cmp ) ;
	for ( int i = 1 ; i <= N ; i++ )
		rnWeightIdx [ nWeightIdx [ i ] ] = i ;
}
 
struct node {
	node *left , *right ;
	int data ;
};
 
node *ver [ MAXN ] ;
 
int upgrade ( node *prev , node *curr , int left , int right , int n ) {
	int node_idx = rnWeightIdx [ n ] ;			// location of the nodes weight if sorted
	if ( left == right ) {
		return curr->data = 1 ;
	}
	int mid = ( left + right ) / 2 ;
	if ( node_idx <= mid ) {
		curr->right = ( ( prev ) ? ( prev->right ) : ( nullptr ) ) ;
		upgrade ( ( ( prev ) ? ( prev->left ) : ( nullptr ) ) , curr->left = new node ( ) , left , mid , n ) ;
		return curr->data = ( ( curr->right ) ? ( curr->right->data ) : ( 0 ) ) + curr->left->data ;
	}
	// else
	curr->left = ( ( prev ) ? ( prev->left ) : ( nullptr ) ) ;
	upgrade ( ( ( prev ) ? ( prev->right ) : ( nullptr ) ) , curr->right = new node ( ) , mid + 1 , right , n ) ;
	return curr->data = ( ( curr->left ) ? ( curr->left->data ) : ( 0 ) ) + curr->right->data ;
}
 
int query ( node *curr , int left , int right , int w ) {
	if ( left > w or curr == nullptr )
		return 0 ;
	if ( right <= w )
		return curr->data ;
	int mid = ( left + right ) / 2 ;
	return query ( curr->left , left , mid , w ) + query ( curr->right , mid + 1 , right , w ) ;
}
 
void dfs ( int u , int parent = 0 ) {
	expParent [ u ][ 0 ] = parent ;
	nLevel [ u ] = nLevel [ parent ] + 1 ;
	upgrade ( ver [ parent ] , ver [ u ] = new node ( ) , 1 , N , u ) ;
	for ( int i = 0 ; i < adj [ u ].size ( ) ; i++ )
		if ( parent != adj [ u ][ i ] )
			dfs ( adj [ u ][ i ] , u ) ;
}
 
int lca ( int u , int v ) {
	if ( nLevel [ u ] < nLevel [ v ] )
		std::swap ( u , v ) ;
	for ( int k = MAXK ; k>=0 ; k-- )
		if ( nLevel [ expParent [ u ][ k ] ] >= nLevel [ v ] )
			u = expParent [ u ][ k ] ;
	if ( u == v )
		return u ;
	for ( int k = MAXK ; k>=0 ; k-- ) {
		if ( expParent [ u ][ k ] != expParent [ v ][ k ] ) {
			u = expParent [ u ][ k ] ;
			v = expParent [ v ][ k ] ;
		}
	}
	assert ( expParent [ u ][ 0 ] && expParent [ u ][ 0 ] == expParent [ v ][ 0 ] ) ;
	return expParent [ u ][ 0 ] ;
}
 
int bs ( int u , int v , int k ) {
	int p = lca ( u , v ) ;
	int left = 1 , right = N ;
	int best = N ;
	while ( left <= right ) {
		int mid = ( left + right ) / 2 ;
		int aU = query ( ver [ u ] , 1 , N , mid ) ;
		int aV = query ( ver [ v ] , 1 , N , mid ) ;
		int aP = query ( ver [ p ] , 1 , N , mid ) ;
#ifdef _DEBUG
		printf ( "route %d - > %d has %d nodes with a weight less/equal than %d\n" , 1 , u , aU , nWeight [ nWeightIdx [ mid ] ] ) ;
		printf ( "route %d - > %d has %d nodes with a weight less/equal than %d\n" , 1 , v , aV , nWeight [ nWeightIdx [ mid ] ] ) ;
		printf ( "route %d - > %d has %d nodes with a weight less/equal than %d\n" , 1 , p , aP , nWeight [ nWeightIdx [ mid ] ] ) ;
#endif
		int a = ( ( aU + aV ) - 2 * aP ) + ( rnWeightIdx [ p ] <= mid ) ;
 
		if ( a == k )
			return mid ;
 
		if ( a < k )
			left = mid + 1 ;
		else
			right = mid - 1 ;
	}
	assert ( 0 ) ;
	return -1 ;
}
 
int main ( void ) {
	scanf ( " %d %d" , &N , &Q ) ;
	MAXK = std::ceil ( log2 ( N ) ) ;
	for ( int i = 1 ; i <= N ; i++ )
		scanf ( "%d" , &nWeight [ i ] ) ;
	compress_weights ( ) ;
	for ( int i = 1 ; i < N ; i++ ) {
		int u , v ;
		scanf ( " %d %d" , &u , &v ) ;
		adj [ u ].push_back ( v ) ;
		adj [ v ].push_back ( u ) ;
	}
	ver [ 0 ] = new node ( ) ;
	dfs ( ( rand ( ) % N ) + 1 ) ;
	for ( int k = 1 ; k <= MAXK ; k++ )
		for ( int i = 1 ; i <= N ; i++ )
			expParent [ i ][ k ] = expParent [ expParent [ i ][ k - 1 ] ][ k - 1 ] ;
	for ( int q = 1 ; q <= Q ; q++ ) {
		int u , v , k ;
		scanf ( " %d %d %d" , &u , &v , &k ) ;
		printf ( "%d\n" , nWeight [ nWeightIdx [ bs ( u , v , k ) ] ] ) ;
	}
	return 0 ;
}
 
