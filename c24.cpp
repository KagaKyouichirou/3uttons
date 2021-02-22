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
	unsigned int a[9];
	colour pre;

	bool operator<(const State& y) const
	{
		for (int i = 0; i < 9; i++)
		{
			if (a[i] < y.a[i]) return true;
			else if (a[i] > y.a[i]) return false;
		}
		return false;
	}
};

State NewState(unsigned int a0, unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, unsigned int a8)
{
	State s;
	s.a[0] = a0;
	s.a[1] = a1;
	s.a[2] = a2;
	s.a[3] = a3;
	s.a[4] = a4;
	s.a[5] = a5;
	s.a[6] = a6;
	s.a[7] = a7;
	s.a[8] = a8;
	s.pre = N;
	return s;
}

bool SimilarState(State x, State y)
{
	for (int i = 0; i < 6; i++)
	{
		if (x.a[i] != y.a[i]) return false;
	}
	return true;
}

void PrintState(State s)
{
	printf("%4d%4d%4d%4d%4d%4d%4d%4d%4d", s.a[0], s.a[1], s.a[2], s.a[3], s.a[4], s.a[5], s.a[6], s.a[7], s.a[8]);
	printf("   %c\n", s.pre < 2 ? (s.pre < 1 ? 'N' : 'Y') : (s.pre < 3 ? 'R' : 'G'));
}

//yellow
State YY(State s)
{
	unsigned int tmp = s.a[0];
	s.a[0] = s.a[1];
	s.a[1] = s.a[2];
	s.a[2] = s.a[5];
	s.a[5] = s.a[8];
	s.a[8] = s.a[7];
	s.a[7] = s.a[6];
	s.a[6] = s.a[3];
	s.a[3] = tmp;
	s.pre = Y;
	return s;
}

//red
State RR(State s)
{
	unsigned int tmp = s.a[0];
	s.a[0] = s.a[3];
	s.a[3] = s.a[6];
	s.a[6] = tmp;
	tmp = s.a[1];
	s.a[1] = s.a[4];
	s.a[4] = s.a[7];
	s.a[7] = tmp;
	tmp = s.a[2];
	s.a[2] = s.a[5];
	s.a[5] = s.a[8];
	s.a[8] = tmp;
	s.pre = R;
	return s;
}

//green
State GG(State s)
{
	unsigned int tmp = s.a[0];
	s.a[0] = s.a[3];
	s.a[3] = s.a[6];
	s.a[6] = s.a[7];
	s.a[7] = s.a[8];
	s.a[8] = s.a[5];
	s.a[5] = s.a[2];
	s.a[2] = s.a[1];
	s.a[1] = tmp;
	s.pre = G;
	return s;
}

int main()
{
	State (*func[3])(State) = {YY, RR, GG};

	std::map<State, State> m;
	std::queue<State> q;

	State x = NewState(8, 3, 6, 7, 0, 5, 4, 2, 1);
	State y = NewState(0, 1, 2, 3, 4, 5, 6, 7, 8);

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
				if (SimilarState(tmp, y))
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
