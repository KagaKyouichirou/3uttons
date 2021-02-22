#include <cstdio>
#include <array>
#include <queue>

struct State
{
	int a, b, c;
};

inline unsigned int Code(State s)
{
	return s.a * 100 + s.b * 10 + s.c;
}

State Decode(unsigned int t)
{
	State s;
	s.a = (t / 100) % 10;
	s.b = (t / 10) % 10;
	s.c = t % 10;
	return s;
}

//yellow
State YX(State s)
{
	s.a = (s.a + 1) % 10;
	s.b = (s.b + 1) % 10;
	return s;
}

//red
State RX(State s)
{
	int tmp = s.c;
	s.c = s.b;
	s.b = s.a;
	s.a = tmp;
	return s;
}

//green
State GX(State s)
{
	s.b = (s.b + 9) % 10;
	s.c = (s.c + 9) % 10;
	return s;
}

//reverse-yellow
State YY(State s)
{
	s.a = (s.a + 9) % 10;
	s.b = (s.b + 9) % 10;
	return s;
}

//reverse-red
State RY(State s)
{
	int tmp = s.a;
	s.a = s.b;
	s.b = s.c;
	s.c = tmp;
	return s;
}

//reverse-green
State GY(State s)
{
	s.b = (s.b + 1) % 10;
	s.c = (s.c + 1) % 10;
	return s;
}

int main()
{
	State (*func[6])(State) = {YX, RX, GX, YY, RY, GY};

	State x, y;
	std::array<unsigned int, 1000> rx, ry;
	std::queue<unsigned int> qx, qy;

	x.a = 0;
	x.b = 0;
	x.c = 0;

	y.a = 6;
	y.b = 6;
	y.c = 6;

	rx.fill(1000);
	ry.fill(1000);

	rx[0] = 1001;
	ry[666] = 1001;

	qx.push(Code(x));
	qy.push(Code(y));

	unsigned int ix, iy, tmp;

	bool halt = false;

	while (not qx.empty() && not qy.empty())
	{
        unsigned int cx = qx.front();
        qx.pop();
        for (int i = 3; i < 6; i++)
        {
            tmp = Code(func[i](Decode(cx)));
            if (1000 == rx[tmp])
            {
                if (1000 != ry[tmp])
                {
                    ix = cx;
                    iy = tmp;
                    halt = true;
                    break;
                }
                else
                {
                    qx.push(tmp);
                    rx[tmp] = cx;
                }
            }
        }
		if (halt) break;

        unsigned int cy = qy.front();
        qy.pop();
        for (int i = 0; i < 3; i++)
        {
            tmp = Code(func[i](Decode(cy)));
            if (1000 == ry[tmp])
            {
                if (1000 != rx[tmp])
                {
                    ix = tmp;
                    iy = cy;
                    halt = true;
                    break;
                }
                else
                {
                    qy.push(tmp);
                    ry[tmp] = cy;
                }
            }
        }
		if (halt) break;
	}

	while (1001 != rx[ix])
	{
		printf("%d\n", ix);
		ix = rx[ix];
	}
	printf("----------\n");
	while (1001 != ry[iy])
	{
		printf("%d\n", iy);
		iy = ry[iy];
	}

	return 0;
}
