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
	unsigned int a, b, c;
	unsigned int sa, sb, sc;
	colour pre;

	bool operator<(const State& y) const
	{
		return a < y.a
			or (a == y.a and b < y.b)
			or (a == y.a and b == y.b and c < y.c)
			or (a == y.a and b == y.b and c == y.c and sa < y.sa)
			or (a == y.a and b == y.b and c == y.c and sa == y.sa and sb < y.sb)
			or (a == y.a and b == y.b and c == y.c and sa == y.sa and sb == y.sb and sc < y.sc)
			or (a == y.a and b == y.b and c == y.c and sa == y.sa and sb == y.sb and sc == y.sc and pre < y.pre);
	}
};

State NewState(unsigned int y, unsigned int r, unsigned int g)
{
	State s;
	s.a = y;
	s.b = r;
	s.c = g;
	s.sa = 0;
	s.sb = 0;
	s.sc = 0;
	s.pre = N;
	return s;
}

bool SimilarState(State x, State y)
{
	return x.a == y.a and x.b == y.b and x.c == y.c;
}

void PrintState(State s)
{
	printf("%4d%4d%4d%4d%4d%4d   %c\n", s.a, s.b, s.c, s.sa, s.sb, s.sc, s.pre < 2 ? (s.pre < 1 ? 'N' : 'Y') : (s.pre < 3 ? 'R' : 'G'));
}

//yellow
State YY(State s)
{
	if (R == s.pre or G == s.pre)
	{
		s.a += s.sb + s.sc;
		s.b -= s.sb;
		s.c -= s.sc;
		s.sa = 0;
		s.sb = 0;
		s.sc = 0;
	}
	else
	{
		s.sa += s.sa < s.a ? 1 : 0;
		s.sb += s.sb < s.b ? 1 : 0;
		s.sc += s.sc < s.c ? 1 : 0;
	}
	s.pre = Y;
	return s;
}

//red
State RR(State s)
{
	if (Y == s.pre or G == s.pre)
	{
		s.a -= s.sa;
		s.b += s.sa + s.sc;
		s.c -= s.sc;
		s.sa = 0;
		s.sb = 0;
		s.sc = 0;
	}
	else
	{
		s.sa += s.sa < s.a ? 1 : 0;
		s.sb += s.sb < s.b ? 1 : 0;
		s.sc += s.sc < s.c ? 1 : 0;
	}
	s.pre = R;
	return s;
}

//green
State GG(State s)
{
	if (Y == s.pre or R == s.pre)
	{
		s.a -= s.sa;
		s.b -= s.sb;
		s.c += s.sa + s.sb;
		s.sa = 0;
		s.sb = 0;
		s.sc = 0;
	}
	else
	{
		s.sa += s.sa < s.a ? 1 : 0;
		s.sb += s.sb < s.b ? 1 : 0;
		s.sc += s.sc < s.c ? 1 : 0;
	}
	s.pre = G;
	return s;
}

int main()
{
	State (*func[3])(State) = {YY, RR, GG};

	std::map<State, State> m;
	std::queue<State> q;

	State x = NewState(8, 7, 3);
	State y = NewState(2, 9, 7);

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
