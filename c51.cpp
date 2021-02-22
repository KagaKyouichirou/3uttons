#include <cstdio>
#include <queue>
#include <map>

enum colour
{
	N,
	Y,
	R,
	G
};

struct State
{
	int a[3][6];
	int yh, yt, rh, rt, gh, gt;
	colour pre;

	bool operator<(const State& y) const
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (a[i][j] < y.a[i][j]) return true;
				else if (a[i][j] > y.a[i][j]) return false;
			}
		}
		return yh < y.yh or (yh == y.yh and rh < y.rh) or (yh == y.yh and rh == y.rh and gh < y.gh);
	}
};

State InitialState()
{
	State s;
	for (int i = 0; i < 3; i++)
	{
		s.a[i][3] = 9;
		s.a[i][4] = 9;
		s.a[i][5] = 9;
	}
	s.a[0][0] = 1;
	s.a[0][1] = 8;
	s.a[0][2] = 5;
	s.a[1][0] = 7;
	s.a[1][1] = 4;
	s.a[1][2] = 0;
	s.a[2][0] = 3;
	s.a[2][1] = 6;
	s.a[2][2] = 2;
	s.yh = 3;
	s.yt = 3;
	s.rh = 3;
	s.rt = 3;
	s.gh = 3;
	s.gt = 3;
	s.pre = N;
	return s;
}

State FinalState()
{
	State s;
	for (int i = 0; i < 3; i++)
	{
		s.a[i][3] = 9;
		s.a[i][4] = 9;
		s.a[i][5] = 9;
	}

	s.a[0][0] = 0;
	s.a[0][1] = 1;
	s.a[0][2] = 2;
	s.a[1][0] = 3;
	s.a[1][1] = 4;
	s.a[1][2] = 5;
	s.a[2][0] = 6;
	s.a[2][1] = 7;
	s.a[2][2] = 8;
	s.yh = 3;
	s.yt = 3;
	s.rh = 3;
	s.rt = 3;
	s.gh = 3;
	s.gt = 3;
	s.pre = N;
	return s;
}

void PrintState(State s)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			printf("%3d", s.a[i][j]);
		}
		printf("|");
	}
	printf("  %c\n", s.pre < 2 ? (s.pre < 1 ? 'N' : 'Y') : (s.pre < 3 ? 'R' : 'G'));
}

//yellow
State YY(State s)
{
	if (R == s.pre and s.rt > s.rh)
	{
		if (s.yt + s.rt - s.rh <= 4)
		{
			for (int i = s.rh; i < s.rt; i++)
			{
				if (3 == s.yt + i -s.rh and 0 != s.a[1][i] / 3) return s;
			}
			for (int i = s.rh; i < s.rt; i++)
			{
				s.a[0][s.yt + i - s.rh] = s.a[1][i];
				s.a[1][i] = 9;
			}
			s.yt += s.rt - s.rh;
			s.yh = s.yt;
			s.rt = s.rh;
			s.pre = Y;
			return s;
		}
		else return s;
	}
	else if (G == s.pre and s.gt > s.gh)
	{
		if (s.yt + s.gt - s.gh <= 4)
		{
			for (int i = s.gh; i < s.gt; i++)
			{
				if (3 == s.yt + i - s.gh and 0 != s.a[2][i] / 3) return s;
			}
			for (int i = s.gh; i < s.gt; i++)
			{
				s.a[0][s.yt + i - s.gh] = s.a[2][i];
				s.a[2][i] = 9;
			}
			s.yt += s.gt - s.gh;
			s.yh = s.yt;
			s.gt = s.gh;
			s.pre = Y;
			return s;
		}
		else return s;
	}
	if (0 < s.yh)
	{
		s.yh--;
		s.pre = Y;
	}
	return s;
}

