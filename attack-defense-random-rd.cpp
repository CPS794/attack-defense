/*input
1
12 12 1 1 0.85 0.85 4.1 0.6 0.7
1 1 1 1 1 1 1 1 1 1 1 1
*/
#include <bits/stdc++.h>
using namespace std;

const int MAXT=1007;
const int MAXN=13;
const int MAXM=1;
const int MAXK=3;
const int MOD=1000000007;
const int INF=2139062143;
const double EPS=1e-7;
const int destroyed[13][13] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,2,3,4,5,6,7,8,9,10,11,12},
	{2,1,7,2,3,4,5,6,8,9,10,11,12},
	{4,1,7,3,9,2,4,5,6,8,10,11,12},
	{6,1,7,3,9,4,10,2,5,6,8,11,12},
	{6,1,7,3,9,6,12,2,4,5,8,10,11},
	{4,1,7,6,12,2,3,4,5,8,9,10,11},
	{2,2,8,1,3,4,5,6,7,9,10,11,12},
	{2,3,9,1,2,4,5,6,7,8,10,11,12},
	{4,3,9,4,10,1,2,5,6,7,8,11,12},
	{8,3,9,4,10,5,11,6,12,1,2,7,8},
	{4,3,9,6,12,1,2,4,5,7,8,10,11},
	{2,6,12,1,2,3,4,5,7,8,9,10,11}
};
const int u[2][14] = {
	{0,60,30,15,0,0,15,45,45,30,0,30,45,-30},
	{0,-30,0,15,30,30,15,-15,-15,0,30,0,-15,60}
};

const string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}

bool zero(double x) { // 实数判0
	return abs(x)<EPS;
}

double cmp(double a,double b) { // 比较实数a和b的大小,返回a-b
	return abs(a-b)<EPS ? 0 : a-b;
}

double w(double p, double t) { // w+: t=chi w-:t=delta
	double one_p = 1.0-p;
	if (zero(p))
	{
		return 0.0;
	}
	if (zero(one_p))
	{
		return pow(p,t)/pow(pow(p,t),1.0/t);
	}
	return pow(p,t)/pow(pow(p,t)+pow(one_p,t),1.0/t);
}

int rd[MAXN];

struct Gen {
	mt19937 g;
	Gen() : g(static_cast<uint32_t>(rand())){}
	size_t operator()(size_t n)
	{
		std::uniform_int_distribution<size_t> d(0, n ? n-1 : 0);
		return d(g);
	}
};

