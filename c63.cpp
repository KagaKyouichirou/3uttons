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
	int a[8];
	int p;
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

State InitialState()
{
	State s;

	for (int i = 0; i < 8; i++)
	{
		s.a[i] = -1;
	}

	s.p = 0;

	s.pre = N;
	return s;
}

bool GoalState(const State s)
{
	for (int i = 3; i < 8; i++)
	{
		if (-1 != s.a[i]) return false;
	}
	return 1 == s.a[0] and 2 == s.a[1] and 8 == s.a[2];
}

void PrintState(State s)
{
	for (int i = 0; i < 8; i++)
	{
		printf("%5d", s.a[i]);
	}
	printf("    %c\n", ToChar(s.pre));
}

//yellow
State YY(State s)
{
	if (8 == s.p) return s;
	s.a[s.p] = 0;
	s.p++;
	s.pre = Y;
	return s;
}

//red
State RR(State s)
{
	if (8 == s.p) return s;
	s.a[s.p] = s.p + 1;
	s.p++;
	s.pre = R;
	return s;
}

//green
State GG(State s)
{
	int i = 0;
	while (i < s.p) {
		if (0 == s.a[i])
		{
			int j = i;
			while (0 == s.a[j]) j++;
			for (int k = j; k < 8; k++)
			{
				s.a[k - 1] = s.a[k];
			}
			s.a[7] = -1;
			s.p--;
			i = j - 1;
		}
		else
		{
			int j = i;
			int sum = 0;
			while (0 < s.a[j])
			{
				sum += s.a[j];
				j++;
			}
			if (sum < 10)
			{
				s.a[i] = sum;
				i++;
			}
			else
			{
				s.a[i] = sum / 10;
				s.a[i + 1] = sum % 10;
				i += 2;
			}
			for (int k = j; k < s.p; k++)
			{
				s.a[k - j + i] = s.a[k];
			}
			s.p -= j - i;
			for (int k = s.p; k < 8; k++)
			{
				s.a[k] = -1;
			}
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