//red
State RR(State s)
{
	if (Y == s.pre and s.yt > s.yh)
	{
		if (s.rt + s.yt - s.yh <= 6)
		{
			for (int i = s.yh; i < s.yt; i++)
			{
				if (3 == s.rt + i - s.yh and 1 != s.a[0][i] / 3) return s;
				if (5 == s.rt + i - s.yh and 2 != s.a[0][i] / 3) return s;
			}
			for (int i = s.yh; i < s.yt; i++)
			{
				s.a[1][s.rt + i - s.yh] = s.a[0][i];
				s.a[0][i] = 9;
			}
			s.rt += s.yt - s.yh;
			s.rh = s.rt;
			s.yt = s.yh;
			s.pre = R;
			return s;
		}
		else return s;
	}
	else if (G == s.pre and s.gt > s.gh)
	{
		if (s.rt + s.gt - s.gh <= 6)
		{
			for (int i = s.gh; i < s.gt; i++)
			{
				if (3 == s.rt + i - s.gh and 1 != s.a[2][i] / 3) return s;
				if (5 == s.rt + i - s.gh and 2 != s.a[2][i] / 3) return s;
			}
			for (int i = s.gh; i < s.gt; i++)
			{
				s.a[1][s.rt + i - s.gh] = s.a[2][i];
				s.a[2][i] = 9;
			}
			s.rt += s.gt - s.gh;
			s.rh = s.rt;
			s.gt = s.gh;
			s.pre = R;
			return s;
		}
		else return s;
	}
	if (0 < s.rh)
	{
		s.rh--;
		s.pre = R;
	}
	return s;
}

//green
State GG(State s)
{
	if (Y == s.pre and s.yt > s.yh)
	{
		if (s.gt + s.yt - s.yh <= 5)
		{
			for (int i = s.yh; i < s.yt; i++)
			{
				if (4 == s.gt + i - s.yh and 0 != s.a[0][i] / 3) return s;
			}
			for (int i = s.yh; i < s.yt; i++)
			{
				s.a[2][s.gt + i - s.yh] = s.a[0][i];
				s.a[0][i] = 9;
			}
			s.gt += s.yt - s.yh;
			s.gh = s.gt;
			s.yt = s.yh;
			s.pre = G;
			return s;
		}
		else return s;
	}
	else if (R == s.pre and s.rt > s.rh)
	{
		if (s.gt + s.rt - s.rh <= 5)
		{
			for (int i = s.rh; i < s.rt; i++)
			{
				if (4 == s.gt + i - s.rh and 0 != s.a[1][i] / 3) return s;
			}
			for (int i = s.rh; i < s.rt; i++)
			{
				s.a[2][s.gt + i - s.rh] = s.a[1][i];
				s.a[1][i] = 9;
			}
			s.gt += s.rt - s.rh;
			s.gh = s.gt;
			s.rt = s.rh;
			s.pre = G;
			return s;
		}
		else return s;
	}
	if (0 < s.gh)
	{
		s.gh--;
		s.pre = G;
	}
	return s;
}

int main()
{
	State (*func[6])(State) = {YY, RR, GG, YY, RR, GG};

	std::map<State, State> mx, my;
	std::queue<State> qx, qy;

	const State x = InitialState();
	const State y = FinalState();

	qx.push(x);
	mx[x] = x;

	qy.push(y);
	my[y] = y;

	State tmp, tx, ty;

	bool keep = true;

	int layer = 0;
	while (not qx.empty() and not qy.empty() and keep)
	{
		printf("Layer:%5d\n",layer);
		layer++;
		int cx = qx.size();
		for (int k = 0; k < cx and keep; k++)
		{
			State sx = qx.front();
			qx.pop();
			for (int i = 0; i < 3 and keep; i++)
			{
				tmp = func[i](sx);
				if (0 == mx.count(tmp))
				{
					mx[tmp] = sx;
					if (1 == my.count(tmp))
					{
						tx = tmp;
						ty = my.find(tmp)->first;
						keep = false;
						break;
					}
					else
					{
						qx.push(tmp);
					}
				}
			}
		}

		int cy = qy.size();
		for (int k = 0; k < cy and keep; k++)
		{
			State sy = qy.front();
			qy.pop();
			for (int i = 3; i < 6 and keep; i++)
			{
				tmp = func[i](sy);
				if (0 == my.count(tmp))
				{
					my[tmp] = sy;
					if (1 == mx.count(tmp))
					{
						tx = mx.find(tmp)->first;
						ty = tmp;
						keep = false;
						break;
					}
					else
					{
						qy.push(tmp);
					}
				}
			}
		}
	}

	while (x < tx or tx < x)
	{
		PrintState(tx);
		tx = mx[tx];
	}

	printf("-------------\n");

	while (y < ty or ty < y)
	{
		PrintState(ty);
		ty = my[ty];
	}

	return 0;
}
