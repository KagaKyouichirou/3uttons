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

char ToChar(colour c)
{
	return c < 2 ? (c < 1 ? 'N' : 'Y') : (c < 3 ? 'R' : 'G');
}

int Sgn(int x)
{
	return x > 0 ? 1 : -1;
}

struct State
{
	int a[4];
	colour pre;

	bool operator<(const State& y) const
	{
		for (int i = 0; i < 4; i++)
		{
			if (a[i] < y.a[i]) return true;
			else if (a[i] > y.a[i]) return false;
		}
		return false;
	}
};

State InitialState()
{
	State s;

	s.a[0] = -7;
	s.a[1] = -13;
	s.a[2] = -43;
	s.a[3] = -53;

	s.pre = N;
	return s;
}

bool GoalState(const State s)
{
	int r = s.a[0] * s.a[1] * s.a[2] * s.a[3];
	return 46483 == r or -46483 == r;
}

void PrintState(State s)
{
	for (int i = 0; i < 4; i++)
	{
		printf("%5d", s.a[i]);
	}
	printf("  %c\n", ToChar(s.pre));
}

//yellow
State YY(State s)
{
	for (int i = 0; i < 4; i++)
	{
		int t = Sgn(s.a[i]);
		switch (s.a[i] * t)
		{
			case 3:
				s.a[i] = 1 * t;
				break;
			case 5:
				s.a[i] = 3 * t;
				break;
			case 13:
				s.a[i] = 11 * t;
				break;
			case 19:
				s.a[i] = 17 * t;
				break;
			case 23:
				s.a[i] = 19 * t;
				break;
			case 29:
				s.a[i] = 23 * t;
				break;
			case 31:
				s.a[i] = 29 * t;
				break;
			case 53:
				s.a[i] = 47 * t;
				break;
			case 59:
				s.a[i] = 53 * t;
				break;
			case 61:
				s.a[i] = 59 * t;
				break;
			case 67:
				s.a[i] = 61 * t;
				break;
		}
	}
	s.pre = Y;
	return s;
}

//red
State RR(State s)
{
	for (int i = 0; i < 4; i++)
	{
		int t = Sgn(s.a[i]);
		switch (s.a[i] * t)
		{
			case 1:
				s.a[i] = -7;
				break;
			case 5:
				s.a[i] = -11;
				break;
			case 7:
				s.a[i] = -8 + 9 * t;
				break;
			case 11:
				s.a[i] = -9 + 14 * t;
				break;
			case 13:
				s.a[i] = -29;
				break;
			case 17:
				s.a[i] = -15 + 22 * t;
				break;
			case 23:
				s.a[i] = -15 + 26 * t;
				break;
			case 29:
				s.a[i] = 13;
				break;
			case 31:
				s.a[i] = -43;
				break;
			case 37:
				s.a[i] = -15 + 32 * t;
				break;
			case 41:
				s.a[i] = -18 + 41 * t;
				break;
			case 43:
				s.a[i] = -18 + 49 * t;
				break;
			case 47:
				s.a[i] = 37;
				break;
			case 59:
				s.a[i] = 41;
				break;
			case 67:
				s.a[i] = 43;
				break;
			default:
				s.a[i] *= -1;
		}
	}
	s.pre = R;
	return s;
}

//green
State GG(State s)
{
	for (int i = 0; i < 4; i++)
	{
		int t = Sgn(s.a[i]);
		switch (s.a[i] * t)
		{
			case 1:
				s.a[i] = 3 * t;
				break;
			case 3:
				s.a[i] = 5 * t;
				break;
			case 11:
				s.a[i] = 13 * t;
				break;
			case 17:
				s.a[i] = 19 * t;
				break;
			case 19:
				s.a[i] = 23 * t;
				break;
			case 23:
				s.a[i] = 29 * t;
				break;
			case 29:
				s.a[i] = 31 * t;
				break;
			case 47:
				s.a[i] = 53 * t;
				break;
			case 53:
				s.a[i] = 59 * t;
				break;
			case 59:
				s.a[i] = 61 * t;
				break;
			case 61:
				s.a[i] = 67 * t;
				break;
		}
	}
	s.pre = G;
	return s;
}

int main()
{
	State (*func[3])(State) = {YY, RR, GG};

	std::map<State, State> m;
	std::queue<State> q;

	const State x = InitialState();

	q.push(x);
	m[x] = x;

	State tmp;

	bool keep = true;

	int layer = 0;
	while (not q.empty() and keep)
	{
		printf("Layer:%5d\n",layer);
		layer++;
		int c = q.size();
		for (int k = 0; k < c and keep; k++)
		{
			State s = q.front();
			q.pop();
			for (int i = 0; i < 3 and keep; i++)
			{
				tmp = func[i](s);
				if (0 == m.count(tmp))
				{
					m[tmp] = s;
					if (GoalState(tmp)) keep = false;
					else q.push(tmp);
				}
			}
		}
	}

	if (not keep) printf("!\n");

	while (x < tmp or tmp < x)
	{
		PrintState(tmp);
		tmp = m[tmp];
	}

	return 0;
}
