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
	unsigned int a[6][6];
	unsigned int pi, pj, remain;
	colour pre;

	bool operator<(const State& y) const
	{
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				if (a[i][j] < y.a[i][j]) return true;
				else if (a[i][j] > y.a[i][j]) return false;
			}
		}
		return pi < y.pi or (pi == y.pi and pj < y.pj);
	}
};

State NewState(unsigned int aa[6][6])
{
	State s;
	s.remain = 0;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			s.a[i][j] = aa[i][j];
			if (0 < aa[i][j]) s.remain++;
		}
	}
	s.pi = 2;
	s.pj = 3;
	s.pre = N;
	return s;
}

bool GoalState(State x)
{
	return 0 == x.remain;
}

void PrintState(State s)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			printf("%c", s.a[i][j] > 0 ? '1' : '0');
		}
	}
	printf("  %c\n", s.pre < 2 ? (s.pre < 1 ? 'N' : 'Y') : (s.pre < 3 ? 'R' : 'G'));
}

//yellow
State YY(State s)
{
	for (int j = 0; j < s.pj; j++)
	{
		if (0 < s.a[s.pi][j])
		{
			s.a[s.pi][j] = 0;
			s.pj = j;
			s.remain--;
			s.pre = Y;
			return s;
		}
	}
	return s;
}

//red
State RR(State s)
{
	for (int i = 0; i < s.pi; i++)
	{
		if (0 < s.a[i][s.pj])
		{
			s.a[i][s.pj] = 0;
			s.pi = i;
			s.remain--;
			s.pre = R;
			return s;
		}
	}
	for (int i = 5; i > s.pi; i--)
	{
		if (0 < s.a[i][s.pj])
		{
			s.a[i][s.pj] = 0;
			s.pi = i;
			s.remain--;
			s.pre = R;
			return s;
		}
	}
	return s;
}

//green
State GG(State s)
{
	for (int j = 5; j > s.pj; j--)
	{
		if (0 < s.a[s.pi][j])
		{
			s.a[s.pi][j] = 0;
			s.pj = j;
			s.remain--;
			s.pre = G;
			return s;
		}
	}
	return s;
}

int main()
{
	State (*func[3])(State) = {YY, RR, GG};

	std::map<State, State> m;
	std::queue<State> q;

	unsigned int xa[6][6] = {{0, 1, 0, 0, 1, 0}, {1, 0, 1, 1, 1, 1}, {0, 0, 0, 0, 0, 1}, {0, 1, 1, 0, 1, 0}, {0, 1, 0, 0, 1, 0}, {1, 0, 0, 1, 0, 1}};
	State x = NewState(xa);

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
