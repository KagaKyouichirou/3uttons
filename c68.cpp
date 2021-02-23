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
	int a[3];
	colour pre;

	bool operator<(const State& y) const
	{
		for (int i = 0; i < 3; i++)
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

	s.a[0] = 2;
	s.a[1] = 3;
	s.a[2] = 4;

	s.pre = N;
	return s;
}

bool GoalState(const State s)
{
	int r1 = s.a[0] + s.a[1] + s.a[2];
	int r2 = s.a[0] * s.a[1] + s.a[0] * s.a[2] + s.a[1] * s.a[2];
	int r3 = s.a[0] * s.a[1] * s.a[2];
	return 28 == r1 and 187 == r2 and 0 == r3;
}

void PrintState(State s)
{
	for (int i = 0; i < 3; i++)
	{
		printf("%5d", s.a[i]);
	}
	printf("    %c\n", ToChar(s.pre));
}

//yellow
State YY(State s)
{
	for (int i = 0; i < 3; i++)
	{
		switch (s.a[i])
		{
			case 0:
			case 1:
				s.a[i] += 7;
				break;
			case 2:
			case 10:
			case 11:
			case 12:
				s.a[i]--;
				break;
			case 3:
				s.a[i] = 0;
				break;
			case 4:
			case 5:
			case 16:
			case 17:
			case 18:
			case 19:
				s.a[i]++;
				break;
			case 6:
			case 7:
			case 8:
			case 9:
				s.a[i] += 15;
				break;
			case 13:
			case 14:
			case 15:
				s.a[i] -= 11;
				break;
			default:
				return InitialState();

		}
	}
	s.pre = Y;
	return s;
}

//red
State RR(State s)
{
	for (int i = 0; i < 3; i++)
	{
		if (24 == s.a[i]) return InitialState();
		else s.a[i]++;
	}
	s.pre = R;
	return s;
}

//green
State GG(State s)
{
	for (int i = 0; i < 3; i++)
	{
		switch (s.a[i])
		{
			case 0:
				s.a[i] = 3;
				break;
			case 1:
			case 9:
			case 10:
			case 11:
				s.a[i]++;
				break;
			case 2:
			case 3:
			case 4:
				s.a[i] += 11;
				break;
			case 5:
			case 6:
			case 17:
			case 18:
			case 19:
			case 20:
				s.a[i]--;
				break;
			case 7:
			case 8:
				s.a[i] -= 7;
				break;
			case 21:
			case 22:
			case 23:
			case 24:
				s.a[i] -= 15;
				break;
			default:
				return InitialState();
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
