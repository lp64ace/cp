#include <iostream>
#include <functional>
#include <cmath>
#include <climits>
#include <cstring>
#include <map>
#include <malloc.h>
#include <assert.h>

int Mat [ 25'000'000 ];

int Rows , Cols , T;

int Coord ( int x , int y ) {
	return y * Cols + x;
}

int GetX ( int coord ) {
	return coord % Cols;
}

int GetY ( int coord ) {
	return coord / Cols;
}

#define _INT_MAX			0x7f7f7f7f
#define _INT_MIN			0x00000000

class SegmentTree {
	int N;
	std::pair<int , int> *MaxArr; // < Value , Index - Rightest >
	std::pair<int , int> *MinArr; // < Value , Index - Leftest >
	int MinWindow;
	int MaxWindow;
protected:
	void ConstructUtil_row ( int index , int left , int right , int *arr , int row ) {
		this->MaxArr [ index ].first = _INT_MIN; this->MaxArr [ index ].second = -1;
		this->MinArr [ index ].first = _INT_MAX; this->MinArr [ index ].second = -1;
		if ( left == right ) {
			this->MaxArr [ index ].first = this->MinArr [ index ].first = arr [ Coord ( left , row ) ];
			this->MaxArr [ index ].second = this->MinArr [ index ].second = left;
			return ;
		}

		int mid = ( left + right ) / 2;
		ConstructUtil_row ( index * 2 + 1 , left , mid , arr , row );
		ConstructUtil_row ( index * 2 + 2 , mid + 1 , right , arr , row );

		if ( this->MaxArr [ index * 2 + 2 ].first >= this->MaxArr [ index * 2 + 1 ].first ) {
			this->MaxArr [ index ] = this->MaxArr [ index * 2 + 2 ];
		} else {
			this->MaxArr [ index ] = this->MaxArr [ index * 2 + 1 ];
		}

		if ( this->MinArr [ index * 2 + 1 ].first <= this->MinArr [ index * 2 + 2 ].first ) {
			this->MinArr [ index ] = this->MinArr [ index * 2 + 1 ];
		} else {
			this->MinArr [ index ] = this->MinArr [ index * 2 + 2 ];
		}
	}

	void ConstructUtil_col ( int index , int left , int right , int *arr , int col ) {
		this->MaxArr [ index ].first = _INT_MIN; this->MaxArr [ index ].second = -1;
		this->MinArr [ index ].first = _INT_MAX; this->MinArr [ index ].second = -1;
		if ( left == right ) {
			this->MaxArr [ index ].first = this->MinArr [ index ].first = arr [ Coord ( col , left ) ];
			this->MaxArr [ index ].second = this->MinArr [ index ].second = left;
			return;
		}

		int mid = ( left + right ) / 2;
		ConstructUtil_col ( index * 2 + 1 , left , mid , arr , col );
		ConstructUtil_col ( index * 2 + 2 , mid + 1 , right , arr , col );

		if ( this->MaxArr [ index * 2 + 2 ].first >= this->MaxArr [ index * 2 + 1 ].first ) {
			this->MaxArr [ index ] = this->MaxArr [ index * 2 + 2 ];
		} else {
			this->MaxArr [ index ] = this->MaxArr [ index * 2 + 1 ];
		}

		if ( this->MinArr [ index * 2 + 1 ].first <= this->MinArr [ index * 2 + 2 ].first ) {
			this->MinArr [ index ] = this->MinArr [ index * 2 + 1 ];
		} else {
			this->MinArr [ index ] = this->MinArr [ index * 2 + 2 ];
		}
	}

