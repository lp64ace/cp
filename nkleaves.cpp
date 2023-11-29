#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>
 
#define MaxN    100001
#define MaxK    11
 
struct line {
	int64_t a;
	int64_t b;
};
 
int K, N;
std::vector<int64_t> w;
std::vector<line> l;
int64_t dp[MaxK][MaxN];
int64_t sum[MaxN], msum[MaxN];
 
/* -------------------------------------------------------------------- */
/** \name Prefix Sum Cost
 * \{ */
 
void init(){
    for(int64_t i = 1; i <= N; i++){
        sum[i] = sum[i-1] + w[i], msum[i] = msum[i-1] + i*w[i];
    }
} 
int64_t cost(int64_t l, int64_t r){ return msum[r]-msum[l]-l*(sum[r]-sum[l]); }
 
/** \} */
 
/* -------------------------------------------------------------------- */
/** \name Line Definition
 * \{ */
 
int64_t a(int k, int j) { return -j; }
int64_t b(int k, int j) { return dp[k - 1][j - 1] - msum[j] + int64_t(j) * sum[j]; }
int64_t c(int i) { return msum[i]; }
int64_t x(int i) { return sum[i]; }
 
/** \} */
 
/* -------------------------------------------------------------------- */
/** \name Utility Functions
 * \{ */
 
double intersect(const line& l1, const line& l2) { return (double(l1.b) - double(l2.b)) / (double(l2.a) - double(l1.a)); }
 
std::pair<double, double> line_range(const line& prev, const line& mid, const line& next) {
	return {intersect(prev, mid) , intersect(mid, next)};
}
std::pair<double, double> line_range(const int64_t& index) {
	const int64_t n = l.size() - 1;
	double lo = (index == 0) ? LLONG_MIN : intersect(l[index - 1], l[index]);
	double hi = (index == n) ? LLONG_MAX : intersect(l[index + 1], l[index]);
	return {lo, hi};
}
bool is_valid_range(const std::pair<double, double>& range) {
	return range.first < range.second;
}
 
bool fix_last_line(const line& newline) {
	if(l.size() >= 2) {
		const line mid = l[l.size() - 1], prev = l[l.size() - 2];
		if(!is_valid_range(line_range(prev, mid, newline))) {
			l.pop_back();
			return true;
		}
	}
	return false;
}
 
/** \} */
 
/* -------------------------------------------------------------------- */
/** \name Convex Hull
 * \{ */
 
void drop_all_lines() { l = std::vector<line>(); }
 
void insert_line(int64_t k, int64_t j) {
	const line& newline = line({a(k, j), b(k, j)});
	while(fix_last_line(newline)) {
		continue;
	}
	l.push_back(newline);
}
 
int64_t quick_search(int64_t i) {
	int64_t left = 0, right = l.size() - 1;
	while(left < right) {
		int64_t mid = (left + right) / 2;
		const std::pair<double, double> range = line_range(mid);
		if(range.first <= x(i) && x(i) <= range.second) {
			return l[mid].a * x(i) + l[mid].b;
		} else if (x(i) <= range.first) {
			right = mid - 1;
		} else {
			left = mid + 1;
		}
	}
	return l[left].a * x(i) + l[left].b;
}
 
/** \} */
 
int main(){
    scanf(" %d%d", &N, &K);
    w.resize(N+1);
    for(int i=1; i<=N; i++){
        scanf(" %lld", &w[i]);
    }
	init();
    for(int i = 1; i <= N; i++) {
        dp[1][i]=cost(1, i);
    }
	for(int k = 2; k <= K; k++) {
		drop_all_lines();
		for(int i = 1; i <= N; i++) {
			/*
			 * Since the old loop would be for j <= i we avoid adding the lines that come later than 
			 * i beforehand to avoid logic complications, altough in action of course adding more leaves 
			 * in a pile would never be optimal, so adding them beforehand would have no diference in the 
			 * result.
			 */
			insert_line(k, i);
			dp[k][i] = std::min(quick_search(i) + c(i), dp[k - 1][i]);
		}
	}
	printf("%lld\n", dp[K][N]);
	return 0;
}
 