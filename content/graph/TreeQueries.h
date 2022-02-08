/**
 * Author: Tyler Hostler-Mathis
 * Date: 2021-11-13
 * License: MIT
 * Source: https://cp-algorithms.com/graph/lca_binary_lifting.html
 * Description: Allows for tree path queries on any associative function
 * Time: construction $O(N \log N)$, queries $O(\log N)$
 * Status: Tested on CSES "Distance Queries"
 */
#pragma once

template <class T, class F> struct TreeQueries {
	vector<vector<pair<int, T>>> adj;
	F f; T le; // any associative function and least val
	int n, l; vector<vi> jmp; vector<vector<T>> assoc; vi dep;
	TreeQueries(vector<vector<pair<int, T>>> &adj,
		F f, T le, int root=0
	) : adj(adj), f(f), le(le), n(sz(adj)), l(ceil(log2(n))),
	jmp(l+1, vi(n)), assoc(l+1, vector<T>(n, le)), dep(n) {
		dfs(root, root, le, 0);
	}
	void dfs(int v, int p, int w, int d) {
		dep[v] = d; jmp[0][v] = p; assoc[0][v] = w;
		for (int lg = 1; lg <= l; ++lg) {
			jmp[lg][v] = jmp[lg-1][jmp[lg-1][v]];
			assoc[lg][v] = f(
					assoc[lg-1][v],
					assoc[lg-1][jmp[lg-1][v]]
				);
		}
		for (auto &[u, w] : adj[v]) if (u != p)
			dfs(u, v, w, d+1);
	}
	pair<int, T> query(int u, int v) { // {lca, assocOnPath}
		if (dep[u] > dep[v]) swap(u, v);
		int lft = dep[v] - dep[u]; T path = le;
		for (int lg = 0; lft; ++lg, lft >>= 1) if (lft & 1) {
			path = f(path, assoc[lg][v]);
			v = jmp[lg][v];
		}
		if (u == v) return {u, path};
		for (int lg = l; lg >= 0; lg--)
			if (jmp[lg][v] != jmp[lg][u]) {
				path = f(path, f(assoc[lg][v], assoc[lg][u]));
				v = jmp[lg][v]; u = jmp[lg][u];
			}
		return {
			jmp[0][v],
			f(path, f(assoc[0][v], assoc[0][u]))
		};
	}
};