	void UpdateUtil_row ( int index , int left , int right , int *arr , int row ) {
		if ( left == right ) {
			if ( this->MaxArr [ index ].first < arr [ Coord ( left , row ) ] ) {
				this->MaxArr [ index ].first = arr [ Coord ( left , row ) ];
			}
			if ( this->MinArr [ index ].first > arr [ Coord ( left , row ) ] ) {
				this->MinArr [ index ].first = arr [ Coord ( left , row ) ];
			}
			this->MaxArr [ index ].second = this->MinArr [ index ].second = left;
			return;
		}

		int mid = ( left + right ) / 2;
		UpdateUtil_row ( index * 2 + 1 , left , mid , arr , row );
		UpdateUtil_row ( index * 2 + 2 , mid + 1 , right , arr , row );

		if ( this->MaxArr [ index * 2 + 2 ].first >= this->MaxArr [ index * 2 + 1 ].first ) {
			this->MaxArr [ index ] = this->MaxArr [ index * 2 + 2 ];
		} else {
			this->MaxArr [ index ] = this->MaxArr [ index * 2 + 1 ];
		}

		if ( this->MinArr [ index * 2 + 1 ].first <= this->MinArr [ index * 2 + 2 ].first ) {
			this->MinArr [ index ] = this->MinArr [ index * 2 + 1 ];
		} else {
			this->MinArr [ index ] = this->MinArr [ index * 2 + 2 ];
		}
	}

	void UpdateUtil_col ( int index , int left , int right , int *arr , int col ) {
		if ( left == right ) {
			if ( this->MaxArr [ index ].first < arr [ Coord ( col , left ) ] ) {
				this->MaxArr [ index ].first = arr [ Coord ( col , left ) ];
			}
			if ( this->MinArr [ index ].first > arr [ Coord ( col , left ) ] ) {
				this->MinArr [ index ].first = arr [ Coord ( col , left ) ];
			}
			this->MaxArr [ index ].second = this->MinArr [ index ].second = left;
			return;
		}

		int mid = ( left + right ) / 2;
		UpdateUtil_col ( index * 2 + 1 , left , mid , arr , col );
		UpdateUtil_col ( index * 2 + 2 , mid + 1 , right , arr , col );

		if ( this->MaxArr [ index * 2 + 2 ].first >= this->MaxArr [ index * 2 + 1 ].first ) {
			this->MaxArr [ index ] = this->MaxArr [ index * 2 + 2 ];
		} else {
			this->MaxArr [ index ] = this->MaxArr [ index * 2 + 1 ];
		}

		if ( this->MinArr [ index * 2 + 1 ].first <= this->MinArr [ index * 2 + 2 ].first ) {
			this->MinArr [ index ] = this->MinArr [ index * 2 + 1 ];
		} else {
			this->MinArr [ index ] = this->MinArr [ index * 2 + 2 ];
		}
	}

	std::pair<int,int> QueryUtil_max ( int index , int l , int r , int ql , int qr ) {
		if ( r < ql || qr < l ) {
			return { _INT_MIN , -1 };
		}
		if ( ql <= l && r <= qr ) {
			return this->MaxArr [ index ];
		}
		int mid = ( l + r ) / 2;
		auto p1 = QueryUtil_max ( index * 2 + 1 , l , mid , ql , qr );
		auto p2 = QueryUtil_max ( index * 2 + 2 , mid + 1 , r , ql , qr );
		if ( p2.first >= p1.first ) {
			return p2;
		}
		return p1;
	}

	std::pair<int , int> QueryUtil_min ( int index , int l , int r , int ql , int qr ) {
		if ( r < ql || qr < l ) {
			return { _INT_MAX , -1 };
		}
		if ( ql <= l && r <= qr ) {
			return this->MinArr [ index ];
		}
		int mid = ( l + r ) / 2;
		auto p1 = QueryUtil_min ( index * 2 + 1 , l , mid , ql , qr );
		auto p2 = QueryUtil_min ( index * 2 + 2 , mid + 1 , r , ql , qr );
		if ( p1.first <= p2.first ) {
			return p1;
		}
		return p2;
	}

