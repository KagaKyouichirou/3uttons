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
	unsigned int a[6];
	unsigned int p;
	bool r;
	colour pre;

	bool operator==(const State& y) const
	{
		for (int i = 0; i < 6; i++)
		{
			if (a[i] != y.a[i]) return false;
		}
		return p == y.p and r == y.r;
	}
};

namespace std
{
    template <>
    struct hash<State>
    {
        size_t operator()(const State& s) const
        {
        	size_t h = 0;
        	for (int i = 0; i < 6; i++)
        	{
        		h *= 6;
        		h += s.a[i];
        	}
        	h *= 5;
        	h += s.p;
        	h *= 2;
        	h += s.r;
            return h;
        }
    };
}

State NewState(unsigned int a0, unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5)
{
	State s;
	s.a[0] = a0;
	s.a[1] = a1;
	s.a[2] = a2;
	s.a[3] = a3;
	s.a[4] = a4;
	s.a[5] = a5;
	s.p = 2;
	s.r = false;
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
	printf("%2d%2d%2d%2d%2d%2d", s.a[0], s.a[1], s.a[2], s.a[3], s.a[4], s.a[5]);
	printf(" %c\n", s.pre < 2 ? (s.pre < 1 ? 'N' : 'Y') : (s.pre < 3 ? 'R' : 'G'));
}

//yellow
State YY(State s)
{
	if (s.p > 0)
	{
		if (s.r)
		{
			unsigned int tmp = s.a[s.p - 1];
			s.a[s.p - 1] = s.a[s.p];
			s.a[s.p] = s.a[s.p + 1];
			s.a[s.p + 1] = s.a[s.p + 2];
			s.a[s.p + 2] = tmp;
		}
		s.p--;
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
	if (s.p + 3 < 6)
	{
		if (s.r)
		{
			unsigned int tmp = s.a[s.p + 3];
			s.a[s.p + 3] = s.a[s.p + 2];
			s.a[s.p + 2] = s.a[s.p + 1];
			s.a[s.p + 1] = s.a[s.p];
			s.a[s.p] = tmp;
		}
		s.p++;
	}
	s.pre = G;
	return s;
}

int main()
{
	State (*func[3])(State) = {YY, RR, GG};

	std::unordered_map<State, State> m;
	std::queue<State> q;

	State x = NewState(5, 2, 4, 1, 3, 0);
	State y = NewState(0, 1, 2, 3, 4, 5);

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
