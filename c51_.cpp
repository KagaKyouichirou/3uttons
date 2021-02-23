#include <cstdio>
#include <queue>
#include <map>

enum mode
{
	Y2R,
	Y2G,
	R2G,
	R2Y,
	G2Y,
	G2R,
	N
};

#define ModeToString(m) (m < 3 ? (m < 2 ? (m < 1 ? "Y2R" : "Y2G") : "R2G") : (m < 5 ? (m < 4 ? "R2Y" : "G2Y") : (m < 6 ? "G2R" : "N")))


struct State
{
	int a[3][6];
	int size[3];
	mode pm;
	int pc, acc;

	bool operator<(const State& y) const
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				if (a[i][j] < y.a[i][j]) return true;
				else if (a[i][j] > y.a[i][j]) return false;
			}
		}
		return false;
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
		s.size[i] = 3;
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
	
	s.pm = N;
	s.pc = 0;
	s.acc = 0;
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
		s.size[i] = 3;
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

	s.pm = N;
	s.pc = 0;
	s.acc = 0;
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
	printf("  %s%3d\n", ModeToString(s.pm), s.pc);
}

//yellow
State YY(State s, const int src, const unsigned int num)
{
	static const int dst = 0;
	if (s.size[dst] + num > 4) return s;
	int j = s.size[src] - num;
	for (int i = 0; i < num; i++)
	{
		if (3 == s.size[dst] + i and 0 != s.a[src][j + i] / 3) return s;
	}
	for (int i = 0; i < num; i++)
	{
		s.a[dst][s.size[dst] + i] = s.a[src][j + i];
		s.a[src][j + i] = 9;
	}
	s.size[src] = j;
	s.size[dst] += num;
	s.pm = 1 == src ? R2Y : G2Y;
	s.pc = num;
	s.acc += num + 1;
	return s;
}

//red
State RR(State s, const int src, const unsigned int num)
{
	static const int dst = 1;
	if (s.size[dst] + num > 6) return s;
	int j = s.size[src] - num;
	for (int i = 0; i < num; i++)
	{
		if (3 == s.size[dst] + i and 1 != s.a[src][j + i] / 3) return s;
		if (5 == s.size[dst] + i and 2 != s.a[src][j + i] / 3) return s;
	}
	for (int i = 0; i < num; i++)
	{
		s.a[dst][s.size[dst] + i] = s.a[src][j + i];
		s.a[src][j + i] = 9;
	}
	s.size[src] = j;
	s.size[dst] += num;
	s.pm = 0 == src ? Y2R : G2R;
	s.pc = num;
	s.acc += num + 1;
	return s;
}

//green
State GG(State s, const int src, const unsigned int num)
{
	static const int dst = 2;
	if (s.size[dst] + num > 5) return s;
	int j = s.size[src] - num;
	for (int i = 0; i < num; i++)
	{
		if (4 == s.size[dst] + i and 0 != s.a[src][j + i] / 3) return s;
	}
	for (int i = 0; i < num; i++)
	{
		s.a[dst][s.size[dst] + i] = s.a[src][j + i];
		s.a[src][j + i] = 9;
	}
	s.size[src] = j;
	s.size[dst] += num;
	s.pm = 0 == src ? Y2G : R2G;
	s.pc = num;
	s.acc += num + 1;
	return s;
}

bool CheckState(const State& s)
{
	return (0 == s.a[1][2] and 1 == s.a[0][0] and 2 == s.a[1][4]
		and 3 == s.a[1][0] and 4 == s.a[1][1] and 5 == s.a[1][3]
		and 6 == s.a[2][0] and 7 == s.a[2][1] and 8 == s.a[0][1]);
}

int main()
{
	State (*func[3])(State, const int, const unsigned int) = {YY, RR, GG};

	std::map<State, State> mx, my;
	std::queue<State> qx, qy;

	const State x = InitialState();
	const State y = FinalState();

	qx.push(x);
	mx[x] = x;

	qy.push(y);
	my[y] = y;

	State u;
	u = GG(y, 0, 2);
	u = YY(u, 1, 1);
	u = RR(u, 0, 2);
	u = RR(u, 2, 1);
	u = YY(u, 2, 1);
	u = YY(u, 2, 1);
	if (CheckState(u)) printf("??????\n");

	State tmp, tx, ty;

	int best = 99999;
	int xacc = 0;
	int yacc = 0;

	int layer = 0;
	while (not qx.empty() and not qy.empty())
	{
		printf("Layer:%5d\n",layer);
		layer++;

		xacc = 99999;
		const int cx = qx.size();
		for (int k = 0; k < cx; k++)
		{
			State sx = qx.front();
			qx.pop();
			if (sx.acc >= best) continue;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 1; j < 3; j++)
				{
					int src = (i + j) % 3;
					for (int k = 1; k <= sx.size[src]; k++)
					{
						tmp = func[i](sx, src, k);
						if (CheckState(tmp)) printf("!XX%4d\n", tmp.acc);
						if (0 == mx.count(tmp))
						{
							mx[tmp] = sx;
							if (1 == my.count(tmp))
							{
								if (tmp.acc + my.find(tmp)->first.acc < best)
								{
									tx = tmp;
									ty = my.find(tmp)->first;
									best = tx.acc + ty.acc;
								}
							}
							else if (tmp.acc < best)
							{
								qx.push(tmp);
								if (tmp.acc < xacc) xacc = tmp.acc;
							}
						}
					}
				}
			}
		}

		yacc = 99999;
		int cy = qy.size();
		for (int k = 0; k < cy; k++)
		{
			State sy = qy.front();
			qy.pop();
			//if (sy.acc >= best) continue;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 1; j < 3; j++)
				{
					int src = (i + j) % 3;
					for (int k = 1; k <= sy.size[src]; k++)
					{
						tmp = func[i](sy, src, k);
						if (0 == my.count(tmp))
						{
							my[tmp] = sy;
							if (1 == mx.count(tmp))
							{
								if (mx.find(tmp)->first.acc + tmp.acc < best)
								{
									tx = mx.find(tmp)->first;
									ty = tmp;
									best = tx.acc + ty.acc;
									if (CheckState(tmp))
									{
										printf("!YYY%4d\n", best);

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
									}
								}
							}
							else// if (tmp.acc < best)
							{
								qy.push(tmp);
								if (tmp.acc < yacc) yacc = tmp.acc;
							}
						}
					}
				}
			}
		}
	}

	printf("!%4d\n", best);

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
