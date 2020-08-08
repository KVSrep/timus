#include <iostream>
#include <string.h>
using namespace std;

long long gcd(long long a, long long b)
{
    while ((a)&&(b)) {
        if (a >= b) a %= b;
        else b %= a;
	}
    return a+b;
}

long long sqrt(long long a)
{
    long long x0 = a+1;
    long long x1 = a, r;
    while (x1<x0) {
        x0 = x1;
        r = a/x0;
        x1 = (x0+r)>>1;
	}
    return x1;
}
    
long long Q(long long y, long long l, long long xb, long long m2, long long a)
{
	long long t1 = y*a - l*xb;
	long long t2 = l*m2 - y*xb;
    return y*t1 + l*t2;
}

long long contfractP(long long n, long long m, int& i) 
{
    i = -1;
    long long w, a;
    long long c = n;
    long long q = 0;
    long long p = 1;
    while (p!=c) {
    	a = n/m;
    	w = n%m;
    	n = m;
    	m = w;
        w = p*a + q;
        q = p;
        p = w;
        i = -i;
    }
    return q;
}


long long mult(long long a, long long b, long long p)
{
	return (a*b)%p;
}


long long power(long long a, long long b, long long p)
{
    long long res = 1;
    while (b) {
        if (b&1) res = mult(res, a, p);
        a = mult(a, a ,p);
        b >>= 1;
	}
    return res;
}


int leg(long long a, long long n)
{
    if (a==1) return 1;
    int e=0, s;
    while (!(a&1))
    {
          a >>= 1;
          ++e;
    }
    int r = n&7;
    if ((!(e&1))||(r==1)||(r==7)) s=1;
    else s=-1;
    if (2&n&a) s=-s;
    if (a==1) return s;
    return s*leg(n%a, a);
}


long long obr(long long a, long long b)
{
    long long x2=1, x1=0, y2=0, y1=1;
    long long q,r,x,y;
    while (b>0)
    {
          q = a/b;
          r = a%b;
          x = x2 - q*x2;
          y = y2 - q*y1;
          a = b;
          b = r;
          x2 = x1;
          x1 = x;
          y2 = y1;
          y1 = y;
    }
    return y2;
}

long long find(long long a, long long n)
{
    long long t = n-1, S=0;
    while (!(t&1))
    {
        t >>= 1;
        ++S;
    }
    long long b=1, d;
    while (leg(b,n)!=-1) ++b;
    long long a1 = obr(n, a);
    if (a1 < 0) a1 += n;
    long long c=power(b,t,n);
    long long r = power(a,(t+1)>>1,n);
    for (long long i=1;i<S;++i)
    {
        d = power(mult(mult(r,r,n),a1,n), 1<<(S-i-1), n);
        if (!((d+1)%n)) r = mult(r,c,n);
        c = mult(c,c,n);
    }
    return r;
}

bool checkprime(long long n) {
    long long t = n-1;
    int S = 0;
    while (!(t&1)) {
        t >>= 1;
        ++S;
    }
    long long a;
    long long arr[] = {2,3};
    for (int i=0;i<2;++i) {
    	a = arr[i];
        if (n==a) return true;
        long long tmp = power(a, t, n);
        if ((tmp==1)||(tmp==n-1)) continue;
        for (int j=1;j<S;++j) {
            tmp = mult(tmp,tmp,n);
            if (tmp==n-1) break;
            if (tmp==1) return false;
    	}
        if (tmp==n-1) continue;
        return false;
	}
    return true;
}

