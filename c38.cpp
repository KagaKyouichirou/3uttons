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
	int a[8];
	int o;
	colour pre;

	bool operator<(const State& y) const
	{
		for (int i = 0; i < 8; i++)
		{
			if (a[i] < y.a[i]) return true;
			else if (a[i] > y.a[i]) return false;
		}
		return false;
	}
};

State NewState()
{
	State s;
	s.a[0] = 1;
	s.a[1] = 0;
	s.a[2] = 0;
	s.a[3] = 2;
	s.a[4] = 3;
	s.a[5] = 2;
	s.a[6] = 2;
	s.a[7] = 1;
	s.o = 4;
	s.pre = N;
	return s;
}

bool GoalState(State s)
{
	return (0 == s.a[0] + s.a[4] + s.a[5] + s.a[7]) and (2 == s.a[2]) and (1 == s.a[1] * s.a[3]) and (3 == s.a[6]);
}

void PrintState(State s)
{
	for (int i = 0; i < 8; i++)
	{
		printf("%2d", s.a[i]);
	}
	printf("  %c\n", s.pre < 2 ? (s.pre < 1 ? 'N' : 'Y') : (s.pre < 3 ? 'R' : 'G'));
}

//yellow
State YY(State s)
{
	if (s.o - 1 >= 0)
	{
		s.a[s.o] = s.a[s.o - 1];
		s.o--;
		s.a[s.o] = 3;
		s.pre = Y;
	}
	return s;
}

//red
State RR(State s)
{
	for (int i = 1; i <= 3; i++) {
		if (0 <= s.o - i and 8 > s.o + i)
		{
			unsigned int x = 2 - s.a[s.o + i];
			unsigned int y = 2 - s.a[s.o - i];
			s.a[s.o - i] = x;
			s.a[s.o + i] = y;
		}
	}
	s.pre = R;
	return s;
}

//green
State GG(State s)
{
	if (s.o + 1 < 8)
	{
		s.a[s.o] = s.a[s.o + 1];
		s.o++;
		s.a[s.o] = 3;
		s.pre = G;
	}
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
