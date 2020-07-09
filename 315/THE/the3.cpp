#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <utility>
#include <cstdio>
#include <iomanip>

using namespace std;

const int MAX_VALUE = 2132132132;

int n, m;
int ammo;
int ammoRoomNumber;
vector < pair <int, int> > ammoRooms;
int chamber, sc_key, scientist;
vector <vector <pair <int, int> > > g;
vector <vector <int> > gg;
int n_odd, n_even;
map <int, int> odd, even;
vector <int> odd_v, even_v;
vector <int> permRooms;

int ok (vector <int> &p)
{
	vector <int> t;
	for (auto i: p)
	{
		if (i < 4)
		{
			t.push_back (i);
		}
	}
	int ret = 1;
	for (int i = 1; i < 4; ++ i)
	{
		if (t[i] < t[i-1])
		{
			ret = 0;
			break;
		}
	}
	return ret && (p[0] == 0);
}

pair <vector <int>, int> dijkstra (int s, int f, vector <int> &keys, int time_begin)
{
	set < pair <int, int> > s1, s2;
	vector <int> d1 (n+1, MAX_VALUE), d2 (n+1, MAX_VALUE);
	vector <int> p1 (n+1, -1),  p2 (n+1, -1);

	if (time_begin == 1)
		s1.insert ( {d1[s], s} ), d1[s] = 0;
	else
		s2.insert ( {d2[s], s} ), d2[s] = 0;

	while (!s1.empty () || !s2.empty ())
	{
		if (!s1.empty ())
		{
			int u = s1.begin ()->second;
			s1.erase (s1.begin ());

			for (auto v: g[u])
			{
				int to = v.first, w = v.second;
				if (!even[to])
				{
					if (f != to && to == scientist && find (keys.begin (), keys.end (), 1) == keys.end ())
					{
						continue;
					}
					if (f != to && to == chamber && find (keys.begin (), keys.end (), 2) == keys.end ())
					{
						continue;
					}
					int ok = 0;
					for (auto i: ammoRooms)
					{
						if (i.first == to)
						{
							ok = 1;
							break;
						}
					}
					if (f != to && ok)
					{
						continue;
					}

					if (d2[to] > d1[u] + w)
					{
						s2.erase ( {d2[to], to} );
						d2[to] = d1[u] + w;
						p2[to] = u;
						s2.insert ( {d2[to], to} );
					}
				}
			}
		}
		if (!s2.empty ())
		{
			int u = s2.begin ()->second;
			s2.erase (s2.begin ());

			for (auto v: g[u])
			{
				int to = v.first, w = v.second;
				if (!odd[to])
				{
					if (f != to && to == scientist && find (keys.begin (), keys.end (), scientist) == keys.end ())
					{
						continue;
					}
					if (f != to && to == chamber && find (keys.begin (), keys.end (), chamber) == keys.end ())
					{
						continue;
					}
					int ok = 0;
					for (auto i: ammoRooms)
					{
						if (i.first == to)
						{
							ok = 1;
							break;
						}
					}
					if (f != to && ok)
					{
						continue;
					}
					if (d1[to] > d2[u] + w)
					{
						s1.erase ( {d1[to], to} );
						d1[to] = d2[u] + w;
						p1[to] = u;
						s1.insert ( {d1[to], to} );
					}
				}
			}
		}
	}
	
	vector <int> ret;
	int u = f;
	int st = 1;
	int rtt;
	if (d1[f] == d2[f] && d1[f] == MAX_VALUE) return {};
	if (d1[f] < d2[f]) st = 1, rtt = 1;
	else st = 0, rtt = 0;

	while (u != s)
	{
		ret.push_back (u);
		if (st == 1) u = p1[u];
		else u = p2[u];
		st ^= 1;
	}
	ret.push_back (s);
	reverse (ret.begin (), ret.end ());

	return {ret, rtt};
}

int main ()
{
	ios_base::sync_with_stdio(0);

	//freopen ("the3.inp", "r", stdin);
	cin >> ammo;
    cin >> n;
	gg.resize (n+1, vector <int> (n+1, 0));
	g.resize (n+1);
	cin >> chamber >> sc_key >> scientist;

    cin >> n_odd;
	odd_v.resize (n_odd);
	for (auto &i: odd_v)
	{
		cin >> i;
		odd[i] = 1;
	}
	
	cin >> n_even; 
	even_v.resize (n_even);
	for (auto &i: even_v)
	{
		cin >> i;
		even[i] = 1;
	}
	cin >> m;
	for (int i = 0; i < m; ++ i)
	{
		int u, v, w; 
		cin >> u >> v >> w;

		if (gg[u][v] != 0 && gg[u][v] < w) continue;
		gg[u][v] = gg[v][u] = w;
	}

	for (int i = 1; i <= n; ++ i)
	{
		for (int j = i+1; j <= n; ++ j)
		{
			if (gg[i][j])
			{
				g[i].push_back ({j, gg[i][j]});
				g[j].push_back ({i, gg[i][j]});
			}
		}
	}

    cin >> ammoRoomNumber;

	for (int i = 0; i < ammoRoomNumber; ++ i)
	{
		int roomNumber,ammoCount;
        cin >> roomNumber >> ammoCount;
		ammoRooms.push_back ({roomNumber, ammoCount});
	}


	vector <int> perm_global = {0, 1, 2, 3};
	permRooms = {1, sc_key, scientist, chamber};
	for (int i = 0; i < ammoRooms.size (); ++ i)
	{
		perm_global.push_back (i+4);
		permRooms.push_back (ammoRooms[i].first);
	}
	
	pair <int, vector <int> > finalAmmo = {0, {}};

	do
	{
		if (ok (perm_global))
		{
			vector <int> perm = perm_global; 

			vector <int> path;
			vector <int> keys;
			int flag = 1;
			int next_time = 1;
			for (int i = 0; i < perm.size ()-1; ++ i)
			{
				int a = perm[i], b = perm[i+1];
				pair <vector <int>, int> found_path = dijkstra (permRooms[a], permRooms[b], keys, next_time);
				if (found_path.first.size () == 0)
				{
					flag = 0;
					break;
				}
				path.insert (path.end (), found_path.first.begin ()+(permRooms[a] != 1), found_path.first.end ());

				if (b >= 1 && b <= 2)
					keys.push_back (permRooms[b]);
				next_time = found_path.second;

				if (perm[i+1] == 3)
				{
					break;
				}
			}
			if (!flag) continue;

			while (perm[perm.size ()-1] != 3) perm.pop_back ();

			int cur = 0;
			vector <pair <int, int> > tempAmmo = ammoRooms;
			for (int i = 0; i < path.size ()-1; ++ i)
			{
				if (cur > ammo)
				{
					cur = MAX_VALUE;
					break;
				}
				for (int j = 0; j < tempAmmo.size (); ++ j)
				{
					if (path[i+1] == tempAmmo[j].first)
					{
						cur -= tempAmmo[j].second;
						tempAmmo[j].second = 0;
						break;
					}
				}
				cur += gg[path[i]][path[i+1]];
			}
			if (cur != MAX_VALUE)
			if (ammo-cur > finalAmmo.first && path.size ())
			{
				finalAmmo.first = ammo-cur;
				finalAmmo.second = path;
			}
		}
	} while (next_permutation (perm_global.begin (), perm_global.end ()));

	cout << finalAmmo.first << endl;
	cout << finalAmmo.second.size () << endl;
	for (auto i: finalAmmo.second)
		cout << i << " ";
	cout << endl;

	return 0;
}