void factor(long long n, long long k, long long*& arr, int& size)
{
	if (checkprime(n)) {
		long long * temp = new long long[2];
		temp[0] = n;
		temp[1] = 1;
		size = 2;
		arr = temp;
		return;
	}
	long long b = sqrt(n);
	if (b*b==n) {
		long long* d;
		int s; 
		factor(b, k, d, s);
		for (int i=0;i<s;i+=2) d[i+1] <<= 1;
		arr = d;
		size = s;
		return;
	}
	++b;
	long long n4 = sqrt(b);
	n4 <<= 1;
	long long m = k;
	while (m<=n4)
	{
		if (!(n%m)) {
			long long* d;
			int s;
			factor(n/m, m, d, s);
			for (int i=0;i<s;i+=2) 
			if (d[i]==m) {
				++d[i+1];
				size = s;
				arr = d;
				return;
			}
			size = s+2;
			long long* t = new long long[size];
			memcpy(t, d, sizeof(long long)*s);
			delete [] d;
			t[s] = m;
			t[s+1] = 1;
			arr = t;
			return;
		}
		m += 2;
	}
	while (true)
	{
		if (!checkprime(m)) {
			m += 2;
			continue;
		}
		if (!(n%m)) {
			long long* d;
			int s;
			factor(n/m, m, d, s);
			for (int i=0;i<s;i+=2) 
			if (d[i]==m) {
				++d[i+1];
				size = s;
				arr = d;
				return;
			}
			size = s+2;
			long long* t = new long long[size];
			memcpy(t, d, sizeof(long long)*s);
			delete [] d;
			t[s] = m;
			t[s+1] = 1;
			arr = t;
			return;
		}
		long long m2 = m*m;
    	long long n2 = n%m2;
    	long long N = n2%m;
    	if (leg(N, m)==-1) {
    		m += 2;
    		continue;
		}
		long long x = find(N, m);
		long long b1 = n2-x*x;
		b1 = b1/m;
		b1 = b1%m;
		int f;
    	long long P = contfractP(m, x<<1, f);
    	long long s0 = (P*b1*f)%m;
    	if (s0 < 0) s0 += m;
    	long long x0 = (x+s0*m-b)%m2;
    	if (x0 < 0) x0 += m2;
    	long long x1 = (-x-s0*m-b)%m2;
    	if (x1 < 0) x1 += m2;
    	x = x0;
    	long long a, w, z, z2;
    	bool xf = false;
    	while (true)
    	{
    		long long v = b*b-n;
    		long long t = x*x + (b<<1)*x + v;
    		long long a1 = t/m2;
    		long long xb = x+b;
    		long long r = x;
    		long long d = m2;
    		bool f = false;
    		long long p=0,l=1,q=1,y=0;
    		do {;
    			a = r/d;
    			w = r%d;
    			r = d;
    			d = w;
    			w = y*a+q;
    			q = y;
    			y = w;
    			w = l*a+p;
    			p = l;
    			l = w;
        		if (f) {
        			f = false;
        			continue;
				}
				f = true;
				z2 = Q(y,l,xb,m2,a1);
    			if (!z2) continue;
    			z = sqrt(z2);
    			if (z*z==z2) {
    				z *= m;
    				t = y*xb - l*m2;
    				long long k1 = gcd(n, t-z);
    				long long k2 = gcd(n, t+z);
    				if ((k1==1)||(k2==1)) continue;
    				long long * d1;
    				int s1;
    				long long * d2;
    				int s2;
    				factor(k1, m, d1, s1);
    				factor(k2, m, d2, s2);
    				int s3 = 0;
    				for (int i=0;i<s1;i+=2) for (int j=0;j<s2;j+=2) if (d2[j]==d1[i]) 
					{
    					++s3;
						d1[i+1] += d2[j+1];
						d2[j+1] = 0;	
					}
					int s23 = s2-(s3<<1);
					if (!s23) {
						delete [] d2;
						arr = d1;
						size = s1;
						return;
					}
					int k=0;
					long long * t3 = new long long[s23];
					for (int i=0;i<s2;i+=2) if (d2[i+1]) 
					{
						t3[k] = d2[i];
						t3[k+1] = d2[i+1];
						k += 2;
					}
					delete [] d2;
					int s4 = s1 + s23;
					long long * t4 = new long long[s4];
					memcpy(t4,t3,sizeof(long long)*s23);
					memcpy(t4+s23,d1,sizeof(long long)*s1);
					delete [] t3;
					delete [] d1;
					arr = t4;
					size = s4;
					return;
				}
			} while ((y<n4)&&(y!=m2)&&(d));
			if (xf) break;
			x = x1;
			xf = true;
		}
		m += 2;
	}	
}


int main()
{
	long long T, n;
	cin>>T;
	for (int i=0;i<T;++i)
	{
		cin>>n;
		long long * res;
		int s=0;
		long long t = n-1;
		while (!(t&1)) t >>= 1;
		if (t!=1) factor(t,3,res,s);
		long long a = n;
		while (a > 1)
		{
			--a;
			if (power(a,n-1,n) != 1) continue;
			if (power(a,(n-1)>>1,n) == 1) continue;
			int k=0;
			bool ok=true;
			while (k < s)
			{
				if (power(a,(n-1)/res[k],n) == 1) ok = false;
				k += 2;
			}
			if (!ok) continue;
			cout<<a<<"\n";
			break;
		}
	}
	return 0;
}