struct Attack
{
	double va,vd;
	int ra[MAXN],rd[MAXN];
	Attack()
	{
		va=-INF;
		vd=INF;
	}
	bool operator <(const Attack &p) const {
		return cmp(va,p.va)>0;
	}
	const Attack& operator =(const Attack& p) {
		va=p.va;
		vd=p.vd;
		for (int i = 0; i < MAXN; ++i)
		{
			ra[i]=p.ra[i];
			rd[i]=p.rd[i];
		}
		return *this;
	}
	friend ostream& operator <<(ostream &os, const Attack &p) {
		os<<"Va: "<<p.va<<", Vd: "<<p.vd<<".\tra: ";
		for (int i = 1; i < MAXN; ++i)
		{
			os<<p.ra[i]<<",";
		}
		os<<"\trd: ";
		for (int i = 1; i < MAXN; ++i)
		{
			os<<p.rd[i]<<",";
		}
		os<<endl;
		return os;
	}
	void random(int ra_total, int rd_total, int k) // k 恒为 12
	{
		va=-INF;
		vd=INF;
		int a[MAXT],b[MAXT];
		int l,r,n;
		memset(a,0,sizeof(a));
		memset(b,0,sizeof(b));
		for (int i = 1; i <= ra_total; ++i)
		{
			a[i] = 1;
		}
		random_shuffle(a+1,a+ra_total+k,Gen());
		l = 0,r = 1,n = 0;
		while(r <= ra_total+k+1)
		{
			if (a[r]==0)
			{
				b[n++]=r-l-1;
				l=r;
			}
			r++;
		}
		for (int i = 0; i < k; ++i)
		{
			ra[i+1] = b[i];
		}
		memset(a,0,sizeof(a));
		memset(b,0,sizeof(b));
		for (int i = 1; i <= rd_total; ++i)
		{
			a[i] = 1;
		}
		random_shuffle(a+1,a+rd_total+k,Gen());
		l = 0,r = 1,n = 0;
		while(r <= rd_total+k+1)
		{
			if (a[r]==0)
			{
				b[n++]=r-l-1;
				l=r;
			}
			r++;
		}
		for (int i = 0; i < k; ++i)
		{
			rd[i+1] = b[i];
		}
		int total = 0;
		for (int i = 1; i <= k; ++i)
		{
			total += ra[i];
		}
		if (total != ra_total)
		{
			cout << "Error:" << *this <<endl;
		}
		total = 0;
		for (int i = 1; i <= k; ++i)
		{
			total += rd[i];
		}
		if (total != rd_total)
		{
			cout << "Error:" << *this <<endl;
		}

	}
	void calculate(int cd, int ca, double g, double l, double lambda, double chi, double delta) {
		double pAB[MAXN]; // 1-6 for A, 7-12 for B
		double p[14];
		double sigma_p[14];
		double pi[2][14]; // 0 for pi(k)+, 1 for pi(k)- 
		memset(pAB,0,sizeof(pAB));
		memset(p,0,sizeof(p));
		memset(sigma_p,0,sizeof(sigma_p));
		double cd2ca2 = 1.0 * cd * cd / ca / ca;
		for (int i = 1; i < MAXN; ++i)
		{
			if (ra[i] == 0 && rd[i] == 0)
			{
				pAB[i] = 1;
			}
			else
			{
				pAB[i] = 1.0 * rd[i] * rd[i] / (1.0 * rd[i] * rd[i] + cd2ca2 * ra[i] * ra[i]);
			}
		}
		pAB[0]=1;
		p[0]=0;
		p[13]=1;
		for (int i = 1; i < 13; ++i)
		{
			double tmp = 1.0;
			for (int j = 1; j < destroyed[i][0]; j+=2)
			{
				tmp *= 1.0 - pAB[destroyed[i][j]] * pAB[destroyed[i][j+1]] ;
			}
			p[i] = tmp;
			for (int j = destroyed[i][0]+1; j < 13; ++j)
			{
				p[i] *= pAB[destroyed[i][j]];
			}
			p[13] -= p[i];
			sigma_p[i] = sigma_p[i-1] + p[i];
		}
		sigma_p[13] = sigma_p[12] + p[13];
		for (int k = 1; k < 14; ++k)
		{
			pi[0][k] = w(1-sigma_p[k-1],chi) - w(1-sigma_p[k],chi);
			pi[1][k] = w(sigma_p[k],delta) - w(sigma_p[k-1],delta);
		}
		vd = 0;
		for (int i = 1; i < 14; ++i)
		{
			if (u[0][i]>0)
			{
				vd += pow(u[0][i],g)*pi[0][i];
			} 
			else if (u[0][i]<0)
			{
				vd -= pow(-u[0][i],l)*pi[1][i]*lambda;
			}
			// cout<<"i: "<<i<<", Vd: "<<vd<<endl;
		}
		va = 0;
		for (int i = 1; i < 14; ++i)
		{
			if (u[1][i]>0)
			{
				va += pow(u[1][i],g)*pi[0][i];
			} 
			else if (u[1][i]<0)
			{
				va -= pow(-u[1][i],l)*pi[1][i]*lambda;
			}
			// cout<<"i: "<<i<<", Va: "<<va<<endl;
		}
		// cout<<endl;
		// cout<<"pAB: ";
		// for (int i = 1; i < MAXN; ++i)
		// {
		// 	// cout<<ra[i]<<",";
		// 	cout<<pAB[i]<<",";
		// }
		// cout<<endl;
		// cout<<"p: ";
		// for (int i = 1; i < 14; ++i)
		// {
		// 	cout<<p[i]<<",";
		// }
		// cout<<endl;
		// for (int i = 1; i < 14; ++i)
		// {
		// 	// cout<<sigma_p[i]<<",";
		// 	cout<<pi[0][i]<<",";
		// }
		// cout<<endl;
		// for (int i = 1; i < 14; ++i)
		// {
		// 	cout<<pi[1][i]<<",";
		// }
		// cout<<endl;
		// cout<<"Va: "<<va<<", Vd: "<<vd<<endl;
	}
};

int i,j,k,n,m,x,y,cnt,tcase,xcase;
int ra_total,rd_total,ca,cd;
double g,l,lambda,chi,delta;
int ra[MAXN];
Attack t,res;
Attack attack[MAXM];
string s;