	static size_t NumAlloc ( int N ) {
		return size_t ( pow ( 2 , ceil ( log2 ( N ) ) + 1 ) );
	}
public:
	SegmentTree ( int N ) {
		this->N = N;
#if 0
		this->MaxArr = new std::pair<int , int> [ NumAlloc ( N ) ];
		this->MinArr = new std::pair<int , int> [ NumAlloc ( N ) ];
#else
		this->MaxArr = ( std::pair<int , int> * ) malloc ( sizeof ( std::pair<int , int> ) * NumAlloc ( N ) );
		this->MinArr = ( std::pair<int , int> * ) malloc ( sizeof ( std::pair<int , int> ) * NumAlloc ( N ) );
#endif
		Clear ( );
	}

	~SegmentTree ( ) {
#if 0
		delete [ ]this->MaxArr; this->MaxArr = NULL;
		delete [ ]this->MinArr; this->MinArr = NULL;
#else
		free ( this->MaxArr ); this->MaxArr = NULL;
		free ( this->MinArr ); this->MinArr = NULL;
#endif
	}

	void Clear ( ) {
		MinWindow = 0; MaxWindow = N - 1;
		memset ( this->MaxArr , 0x00 , NumAlloc ( N ) * sizeof ( std::pair<int , int> ) );
		memset ( this->MinArr , 0x7f , NumAlloc ( N ) * sizeof ( std::pair<int , int> ) );
	}

	void ConstructRow ( int *arr , int row ) {
		MinWindow = 0; MaxWindow = N - 1;
		this->ConstructUtil_row ( 0 , 0 , N - 1 , arr , row );
	}

	void ConstructCol ( int *arr , int col ) {
		MinWindow = 0; MaxWindow = N - 1;
		this->ConstructUtil_col ( 0 , 0 , N - 1 , arr , col );
	}

	void ConstructRowEx ( int *arr , int from , int to , int row ) {
		MinWindow = from; MaxWindow = to;
		this->ConstructUtil_row ( 0 , from , to , arr , row );
	}

	void ConstructColEx ( int *arr , int from , int to , int row ) {
		MinWindow = from; MaxWindow = to;
		this->ConstructUtil_col ( 0 , from , to , arr , row );
	}

	void AppendRow ( int *arr , int row ) {
		this->UpdateUtil_row ( 0 , 0 , N - 1 , arr , row );
	}

	void AppendCol ( int *arr , int col ) {
		this->UpdateUtil_col ( 0 , 0 , N - 1 , arr , col );
	}

	int QueryMax ( int l , int r ) {
		return this->QueryUtil_max ( 0 , MinWindow , MaxWindow , l , r ).first;
	}

	int QueryMax ( int i ) {
		return this->QueryUtil_max ( 0 , MinWindow , MaxWindow , i , i ).first;
	}

	int QueryMin ( int l , int r ) {
		return this->QueryUtil_min ( 0 , MinWindow , MaxWindow , l , r ).first;
	}

	int QueryMin ( int i ) {
		return this->QueryUtil_min ( 0 , MinWindow , MaxWindow , i , i ).first;
	}

	std::pair<int , int> QueryMaxEx ( int l , int r ) {
		return this->QueryUtil_max ( 0 , MinWindow , MaxWindow , l , r );
	}

	std::pair<int , int> QueryMaxEx ( int i ) {
		return this->QueryUtil_max ( 0 , MinWindow , MaxWindow , i , i );
	}

	std::pair<int , int> QueryMinEx ( int l , int r ) {
		return this->QueryUtil_min ( 0 , MinWindow , MaxWindow , l , r );
	}

	std::pair<int , int> QueryMinEx ( int i ) {
		return this->QueryUtil_min ( 0 , MinWindow , MaxWindow , i , i );
	}
};

SegmentTree *MatRows [ 50000 ];

