//Problem: https://www.spoj.com/problems/TAP2014K/
#include <bits/stdc++.h>
#define sqr(a) ((a)*(a))
#define rsz resize
#define forr(i,a,b) for(int i=(a);i<(b);i++)
#define forn(i,n) forr(i,0,n)
#define dforn(i,n) for(int i=n-1;i>=0;i--)
#define forall(it,v) for(auto it=v.begin();it!=v.end();it++)
#define sz(c) ((int)c.size())
#define zero(v) memset(v, 0, sizeof(v))
#define pb push_back
#define mp make_pair
#define lb lower_bound
#define ub upper_bound
#define fst first
#define snd second

using namespace std;

typedef long long ll;
typedef pair<int,int> ii;

// An array represented as a treap, where the "key" is the index of the node in the array
// However, the key is not stored explicitly, but can be calculated as the sum of the
// sizes of the left childs of the ancestors where the node is in the right subtree of it
// (commented parts are specific to range sum queries problem)
// rng = random number generator, works better than rand in some cases
mt19937 rng;
typedef struct item *pitem;
struct item {
	int pr, cnt, val;
	bool rev;
	int sum; // (paramters for range query)
	int add; // (parameters for lazy prop)
	pitem l, r;
	item(int val): pr(rng()), cnt(1), val(val), l(0), r(0), rev(0), sum(val), add(0){}
};
void push(pitem node) {
	if(node){
		if(node->rev) {
			swap(node->l, node->r);
			if(node->l) node->l->rev ^= true;
			if(node->r) node->r->rev ^= true;
			node->rev = false;
		}
		node->val += node->add; node->sum += node->cnt * node->add;
		if(node->l) node->l->add += node->add;
		if(node->r) node->r->add += node->add;
		node->add = 0;
	}
}
int cnt(pitem t) {return t ? t->cnt : 0;}
int sum(pitem t) {return t ? push(t), t->sum : 0;}
void upd_cnt(pitem t) {
	if(t) {
		t->cnt = cnt(t->l) + cnt(t->r) + 1;
		t->sum=t->val+sum(t->l)+sum(t->r);
	}
}
void split(pitem node, pitem& L, pitem& R, int sz) { //sz: desired size of L O(log)
	if(!node) {L = R = 0; return;}
	push(node);
	//If node's left child has at least sz nodes, go left
	if(sz <= cnt(node->l)) split(node->l, L, node->l, sz), R = node;
	//If node's left child has less than sz nodes, go right changing wanted sz
	else split(node->r, node->r, R, sz-1-cnt(node->l)), L = node;
	upd_cnt(node);
}
void merge(pitem& result, pitem L, pitem R) {// O(log)
	push(L); push(R);
	if(!L || !R) result = L ? L : R;
	else if(L->pr > R->pr) merge(L->r, L->r, R), result = L;
	else merge(R->l, L, R->l), result = R;
	upd_cnt(result);
}
void insert(pitem& node, pitem x, int pos) {// 0-index O(log)
	pitem l,r;
	split(node, l, r, pos);
	merge(l, l, x);
	merge(node, l, r);
}
void erase(pitem& node, int pos) {// 0-index O(log)
	if(!node) return;
	push(node);
    if(pos == cnt(node->l)) merge(node, node->l, node->r);
    else if(pos < cnt(node->l)) erase(node->l, pos);
    else erase(node->r, pos-1-cnt(node->l));
    upd_cnt(node);
}
void reverse(pitem &node, int L, int R) {//[L, R) O(log)
    pitem t1, t2, t3;
    split(node, t1, t2, L);
    split(t2, t2, t3, R-L);
    t2->rev ^= true;
    merge(node, t1, t2);
    merge(node, node, t3);
}
void add(pitem &node, int L, int R, int x) {//[L, R) O(log)
	pitem t1, t2, t3;
	split(node, t1, t2, L);
	split(t2, t2, t3, R-L);
	t2->add += x;
	merge(node, t1, t2);
	merge(node, node, t3);
}
/*int get(pitem &node, int L, int R) {//[L, R) O(log)
	pitem t1, t2, t3;
	split(node, t1, t2, L);
	split(t2, t2, t3, R-L);
	push(t2);
	int ret = t2->sum;
	merge(node, t1, t2);
	merge(node, node, t3);
	return ret;
}*/
void output(pitem t){ // useful for debugging
    if(!t)return;
    push(t);
    output(t->l);cout << (char)('a'+t->val%26);output(t->r);
}

int main()
{
	#ifdef REJUNTE
	freopen("input.in","r",stdin);
	//freopen("output.out","w",stdout);
	#endif
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	int t;
	cin >> t;
	forn(T,t)
	{
		string s;
		int n;
		cin >> s >> n;
		pitem root = NULL;
		forn(i,s.size()) insert(root, new item(s[i]-'a'), i);
		forn(i,n)
		{
			int a,b,c,d;
			cin >> a >> b >> c >> d;
			a--;c--;
			pitem t1,t2,t3,t4,t5;
			split(root, t1, t2, a);
			split(t2, t2, t3, b-a);
			split(t3, t3, t4, c-b);
			split(t4, t4, t5, d-c);
			reverse(t2, 0, b-a);
			reverse(t4, 0, d-c);
			add(t2, 0, b-a, 1);
			add(t4, 0, d-c, 1);
			merge(root, t1, t4);
			merge(root, root, t3);
			merge(root, root, t2);
			merge(root, root, t5);
		}
		output(root);
		cout << '\n';
	}
	return 0;
}