void getRa(Attack a, int index, int remain)
{
	if (index == 12)
	{
		a.ra[12] = remain;
		a.calculate(cd,ca,g,l,lambda,chi,delta);
		for (int i = 0; i < MAXM; ++i)
		{
			if (a < attack[i])
			{
				for (int j = MAXM-1; j > i ; --j)
				{
					attack[j] = attack[j-1];
				}
				attack[i] = a;
				break;
			}
		}
		// if (cnt<20)
		// {
		// 	cout<<"A: "<<a<<endl;
		// }
		// cout<<"A: "<<a<<endl;
		if (a < res)
		{
			// cout<<"Res: "<<res<<"A: "<<a<<endl<<endl;
			res = a;
		}
		// cnt++;
		// if (cnt>20)
		// {
		// 	exit(0);
		// }
	}
	else
	{
		// if (index == 2)
		// {
		// 	cout<<fixed<<setprecision(4)<<"Over All:["<<(ra_total-remain)*100.0/ra_total<<"%]-----------"<< currentDateTime() <<" -----------"<<endl;
		// 	cout<<"\tCurrent Top "<<MAXK<<" Results: "<<endl;
		// 	for (int i = 0; i < MAXK; ++i)
		// 	{
		// 		cout<<setprecision(10)<<"\tResult "<<i+1<<": "<<attack[i];
		// 	}
		// }
		// if (index == 3)
		// {
		// 	cout<<fixed<<setprecision(4)<<"\tSub:["<<(ra_total-remain)*100.0/ra_total<<"%]----- "<< currentDateTime() <<" -----"<<endl;
		// 	cout<<"\t\tCurrent Best Result: "<<endl;
		// 	cout<<setprecision(10)<<"\t\t"<<attack[0]<<endl;
		// }
		for (int i = 0; i <= remain; ++i)
		{
			a.ra[index] = i;
			getRa(a, index+1, remain-i);
		}
	}
}

int main()
{
	tcase = 1;
	srand(time(0));

	#ifdef Smile
		freopen("s.in","r",stdin);
		freopen("s.out","w",stdout);
		cin>>tcase;
	#endif

	cout<<"========================= "<< currentDateTime() <<" =========================="<<endl;
	xcase=0;
	while (xcase++ < tcase)
	{
		memset(rd,0,sizeof(rd));
		memset(attack,0,sizeof(attack));
		for (int i = 0; i < MAXM; ++i)
		{
			attack[i].va=-INF;
			attack[i].vd=INF;
		}
		cnt=0;
		cin>>rd_total>>ra_total>>cd>>ca;
		cin>>g>>l>>lambda>>chi>>delta;

		for (int i = 1; i < MAXN; ++i)
		{
			cin>>t.ra[i];
		}
		for (int i = 1; i < MAXN; ++i)
		{
			cin>>t.rd[i];
		}
		t.calculate(cd,ca,g,l,lambda,chi,delta);
		cout<<setprecision(10)<<t<<endl;
		
		// for (int i = 0; i < 100; ++i)
		// {
		// 	// t.random(rand()%(MAXT-24)+1,rand()%(MAXT-24)+1,12);
		// 	t.random(12,12,12);
		// 	// cout<<t;
		// 	t.calculate(cd,ca,g,l,lambda,chi,delta);
		// 	cout<<setprecision(10)<<t;
		// }
		
		// for (int i = 1; i < MAXN; ++i)
		// {
		// 	cin>>rd[i];
		// }
		// for (int i = 1; i < MAXN; ++i)
		// {
		// 	t.ra[i] = 1;
		// 	t.rd[i] = 1;
		// }
		// // t.random(12,12,12);
		// t.calculate(cd,ca,g,l,lambda,chi,delta);
		// // cout<<t<<endl;
		// res = t;
		// res.va = 0;
		// // attack[cnt++]=t;
		// // getRa(t,1,ra_total);
		// // for (int i = 0; i < MAXM; ++i)
		// // {
		// // 	cout<<setprecision(10)<<"Result "<<i+1<<": "<<attack[i]<<endl;
		// // }
		// // cout<<setprecision(10)<<"Result: "<<res<<endl;
		// cout<<"========================= "<< currentDateTime() <<" =========================="<<endl;
		
	}
	#ifndef Smile
		system("pause");
	#endif
	return 0;
}