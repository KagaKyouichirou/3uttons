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
	colour a[12], b[8], c[4];
	colour pre;

	bool operator<(const State& y) const
	{
		for (int i = 0; i < 12; i++)
		{
			if (a[i] < y.a[i]) return true;
			else if (a[i] > y.a[i]) return false;
		}
		for (int i = 0; i < 8; i++)
		{
			if (b[i] < y.b[i]) return true;
			else if (b[i] > y.b[i]) return false;
		}
		for (int i = 0; i < 4; i++)
		{
			if (c[i] < y.c[i]) return true;
			else if (c[i] > y.c[i]) return false;
		}
		return false;
	}
};

State InitialState()
{
	State s;

	s.a[0] = Y;
	s.a[1] = R;
	s.a[2] = G;
	s.a[3] = G;
	s.a[4] = G;
	s.a[5] = G;
	s.a[6] = R;
	s.a[7] = R;
	s.a[8] = G;
	s.a[9] = R;
	s.a[10] = G;
	s.a[11] = G;

	s.b[0] = R;
	s.b[1] = G;
	s.b[2] = Y;
	s.b[3] = G;
	s.b[4] = R;
	s.b[5] = Y;
	s.b[6] = G;
	s.b[7] = Y;

	s.c[0] = R;
	s.c[1] = G;
	s.c[2] = G;
	s.c[3] = R;

	s.pre = N;
	return s;
}

State FinalState()
{
	State s;
	for (int i = 0; i < 12; i++)
	{
		s.a[i] = G;
	}
	for (int i = 0; i < 8; i++)
	{
		s.b[i] = R;
	}
	for (int i = 0; i < 4; i++)
	{
		s.c[i] = Y;
	}
	s.pre = N;
	return s;
}

bool SimilarState(const State& x, const State& y)
{
    return not (x < y) and not (y < x);
}

void PrintState(State s)
{
	for (int i = 0; i < 12; i++)
	{
		printf("%c", s.a[i] < 2 ? (s.a[i] < 1 ? 'N' : 'Y') : (s.a[i] < 3 ? 'R' : 'G'));
	}
	for (int i = 0; i < 8; i++)
	{
		printf("%c", s.b[i] < 2 ? (s.b[i] < 1 ? 'N' : 'Y') : (s.b[i] < 3 ? 'R' : 'G'));
	}
	for (int i = 0; i < 4; i++)
	{
		printf("%c", s.c[i] < 2 ? (s.c[i] < 1 ? 'N' : 'Y') : (s.c[i] < 3 ? 'R' : 'G'));
	}
	printf("  %c\n", s.pre < 2 ? (s.pre < 1 ? 'N' : 'Y') : (s.pre < 3 ? 'R' : 'G'));
}

//yellow
State YX(State s)
{
	colour tmp = s.a[0];
	for (int i = 0; i + 1 < 12; i++)
	{
		s.a[i] = s.a[i + 1];
	}
	s.a[11] = tmp;
	tmp = s.b[0];
	for (int i = 0; i + 1 < 8; i++)
	{
		s.b[i] = s.b[i + 1];
	}
	s.b[7] = tmp;
	tmp = s.c[0];
	for (int i = 0; i + 1 < 4; i++)
	{
		s.c[i] = s.c[i + 1];
	}
	s.c[3] = tmp;
	s.pre = Y;
	return s;
}

//red
State RX(State s)
{
	colour tmp = s.a[0];
	s.a[0] = s.b[0];
	s.b[0] = s.c[0];
	s.c[0] = s.c[2];
	s.c[2] = s.b[4];
	s.b[4] = s.a[6];
	s.a[6] = tmp;
	s.pre = R;
	return s;
}

//green
State GX(State s)
{
	colour tmp = s.a[11];
	for (int i = 11; i > 0; i--)
	{
		s.a[i] = s.a[i - 1];
	}
	s.a[0] = tmp;
	tmp = s.b[7];
	for (int i = 7; i > 0; i--)
	{
		s.b[i] = s.b[i - 1];
	}
	s.b[0] = tmp;
	tmp = s.c[3];
	for (int i = 3; i > 0; i--)
	{
		s.c[i] = s.c[i - 1];
	}
	s.c[0] = tmp;
	s.pre = G;
	return s;
}

//yellow-reverse
State YY(State s)
{
	s = GX(s);
	s.pre = Y;
	return s;
}

//red-reverse
State RY(State s)
{
	colour tmp = s.a[6];
	s.a[6] = s.b[4];
	s.b[4] = s.c[2];
	s.c[2] = s.c[0];
	s.c[0] = s.b[0];
	s.b[0] = s.a[0];
	s.a[0] = tmp;
	s.pre = R;
	return s;
}

//green-reverse
State GY(State s)
{
	s = YX(s);
	s.pre = G;
	return s;
}

int main()
{
	State (*func[6])(State) = {YX, RX, GX, YY, RY, GY};

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

	while (not qx.empty() and not qy.empty() and keep)
	{
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
