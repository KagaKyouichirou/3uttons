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

void swap(colour& x, colour& y)
{
	colour t = x;
	x = y;
	y = t;
}

struct State
{
	colour a[12];
	colour pre;

	bool operator<(const State& y) const
	{
		for (int i = 0; i < 12; i++)
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

	s.a[0] = N;
	s.a[1] = R;
	s.a[2] = N;
	s.a[3] = N;
	s.a[4] = R;
	s.a[5] = G;
	s.a[6] = G;
	s.a[7] = G;
	s.a[8] = R;
	s.a[9] = N;
	s.a[10] = G;
	s.a[11] = R;

	s.pre = N;
	return s;
}

bool GoalState(const State s)
{
	for (int i = 0; i < 12; i++)
	{
		if (N != s.a[i]) return false;
	}
	return true;
}

void PrintState(State s)
{
	for (int i = 0; i < 12; i++)
	{
		printf("%c ", ToChar(s.a[i]));
	}
	printf("  %c\n", ToChar(s.pre));
}

void CheckState(State& s)
{
	for (int i = 0; i < 12; i += 3)
	{
		int t = s.a[i] * s.a[i + 1] * s.a[i + 2] * s.a[(i + 3) % 12];
		if (16 == t or 81 == t)
		{
			s.a[i] = N;
			s.a[i + 1] = N;
			s.a[i + 2] = N;
			s.a[(i + 3) % 12] = N;
		}
	}
}

//yellow
State YY(State s)
{
	swap(s.a[1], s.a[8]);
	swap(s.a[2], s.a[7]);
	CheckState(s);
	s.pre = Y;
	return s;
}

//red
State RR(State s)
{
	colour t = s.a[0];
	for (int i = 0; i + 1 < 12; i++)
	{
		s.a[i] = s.a[i + 1];
	}
	s.a[11] = t;
	CheckState(s);
	s.pre = R;
	return s;
}

//green
State GG(State s)
{
	swap(s.a[4], s.a[11]);
	swap(s.a[5], s.a[10]);
	CheckState(s);
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

	while (x < tmp or tmp < x)
	{
		PrintState(tmp);
		tmp = m[tmp];
	}

	return 0;
}
