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
	colour a[7], b[7];
	colour pre;

	bool operator<(const State& y) const
	{
		for (int i = 0; i < 7; i++)
		{
			if (a[i] < y.a[i]) return true;
			else if (a[i] > y.a[i]) return false;
		}
		for (int i = 0; i < 7; i++)
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
	s.a[0] = Y;
	s.a[1] = G;
	s.a[2] = G;
	s.a[3] = Y;
	s.a[4] = Y;
	s.a[5] = G;
	s.a[6] = G;
	s.b[0] = Y;
	s.b[1] = Y;
	s.b[2] = G;
	s.b[3] = Y;
	s.b[4] = G;
	s.b[5] = G;
	s.b[6] = Y;
	s.pre = N;
	return s;
}

bool GoalState(State s)
{
	int y = 0;
	for (int i = 0; i < 7; i++)
    {
        y += s.a[i];
    }
    return (7 == y) or (21 == y);
}

void PrintState(State s)
{
	printf("  %c\n", s.pre < 2 ? (s.pre < 1 ? 'N' : 'Y') : (s.pre < 3 ? 'R' : 'G'));
}

//yellow
State YY(State s)
{
	colour tmp = s.a[6];
	for (int i = 6; i > 0; i--)
	{
		s.a[i] = s.a[i - 1];
	}
	s.a[0] = tmp;
	s.pre = Y;
	return s;
}

//red
State RR(State s)
{
	for (int i = 1; i <= 5; i++)
	{
		colour tmp = s.a[i];
		s.a[i] = s.b[i];
		s.b[i] = tmp;
	}
	s.pre = R;
	return s;
}

//green
State GG(State s)
{
	colour tmp = s.b[6];
	for (int i = 6; i > 0; i--)
	{
		s.b[i] = s.b[i - 1];
	}
	s.b[0] = tmp;
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
