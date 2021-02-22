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
	int a[3][5];
	int yh, yt, rh, rt, gh, gt;
	colour pre;

	bool operator<(const State& y) const
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (a[i][j] < y.a[i][j]) return true;
				else if (a[i][j] > y.a[i][j]) return false;
			}
		}
		return yh < y.yh or (yh == y.yh and rh < y.rh) or (yh == y.yh and rh == y.rh and gh < y.gh);
	}
};

State NewState()
{
	State s;
	for (int i = 0; i < 3; i++)
	{
		s.a[i][3] = -1;
		s.a[i][4] = -1;
	}
	s.a[0][0] = 8;
	s.a[0][1] = 1;
	s.a[0][2] = 5;
	s.a[1][0] = 3;
	s.a[1][1] = 0;
	s.a[1][2] = 7;
	s.a[2][0] = 6;
	s.a[2][1] = 4;
	s.a[2][2] = 2;
	s.yh = 3;
	s.yt = 3;
	s.rh = 3;
	s.rt = 3;
	s.gh = 3;
	s.gt = 3;
	s.pre = N;
	return s;
}

bool GoalState(const State& s)
{
	return (0 == s.a[0][0] and 1 == s.a[0][1] and 2 == s.a[0][2]
    and 3 == s.a[1][0] and 4 == s.a[1][1] and 5 == s.a[1][2]
		and 6 == s.a[2][0] and 7 == s.a[2][1] and 8 == s.a[2][2]);
}

void PrintState(State s)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			printf("%3d", s.a[i][j]);
		}
	}
	printf("  %c\n", s.pre < 2 ? (s.pre < 1 ? 'N' : 'Y') : (s.pre < 3 ? 'R' : 'G'));
}

//yellow
State YY(State s)
{
	if (R == s.pre and s.rt > s.rh)
	{
		if (s.yt + s.rt - s.rh <= 5)
		{
			for (int i = s.rh; i < s.rt; i++)
			{
				s.a[0][s.yt + i - s.rh] = s.a[1][i];
				s.a[1][i] = -1;
			}
			s.yt += s.rt - s.rh;
			s.yh = s.yt;
			s.rt = s.rh;
			s.pre = Y;
			return s;
		}
		else return s;
	}
	else if (G == s.pre and s.gt > s.gh)
	{
		if (s.yt + s.gt - s.gh <= 5)
		{
			for (int i = s.gh; i < s.gt; i++)
			{
				s.a[0][s.yt + i - s.gh] = s.a[2][i];
				s.a[2][i] = -1;
			}
			s.yt += s.gt - s.gh;
			s.yh = s.yt;
			s.gt = s.gh;
			s.pre = Y;
			return s;
		}
		else return s;
	}
	if (0 < s.yh)
	{
		s.yh--;
		s.pre = Y;
	}
	return s;
}

//red
State RR(State s)
{
	if (Y == s.pre and s.yt > s.yh)
	{
		if (s.rt + s.yt - s.yh <= 5)
		{
			for (int i = s.yh; i < s.yt; i++)
			{
				s.a[1][s.rt + i - s.yh] = s.a[0][i];
				s.a[0][i] = -1;
			}
			s.rt += s.yt - s.yh;
			s.rh = s.rt;
			s.yt = s.yh;
			s.pre = R;
			return s;
		}
		else return s;
	}
	else if (G == s.pre and s.gt > s.gh)
	{
		if (s.rt + s.gt - s.gh <= 5)
		{
			for (int i = s.gh; i < s.gt; i++)
			{
				s.a[1][s.rt + i - s.gh] = s.a[2][i];
				s.a[2][i] = -1;
			}
			s.rt += s.gt - s.gh;
			s.rh = s.rt;
			s.gt = s.gh;
			s.pre = R;
			return s;
		}
		else return s;
	}
	if (0 < s.rh)
	{
		s.rh--;
		s.pre = R;
	}
	return s;
}

//green
State GG(State s)
{
  if (Y == s.pre and s.yt > s.yh)
	{
		if (s.gt + s.yt - s.yh <= 5)
		{
			for (int i = s.yh; i < s.yt; i++)
			{
				s.a[2][s.gt + i - s.yh] = s.a[0][i];
				s.a[0][i] = -1;
			}
			s.gt += s.yt - s.yh;
			s.gh = s.gt;
			s.yt = s.yh;
			s.pre = G;
			return s;
		}
		else return s;
	}
	else if (R == s.pre and s.rt > s.rh)
	{
		if (s.gt + s.rt - s.rh <= 5)
		{
			for (int i = s.rh; i < s.rt; i++)
			{
				s.a[2][s.yt + i - s.rh] = s.a[1][i];
				s.a[1][i] = -1;
			}
			s.gt += s.rt - s.rh;
			s.gh = s.gt;
			s.rt = s.rh;
			s.pre = G;
			return s;
		}
		else return s;
	}
	if (0 < s.gh)
	{
		s.gh--;
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

	int layer = 0;
	while (not q.empty() and keep)
	{
	  printf("%d\n", layer);
	  layer++;
	  for (int k = 0; k < q.size() and keep; k++)
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
				  else
				  {
					  q.push(tmp);
				  }
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
