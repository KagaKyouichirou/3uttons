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
	unsigned int a[4], b[4];
	colour pre;

	bool operator<(const State& y) const
	{
		for (int i = 0; i < 4; i++)
		{
			if (a[i] < y.a[i]) return true;
			else if (a[i] > y.a[i]) return false;
		}
		for (int i = 0; i < 4; i++)
		{
			if (b[i] < y.b[i]) return true;
			else if (b[i] > y.b[i]) return false;
		}
		return false;
	}
};

State NewState()
{
	State s;
	s.a[0] = 4;
	s.a[1] = 7;
	s.a[2] = 0;
	s.a[3] = 3;
	s.b[0] = 5;
	s.b[1] = 1;
	s.b[2] = 2;
	s.b[3] = 6;
	s.pre = N;
	return s;
}

bool GoalState(State s)
{
	int r = 0;
	for (int i = 0; i < 4; i++)
	{
		r *= 8;
		r += s.a[i];
	}
	for (int i = 0; i < 4; i++)
	{
		r *= 8;
		r += s.b[i];
	}
    return 342391 == r;
}

void PrintState(State s)
{
	printf("  %c\n", s.pre < 2 ? (s.pre < 1 ? 'N' : 'Y') : (s.pre < 3 ? 'R' : 'G'));
}

//yellow
State YY(State s)
{
	int tmp = s.a[0];
	for (int i = 0; i + 1 < 4; i++)
	{
		s.a[i] = s.a[i + 1];
	}
	s.a[3] = tmp;
	s.pre = Y;
	return s;
}

//red
State RR(State s)
{
	int tmp = s.a[2];
	s.a[2] = s.b[2];
	s.b[2] = tmp;
	s.pre = R;
	return s;
}

//green
State GG(State s)
{
	int tmp = s.b[0];
	for (int i = 0; i + 1 < 4; i++)
	{
		s.b[i] = s.b[i + 1];
	}
	s.b[3] = tmp;
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
					printf("!\n");
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
