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
	bool a[7][7];
	int pi, pj, pk, r;
	colour pre;

	bool operator<(const State& y) const
	{
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (a[i][j] < y.a[i][j]) return true;
				else if (a[i][j] > y.a[i][j]) return false;
			}
		}
		return pi < y.pi or (pi == y.pi and pj < y.pj) or (pi == y.pi and pj == y.pj and pk < y.pk);
	}
};

State NewState()
{
	State s;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			s.a[i][j] = false;
		}
	}
	s.a[1][2] = true;
	s.a[2][5] = true;
	s.a[4][3] = true;
	s.a[5][2] = true;
	s.pi = 3;
	s.pj = 3;
	s.pk = 3;
	s.r = 4;
	s.pre = N;
	return s;
}

bool GoalState(State s)
{
	return 0 == s.r;
}

void PrintState(State s)
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (s.a[i][j]) printf("%3d%3d", i, j);
		}
	}
	printf("  %c\n", s.pre < 2 ? (s.pre < 1 ? 'N' : 'Y') : (s.pre < 3 ? 'R' : 'G'));
}

//yellow
State YY(State s)
{
	if (s.pk > 2) s.pk--;
	else s.pk = 4;
	s.pre = Y;
	return s;
}

//red
State RR(State s)
{
	s.pi -= 3;
	s.pj -= s.pk;
	int x = s.pj;
	int y = -s.pi;
	s.pi = x + 3;
	s.pj = y + s.pk;
	if (0 <= s.pi and 7 > s.pi and 0 <= s.pj and 7 > s.pj)
	{
		if (1 == s.pi and 3 == s.pj) s.pi = 2;
		s.r -= s.a[s.pi][s.pj];
		s.a[s.pi][s.pj] = false;
	}
	s.pre = R;
	return s;
}

//green
State GG(State s)
{
	if (s.pk < 4) s.pk++;
	else s.pk = 2;
	s.pre = G;
	return s;
}

int main()
{
	State (*func[3])(State) = {YY, RR, GG};

	std::map<State, State> m;
	std::queue<State> q;

	State x = NewState();

	q.push(x);
	m[x] = x;

	State tmp;

	bool keep = true;

	while (not q.empty() and keep)
	{
		State s = q.front();
		q.pop();
		for (int i = 0; i < 3; i++)
		{
			tmp = func[i](s);
			if (0 == m.count(tmp))
			{
				m[tmp] = s;
				if (GoalState(tmp))
				{
					keep = false;
					break;
				}
				else
				{
					q.push(tmp);
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
