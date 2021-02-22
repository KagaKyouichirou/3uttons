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

const colour fbd[25] = {N, N, R, N, N, N, N, Y, N, N, N, G, N, N, N, N, R, R, N, N, N, N, G, G, N};

struct State
{
	colour a[25];
	unsigned int p;
	colour pre;

	bool operator<(const State& y) const
	{
		for (int i = 0; i < 25; i++)
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
	for (int i = 0; i < 25; i++)
	{
		s.a[i] = N;
	}
	s.p = 0;
	s.pre = N;
	return s;
}

bool GoalState(State s)
{
	return 25 == s.p;
}

void PrintState(State s)
{
	for (int i = 0; i < 25; i++)
	{
		printf("%c", s.a[i] < 2 ? (s.a[i] < 1 ? 'N' : 'Y') : (s.a[i] < 3 ? 'R' : 'G'));
	}
	printf("  %c\n", s.pre < 2 ? (s.pre < 1 ? 'N' : 'Y') : (s.pre < 3 ? 'R' : 'G'));
}

//yellow
State YY(State s)
{
	if (s.p + 1 < 25 and Y != fbd[s.p] and Y != fbd[s.p + 1])
	{
		s.a[s.p] = Y;
		s.a[s.p + 1] = Y;
		s.p += 2;
		s.pre = Y;
	}
	return s;
}

//red
State RR(State s)
{
	if (s.p + 2 < 25 and R != fbd[s.p] and R != fbd[s.p + 1] and R != fbd[s.p + 2])
	{
		s.a[s.p] = R;
		s.a[s.p + 1] = R;
		s.a[s.p + 2] = R;
		s.p += 3;
		s.pre = R;
	}
	return s;
}

//green
State GG(State s)
{
	if (s.p + 3 < 25 and G != fbd[s.p] and G != fbd[s.p + 1] and G != fbd[s.p + 2] and G != fbd[s.p + 3])
	{
		s.a[s.p] = G;
		s.a[s.p + 1] = G;
		s.a[s.p + 2] = G;
		s.a[s.p + 3] = G;
		s.p += 4;
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