int main ( void ) {
#ifdef _DEBUG
	freopen ( "input.txt" , "r" , stdin );
#endif

	scanf ( " %d%d%d" , &Rows , &Cols , &T );

	for ( int row = 0; row < Rows; row++ ) {
		for ( int col = 0; col < Cols; col++ ) {
			scanf ( " %d" , &Mat [ Coord ( col , row ) ] );
#ifdef _DEBUG
			printf ( "%d " , Mat [ Coord ( col , row ) ] );
#endif
		}
		MatRows [ row ] = new SegmentTree ( Cols );
		MatRows [ row ]->ConstructRow ( Mat , row );
#ifdef _DEBUG
		printf ( "\n" );
#endif
	}

	// A dynamic segment tree that stores every row from y1 to y2.
	// Stores all the min/max elements in the range y1 to y2 in their 
	// respective column indices.
	SegmentTree RowsMergedDown ( Cols );

	int best = 0;

	for ( int y1 = 0; y1 < Rows; y1++ ) {
		// Reset Line.
		RowsMergedDown.Clear ( );
		for ( int y2 = y1; y2 < Rows; y2++ ) {
			// Add the new row.
			RowsMergedDown.AppendRow ( Mat , y2 );

			for ( int x1 = 0; x1 < Cols; x1++ ) {
				int left = x1 , right = Cols - 1;
				while ( left < right ) {
					int mid = ( left + right ) / 2;
					// Find the maximum ( increasing sequence ).
					int Max = RowsMergedDown.QueryMax ( x1 , mid );
					// Find the minimum ( decreasing sequence ).
					int Min = RowsMergedDown.QueryMin ( x1 , mid );

					if ( Max - Min <= T ) { // At the end : Max_left - Min_left > T.
						left = mid + 1;
					} else {
						right = mid - 1;
					}
				}

				while ( RowsMergedDown.QueryMax ( x1 , left ) - RowsMergedDown.QueryMin ( x1 , left ) > T ) {
					left--;
				}
				
				// for [y1,y2] [x1,left] has an acceptable AltDifference and therefore all the rects inside it.

				// Maximum element in the left side. ( static )
				int MaxLeft = RowsMergedDown.QueryMax ( x1 );

				while ( left >= x1 ) {
					// Is it worth calculating ?
					if ( ( y2 - y1 + 1 ) * ( left - x1 + 1 ) > best ) {
						// Find Where the Biggest Element is Located [ column index ].
						auto MaxEx = RowsMergedDown.QueryMaxEx ( x1 , left );

						// Check if the max element is located at the top.
						bool ValidTop = MatRows [ y1 ]->QueryMax ( x1 , left ) == MaxEx.first;
						// Check if the max element is located at the bottom.
						bool ValidBot = MatRows [ y2 ]->QueryMax ( x1 , left ) == MaxEx.first;

#if 0
						printf ( "Checking Box [%d %d %d %d] [left:%d,right:%d,top:%d,bottom:%d] %d, area : %d\n" ,
							 x1 , left , y1 , y2 ,
							 MaxLeft ,
							 RowsMergedDown.QueryMax ( left ) ,
							 MatRows [ y1 ]->QueryMax ( x1 , left ) ,
							 MatRows [ y2 ]->QueryMax ( x1 , left ) ,
							 MaxEx.first , ( y2 - y1 + 1 ) * ( left - x1 + 1 ) );
#endif

						// If the maximum element is located in the top , bottom or left side.
						// ( Quick to calculate ).
						if ( ValidTop || ValidBot || MaxLeft == MaxEx.first ) {
							best = ( y2 - y1 + 1 ) * ( left - x1 + 1 );
							break;
						}

						// Maximum element in the right side. ( sliding )
						int MaxRight = RowsMergedDown.QueryMax ( left );

						if ( MaxRight == MaxEx.first ) {
							best = ( y2 - y1 + 1 ) * ( left - x1 + 1 );
							break;
						}
						left--;
					} else {
						break;
					}
				}
			}
		}
	}

	printf ( "%d\n" , best );

	return 0;
}
