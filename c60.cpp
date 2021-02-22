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
	int a[6];
	colour pre;

	bool operator<(const State& y) const
	{
		for (int i = 0; i < 6; i++)
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
	s.a[0] = -4;
	s.a[1] = 0;
	s.a[2] = 4;
	s.a[3] = 16;
	s.a[4] = -21;
	s.a[5] = 5;
	s.pre = N;
	return s;
}

bool GoalState(State s)
{
	for (int i = 0; i < 6; i++)
	{
		if (0 != s.a[i]) return false;
	}
    return true;
}

void PrintState(State s)
{
	for (int i = 0; i < 6; i++)
	{
		printf("%4d", s.a[i]);
	}
	printf("  %c\n", s.pre < 2 ? (s.pre < 1 ? 'N' : 'Y') : (s.pre < 3 ? 'R' : 'G'));
}

//yellow
State YY(State s)
{
	s.a[0]++;
	s.a[1]++;
	s.a[2]--;
	s.a[3] -= 2;
	s.a[4] += 2;
	s.a[5]--;
	s.pre = Y;
	return s;
}

//red
State RR(State s)
{
	s.a[1] -= 3;
	s.a[3]++;
	s.a[4]++;
	s.a[5]++;
	s.pre = R;
	return s;
}

//green
State GG(State s)
{
	s.a[0]--;
	s.a[1]++;
	s.a[2]++;
	s.a[4]--;
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
