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
	bool a[3][3];
	colour pre;

	bool operator<(const State& y) const
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (a[i][j] < y.a[i][j]) return true;
				else if (a[i][j] > y.a[i][j]) return false;
			}
		}
		return false;
	}
};

State NewState()
{
	State s;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			s.a[i][j] = false;
		}
	}
	s.a[2][2] = true;
	s.pre = N;
	return s;
}

bool GoalState(State s)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (not s.a[i][j]) return false;
		}
	}
	return true;
}

void PrintState(State s)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("%c", s.a[i][j] ? '1' : '0');
		}
	}
	printf("  %c\n", s.pre < 2 ? (s.pre < 1 ? 'N' : 'Y') : (s.pre < 3 ? 'R' : 'G'));
}

//yellow
State YY(State s)
{
	bool tmp = s.a[0][0];
	s.a[0][0] = s.a[1][0];
	s.a[1][0] = s.a[2][0];
	s.a[2][0] = tmp;
	s.pre = Y;
	return s;
}

//red
State RR(State s)
{
	bool tmp = s.a[0][1];
	s.a[0][1] = s.a[1][0];
	s.a[1][0] = tmp;
	tmp = s.a[0][2];
	s.a[0][2] = s.a[1][1];
	s.a[1][1] = s.a[2][0];
	s.a[2][0] = tmp;
	tmp = s.a[1][2];
	s.a[1][2] = s.a[2][1];
	s.a[2][1] = tmp;
	s.pre = R;
	return s;
}

//green
State GG(State s)
{
	s.a[2][0] = not s.a[2][0];
	s.a[2][1] = not s.a[2][1];
	s.a[2][2] = not s.a[2][2];
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
