#include "path.h"
#include <queue>
#include <stack>
using namespace std;

path::path(const PNG & im, pair<int,int> s, pair<int,int> e)
   :start(s),end(e),image(im){
    BFS();
}

void path::BFS(){
	// initialize working vectors
	vector<vector<bool>> V(image.height(), vector<bool> (image.width(),false));
	vector<vector<pair<int,int>>> P(image.height(), vector<pair<int,int>> (image.width(),end));
	
    queue<pair<int, int>> q;
    q.push(start);
	V[start.second][start.first] = true;
	
    while (!q.empty()) {
        pair<int, int> curr = q.front();
        q.pop();

		vector<pair<int, int>> adj = neighbors(curr);
		for (size_t i = 0; i < adj.size(); i++) {
			if (good(V, curr, adj[i])) {
				V[adj[i].second][adj[i].first] = true;
				P[adj[i].second][adj[i].first] = curr;
				q.push(adj[i]);
			}
		}
	}

	pathPts = assemble(P, start, end);
}

PNG path::render(){
	PNG rendered = image;
    for(unsigned int i = 0; i < pathPts.size(); i++)
		*rendered.getPixel(pathPts[i].first, pathPts[i].second) = RGBAPixel(255, 0, 0);
	return rendered;
}

vector<pair<int,int>> path::getPath() { return pathPts;}

int path::length() { return pathPts.size();}

bool path::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next) {
	pair<size_t, size_t> t = next;
    if(next.second < 0 || t.second >= v.size() || next.first < 0 || t.first >= v[0].size())
		return false;
	if(v[next.second][next.first])
		return false;
	if(!closeEnough(*image.getPixel(curr.first, curr.second), *image.getPixel(next.first, next.second)))
		return false;
	return true;
}

vector<pair<int,int>> path::neighbors(pair<int,int> curr) {
	vector<pair<int,int>> n;

	n.push_back(pair<int, int>(curr.first, curr.second - 1));
	n.push_back(pair<int, int>(curr.first, curr.second + 1));
	n.push_back(pair<int, int>(curr.first + 1, curr.second));
	n.push_back(pair<int, int>(curr.first - 1, curr.second));
	
	return n;
}

vector<pair<int,int>> path::assemble(vector<vector<pair<int,int>>> & p,pair<int,int> s, pair<int,int> e) {
	stack<pair<int, int>> S;
	S.push(e);
	while(S.top() != s) {
		S.push(p[S.top().second][S.top().first]);
	}
	
	vector<pair<int, int>> P;
	while(!S.empty()) {
		P.push_back(S.top());
		S.pop();
	}
	return P;
}

bool path::closeEnough(RGBAPixel p1, RGBAPixel p2){
   int dist = (p1.r-p2.r)*(p1.r-p2.r) + (p1.g-p2.g)*(p1.g-p2.g) +
               (p1.b-p2.b)*(p1.b-p2.b);

   return (dist <= 80);
}
