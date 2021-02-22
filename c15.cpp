#include <cstdio>
#include <queue>
#include <unordered_map>

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
	bool r;
	colour pre;

	bool operator==(const State& y) const
	{
		return a == y.a and b == y.b and c == y.c and r == y.r;
	}
};

namespace std
{
    template <>
    struct hash<State>
    {
        size_t operator()(const State& s) const
        {
            return s.a * 49 + s.b * 7 + s.c + s.r * 343;
        }
    };
}

State NewState(unsigned int y, unsigned int r, unsigned int g)
{
	State s;
	s.a = y;
	s.b = r;
	s.c = g;
	s.r = false;
	s.pre = N;
	return s;
}

bool SimilarState(State x, State y)
{
	return x.a == y.a and x.b == y.b and x.c == y.c;
}

void PrintState(State s)
{
	printf("%4d%4d%4d   %c\n", s.a, s.b, s.c, s.pre < 2 ? (s.pre < 1 ? 'N' : 'Y') : (s.pre < 3 ? 'R' : 'G'));
}

//yellow
State YY(State s)
{
	if (s.r)
	{
		if (0 == s.a)
		{
			s.a = s.b - 1;
			s.b = s.c - 1;
			s.c = 6;
		}
		else
		{
			s.a--;
			s.b--;
			s.c--;
		}
	}
	else
	{
		if (s.a > 0) s.a--;
		if (s.b > s.a + 1) s.b--;
		if (s.c > s.b + 1) s.c--;
	}
	s.pre = Y;
	return s;
}

//red
State RR(State s)
{
	s.r = not s.r;
	s.pre = R;
	return s;
}

//green
State GG(State s)
{
	if (s.r)
	{
		if (6 == s.c)
		{
			s.c = s.b + 1;
			s.b = s.a + 1;
			s.a = 0;
		}
		else
		{
			s.c++;
			s.b++;
			s.a++;
		}
	}
	else
	{
		if (s.c < 6) s.c++;
		if (s.b < s.c - 1) s.b++;
		if (s.a < s.b - 1) s.a++;
	}
	s.pre = G;
	return s;
}

int main()
{
	State (*func[3])(State) = {YY, RR, GG};

	std::unordered_map<State, State> m;
	std::queue<State> q;

	State x = NewState(2, 3, 4);
	State y = NewState(0, 3, 5);

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

	while (not (x == tmp))
	{
		PrintState(tmp);
		tmp = m[tmp];
	}

	return 0;
}
